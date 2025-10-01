#include "secure_log.h"
#include "flash_mem.h"
#include <stdio.h>
#include <string.h>

#define XOR_KEY 0xA5

void mask_pan(const char *pan, char *masked_pan)
{
    int len = strlen(pan);
    if (len < 10)
    {
        strcpy(masked_pan, pan);
        return;
    }
    strncpy(masked_pan, pan, 6);
    for (int i = 6; i < len - 4; i++)
    {
        masked_pan[i] = '*';
    }
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
    if (data_len > 254) {
        return -1;
    }

    uint8_t buffer[256];
    
    buffer[0] = (uint8_t)data_len;
    
    memcpy(buffer + 1, transaction_data, data_len);

    xor_cipher(buffer, data_len + 1);

    return flash_write(log_address, buffer, data_len + 1);
}