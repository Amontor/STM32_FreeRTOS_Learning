#include "W25Qxx.h"

/**
 * @brief    SPI发送指定长度的数据
 * @param    buf  —— 发送数据缓冲区首地址
 * @param    size —— 要发送数据的字节数
 * @retval   成功返回HAL_OK
 */
static HAL_StatusTypeDef SPI_Transmit(uint8_t* send_buf, uint16_t size)
{
    return HAL_SPI_Transmit(&hspi1, send_buf, size, W25Qx_TIMEOUT_VALUE);
}
 
/**
 * @brief   SPI接收指定长度的数据
 * @param   buf  —— 接收数据缓冲区首地址
 * @param   size —— 要接收数据的字节数
 * @retval  成功返回HAL_OK
 */
static HAL_StatusTypeDef SPI_Receive(uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_Receive(&hspi1, recv_buf, size, W25Qx_TIMEOUT_VALUE);
}
 
/**
 * @brief   SPI在发送数据的同时接收指定长度的数据
 * @param   send_buf  —— 接收数据缓冲区首地址
 * @param   recv_buf  —— 接收数据缓冲区首地址
 * @param   size —— 要发送/接收数据的字节数
 * @retval  成功返回HAL_OK
 */
static HAL_StatusTypeDef SPI_TransmitReceive(uint8_t* send_buf, uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_TransmitReceive(&hspi1, send_buf, recv_buf, size, W25Qx_TIMEOUT_VALUE);
}

/*-----------------------------------split line------------------------------------------*/
static void BSP_W25Qx_Reset(void)
{
    uint8_t cmd[2] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};

    W25Qx_Enable();
    /* Send the reset command */
    SPI_Transmit(cmd, 2);
    W25Qx_Disable();
}

/**********************************************************************************
 * 函数功能: 获取设备状态
 */
static uint8_t BSP_W25Qx_GetStatus(void)
{
    uint8_t cmd[] = {READ_STATUS_REG1_CMD};
    uint8_t status;

    W25Qx_Enable();
    /* Send the read status command */
    SPI_Transmit(cmd, 1);
    /* Reception of the data */
    SPI_Receive(&status, 1);
    W25Qx_Disable();

    /* Check the value of the register */
    if ((status & W25Q128FV_FSR_BUSY) != 0)
    {
        return W25Qx_BUSY;
    }
    else
    {
        return W25Qx_OK;
    }
}

/**********************************************************************************
 * 函数功能: 模块初始化
 */
uint8_t BSP_W25Qx_Init(void)
{
    BSP_W25Qx_Reset();
    return BSP_W25Qx_GetStatus();
}

/**********************************************************************************
 * 函数功能: 写使能
 */
uint8_t BSP_W25Qx_WriteEnable(void)
{
    uint8_t cmd[] = {WRITE_ENABLE_CMD};
    uint32_t tickstart = HAL_GetTick();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    SPI_Transmit(cmd, 1);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
        {
            return W25Qx_TIMEOUT;
        }
    }

    return W25Qx_OK;
}

/**********************************************************************************
 * 函数功能: 获取设备ID
 */
void BSP_W25Qx_Read_ID(uint8_t *ID)
{
    uint8_t cmd[] = {READ_JEDEC_ID_CMD};

    W25Qx_Enable();
    /* Send the read ID command */
    SPI_Transmit(cmd, 1);
    /* Reception of the data */
    SPI_Receive(ID, 3);
    W25Qx_Disable();
}

/**********************************************************************************
 * 函数功能: 读数据
 * 输入参数: 缓存数组指针、读地址、字节数
 */
uint8_t BSP_W25Qx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);

    W25Qx_Enable();
    /* Send the read ID command */
    SPI_Transmit(cmd, 4);
    /* Reception of the data */
    if (SPI_Receive(pData, Size) != HAL_OK)
    {
        return W25Qx_ERROR;
    }
    W25Qx_Disable();
    return W25Qx_OK;
}

/**********************************************************************************
 * 函数功能: 写数据   (可以跨页写入)
 * 输入参数: 缓存数组指针、写地址、字节数
 */
uint8_t BSP_W25Qx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[4];
    uint32_t end_addr, current_size, current_addr;
    uint32_t tickstart = HAL_GetTick();

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= WriteAddr)
    {
        current_addr += W25Q128FV_PAGE_SIZE;
    }
    current_size = current_addr - WriteAddr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
        current_size = Size;
    }

    /* Initialize the adress variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* Perform the write page by page */
    do
    {
        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t)(current_addr >> 16);
        cmd[2] = (uint8_t)(current_addr >> 8);
        cmd[3] = (uint8_t)(current_addr);

        /* Enable write operations */
        BSP_W25Qx_WriteEnable();

        W25Qx_Enable();
        /* Send the command */
        if (SPI_Transmit(cmd, 4) != HAL_OK)
        {
            return W25Qx_ERROR;
        }

        /* Transmission of the data */
        if (SPI_Transmit(pData, current_size) != HAL_OK)
        {
            return W25Qx_ERROR;
        }
        W25Qx_Disable();
        /* Wait the end of Flash writing */
        while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
        {
            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
            {
                return W25Qx_TIMEOUT;
            }
        }

        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
    } while (current_addr < end_addr);

    return W25Qx_OK;
}

/**********************************************************************************
 * 函数功能: 扇区擦除
 * 输入参数: 地址
 */
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)(Address >> 16);
    cmd[2] = (uint8_t)(Address >> 8);
    cmd[3] = (uint8_t)(Address);

    /* Enable write operations */
    BSP_W25Qx_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    SPI_Transmit(cmd, 4);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME)
        {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}

/**********************************************************************************
 * 函数功能: 芯片擦除
 */
uint8_t BSP_W25Qx_Erase_Chip(void)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = CHIP_ERASE_CMD;

    /* Enable write operations */
    BSP_W25Qx_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    SPI_Transmit(cmd, 1);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus() != W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Q128FV_BULK_ERASE_MAX_TIME)
        {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}
