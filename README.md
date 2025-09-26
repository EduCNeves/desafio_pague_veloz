

# Desafio Técnico - Desenvolvedor Backend C Embarcado (PagueVeloz)

Este repositório contém a implementação do desafio técnico proposto pela PagueVeloz para a vaga de Desenvolvedor Backend C Embarcado. O objetivo é construir uma biblioteca em C que simula as funcionalidades essenciais de um terminal POS (Point of Sale).

**Status do Projeto:** 🚧 Em andamento - Dia 3 de 7 concluído. 🚧

## Funcionalidades Implementadas (Dia 1)

  * **Estrutura do Projeto:** Organização de diretórios para código-fonte (`src`), cabeçalhos (`include`), testes (`tests`) e documentação (`docs`).
  * **Módulo de Validação (Algoritmo de Luhn):** Implementada a função `bool luhn_validate(const char *pan)` para validar números de cartão.
      * *Arquivos:* `src/luhn.c`, `include/luhn.h`.

## Funcionalidades Implementadas (Dia 2)

  * **Módulo de Persistência (Memória Flash Simulada):** Um módulo para simular uma memória flash de 4 KB, abstraindo o acesso ao hardware.
      * *Arquivos:* `src/flash_mem.c`, `include/flash_mem.h`.
  * **Módulo de Log Seguro:** Utiliza o módulo de flash para gravar logs com mascaramento de PAN e cifragem XOR.
      * *Arquivos:* `src/secure_log.c`, `include/secure_log.h`.

## Funcionalidades Implementadas (Dia 3)

  * **Módulo de Mensagens ISO 8583 (Builder):** Foi implementado o montador de mensagens ISO 8583. Este módulo converte os dados de uma transação (armazenados em uma `struct` C) para o formato de mensagem padrão, pronto para transmissão.
      * **Geração de Bitmap:** A lógica para gerar o bitmap de 16 bytes (primário e secundário) foi implementada, indicando corretamente os campos presentes.
      * **Empacotamento:** A função `iso8583_pack()` serializa o MTI, o bitmap e os campos de dados em um único buffer.
      * *Arquivos:* `src/iso8583.c`, `include/iso8583.h`.

## Testes Unitários

O projeto possui um conjunto de testes automatizados para garantir a corretude de cada módulo.

  * **Testes de Luhn:** Valida o algoritmo de Luhn contra diversos casos.
  * **Testes de Persistência:** Valida o fluxo de escrita/leitura na flash, mascaramento de PAN e cifragem.
  * **Testes do Montador ISO 8583:** Um terceiro executável (`tests/test_iso8583.c`) foi adicionado para validar a correta montagem de uma mensagem de venda (0200).

## Como Compilar e Testar

### Pré-requisitos

  * `gcc` (Compilador C)
  * `make` (Ferramenta de automação de build)

### Passo a Passo

1.  **Compile e execute os testes:**
    O `Makefile` compila todos os módulos e roda o conjunto completo de testes com um único comando:

    ```bash
    make test
    ```

2.  **Limpe os arquivos gerados:**
    Para remover os executáveis e arquivos temporários, use o comando:

    ```bash
    make clean
    ```

## Resultados Esperados

Ao executar `make test`, a saída de todos os conjuntos de testes será exibida, confirmando que os módulos estão funcionando corretamente:

```bash
Executando testes para o Algoritmo de Luhn...
  [PASSOU] Numero valido (49927398716)
  [PASSOU] Numero invalido (49927398717)
  [PASSOU] Numero valido com espacos
  [PASSOU] String vazia
  [PASSOU] Ponteiro nulo
Todos os testes de Luhn passaram com sucesso!

Executando testes de Persistencia Segura...
  [PASSOU] Flash Write e Read
  [PASSOU] Mascaramento de PAN
  [PASSOU] Cifra XOR
  [PASSOU] Gravacao e Leitura de Log Seguro
Todos os testes de persistencia passaram com sucesso!

Executando testes do Montador ISO 8583...
  [PASSOU] Empacotamento de mensagem 0200
Todos os testes de ISO 8583 passaram com sucesso!

```

## Próximos Passos

O plano para os próximos dias inclui o desenvolvimento dos seguintes módulos:

  - [x] Implementar o módulo de persistência segura em memória flash simulada.
  - [x] Desenvolver o builder (montador) de mensagens ISO 8583.
  - [ ] Desenvolver o parser (analisador) de mensagens ISO 8583.
  - [ ] Criar a Máquina de Estados Finitos (FSM) para gerenciar o fluxo da transação.
  - [ ] Integrar todos os módulos em uma aplicação de linha de comando (CLI) para demonstração.