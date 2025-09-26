# Compilador
CC = gcc
# Flags de compilação: -Wall (todos os warnings), -Iinclude (procurar headers em include/)
CFLAGS = -Wall -Iinclude

# Diretórios
SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin

# Arquivos fonte
SRCS = $(SRC_dir)/luhn.c $(SRC_DIR)/flash_mem.c $(SRC_DIR)/secure_log.c $(SRC_DIR)/iso8583.c

# Alvos dos executáveis de teste
TEST_LUHN_EXEC = $(BIN_DIR)/test_luhn
TEST_PERSISTENCE_EXEC = $(BIN_DIR)/test_persistence
TEST_ISO8583_EXEC = $(BIN_DIR)/test_iso8583

# REGRAS DE COMPILAÇÃO

# Regra para criar o executável de teste de luhn
$(TEST_LUHN_EXEC): $(SRC_DIR)/luhn.c $(TEST_DIR)/test_luhn.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para criar o executável de teste de persistência
# Note que ele precisa de flash_mem.c e secure_log.c
$(TEST_PERSISTENCE_EXEC): $(SRC_DIR)/flash_mem.c $(SRC_DIR)/secure_log.c $(TEST_DIR)/test_persistence.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para criar o executável de teste de iso8583
$(TEST_ISO8583_EXEC): $(SRC_DIR)/iso8583.c $(TEST_DIR)/test_iso8583.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para rodar TODOS os testes
test: $(TEST_LUHN_EXEC) $(TEST_PERSISTENCE_EXEC) $(TEST_ISO8583_EXEC)
	./$(TEST_LUHN_EXEC)
	./$(TEST_PERSISTENCE_EXEC)
	./$(TEST_ISO8583_EXEC)

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(BIN_DIR)

# Evita que o make confunda os alvos com arquivos de mesmo nome
.PHONY: test clean