# Desafio Técnico - Desenvolvedor Backend C Embarcado (PagueVeloz)

Este repositório contém a implementação do desafio técnico proposto pela PagueVeloz. O objetivo foi construir uma biblioteca em C simulando as funcionalidades essenciais de um terminal POS (Point of Sale), incluindo processamento de mensagens ISO 8583, validação de cartão, fluxo de transação e persistência segura.

## Funcionalidades Implementadas

* **Módulo de Validação (Algoritmo de Luhn):** Função para validar números de cartão contra erros de digitação.
* **Módulo de Persistência Segura:** Simulação de uma memória flash de 4 KB persistida em um arquivo (`virtual_flash.bin`), com funções para mascaramento de PAN e cifragem XOR dos logs.
* **Módulo de Mensagens ISO 8583:**
    * **Builder:** Converte dados de uma transação em uma mensagem ISO 8583 (MTI 0200) formatada, incluindo a geração de bitmap primário e secundário.
    * **Parser:** Interpreta um buffer de mensagem ISO 8583, extraindo o MTI, o bitmap e todos os campos de dados.
* **Máquina de Estados (FSM):** Gerencia todo o fluxo da transação, desde o estado ocioso até a aprovação/recusa, incluindo a lógica de reverso por time-out.
* **Aplicação CLI:** Um executável de linha de comando (`pagueveloz_cli`) com dois modos:
    * `demo`: Executa uma simulação completa de uma transação de venda bem-sucedida.
    * `list`: Lê, decifra e exibe os logs de transação persistidos pelo modo demo.

## Cobertura de Testes

O projeto utiliza um conjunto de testes unitários para cada módulo. A cobertura de código, gerada com `gcov` e `lcov`, atinge **93.2%**, cumprindo o requisito de >80%.

Para gerar o relatório localmente, execute `make coverage` e abra o arquivo `coverage_report/index.html`.

## Diagrama da FSM

O fluxo da transação é controlado pela seguinte Máquina de Estados:

![Diagrama da FSM](docs/diagram.png)

## Como Compilar e Executar

### Pré-requisitos
* `gcc`
* `make`
* `lcov` (para gerar o relatório de cobertura)

### Comandos

```bash
# Para compilar a aplicação principal
make app

# Para rodar a demonstração de transação
./bin/pagueveloz_cli demo

# Para listar os logs da demonstração
./bin/pagueveloz_cli list

# Para rodar todos os testes unitários
make test

# Para gerar o relatório de cobertura de testes
make coverage

# Para limpar todos os arquivos gerados

make clean
