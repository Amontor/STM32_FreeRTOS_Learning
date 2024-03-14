#include "stm32f1xx_hal.h"
#include "Internal_flash.h"

const uint32_t FLASH_WHOLE_SIZE = 0x40000;   // 一共256KB

#define MYFlash_PageAddr  0x0803F800        // 最后一页起始地址 
#define Flash_PageSize    FLASH_PAGE_SIZE   // 页大小

#define CORE_ID UID_BASE  // 96位芯片id

uint16_t Store_Data[512] = {0};  // 存储数据, 一共1KB，占最后一页的一半

static uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
    return *(__IO uint16_t*)Address;
}

uint16_t get_flash_size(void)  // 芯片flash大小寄存器
{
    return FLASH_ReadHalfWord(FLASH_SIZE_DATA_REGISTER);
}

static int Internal_ErasePage(void)
{
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef eraseInit;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = MYFlash_PageAddr;
    eraseInit.Banks = FLASH_BANK_1;
    eraseInit.NbPages = 1;
    if(HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
    {
        return -1;
    }
    return 0;
}

void Store_Init(void)
{
    if(FLASH_ReadHalfWord(MYFlash_PageAddr) != 0x5A5A)
    {
        HAL_FLASH_Unlock();
        Internal_ErasePage();
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MYFlash_PageAddr, 0x5A5A);
        for(uint16_t i = 1;i < 512;i++)
        {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MYFlash_PageAddr + i * 2, Store_Data[i]);
        }
        HAL_FLASH_Lock();
    }
    else
    {
        for(uint16_t i = 0;i < 512;i++)
        {
            Store_Data[i] = FLASH_ReadHalfWord(MYFlash_PageAddr + i * 2);
        }
    }
}

void Store_Save(void)
{
    HAL_FLASH_Unlock();
    Internal_ErasePage();
    for(uint16_t i = 0;i < 512;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MYFlash_PageAddr + i * 2, Store_Data[i]);
    }
    HAL_FLASH_Lock();
}
