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

int write_secure_log(uint32_t log_address, const char *transaction_data) {
    uint32_t data_len = strlen(transaction_data);
    if (data_len > 254) { // Garante que o tamanho cabe em um byte
        return -1; // Log muito grande
    }

    // Buffer para [tamanho] + [dados]
    uint8_t buffer[256];
    
    // O primeiro byte do buffer será o tamanho do log
    buffer[0] = (uint8_t)data_len;
    
    // Copia os dados do log para o resto do buffer
    memcpy(buffer + 1, transaction_data, data_len);

    // Cifra o buffer completo (tamanho + dados)
    xor_cipher(buffer, data_len + 1);

    // Escreve na memória flash
    return flash_write(log_address, buffer, data_len + 1);
}