#include "iso8583.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para gerar o bitmap
static void generate_bitmap(const Iso8583Message *msg, uint8_t *bitmap)
{
    memset(bitmap, 0, 16); // Limpa o bitmap (primário e secundário)

    // Define o bit 1 para indicar a presença do bitmap secundário,
    // já que o campo 128 é obrigatório neste desafio.
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

    // 1. MTI (4 bytes)
    memcpy(buffer + offset, msg->mti, 4);
    offset += 4;

    // 2. Bitmap (16 bytes, pois o campo 128 está presente)
    uint8_t bitmap[16];
    generate_bitmap(msg, bitmap);
    // Converte o bitmap binário para uma string hexadecimal de 32 caracteres
    for (int i = 0; i < 16; i++)
    {
        sprintf((char *)(buffer + offset + (i * 2)), "%02X", bitmap[i]);
    }
    offset += 32;

    // 3. Campos de Dados
    for (int i = 2; i <= MAX_FIELDS; i++)
    {
        if (msg->is_field_set[i])
        {
            const char *value = msg->fields[i];
            int len = strlen(value);

            // Adiciona o campo ao buffer.
            // Nota: Uma implementação real lidaria com formatos LLVAR, LLLVAR, etc.
            // Para este desafio, vamos simplesmente concatenar os valores.
            memcpy(buffer + offset, value, len);
            offset += len;
        }
    }
    return offset; // Retorna o tamanho total da mensagem
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