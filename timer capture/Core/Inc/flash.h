
#ifndef FLASH_H
    #define FLASH_H
    #include "stm32f1xx.h" 

#define ADDRESS		0x800FC00

void flash_unlock(void);
void flash_lock(void);
uint8_t flash_ready(void);
void flash_erase_all_pages(void);
void flash_erase_page(uint32_t address);
void flash_write(uint32_t address,uint32_t data);




#endif // !FLASH_H
