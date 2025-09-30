#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para a função sleep()

// Inclui todos os nossos módulos da biblioteca
#include "luhn.h"
#include "flash_mem.h"
#include "secure_log.h"
#include "iso8583.h"
#include "fsm.h"

// Função para o modo de demonstração
void run_demo_mode() {
    printf("--- MODO DEMONSTRACAO ---\n\n");

    // 1. Inicialização dos sistemas
    FSM fsm;
    fsm_init(&fsm);
    flash_init(); // Limpa a memória flash para a demo
    printf("\n");

    // 2. Simula a inserção do cartão
    printf("--> Evento: Cartao inserido\n");
    fsm_handle_event(&fsm, FSM_EVENT_CARD_INSERT);
    sleep(1);

    // 3. Simula a digitação do PIN e a montagem da mensagem ISO
    printf("--> Evento: PIN OK\n");
    fsm_handle_event(&fsm, FSM_EVENT_PIN_OK);
    
    // -- INTEGRAÇÃO REAL --
    // Neste ponto, a FSM nos diz que devemos montar a mensagem
    Iso8583Message tx_message;
    iso8583_init_message(&tx_message);
    strcpy(tx_message.mti, "0200");
    iso8583_set_field(&tx_message, 2, "1234567890123456");
    iso8583_set_field(&tx_message, 4, "000000010000"); // R$ 100.00
    iso8583_set_field(&tx_message, 41, "TERM0001");
    iso8583_set_field(&tx_message, 128, "11223344AABBCCDD");

    uint8_t iso_buffer[512] = {0};
    int packed_len = iso8583_pack(&tx_message, iso_buffer);
    printf("    Mensagem ISO 8583 montada com %d bytes.\n", packed_len);
    sleep(1);

    // 4. Simula o envio bem-sucedido da mensagem
    printf("--> Evento: Mensagem enviada com sucesso\n");
    fsm_handle_event(&fsm, FSM_EVENT_SEND_SUCCESS);
    sleep(2); // Simula o tempo de espera da resposta

    // 5. Simula o recebimento de uma resposta APROVADA
    // (Aqui, o parser seria usado para ler a resposta real)
    printf("--> Evento: Resposta APROVADA recebida\n");
    fsm_handle_event(&fsm, FSM_EVENT_RESPONSE_RECEIVED_APPROVED);
    
    // -- INTEGRAÇÃO REAL --
    // A FSM está no estado APROVADO, então devemos gravar o log
    if (fsm.current_state == FSM_STATE_APPROVED) {
        printf("    Gravando log da transacao...\n");
        char masked_pan[20];
        mask_pan(tx_message.fields[2], masked_pan);
        
        char log_entry[100];
        sprintf(log_entry, "STAN:%06u|PAN:%s|VALOR:%s|RESP:APROVADA", 
                fsm.stan, masked_pan, tx_message.fields[4]);
        
        // Grava no primeiro slot de log (endereço 0)
        write_secure_log(0, log_entry);
        printf("    Log gravado com sucesso!\n");
    }
    sleep(1);
    
    // 6. Finaliza a transação
    printf("--> Evento: Transacao finalizada\n");
    fsm_handle_event(&fsm, FSM_EVENT_FINISH_TRANSACTION);
    iso8583_free_message(&tx_message);
    printf("\n--- DEMONSTRACAO CONCLUIDA ---\n");
}

// Versão final e limpa da função run_list_mode
void run_list_mode() {
    printf("--- MODO LISTAGEM DE LOGS ---\n\n");
    
    uint8_t read_buffer[256] = {0};
    uint8_t log_len = 0;

    // 1. Lê o primeiro byte (criptografado) para descobrir o tamanho do log
    if (flash_read(0, &log_len, 1) != 0) {
        printf("Nenhum log encontrado.\n");
        printf("\n--- LISTAGEM CONCLUIDA ---\n");
        return;
    }

    // 2. Decifra apenas o byte de tamanho
    xor_cipher(&log_len, 1);
    
    // 3. Verifica se o tamanho é válido antes de prosseguir
    if (log_len > 0 && log_len < sizeof(read_buffer) && flash_read(1, read_buffer, log_len) == 0) {
        // 4. Decifra o buffer do log com o tamanho correto
        xor_cipher(read_buffer, log_len);
        
        // 5. Adiciona o terminador nulo no final da string lida
        read_buffer[log_len] = '\0'; 
        
        printf("Log #1 (decifrado): %s\n", (char*)read_buffer);
    } else {
        printf("Nenhum log valido encontrado.\n");
    }
    
    printf("\n--- LISTAGEM CONCLUIDA ---\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s [demo|list]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "demo") == 0) {
        run_demo_mode();
    } else if (strcmp(argv[1], "list") == 0) {
        run_list_mode();
    } else {
        fprintf(stderr, "Modo desconhecido: '%s'. Use 'demo' ou 'list'.\n", argv[1]);
        return 1;
    }

    return 0;
}