#include "luhn.h"
#include <ctype.h>
#include <string.h>


bool luhn_validate(const char *pan)
{
    if (pan == NULL)
    {
        return false;
    }

    int sum = 0;
    int n_digits = 0;
    int len = strlen(pan);

    // Itera sobre a string da direita para a esquerda
    for (int i = len - 1; i >= 0; i--)
    {
        // Ignora caracteres que não são dígitos
        if (!isdigit(pan[i]))
        {
            continue;
        }

        int digit = pan[i] - '0';

        // Dobra o valor de cada segundo dígito (da direita para a esquerda)
        if (n_digits % 2 == 1)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit -= 9; // Equivalente a somar os dois dígitos
            }
        }

        sum += digit;
        n_digits++;
    }

    return (n_digits > 1) && (sum % 10 == 0);
}