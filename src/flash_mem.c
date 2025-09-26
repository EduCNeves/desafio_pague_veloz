#include "flash_mem.h"
#include <string.h> // Para memset e memcpy

// Nossa memória flash virtual. 'static' a torna privada a este arquivo.
static uint8_t virtual_flash[FLASH_SIZE];

void flash_init(void)
{
    // Zera toda a memória para simular um estado limpo
    memset(virtual_flash, 0x00, FLASH_SIZE);
}

int flash_write(uint32_t address, const uint8_t *data, uint32_t size)
{
    // Validação de limites: não podemos escrever além do final da memória
    if ((address + size) > FLASH_SIZE)
    {
        return -1; // Erro: acesso fora dos limites
    }
    memcpy(&virtual_flash[address], data, size);
    return 0; // Sucesso
}

int flash_read(uint32_t address, uint8_t *buffer, uint32_t size)
{
    // Validação de limites
    if ((address + size) > FLASH_SIZE)
    {
        return -1; // Erro: acesso fora dos limites
    }
    memcpy(buffer, &virtual_flash[address], size);
    return 0; // Sucesso
}