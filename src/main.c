#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "luhn.h"
#include "flash_mem.h"
#include "secure_log.h"
#include "iso8583.h"
#include "fsm.h"

void run_demo_mode() {
    printf("--- MODO DEMONSTRACAO ---\n\n");

    FSM fsm;
    fsm_init(&fsm);
    flash_init();
    printf("\n");

    printf("--> Evento: Cartao inserido\n");
    fsm_handle_event(&fsm, FSM_EVENT_CARD_INSERT);
    sleep(1);

    printf("--> Evento: PIN OK\n");
    fsm_handle_event(&fsm, FSM_EVENT_PIN_OK);
    
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

    printf("--> Evento: Mensagem enviada com sucesso\n");
    fsm_handle_event(&fsm, FSM_EVENT_SEND_SUCCESS);
    sleep(2); 

    printf("--> Evento: Resposta APROVADA recebida\n");
    fsm_handle_event(&fsm, FSM_EVENT_RESPONSE_RECEIVED_APPROVED);
    
    if (fsm.current_state == FSM_STATE_APPROVED) {
        printf("    Gravando log da transacao...\n");
        char masked_pan[20];
        mask_pan(tx_message.fields[2], masked_pan);
        
        char log_entry[100];
        sprintf(log_entry, "STAN:%06u|PAN:%s|VALOR:%s|RESP:APROVADA", fsm.stan, masked_pan, tx_message.fields[4]);
        
        write_secure_log(0, log_entry);
        printf("    Log gravado com sucesso!\n");
    }
    sleep(1);
    
    printf("--> Evento: Transacao finalizada\n");
    fsm_handle_event(&fsm, FSM_EVENT_FINISH_TRANSACTION);
    iso8583_free_message(&tx_message);
    printf("\n--- DEMONSTRACAO CONCLUIDA ---\n");
}

void run_list_mode() {
    printf("--- MODO LISTAGEM DE LOGS ---\n\n");
    
    uint8_t read_buffer[256] = {0};
    uint8_t log_len = 0;

    if (flash_read(0, &log_len, 1) != 0) {
        printf("Nenhum log encontrado.\n");
        printf("\n--- LISTAGEM CONCLUIDA ---\n");
        return;
    }

    xor_cipher(&log_len, 1);
    
    if (log_len > 0 && log_len < sizeof(read_buffer) && flash_read(1, read_buffer, log_len) == 0) {
        xor_cipher(read_buffer, log_len);
        
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