#include "flash_mem.h"
#include "secure_log.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_flash_initialization() {
    printf("Testando inicializacao da memoria flash...\n");
    
    flash_init();
    
    uint8_t buffer[10] = {0xFF};
    flash_read(0, buffer, 10);
    
    for (int i = 0; i < 10; i++) {
        assert(buffer[i] == 0x00);
    }
    
    printf("  OK - Flash inicializada com zeros\n");
}

void test_basic_write_read() {
    printf("\nTestando escrita e leitura basica...\n");
    
    flash_init();
    
    char* test_string = "PagueVeloz";
    int len = strlen(test_string);
    
    int result = flash_write(0, (uint8_t*)test_string, len);
    assert(result == 0);
    printf("  OK - Dados escritos\n");
    
    char read_buffer[20] = {0};
    result = flash_read(0, (uint8_t*)read_buffer, len);
    assert(result == 0);
    assert(strcmp(test_string, read_buffer) == 0);
    printf("  OK - Dados lidos corretamente\n");
}

void test_multiple_addresses() {
    printf("\nTestando escrita em multiplos enderecos...\n");
    
    flash_init();
    
    char* str1 = "POS";
    char* str2 = "Terminal";
    char* str3 = "Transacao";
    
    // Escrevo em três endereços diferentes
    flash_write(0, (uint8_t*)str1, strlen(str1));
    flash_write(100, (uint8_t*)str2, strlen(str2));
    flash_write(500, (uint8_t*)str3, strlen(str3));
    
    // Leio cada um deles
    char buf1[10] = {0}, buf2[20] = {0}, buf3[20] = {0};
    flash_read(0, (uint8_t*)buf1, strlen(str1));
    flash_read(100, (uint8_t*)buf2, strlen(str2));
    flash_read(500, (uint8_t*)buf3, strlen(str3));
    
    assert(strcmp(str1, buf1) == 0);
    assert(strcmp(str2, buf2) == 0);
    assert(strcmp(str3, buf3) == 0);
    
    printf("  OK - Tres enderecos diferentes funcionando\n");
}

void test_flash_boundaries() {
    printf("\nTestando limites da memoria flash...\n");
    
    flash_init();
    
    // Testo escrita no limite
    uint8_t data = 0xAA;
    int result = flash_write(FLASH_SIZE - 1, &data, 1);
    assert(result == 0);
    printf("  OK - Escrita no ultimo byte aceita\n");
    
    // Testo escrita além do limite
    result = flash_write(FLASH_SIZE, &data, 1);
    assert(result == -1);
    printf("  OK - Escrita alem do limite rejeitada\n");
    
    // Testo leitura além do limite
    result = flash_read(FLASH_SIZE, &data, 1);
    assert(result == -1);
    printf("  OK - Leitura alem do limite rejeitada\n");
    
    // Testo escrita que ultrapassa o limite
    uint8_t big_buffer[100];
    result = flash_write(FLASH_SIZE - 50, big_buffer, 100);
    assert(result == -1);
    printf("  OK - Escrita que ultrapassa limite rejeitada\n");
}

// Agora testo o mascaramento de PAN
void test_pan_masking() {
    printf("\nTestando mascaramento de PAN...\n");
    
    char masked[20];
    
    // PAN padrão de 16 dígitos
    mask_pan("1234567890123456", masked);
    assert(strcmp(masked, "123456******3456") == 0);
    printf("  OK - PAN de 16 digitos mascarado: %s\n", masked);
    
    // PAN de 15 dígitos (Amex)
    mask_pan("374245455400126", masked);
    assert(strcmp(masked, "374245*****0126") == 0);
    printf("  OK - PAN de 15 digitos mascarado: %s\n", masked);
    
    // PAN mais longo
    mask_pan("12345678901234567890", masked);
    assert(strcmp(masked, "123456**********7890") == 0);
    printf("  OK - PAN longo mascarado: %s\n", masked);
    
    // PAN curto demais para mascarar
    mask_pan("123456789", masked);
    assert(strcmp(masked, "123456789") == 0);
    printf("  OK - PAN curto mantido sem mascara\n");
}

// Testo a cifra XOR
void test_xor_cipher() {
    printf("\nTestando cifra XOR...\n");
    
    char original[] = "Dados Confidenciais";
    char copy[50];
    strcpy(copy, original);
    
    int len = strlen(copy);
    
    // Cifro
    xor_cipher((uint8_t*)copy, len);
    assert(strcmp(copy, original) != 0);
    printf("  OK - Dados cifrados (diferentes do original)\n");
    
    // Decifro
    xor_cipher((uint8_t*)copy, len);
    assert(strcmp(copy, original) == 0);
    printf("  OK - Dados decifrados (iguais ao original)\n");
}

// Verifico se a cifra realmente altera os bytes
void test_xor_actually_encrypts() {
    printf("\nTestando se XOR realmente cifra...\n");
    
    uint8_t data[] = {0x00, 0x11, 0x22, 0x33, 0x44};
    uint8_t original[5];
    memcpy(original, data, 5);
    
    xor_cipher(data, 5);
    
    int changes = 0;
    for (int i = 0; i < 5; i++) {
        if (data[i] != original[i]) changes++;
    }
    
    assert(changes > 0);
    printf("  OK - %d bytes foram alterados pela cifra\n", changes);
}

