#include "flash_mem.h"
#include "secure_log.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void run_persistence_tests()
{
    printf("Executando testes de Persistencia Segura...\n");

    // Teste 1: Flash Write/Read
    flash_init();
    char *test_data = "Hello Flash!";
    int len = strlen(test_data);
    assert(flash_write(0, (uint8_t *)test_data, len) == 0);

    char read_buffer[100] = {0};
    assert(flash_read(0, (uint8_t *)read_buffer, len) == 0);
    assert(strcmp(test_data, read_buffer) == 0);
    printf("  [PASSOU] Flash Write e Read\n");

    // Teste 2: Mascaramento de PAN
    char masked_buffer[20];
    mask_pan("1234567890123456", masked_buffer);
    assert(strcmp(masked_buffer, "123456******3456") == 0);
    printf("  [PASSOU] Mascaramento de PAN\n");

    // Teste 3: Cifra XOR (ida e volta)
    char cipher_data[] = "Dados Secretos";
    int cipher_len = strlen(cipher_data);
    xor_cipher((uint8_t *)cipher_data, cipher_len);     // Cifra
    assert(strcmp(cipher_data, "Dados Secretos") != 0); // Deve ser diferente
    xor_cipher((uint8_t *)cipher_data, cipher_len);     // Decifra
    assert(strcmp(cipher_data, "Dados Secretos") == 0); // Deve ser igual de novo
    printf("  [PASSOU] Cifra XOR\n");

    // Teste 4: Gravação e Leitura de Log Seguro
    char pan[] = "49927398716";
    char masked_pan[20];
    mask_pan(pan, masked_pan);

    char log_entry[100];
    sprintf(log_entry, "VENDA APROVADA: %s", masked_pan);

    assert(write_secure_log(100, log_entry) == 0);

    char encrypted_log[100] = {0};
    assert(flash_read(100, (uint8_t *)encrypted_log, strlen(log_entry)) == 0);

    // Decifra para verificar
    xor_cipher((uint8_t *)encrypted_log, strlen(log_entry));
    assert(strcmp(encrypted_log, log_entry) == 0);
    printf("  [PASSOU] Gravacao e Leitura de Log Seguro\n");

    printf("Todos os testes de persistencia passaram com sucesso!\n\n");
}

int main()
{
    run_persistence_tests();
    return 0;
}