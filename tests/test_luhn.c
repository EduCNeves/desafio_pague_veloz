#include "luhn.h"
#include <assert.h>
#include <stdio.h>


void run_luhn_tests()
{
    printf("Executando testes para o Algoritmo de Luhn...\n");

    assert(luhn_validate("49927398716") == true);
    printf("  [PASSOU] Numero valido (49927398716)\n");

    assert(luhn_validate("49927398717") == false);
    printf("  [PASSOU] Numero invalido (49927398717)\n");

    assert(luhn_validate("7992 7398 713") == true);
    printf("  [PASSOU] Numero valido com espacos\n");

    assert(luhn_validate("") == false);
    printf("  [PASSOU] String vazia\n");

    assert(luhn_validate(NULL) == false);
    printf("  [PASSOU] Ponteiro nulo\n");

    printf("Todos os testes de Luhn passaram com sucesso!\n\n");
}

int main()
{
    run_luhn_tests();
    return 0;
}