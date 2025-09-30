# Compilador
CC = gcc
# Flags de compilação
CFLAGS = -Wall -Iinclude
# Flags especiais para gerar cobertura de testes
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

# Diretórios
SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin

# Arquivos fonte da biblioteca
LIB_SRCS = $(SRC_DIR)/luhn.c $(SRC_DIR)/flash_mem.c $(SRC_DIR)/secure_log.c $(SRC_DIR)/iso8s583.c $(SRC_DIR)/fsm.c

# Executável final
APP_EXEC = $(BIN_DIR)/pagueveloz_cli

# --- REGRAS PRINCIPAIS ---

app: $(APP_EXEC)
$(APP_EXEC): $(SRC_DIR)/main.c $(LIB_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Aplicação final compilada em: $(APP_EXEC)"

test:
	$(MAKE) CFLAGS="$(CFLAGS)" test_all

# --- REGRAS DE COBERTURA ---

coverage:
	@echo "Gerando relatorio de cobertura..."
	# Compila e roda os testes com as flags de cobertura
	$(MAKE) CFLAGS="$(CFLAGS) $(COVERAGE_FLAGS)" LDFLAGS="$(COVERAGE_FLAGS)" test_all
	# Limpa dados de cobertura antigos e gera o novo
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/tests/*' --output-file coverage.info # Remove arquivos de teste do relatório
	genhtml coverage.info --output-directory coverage_report
	@echo "Relatorio de cobertura gerado em: coverage_report/index.html"

# --- REGRAS DE TESTE E LIMPEZA ---

# Roda todos os executáveis de teste
test_all: bin/test_luhn bin/test_persistence bin/test_iso8583 bin/test_fsm
	./bin/test_luhn
	./bin/test_persistence
	./bin/test_iso8583
	./bin/test_fsm

clean:
	rm -rf $(BIN_DIR) *.gcda *.gcno coverage.info coverage_report


#
# --- REGRAS DE TESTE (AUXILIARES) ---
#
test_luhn: bin/test_luhn
bin/test_luhn: $(SRC_DIR)/luhn.c $(TEST_DIR)/test_luhn.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test_persistence: bin/test_persistence
bin/test_persistence: $(SRC_DIR)/flash_mem.c $(SRC_DIR)/secure_log.c $(TEST_DIR)/test_persistence.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test_iso8583: bin/test_iso8583
bin/test_iso8583: $(SRC_DIR)/iso8583.c $(TEST_DIR)/test_iso8583.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test_fsm: bin/test_fsm
bin/test_fsm: $(SRC_DIR)/fsm.c $(TEST_DIR)/test_fsm.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: app test clean