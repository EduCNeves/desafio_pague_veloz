#include "iso8583.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tabela com os tamanhos fixos dos campos
static const int FIELD_LENGTHS[MAX_FIELDS + 1] = {
    [2] = 16, [3] = 6, [4] = 12, [7] = 10, [11] = 6, [12] = 6, 
    [37] = 12, [41] = 8, [49] = 3, [128] = 16
};

static void generate_bitmap(const Iso8583Message *msg, uint8_t *bitmap)
{
    memset(bitmap, 0, 16);

    bitmap[0] |= 0x80;

    for (int i = 2; i <= MAX_FIELDS; i++)
    {
        if (msg->is_field_set[i])
        {
            int byte_index = (i - 1) / 8;
            int bit_index = (i - 1) % 8;
            bitmap[byte_index] |= (1 << (7 - bit_index));
        }
    }
}

void iso8583_init_message(Iso8583Message *msg)
{
    memset(msg->mti, 0, sizeof(msg->mti));
    for (int i = 0; i <= MAX_FIELDS; i++)
    {
        msg->fields[i] = NULL;
        msg->is_field_set[i] = false;
    }
}

int iso8583_set_field(Iso8583Message *msg, int field_index, const char *value)
{
    if (field_index < 2 || field_index > MAX_FIELDS)
    {
        return -1; // Campo inválido
    }
    // Libera o valor antigo se já existir
    if (msg->fields[field_index] != NULL)
    {
        free(msg->fields[field_index]);
    }
    // Aloca memória para o novo valor e o copia
    msg->fields[field_index] = strdup(value);
    if (msg->fields[field_index] == NULL)
    {
        return -1; // Falha na alocação de memória
    }
    msg->is_field_set[field_index] = true;
    return 0;
}

int iso8583_pack(const Iso8583Message *msg, uint8_t *buffer)
{
    int offset = 0;

    memcpy(buffer + offset, msg->mti, 4);
    offset += 4;

    uint8_t bitmap[16];
    generate_bitmap(msg, bitmap);
    for (int i = 0; i < 16; i++)
    {
        sprintf((char *)(buffer + offset + (i * 2)), "%02X", bitmap[i]);
    }
    offset += 32;

    for (int i = 2; i <= MAX_FIELDS; i++)
    {
        if (msg->is_field_set[i])
        {
            const char *value = msg->fields[i];
            int len = strlen(value);

            memcpy(buffer + offset, value, len);
            offset += len;
        }
    }
    return offset;
}

void iso8583_free_message(Iso8583Message *msg)
{
    for (int i = 0; i <= MAX_FIELDS; i++)
    {
        if (msg->fields[i] != NULL)
        {
            free(msg->fields[i]);
            msg->fields[i] = NULL;
        }
    }
}

static bool is_bit_set(const uint8_t* bitmap, int field_index) {
    int byte_index = (field_index - 1) / 8;
    int bit_index = (field_index - 1) % 8;
    return (bitmap[byte_index] & (1 << (7 - bit_index))) != 0;
}

static void hex_to_bytes(const char* hex_str, uint8_t* byte_array, int hex_len) {
    for (int i = 0; i < hex_len / 2; i++) {
        sscanf(hex_str + 2 * i, "%2hhx", &byte_array[i]);
    }
}

int iso8583_unpack(const uint8_t *buffer, int len, Iso8583Message *msg) {
    iso8583_init_message(msg);
    int offset = 0;

    strncpy(msg->mti, (const char*)buffer, 4);
    msg->mti[4] = '\0';
    offset += 4;

    uint8_t bitmap[16];
    hex_to_bytes((const char*)(buffer + offset), bitmap, 32);
    offset += 32;

    for (int i = 2; i <= MAX_FIELDS; i++) {
        if (is_bit_set(bitmap, i)) {
            int field_len = FIELD_LENGTHS[i];
            if (offset + field_len > len) return -1; // Erro, buffer curto

            char fieldValue[512] = {0};
            strncpy(fieldValue, (const char*)(buffer + offset), field_len);
            iso8583_set_field(msg, i, fieldValue);
            
            offset += field_len;
        }
    }
    return offset;
}