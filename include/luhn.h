#ifndef LUHN_H
#define LUHN_H

// Inclui a biblioteca para usar 'bool'
#include <stdbool.h>

/**
 * @brief Valida uma string de número de cartão usando o algoritmo de Luhn.
 *
 * @param pan A string contendo o número do cartão a ser validado.
 * @return true se o número do cartão for válido, false caso contrário.
 */
bool luhn_validate(const char *pan);

#endif // LUHN_H