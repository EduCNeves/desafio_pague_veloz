#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "iso8583.h"

void test_message_initialization() {
    printf("Testando inicializacao de mensagem...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    assert(strlen(msg.mti) == 0);
    
    for (int i = 0; i <= MAX_FIELDS; i++) {
        assert(msg.is_field_set[i] == false);
        assert(msg.fields[i] == NULL);
    }
    
    printf("  OK - Mensagem inicializada corretamente\n");
}

void test_field_setting() {
    printf("\nTestando definicao de campos...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    // Defino alguns campos básicos
    assert(iso8583_set_field(&msg, 2, "1234567890123456") == 0);
    assert(iso8583_set_field(&msg, 4, "000000050000") == 0);
    assert(iso8583_set_field(&msg, 41, "TERM0001") == 0);
    
    // Verifico se foram armazenados
    assert(msg.is_field_set[2] == true);
    assert(strcmp(msg.fields[2], "1234567890123456") == 0);
    printf("  OK - Campo 2 (PAN) definido\n");
    
    assert(msg.is_field_set[4] == true);
    assert(strcmp(msg.fields[4], "000000050000") == 0);
    printf("  OK - Campo 4 (Valor) definido\n");
    
    assert(msg.is_field_set[41] == true);
    assert(strcmp(msg.fields[41], "TERM0001") == 0);
    printf("  OK - Campo 41 (Terminal ID) definido\n");
    
    // Campos não definidos devem continuar false
    assert(msg.is_field_set[3] == false);
    assert(msg.is_field_set[7] == false);
    
    iso8583_free_message(&msg);
}

void test_invalid_fields() {
    printf("\nTestando validacao de campos invalidos...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    assert(iso8583_set_field(&msg, 0, "teste") == -1);
    printf("  OK - Campo 0 rejeitado\n");
    
    assert(iso8583_set_field(&msg, 1, "teste") == -1);
    printf("  OK - Campo 1 rejeitado\n");
    
    assert(iso8583_set_field(&msg, MAX_FIELDS + 1, "teste") == -1);
    printf("  OK - Campo acima do limite rejeitado\n");
    
    iso8583_free_message(&msg);
}

void test_full_message_packing() {
    printf("\nTestando empacotamento de mensagem completa...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    strcpy(msg.mti, "0200");
    
    // Defino todos os campos requeridos
    iso8583_set_field(&msg, 2, "4532015112830366");
    iso8583_set_field(&msg, 3, "000001");
    iso8583_set_field(&msg, 4, "000000100000");
    iso8583_set_field(&msg, 7, "0930120000");
    iso8583_set_field(&msg, 11, "123456");
    iso8583_set_field(&msg, 12, "120000");
    iso8583_set_field(&msg, 37, "REF000123456");
    iso8583_set_field(&msg, 41, "TERM9999");
    iso8583_set_field(&msg, 49, "986");
    iso8583_set_field(&msg, 128, "0123456789ABCDEF");
    
    uint8_t buffer[512] = {0};
    int packed_len = iso8583_pack(&msg, buffer);
    
    assert(packed_len > 0);
    printf("  OK - Mensagem empacotada com %d bytes\n", packed_len);
    
    assert(strncmp((char*)buffer, "0200", 4) == 0);
    printf("  OK - MTI correto no inicio\n");
    
    assert(packed_len >= 36);
    printf("  OK - Tamanho minimo valido\n");
    
    iso8583_free_message(&msg);
}

void test_pack_unpack_roundtrip() {
    printf("\nTestando ciclo completo (pack + unpack)...\n");
    
    Iso8583Message original;
    iso8583_init_message(&original);
    
    strcpy(original.mti, "0210");
    iso8583_set_field(&original, 2, "5555555555554444");
    iso8583_set_field(&original, 3, "000002");
    iso8583_set_field(&original, 4, "000000025000");
    iso8583_set_field(&original, 7, "0930130000");
    iso8583_set_field(&original, 11, "654321");
    iso8583_set_field(&original, 12, "130000");
    iso8583_set_field(&original, 37, "REF999888777");
    iso8583_set_field(&original, 41, "TERM1234");
    iso8583_set_field(&original, 49, "986");
    iso8583_set_field(&original, 128, "FEDCBA9876543210");
    
    // Empacoto
    uint8_t buffer[512] = {0};
    int packed_len = iso8583_pack(&original, buffer);
    assert(packed_len > 0);
    printf("  OK - Mensagem original empacotada\n");
    
    Iso8583Message unpacked;
    int unpacked_len = iso8583_unpack(buffer, packed_len, &unpacked);
    assert(unpacked_len > 0);
    printf("  OK - Mensagem desempacotada\n");
    
    assert(strcmp(original.mti, unpacked.mti) == 0);
    printf("  OK - MTI preservado\n");
    
    int fields_to_check[] = {2, 3, 4, 7, 11, 12, 37, 41, 49, 128};
    for (int i = 0; i < 10; i++) {
        int field = fields_to_check[i];
        assert(unpacked.is_field_set[field] == true);
        assert(strcmp(original.fields[field], unpacked.fields[field]) == 0);
    }
    printf("  OK - Todos os campos preservados\n");
    
    iso8583_free_message(&original);
    iso8583_free_message(&unpacked);
}

void test_bitmap_generation() {
    printf("\nTestando geracao de bitmaps...\n");
    
    Iso8583Message msg1, msg2;
    iso8583_init_message(&msg1);
    iso8583_init_message(&msg2);
    
    strcpy(msg1.mti, "0200");
    strcpy(msg2.mti, "0200");
    
    iso8583_set_field(&msg1, 2, "1234567890123456");
    iso8583_set_field(&msg1, 128, "0000000000000000");
    
    iso8583_set_field(&msg2, 2, "1234567890123456");
    iso8583_set_field(&msg2, 3, "000001");
    iso8583_set_field(&msg2, 4, "000000100000");
    iso8583_set_field(&msg2, 128, "0000000000000000");
    
    uint8_t buffer1[512] = {0};
    uint8_t buffer2[512] = {0};
    
    iso8583_pack(&msg1, buffer1);
    iso8583_pack(&msg2, buffer2);
    
    int bitmap_differs = memcmp(buffer1 + 4, buffer2 + 4, 32);
    assert(bitmap_differs != 0);
    printf("  OK - Bitmaps diferentes para campos diferentes\n");
    
    iso8583_free_message(&msg1);
    iso8583_free_message(&msg2);
}

void test_field_overwrite() {
    printf("\nTestando sobrescrita de campos...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    iso8583_set_field(&msg, 2, "1111111111111111");
    assert(strcmp(msg.fields[2], "1111111111111111") == 0);
    printf("  OK - Valor inicial definido\n");
    
    iso8583_set_field(&msg, 2, "9999999999999999");
    assert(strcmp(msg.fields[2], "9999999999999999") == 0);
    printf("  OK - Campo sobrescrito com sucesso\n");
    
    iso8583_free_message(&msg);
}

void test_sparse_fields() {
    printf("\nTestando mensagem com campos esparsos...\n");
    
    Iso8583Message msg;
    iso8583_init_message(&msg);
    
    strcpy(msg.mti, "0200");
    
    iso8583_set_field(&msg, 2, "1234567890123456");
    iso8583_set_field(&msg, 11, "123456");
    iso8583_set_field(&msg, 49, "986");
    iso8583_set_field(&msg, 128, "0123456789ABCDEF");
    
    uint8_t buffer[512] = {0};
    int packed_len = iso8583_pack(&msg, buffer);
    assert(packed_len > 0);
    
    Iso8583Message unpacked;
    iso8583_unpack(buffer, packed_len, &unpacked);
    
    assert(unpacked.is_field_set[2] == true);
    assert(unpacked.is_field_set[11] == true);
    assert(unpacked.is_field_set[49] == true);
    assert(unpacked.is_field_set[128] == true);
    
    assert(unpacked.is_field_set[3] == false);
    assert(unpacked.is_field_set[4] == false);
    assert(unpacked.is_field_set[7] == false);
    
    printf("  OK - Campos esparsos tratados corretamente\n");
    
    iso8583_free_message(&msg);
    iso8583_free_message(&unpacked);
}

int main() {
    printf("=============================================\n");
    printf("  SUITE DE TESTES - MENSAGENS ISO 8583\n");
    printf("=============================================\n\n");
    
    test_message_initialization();
    test_field_setting();
    test_invalid_fields();
    test_full_message_packing();
    test_pack_unpack_roundtrip();
    test_bitmap_generation();
    test_field_overwrite();
    test_sparse_fields();
    
    printf("\n=============================================\n");
    printf("  TODOS OS TESTES PASSARAM! ✓\n");
    printf("=============================================\n");
    
    return 0;
}