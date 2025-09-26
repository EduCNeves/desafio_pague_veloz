# Compilador
CC = gcc
# Flags de compilação: -Wall (todos os warnings), -Iinclude (procurar headers em include/)
CFLAGS = -Wall -Iinclude

# Diretórios
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
BIN_DIR = bin

# Arquivos fonte
LUHN_SRC = $(SRC_DIR)/luhn.c

# Alvo principal: o executável de teste
TEST_EXEC = $(BIN_DIR)/test_luhn

#
# REGRAS DE COMPILAÇÃO
#

# Regra para criar o executável de teste
$(TEST_EXEC): $(LUHN_SRC) $(TEST_DIR)/test_luhn.c
	@mkdir -p $(BIN_DIR) # Cria o diretório bin se não existir
	$(CC) $(CFLAGS) $^ -o $@

# Regra para rodar os testes
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

# Evita que o make confunda o alvo "test" com um arquivo chamado "test"
.PHONY: test clean