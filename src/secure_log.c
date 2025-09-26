#include "secure_log.h"
#include "flash_mem.h" // Nosso módulo de flash
#include <stdio.h>
#include <string.h>

// Chave simples para a cifra XOR. Em um sistema real, seria mais complexa.
#define XOR_KEY 0xA5

void mask_pan(const char *pan, char *masked_pan)
{
    int len = strlen(pan);
    if (len < 10)
    { // Se for muito curto para mascarar
        strcpy(masked_pan, pan);
        return;
    }
    // Copia os primeiros 6 dígitos
    strncpy(masked_pan, pan, 6);
    // Adiciona os asteriscos
    for (int i = 6; i < len - 4; i++)
    {
        masked_pan[i] = '*';
    }
    // Copia os últimos 4 dígitos
    strcpy(&masked_pan[len - 4], &pan[len - 4]);
}

void xor_cipher(uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        data[i] = data[i] ^ XOR_KEY;
    }
}

int write_secure_log(uint32_t log_address, const char *transaction_data)
{
    // Cria uma cópia local para poder modificar (cifrar)
    char buffer[256]; // Buffer para o log
    strncpy(buffer, transaction_data, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; // Garante terminação nula

    uint32_t data_len = strlen(buffer);

    // Cifra o buffer
    xor_cipher((uint8_t *)buffer, data_len);

    // Escreve na memória flash
    return flash_write(log_address, (const uint8_t *)buffer, data_len);
}