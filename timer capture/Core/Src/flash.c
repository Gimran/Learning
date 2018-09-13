#include "flash.h"



//#define FLASH_KEY1 ((uint32_t)0x45670123)
//#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

typedef	struct
{
char Parameter1;		// 1 byte
uint8_t Parameter2;		// 1 byte
uint16_t Parameter3;	// 2 byte
uint32_t Parameter4;	// 4 byte

                      // 8 byte = 2  32-bits words.  It's - OK
                      // !!! Full size (bytes) must be a multiple of 4 !!!
} tpSettings;


void flash_unlock(void) {
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

void flash_lock() {
  FLASH->CR |= FLASH_CR_LOCK;
}

//������� ��������� true ����� ����� ������� ��� ������ ������.
uint8_t flash_ready(void) {
  return !(FLASH->SR & FLASH_SR_BSY);
}
 
//������� ������� ��� ��������. ��� � ������ �������� ���������������� 
void flash_erase_all_pages(void) {
  FLASH->CR |= FLASH_CR_MER; //������������� ��� �������� ���� �������
  FLASH->CR |= FLASH_CR_STRT; //������ ��������
  while(!flash_ready()) // �������� ����������.. ���� ��� ��� �������� �� � ���� �����...
    ;
  FLASH->CR &= FLASH_CR_MER;
}
 
//������� ������� ���� ��������. � �������� ������ ����� ������������ �����
//������������� ��������� ������� ��� �������� ������� ����� ��������.

void flash_erase_page(uint32_t address) {
  FLASH->CR|= FLASH_CR_PER; //������������� ��� �������� ����� ��������
  FLASH->AR = address; // ������ � �����
  FLASH->CR|= FLASH_CR_STRT; // ��������� �������� 
  while(!flash_ready())
    ;  //���� ���� �������� ��������. 
  FLASH->CR&= ~FLASH_CR_PER; //���������� ��� �������
}

void flash_write(uint32_t address,uint32_t data) {
  FLASH->CR |= FLASH_CR_PG; //��������� ���������������� �����
  while(!flash_ready()) //������� ���������� ����� � ������
    ;
  *(__IO uint16_t*)address = (uint16_t)data; //����� ������� 2 ����
  while(!flash_ready())
    ;
  address+=2;
  data>>=16;
  *(__IO uint16_t*)address = (uint16_t)data; //����� ������� 2 �����
  while(!flash_ready())
    ;
  FLASH->CR &= ~(FLASH_CR_PG); //��������� ���������������� �����
}

