Desafio Técnico - Desenvolvedor Backend C Embarcado (PagueVeloz)
Este repositório contém a implementação do desafio técnico proposto pela PagueVeloz para a vaga de Desenvolvedor Backend C Embarcado. O objetivo é construir uma biblioteca em C que simula as funcionalidades essenciais de um terminal POS (Point of Sale).

Status do Projeto: 🚧 Em andamento - Dia 1 de 7 concluído. 🚧

Funcionalidades Implementadas (Dia 1)
Nesta primeira etapa, a base do projeto foi estabelecida e o primeiro módulo funcional foi entregue e testado.

Estrutura do Projeto:

O projeto foi organizado em uma estrutura de diretórios clara para separar o código-fonte (src), os cabeçalhos (include), os testes (tests) e a documentação (docs).

Módulo de Validação (Algoritmo de Luhn):

Foi implementada a função bool luhn_validate(const char *pan).

Objetivo: Validar números de identificação, como os de cartões de crédito, para detectar erros simples de digitação.

Arquivos: src/luhn.c, include/luhn.h.

Testes Unitários:

Um executável de teste dedicado (tests/test_luhn.c) foi criado para validar o módulo Luhn de forma isolada.

Os testes cobrem casos de sucesso (números válidos), casos de falha (números inválidos) e casos de borda (strings vazias, ponteiros nulos).

Sistema de Build (Makefile):

Um Makefile foi configurado para automatizar o processo de compilação e teste, tornando o desenvolvimento mais eficiente.

Como Compilar e Testar
Pré-requisitos
gcc (Compilador C)

make (Ferramenta de automação de build)

Passo a Passo
Clone o repositório (se aplicável):

''' Bash

git clone <url-do-seu-repositorio>
cd pagueveloz_challenge
'''
Compile e execute os testes:
O Makefile foi configurado para compilar todos os arquivos necessários e rodar os testes com um único comando:

'''Bash

make test
'''

Limpe os arquivos gerados:
Para remover os arquivos compilados e limpar o projeto, use o comando:

'''Bash

make clean
'''

Resultados Esperados
Ao executar o comando make test, a seguinte saída deve ser exibida no terminal. Isso confirma que o módulo de validação de Luhn está funcionando corretamente para todos os casos de teste definidos.

''' bash
Executando testes para o Algoritmo de Luhn...
  [PASSOU] Numero valido (49927398716)
  [PASSOU] Numero invalido (49927398717)
  [PASSOU] Numero valido com espacos
  [PASSOU] String vazia
  [PASSOU] Ponteiro nulo
Todos os testes de Luhn passaram com sucesso!
'''

Próximos Passos
O plano para os próximos dias inclui o desenvolvimento dos seguintes módulos:

[ ] Implementar o módulo de persistência segura em memória flash simulada.

[ ] Desenvolver o builder e o parser para mensagens no padrão ISO 8583.

[ ] Criar a Máquina de Estados Finitos (FSM) para gerenciar o fluxo da transação.

[ ] Integrar todos os módulos em uma aplicação de linha de comando (CLI) para demonstração.
