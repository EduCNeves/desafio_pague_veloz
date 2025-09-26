#ifndef ISO8583_H
#define ISO8583_H

#include <stdbool.h>
#include <stdint.h>


// Define o número máximo de campos no padrão ISO 8583
#define MAX_FIELDS 128

// Estrutura para armazenar os dados de uma mensagem ISO 8583
typedef struct
{
    char mti[5]; // Message Type Indicator (ex: "0200")

    // Armazena os campos de dados. Usamos ponteiros para flexibilidade.
    // Em uma implementação real, poderiam ser de tamanho fixo para evitar alocação dinâmica.
    char *fields[MAX_FIELDS + 1];

    // Controle para saber quais campos foram definidos
    bool is_field_set[MAX_FIELDS + 1];

} Iso8583Message;

/**
 * @brief Inicializa uma estrutura de mensagem ISO 8583, limpando todos os campos.
 * @param msg Ponteiro para a mensagem a ser inicializada.
 */
void iso8583_init_message(Iso8583Message *msg);

/**
 * @brief Define o valor de um campo de dados na mensagem.
 * @param msg Ponteiro para a mensagem.
 * @param field_index O número do campo (de 2 a 128).
 * @param value O valor a ser atribuído ao campo.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int iso8583_set_field(Iso8583Message *msg, int field_index, const char *value);

/**
 * @brief Empacota a estrutura da mensagem em um buffer de bytes formatado.
 * @param msg Ponteiro para a mensagem a ser empacotada.
 * @param buffer O buffer de saída onde a mensagem formatada será escrita.
 * @return O comprimento total da mensagem empacotada, ou -1 em caso de erro.
 */
int iso8583_pack(const Iso8583Message *msg, uint8_t *buffer);

/**
 * @brief Libera a memória alocada para os campos da mensagem.
 * @param msg Ponteiro para a mensagem.
 */
void iso8583_free_message(Iso8583Message *msg);

#endif // ISO8583_H