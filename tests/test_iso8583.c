#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "iso8583.h"

void run_iso8583_roundtrip_test() {
    printf("Executando teste de 'Ida e Volta' do ISO 8583...\n");

    // 1. Crie e popule a mensagem original
    Iso8583Message msg_original;
    iso8583_init_message(&msg_original);

    strcpy(msg_original.mti, "0200");
    iso8583_set_field(&msg_original, 2, "1234567890123456");
    iso8583_set_field(&msg_original, 3, "000001");
    iso8583_set_field(&msg_original, 4, "000000019990");
    iso8583_set_field(&msg_original, 7, "0929105732");
    iso8583_set_field(&msg_original, 11, "123456");
    iso8583_set_field(&msg_original, 12, "105732");
    iso8583_set_field(&msg_original, 37, "REF123456789");
    iso8583_set_field(&msg_original, 41, "TERM0001");
    iso8583_set_field(&msg_original, 49, "986");
    iso8583_set_field(&msg_original, 128, "AABBCCDD11223344");

    // 2. Empacote a mensagem original em um buffer
    uint8_t buffer[512] = {0};
    int packed_len = iso8583_pack(&msg_original, buffer);
    assert(packed_len > 0);
    printf("  [INFO] Mensagem empacotada com sucesso (tamanho: %d bytes).\n", packed_len);

    // 3. Analise o buffer para preencher uma nova mensagem
    Iso8583Message msg_desempacotada;
    int unpacked_len = iso8583_unpack(buffer, packed_len, &msg_desempacotada);
    assert(unpacked_len > 0);
    printf("  [INFO] Mensagem desempacotada com sucesso.\n");
    
    // 4. Compare a mensagem original com a desempacotada
    assert(strcmp(msg_original.mti, msg_desempacotada.mti) == 0);
    for (int i = 2; i <= 128; i++) {
        // Compara apenas os campos que foram definidos na mensagem original
        if (msg_original.is_field_set[i]) {
            assert(msg_desempacotada.is_field_set[i] == true);
            assert(strcmp(msg_original.fields[i], msg_desempacotada.fields[i]) == 0);
        }
    }
    printf("  [PASSOU] Mensagem original e desempacotada sao identicas.\n");

    // Libera a memória alocada para ambas as mensagens
    iso8583_free_message(&msg_original);
    iso8583_free_message(&msg_desempacotada);

    printf("Todos os testes de ISO 8583 (Ida e Volta) passaram com sucesso!\n\n");
}

int main() {
    run_iso8583_roundtrip_test();
    return 0;
}