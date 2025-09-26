#include "luhn.h" // Inclui a função que queremos testar
#include <assert.h>
#include <stdio.h>


void run_luhn_tests()
{
    printf("Executando testes para o Algoritmo de Luhn...\n");

    // Caso de Teste 1: Número Válido
    assert(luhn_validate("49927398716") == true);
    printf("  [PASSOU] Numero valido (49927398716)\n");

    // Caso de Teste 2: Número Inválido
    assert(luhn_validate("49927398717") == false);
    printf("  [PASSOU] Numero invalido (49927398717)\n");

    // Caso de Teste 3: Número válido com espaços (deve ser ignorado)
    assert(luhn_validate("7992 7398 713") == true);
    printf("  [PASSOU] Numero valido com espacos\n");

    // Caso de Teste 4: String vazia
    assert(luhn_validate("") == false);
    printf("  [PASSOU] String vazia\n");

    // Caso de Teste 5: Ponteiro Nulo
    assert(luhn_validate(NULL) == false);
    printf("  [PASSOU] Ponteiro nulo\n");

    printf("Todos os testes de Luhn passaram com sucesso!\n\n");
}

int main()
{
    run_luhn_tests();
    return 0;
}