// Testo gravação de log seguro completo
void test_secure_log_write() {
    printf("\nTestando gravacao de log seguro...\n");
    
    flash_init();
    
    char* log_data = "VENDA|STAN:123456|VALOR:100.00|APROVADO";
    
    // Gravo o log cifrado
    int result = write_secure_log(0, log_data);
    assert(result == 0);
    printf("  OK - Log gravado com sucesso\n");
    
    // Leio o tamanho (primeiro byte cifrado)
    uint8_t size_byte;
    flash_read(0, &size_byte, 1);
    xor_cipher(&size_byte, 1);
    
    assert(size_byte == strlen(log_data));
    printf("  OK - Tamanho do log correto: %d bytes\n", size_byte);
    
    // Leio os dados cifrados
    uint8_t encrypted_data[100];
    flash_read(1, encrypted_data, size_byte);
    
    // Decifro
    xor_cipher(encrypted_data, size_byte);
    encrypted_data[size_byte] = '\0';
    
    assert(strcmp((char*)encrypted_data, log_data) == 0);
    printf("  OK - Dados do log recuperados corretamente\n");
}

// Gravo múltiplos logs em sequência
void test_multiple_logs() {
    printf("\nTestando multiplos logs...\n");
    
    flash_init();
    
    char* log1 = "LOG1:Transacao iniciada";
    char* log2 = "LOG2:Cartao lido";
    char* log3 = "LOG3:PIN validado";
    
    // Gravo três logs em endereços diferentes
    write_secure_log(0, log1);
    write_secure_log(100, log2);
    write_secure_log(200, log3);
    
    printf("  OK - Tres logs gravados\n");
    
    // Leio e verifico cada um
    uint8_t size, buffer[100];
    
    // Log 1
    flash_read(0, &size, 1);
    xor_cipher(&size, 1);
    flash_read(1, buffer, size);
    xor_cipher(buffer, size);
    buffer[size] = '\0';
    assert(strcmp((char*)buffer, log1) == 0);
    printf("  OK - Log 1 recuperado\n");
    
    // Log 2
    flash_read(100, &size, 1);
    xor_cipher(&size, 1);
    flash_read(101, buffer, size);
    xor_cipher(buffer, size);
    buffer[size] = '\0';
    assert(strcmp((char*)buffer, log2) == 0);
    printf("  OK - Log 2 recuperado\n");
    
    // Log 3
    flash_read(200, &size, 1);
    xor_cipher(&size, 1);
    flash_read(201, buffer, size);
    xor_cipher(buffer, size);
    buffer[size] = '\0';
    assert(strcmp((char*)buffer, log3) == 0);
    printf("  OK - Log 3 recuperado\n");
}

// Testo log com PAN mascarado
void test_log_with_masked_pan() {
    printf("\nTestando log com PAN mascarado...\n");
    
    flash_init();
    
    char pan[] = "4532015112830366";
    char masked_pan[20];
    mask_pan(pan, masked_pan);
    
    char log_entry[100];
    sprintf(log_entry, "VENDA|PAN:%s|APROVADA", masked_pan);
    
    // O log não deve conter o PAN completo
    assert(strstr(log_entry, "4532015112830366") == NULL);
    assert(strstr(log_entry, "******") != NULL);
    printf("  OK - PAN mascarado no log\n");
    
    write_secure_log(0, log_entry);
    printf("  OK - Log com PAN mascarado gravado\n");
}

// Verifico tamanho máximo de log
void test_log_size_limit() {
    printf("\nTestando limite de tamanho do log...\n");
    
    flash_init();
    
    char huge_log[300];
    memset(huge_log, 'X', 299);
    huge_log[299] = '\0';
    
    int result = write_secure_log(0, huge_log);
    assert(result == -1);
    printf("  OK - Log muito grande rejeitado\n");
    
    char max_log[255];
    memset(max_log, 'Y', 254);
    max_log[254] = '\0';
    
    result = write_secure_log(0, max_log);
    assert(result == 0);
    printf("  OK - Log no tamanho maximo aceito\n");
}

void test_realistic_transaction_log() {
    printf("\nTestando log de transacao realista...\n");
    
    flash_init();
    
    char pan[] = "5555555555554444";
    char masked_pan[20];
    mask_pan(pan, masked_pan);
    
    char log[150];
    sprintf(log, "STAN:000123|PAN:%s|VALOR:000000015000|TERMINAL:TERM0001|RESP:00", 
            masked_pan);
    
    write_secure_log(0, log);
    printf("  OK - Log realista gravado\n");
    
    uint8_t size, buffer[150];
    flash_read(0, &size, 1);
    xor_cipher(&size, 1);
    flash_read(1, buffer, size);
    xor_cipher(buffer, size);
    buffer[size] = '\0';
    
    assert(strstr((char*)buffer, "STAN:000123") != NULL);
    assert(strstr((char*)buffer, "555555") != NULL);
    assert(strstr((char*)buffer, "******") != NULL);
    assert(strstr((char*)buffer, "4444") != NULL);
    
    printf("  OK - Log realista recuperado e validado\n");
}

int main() {
    printf("===============================================\n");
    printf("  SUITE DE TESTES - PERSISTENCIA SEGURA\n");
    printf("===============================================\n\n");
    
    test_flash_initialization();
    test_basic_write_read();
    test_multiple_addresses();
    test_flash_boundaries();
    test_pan_masking();
    test_xor_cipher();
    test_xor_actually_encrypts();
    test_secure_log_write();
    test_multiple_logs();
    test_log_with_masked_pan();
    test_log_size_limit();
    test_realistic_transaction_log();
    
    printf("\n===============================================\n");
    printf("  TODOS OS TESTES PASSARAM! ✓\n");
    printf("===============================================\n");
    
    return 0;
}