#include "iso8583.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void run_iso8583_tests()
{
    printf("Executando testes do Montador ISO 8583...\n");

    Iso8583Message msg;
    iso8583_init_message(&msg);

    // Configurando uma mensagem de venda (MTI 0200)
    strcpy(msg.mti, "0200");
    iso8583_set_field(&msg, 3, "000001");             // Processing Code
    iso8583_set_field(&msg, 41, "TERM0001");          // Terminal ID
    iso8583_set_field(&msg, 128, "AABBCCDD11223344"); // MAC

    // Buffer para a mensagem empacotada
    uint8_t buffer[512] = {0};
    int len = iso8583_pack(&msg, buffer);

    // O resultado esperado é: MTI + BITMAP_HEX + Campo3 + Campo41 + Campo128
    const char *expected_message = "0200"
                                   "C0000000000000000000020000000000" // Bitmap em Hex
                                   "000001"                           // Campo 3
                                   "TERM0001"                         // Campo 41
                                   "AABBCCDD11223344";                // Campo 128

    assert(len == strlen(expected_message));
    assert(strncmp((char *)buffer, expected_message, len) == 0);

    printf("  [PASSOU] Empacotamento de mensagem 0200\n");

    iso8583_free_message(&msg);
    printf("Todos os testes de ISO 8583 passaram com sucesso!\n\n");
}

int main()
{
    run_iso8583_tests();
    return 0;
}