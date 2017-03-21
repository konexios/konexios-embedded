#include "flashmbed.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_flash.h"

static uint8_t __eeprom[0x4000] __attribute__((section(".eeprom"), used));

char *flash_start() {
  return __eeprom;
}

int write_flash(char *flash, size_t size) {
  *((int*)(flash)) = FLASH_MAGIC_NUMBER;
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_EOP | FLASH_FLAG_OPERR |FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR );
  FLASH_EraseInitTypeDef eraser;
  eraser.TypeErase = TYPEERASE_SECTORS;
  eraser.Banks = FLASH_BANK_1;
  eraser.Sector = 1;
  eraser.NbSectors = 1;
  eraser.VoltageRange = VOLTAGE_RANGE_3;
  uint32_t sectorerr = 0;
  if( HAL_OK != HAL_FLASHEx_Erase( &eraser, &sectorerr ) || sectorerr != 0xFFFFFFFF ) {
    HAL_FLASH_Lock();
    printf("FLASH is broken\r\n");
    return -1;
  }
  int data, i = 0;
  char *raw = flash;
  while(i < size ) {
    data = *(raw + i);
    HAL_FLASH_Program(TYPEPROGRAM_BYTE, (int)(__eeprom+i), data);
    i++;
  }
  HAL_FLASH_Lock();
  return 0;
}


int geti() {
  char c = (char) getchar();
  return atoi(&c);
}

void menu() {
  while (1) {
    printf("\r\n* MENU *\r\n");
    printf("1. set  wifi configuration\r\n");
    printf("2. read wifi configuration\r\n");
//    printf("3. erease memory\r\n");
    printf("3. exit\r\n");
    printf(" select? ");
    int c = geti();
    printf("\r\n");
    switch(c)
    {
      case 1:
      break;
      case 2:
      break;
//      case 3:
//        SOFBlock::format(FLASH_SECTOR_1);
//      break;
      default:
      return;
    }
  }
}

#ifdef __cplusplus
}
#endif
