#ifndef SECURE_LOG_H
#define SECURE_LOG_H

#include <stdint.h>

/**
 * @brief Mascara um número de PAN (Primary Account Number).
 * @param pan O PAN original.
 * @param masked_pan O buffer onde o PAN mascarado será salvo. Deve ter tamanho suficiente.
 */
void mask_pan(const char *pan, char *masked_pan);

/**
 * @brief Cifra ou decifra um bloco de dados usando um XOR simples.
 * @param data Ponteiro para os dados.
 * @param size Tamanho dos dados em bytes.
 */
void xor_cipher(uint8_t *data, uint32_t size);

/**
 * @brief Monta, cifra e grava o log de uma transação na memória flash.
 * @param log_address Endereço na flash para gravar o log.
 * @param transaction_data String com os dados da transação.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int write_secure_log(uint32_t log_address, const char *transaction_data);

#endif // SECURE_LOG_H