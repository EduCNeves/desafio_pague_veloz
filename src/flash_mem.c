#include "flash_mem.h"
#include <stdio.h>
#include <string.h>

// O nome do arquivo que vai simular chip de memória flash
#define FLASH_FILE_NAME "virtual_flash.bin"

void flash_init(void) {
    FILE *file = fopen(FLASH_FILE_NAME, "wb");
    if (file == NULL) {
        perror("Erro ao inicializar a memoria flash");
        return;
    }

    // Preenche o arquivo com zeros para simular uma memória limpa
    uint8_t zero_byte = 0;
    for (int i = 0; i < FLASH_SIZE; i++) {
        fwrite(&zero_byte, 1, 1, file);
    }
    fclose(file);
}

int flash_write(uint32_t address, const uint8_t *data, uint32_t size) {
    if ((address + size) > FLASH_SIZE) {
        return -1; // Erro: acesso fora dos limites
    }

    // "r+b" abre o arquivo para leitura e escrita sem apagar o conteúdo
    FILE *file = fopen(FLASH_FILE_NAME, "r+b");
    if (file == NULL) {
        // Se o arquivo não existir, inicialize-o primeiro
        flash_init();
        file = fopen(FLASH_FILE_NAME, "r+b");
        if (file == NULL) {
            perror("Nao foi possivel abrir o arquivo da flash");
            return -1;
        }
    }
    
    // Move o "cursor" do arquivo para o endereço desejado
    fseek(file, address, SEEK_SET);
    
    // Escreve os dados
    fwrite(data, 1, size, file);
    
    fclose(file);
    return 0; // Sucesso
}

int flash_read(uint32_t address, uint8_t *buffer, uint32_t size) {
    if ((address + size) > FLASH_SIZE) {
        return -1; // Erro: acesso fora dos limites
    }

    FILE *file = fopen(FLASH_FILE_NAME, "rb"); // Abre para leitura binária
    if (file == NULL) {
        perror("Nao foi possivel abrir o arquivo da flash para leitura");
        return -1;
    }
    
    // Move o "cursor" para o endereço
    fseek(file, address, SEEK_SET);
    
    // Lê os dados para o buffer
    fread(buffer, 1, size, file);
    
    fclose(file);
    return 0; // Sucesso
}