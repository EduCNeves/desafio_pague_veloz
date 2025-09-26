#ifndef FLASH_MEM_H
#define FLASH_MEM_H

#include <stdint.h> // Para usar tipos como uint8_t, uint32_t

// Tamanho da nossa memória flash virtual, conforme especificado
#define FLASH_SIZE 4096

/**
 * @brief Inicializa a memória flash virtual (preenche com 0x00).
 */
void flash_init(void);

/**
 * @brief Escreve dados em um endereço da memória flash.
 * @param address O endereço inicial onde os dados serão escritos.
 * @param data O ponteiro para os dados a serem escritos.
 * @param size O número de bytes a serem escritos.
 * @return 0 em caso de sucesso, -1 em caso de erro (ex: fora dos limites).
 */
int flash_write(uint32_t address, const uint8_t *data, uint32_t size);

/**
 * @brief Lê dados de um endereço da memória flash.
 * @param address O endereço inicial de onde os dados serão lidos.
 * @param buffer O buffer onde os dados lidos serão armazenados.
 * @param size O número de bytes a serem lidos.
 * @return 0 em caso de sucesso, -1 em caso de erro (ex: fora dos limites).
 */
int flash_read(uint32_t address, uint8_t *buffer, uint32_t size);

#endif // FLASH_MEM_H