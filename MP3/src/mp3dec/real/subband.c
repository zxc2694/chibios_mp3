/* ***** BEGIN LICENSE BLOCK ***** 
 * Version: RCSL 1.0/RPSL 1.0 
 *  
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc. All Rights Reserved. 
 *      
 * The contents of this file, and the files included with this file, are 
 * subject to the current version of the RealNetworks Public Source License 
 * Version 1.0 (the "RPSL") available at 
 * http://www.helixcommunity.org/content/rpsl unless you have licensed 
 * the file under the RealNetworks Community Source License Version 1.0 
 * (the "RCSL") available at http://www.helixcommunity.org/content/rcsl, 
 * in which case the RCSL will apply. You may also obtain the license terms 
 * directly from RealNetworks.  You may not use this file except in 
 * compliance with the RPSL or, if you have a valid RCSL with RealNetworks 
 * applicable to this file, the RCSL.  Please see the applicable RPSL or 
 * RCSL for the rights, obligations and limitations governing use of the 
 * contents of the file.  
 *  
 * This file is part of the Helix DNA Technology. RealNetworks is the 
 * developer of the Original Code and owns the copyrights in the portions 
 * it created. 
 *  
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * Technology Compatibility Kit Test Suite(s) Location: 
 *    http://www.helixcommunity.org/content/tck 
 * 
 * Contributor(s): 
 *  
 * ***** END LICENSE BLOCK ***** */ 

/**************************************************************************************
 * Fixed-point MP3 decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * June 2003
 *
 * subband.c - subband transform (synthesis filterbank implemented via 32-point DCT
 *               followed by polyphase filter)
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"
#include "stm32f4xx.h"
#include "st_stm32f4xx.h"
#include "st_stm32f4xx_gpio.h"
#include "st_stm32f4xx_rcc.h"
#include "st_stm32f4xx_i2c.h"

/**************************************************************************************
 * Function:    Subband
 *
 * Description: do subband transform on all the blocks in one granule, all channels
 *
 * Inputs:      filled MP3DecInfo structure, after calling IMDCT for all channels
 *              vbuf[ch] and vindex[ch] must be preserved between calls
 *
 * Outputs:     decoded PCM data, interleaved LRLRLR... if stereo
 *
 * Return:      0 on success,  -1 if null input pointers
 **************************************************************************************/
int Subband(MP3DecInfo *mp3DecInfo, short *pcmBuf)
{
	int b;
	HuffmanInfo *hi;
	IMDCTInfo *mi;
	SubbandInfo *sbi;

	/* validate pointers */
	if (!mp3DecInfo || !mp3DecInfo->HuffmanInfoPS || !mp3DecInfo->IMDCTInfoPS || !mp3DecInfo->SubbandInfoPS)
		return -1;

	hi = (HuffmanInfo *)mp3DecInfo->HuffmanInfoPS;
	mi = (IMDCTInfo *)(mp3DecInfo->IMDCTInfoPS);
	sbi = (SubbandInfo*)(mp3DecInfo->SubbandInfoPS);

	if (mp3DecInfo->nChans == 2) {
		/* stereo */
		for (b = 0; b < BLOCK_SIZE; b++) {
			FDCT32(mi->outBuf[0][b], sbi->vbuf + 0*32, sbi->vindex, (b & 0x01), mi->gb[0]);
			FDCT32(mi->outBuf[1][b], sbi->vbuf + 1*32, sbi->vindex, (b & 0x01), mi->gb[1]);
			PolyphaseStereo(pcmBuf, sbi->vbuf + sbi->vindex + VBUF_LENGTH * (b & 0x01), polyCoef);
			sbi->vindex = (sbi->vindex - (b & 0x01)) & 7;
			pcmBuf += (2 * NBANDS);
		}
	} else {
		/* mono */
		for (b = 0; b < BLOCK_SIZE; b++) {
			FDCT32(mi->outBuf[0][b], sbi->vbuf + 0*32, sbi->vindex, (b & 0x01), mi->gb[0]);
			PolyphaseMono(pcmBuf, sbi->vbuf + sbi->vindex + VBUF_LENGTH * (b & 0x01), polyCoef);
			sbi->vindex = (sbi->vindex - (b & 0x01)) & 7;
			pcmBuf += NBANDS;
		}
	}

	return 0;
}
/////////////////////////////////////////////////////////////
///////////////////////// Delay ////////////////////////////
/////////////////////////////////////////////////////////////

void Delay_nop(vu32 nCnt)
{
	while (nCnt--) {
	}
}

void Delay_1us(vu32 nCnt_1us)
{
	u32 nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}

void Delay_10us(vu32 nCnt_10us)
{
	u32 nCnt;

	for (; nCnt_10us != 0; nCnt_10us--)
		for (nCnt = 556; nCnt != 0; nCnt--);
}

void Delay_100us(vu32 nCnt_100us)
{
	u32 nCnt;

	for (; nCnt_100us != 0; nCnt_100us--)
		for (nCnt = 5400; nCnt != 0; nCnt--);
}

void Delay_1ms(vu32 nCnt_1ms)
{
	u32 nCnt;

	for (; nCnt_1ms != 0; nCnt_1ms--)
		for (nCnt = 56580; nCnt != 0; nCnt--);
}

void Delay_10ms(vu32 nCnt_10ms)
{
	u32 nCnt;

	for (; nCnt_10ms != 0; nCnt_10ms--)
		for (nCnt = 559928; nCnt != 0; nCnt--);
}

void Delay_100ms(vu32 nCnt_100ms)
{
	u32 nCnt;

	for (; nCnt_100ms != 0; nCnt_100ms--)
		for (nCnt = 5655000; nCnt != 0; nCnt--);
}













































// #define I2Cx                  I2C1
// #define I2Cx_CLK              RCC_APB1Periph_I2C1

// #define I2Cx_SCL_PIN          GPIO_Pin_8
// #define I2Cx_SCL_GPIO_PORT    GPIOB
// #define I2Cx_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOB
// #define I2Cx_SCL_SOURCE       GPIO_PinSource8
// #define I2Cx_SCL_AF           GPIO_AF_I2C1

// #define I2Cx_SDA_PIN          GPIO_Pin_9
// #define I2Cx_SDA_GPIO_PORT    GPIOB
// #define I2Cx_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOB
// #define I2Cx_SDA_SOURCE       GPIO_PinSource9
// #define I2Cx_SDA_AF           GPIO_AF_I2C1

// #define I2Cx_DR_ADDR          ((u32)0x40005410)
// #define I2Cx_SPEED            ((u32)400000)

// #define DMAx                  DMA1
// #define DMAx_CLK              RCC_AHB1Periph_DMA1

// #define DMAx_TX_CHANNEL       DMA_Channel_1
// #define DMAx_TX_STREAM        DMA1_Stream6
// #define DMAx_TX_FLAG_FEIF     DMA_FLAG_FEIF6
// #define DMAx_TX_FLAG_DMEIF    DMA_FLAG_DMEIF6
// #define DMAx_TX_FLAG_TEIF     DMA_FLAG_TEIF6
// #define DMAx_TX_FLAG_HTIF     DMA_FLAG_HTIF6
// #define DMAx_TX_FLAG_TCIF     DMA_FLAG_TCIF6
// #define DMAx_TX_IRQn          DMA1_Stream6_IRQn

// #define DMAx_RX_CHANNEL       DMA_Channel_1
// #define DMAx_RX_STREAM        DMA1_Stream0
// #define DMAx_RX_FLAG_FEIF     DMA_FLAG_FEIF0
// #define DMAx_RX_FLAG_DMEIF    DMA_FLAG_DMEIF0
// #define DMAx_RX_FLAG_TEIF     DMA_FLAG_TEIF0
// #define DMAx_RX_FLAG_HTIF     DMA_FLAG_HTIF0
// #define DMAx_RX_FLAG_TCIF     DMA_FLAG_TCIF0
// #define DMAx_RX_IRQn          DMA1_Stream0_IRQn

// #define I2C_TIMEOUT           ((u32)65535)
// /*=====================================================================================================*/
// /*=====================================================================================================*/
// vu8 *I2C_ReadPtr;
// vu8 *I2C_WritePtr;
// vu32 I2C_TimeCnt = I2C_TIMEOUT;
// /*=====================================================================================================*/
// /*=====================================================================================================*/
// DMA_InitTypeDef DMA_InitStruct;
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_Config
// **功能 : I2C 設定 & 配置
// **輸入 : None
// **輸出 : None
// **使用 : I2C_Config();
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void I2C_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	NVIC_InitTypeDef NVIC_InitStruct;
// 	I2C_InitTypeDef I2C_InitStruct;

// 	RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
// 	RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);
// 	RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK | I2Cx_SDA_GPIO_CLK, ENABLE);

// 	RCC_APB1PeriphResetCmd(I2Cx_CLK, DISABLE);

// 	GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
// 	GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);

// 	/* SCL I2Cx_SCL_PIN */  /* SDA I2Cx_SDA_PIN*/
// 	GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_PIN;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
// 	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
// 	GPIO_InitStruct.GPIO_Pin = I2Cx_SDA_PIN;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
// 	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

// 	NVIC_InitStruct.NVIC_IRQChannel = DMAx_RX_IRQn;
// 	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 3;
// 	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStruct);
// 	NVIC_InitStruct.NVIC_IRQChannel = DMAx_TX_IRQn;
// 	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 3;
// 	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStruct);

// 	DMA_ClearFlag(DMAx_TX_STREAM, DMAx_TX_FLAG_FEIF | DMAx_TX_FLAG_DMEIF | DMAx_TX_FLAG_TEIF |
// 		      DMAx_TX_FLAG_HTIF | DMAx_TX_FLAG_TCIF);
// 	DMA_Cmd(DMAx_TX_STREAM, DISABLE);
// 	DMA_DeInit(DMAx_TX_STREAM);
// 	DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
// 	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
// 	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
// 	DMA_InitStruct.DMA_BufferSize = 0xFFFF;
// 	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 	DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

// 	DMA_ClearFlag(DMAx_RX_STREAM, DMAx_RX_FLAG_FEIF | DMAx_RX_FLAG_DMEIF | DMAx_RX_FLAG_TEIF |
// 		      DMAx_RX_FLAG_HTIF | DMAx_RX_FLAG_TCIF);
// 	DMA_Cmd(DMAx_RX_STREAM, DISABLE);
// 	DMA_DeInit(DMAx_RX_STREAM);
// 	DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
// 	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
// 	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
// 	DMA_InitStruct.DMA_BufferSize = 0xFFFF;
// 	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 	DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

// 	DMA_ITConfig(DMAx_TX_STREAM, DMA_IT_TC, ENABLE);
// 	DMA_ITConfig(DMAx_RX_STREAM, DMA_IT_TC, ENABLE);

// 	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
// 	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
// 	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
// 	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
// 	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
// 	I2C_InitStruct.I2C_ClockSpeed = I2Cx_SPEED;
// 	I2C_Cmd(I2Cx, ENABLE);
// 	I2C_Init(I2Cx, &I2C_InitStruct);

// 	I2C_DMACmd(I2Cx, ENABLE);
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_ReadBytes
// **功能 : I2C Read Bytes
// **輸入 : SlaveAddr, ReadBuf, NumByte
// **輸出 : Status
// **使用 : I2C_ReadBytes(SlaveAddr, ReadBuf, NumByte);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u32 I2C_ReadBytes(u8 SlaveAddr, u8 *ReadBuf, u8 NumByte)
// {
// 	I2C_ReadPtr = &NumByte;

// 	I2C_GenerateSTART(I2Cx, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Receiver);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
// 	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
// 	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
// 	DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
// 	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 	DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

// 	I2C_DMALastTransferCmd(I2Cx, ENABLE);

// 	DMA_Cmd(DMAx_RX_STREAM, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (NumByte > 0)
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	return SUCCESS;
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_WriteBytes
// **功能 : I2C Write Bytes
// **輸入 : SlaveAddr, WriteBuf, NumByte
// **輸出 : Status
// **使用 : I2C_WriteByte(SlaveAddr, WriteBuf, NumByte);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u32 I2C_WriteBytes(u8 SlaveAddr, u8 *WriteBuf, u8 NumByte)
// {
// 	NumByte--;
// 	I2C_WritePtr = &NumByte;

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_GenerateSTART(I2Cx, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_TimeCnt = I2C_TIMEOUT;
// 	I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_SendData(I2Cx, WriteBuf[0]);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	if (NumByte > 0) {
// 		DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
// 		DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 		DMA_InitStruct.DMA_Memory0BaseAddr = (u32)(WriteBuf + 1);
// 		DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
// 		DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
// 		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 		DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 		DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 		DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 		DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 		DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 		DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

// 		DMA_Cmd(DMAx_TX_STREAM, ENABLE);

// 	} else {
// 		I2C_GenerateSTOP(I2Cx, ENABLE);
// 	}

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (NumByte > 0)
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	return SUCCESS;
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_DMA_ReadReg
// **功能 : I2C DMA Read Reg
// **輸入 : *ReadBuf, SlaveAddr, ReadAddr, NumByte
// **輸出 : Status
// **使用 : I2C_DMA_Read(SlaveAddr, ReadAddr, ReadBuf, NumByte);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u32 I2C_DMA_ReadReg(u8 SlaveAddr, u8 ReadAddr, u8 *ReadBuf, u8 NumByte)
// {
// 	I2C_ReadPtr = &NumByte;

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_GenerateSTART(I2Cx, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_SendData(I2Cx, ReadAddr);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_GenerateSTART(I2Cx, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Receiver);

// 	if (NumByte < 2) {
// 		I2C_TimeCnt = I2C_TIMEOUT;

// 		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
// 			if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 		I2C_AcknowledgeConfig(I2Cx, DISABLE);
// 		(void)I2Cx->SR2;

// 		I2C_GenerateSTOP(I2Cx, ENABLE);

// 		I2C_TimeCnt = I2C_TIMEOUT;

// 		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET)
// 			if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 		*ReadBuf = I2C_ReceiveData(I2Cx);

// 		NumByte--;

// 		I2C_TimeCnt = I2C_TIMEOUT;

// 		while (I2Cx->CR1 & I2C_CR1_STOP)
// 			if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 		I2C_AcknowledgeConfig(I2Cx, ENABLE);

// 	} else {
// 		I2C_TimeCnt = I2C_TIMEOUT;

// 		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
// 			if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 		DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
// 		DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 		DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
// 		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
// 		DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
// 		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 		DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 		DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 		DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 		DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 		DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 		DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

// 		I2C_DMALastTransferCmd(I2Cx, ENABLE);

// 		DMA_Cmd(DMAx_RX_STREAM, ENABLE);
// 	}

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (NumByte > 0)
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	return SUCCESS;
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_DMA_WriteReg
// **功能 : I2C DMA Write Reg
// **輸入 : WriteAddr, WriteAddr, *WriteBuf, NumByte
// **輸出 : Status
// **使用 : I2C_DMA_Write(SlaveAddr, WriteAddr, WriteBuf, NumByte);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u32 I2C_DMA_WriteReg(u8 SlaveAddr, u8 WriteAddr, u8 *WriteBuf, u8 NumByte)
// {
// 	I2C_WritePtr = &NumByte;

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_GenerateSTART(I2Cx, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_TimeCnt = I2C_TIMEOUT;
// 	I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_SendData(I2Cx, WriteAddr);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
// 		if ((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
// 	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
// 	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)WriteBuf;
// 	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
// 	DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
// 	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
// 	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
// 	DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

// 	DMA_Cmd(DMAx_TX_STREAM, ENABLE);

// 	I2C_TimeCnt = I2C_TIMEOUT;

// 	while (NumByte > 0)
// 		if ((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	return SUCCESS;
// }
// /*=====================================================================================================*/
// =====================================================================================================*
// **函數 : I2C_TimeOut
// **功能 : I2C TimeOut
// **輸入 : None
// **輸出 : None
// **使用 : I2C_TimeOut();
// **=====================================================================================================
// /*=====================================================================================================*/
// u32 I2C_TimeOut(void)
// {
// 	while (1) {
// 		Delay_1ms(200);
// 	}
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_TX_DMA_IRQ
// **功能 : I2C Send DMA IRQ
// **輸入 : None
// **輸出 : None
// **使用 : I2C_TX_DMA_IRQ();
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void I2C_TX_DMA_IRQ(void)
// {
// 	if (DMA_GetFlagStatus(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF) != RESET) {
// 		DMA_Cmd(DMAx_TX_STREAM, DISABLE);
// 		DMA_ClearFlag(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF);

// 		I2C_TimeCnt = I2C_TIMEOUT;

// 		while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))
// 			if ((I2C_TimeCnt--) == 0) I2C_TimeOut();

// 		I2C_GenerateSTOP(I2Cx, ENABLE);
// 		*I2C_WritePtr = 0;
// 	}
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : I2C_RX_DMA_IRQ
// **功能 : I2C Recv DMA IRQ
// **輸入 : None
// **輸出 : None
// **使用 : I2C_RX_DMA_IRQ();
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void I2C_RX_DMA_IRQ(void)
// {
// 	if (DMA_GetFlagStatus(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF) != RESET) {
// 		I2C_GenerateSTOP(I2Cx, ENABLE);
// 		DMA_Cmd(DMAx_RX_STREAM, DISABLE);
// 		DMA_ClearFlag(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF);
// 		*I2C_ReadPtr = 0;
// 	}
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*/

// /////////////////////////////////////////////////////////////
// ///////////////////////// sensor ///////////////////////////
// /////////////////////////////////////////////////////////////

// SensorAcc Acc = {0};
// SensorGyr Gyr = {0};
// SensorMag Mag = {0};
// SensorTemp Temp = {0};
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : Sensor_Config
// **功能 : Sensor Config
// **輸入 : None
// **輸出 : None
// **使用 : Sensor_Config();
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void Sensor_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStruct;

// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

// 	/* INT PC3 */
// 	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
// 	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(GPIOC, &GPIO_InitStruct);

// 	I2C_Config();
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : Sensor_Init
// **功能 : Sensor Init
// **輸入 : None
// **輸出 : None
// **使用 : Sensor_Init();
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u8 Sensor_Init(void)
// {
// 	u8 Sta = ERROR;

// 	Acc.X = 0;
// 	Acc.Y = 0;
// 	Acc.Z = 0;
// 	Acc.OffsetX = +7;
// 	Acc.OffsetY = +146;
// 	Acc.OffsetZ = -291;
// 	Acc.TrueX = 0.0f;
// 	Acc.TrueY = 0.0f;
// 	Acc.TrueZ = 0.0f;

// 	Gyr.X = 0;
// 	Gyr.Y = 0;
// 	Gyr.Z = 0;
// 	Gyr.OffsetX = 0;
// 	Gyr.OffsetY = 0;
// 	Gyr.OffsetZ = 0;
// 	Gyr.TrueX = 0.0f;
// 	Gyr.TrueY = 0.0f;
// 	Gyr.TrueZ = 0.0f;

// 	Mag.X = 0;
// 	Mag.Y = 0;
// 	Mag.Z = 0;
// 	Mag.AdjustX = 0;
// 	Mag.AdjustY = 0;
// 	Mag.AdjustZ = 0;
// 	Mag.TrueX = 0.0f;
// 	Mag.TrueY = 0.0f;
// 	Mag.TrueZ = 0.0f;

// 	Temp.T = 0;
// 	Temp.OffsetT = TEMP_OFFSET;
// 	Temp.TrueT = 0.0f;

// 	Sta = MPU9150_Init();
// 	MS5611_Init(&Baro);

// 	return Sta;
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : EllipseFitting
// **功能 : Ellipse Fitting
// **輸入 : Ans, MagDataX, MagDataY, Num
// **輸出 : None
// **使用 : EllipseFitting(Ellipse, MagDataX, MagDataY, 8);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void EllipseFitting(float *Ans, volatile s16 *MagDataX, volatile s16 *MagDataY, u8 Num)
// {
// 	s8 i, j, k;
// 	float temp, temp1, temp2;
// 	float tempArrX[8] = {0};
// 	float tempArrY[8] = {0};

// 	float MAG_X1Y0 = 0.0f;
// 	float MAG_X2Y0 = 0.0f;
// 	float MAG_X3Y0 = 0.0f;
// 	float MAG_X0Y1 = 0.0f;
// 	float MAG_X0Y2 = 0.0f;
// 	float MAG_X0Y3 = 0.0f;
// 	float MAG_X0Y4 = 0.0f;
// 	float MAG_X1Y1 = 0.0f;
// 	float MAG_X2Y1 = 0.0f;
// 	float MAG_X1Y2 = 0.0f;
// 	float MAG_X1Y3 = 0.0f;
// 	float MAG_X2Y2 = 0.0f;
// 	float MAG_X3Y1 = 0.0f;

// 	float MagArr[5][6] = {{0}};

// 	for (i = 0; i < Num; i++) {
// 		tempArrX[i] = (float)MagDataX[i] / 1000.0f;
// 		tempArrY[i] = (float)MagDataY[i] / 1000.0f;
// 	}

// 	for (i = 0; i < Num; i++) {
// 		MAG_X1Y0 += tempArrX[i];
// 		MAG_X2Y0 += tempArrX[i] * tempArrX[i];
// 		MAG_X3Y0 += tempArrX[i] * tempArrX[i] * tempArrX[i];
// 		MAG_X0Y1 += tempArrY[i];
// 		MAG_X0Y2 += tempArrY[i] * tempArrY[i];
// 		MAG_X0Y3 += tempArrY[i] * tempArrY[i] * tempArrY[i];
// 		MAG_X0Y4 += tempArrY[i] * tempArrY[i] * tempArrY[i] * tempArrY[i];
// 		MAG_X1Y1 += tempArrX[i] * tempArrY[i];
// 		MAG_X2Y1 += tempArrX[i] * tempArrX[i] * tempArrY[i];
// 		MAG_X1Y2 += tempArrX[i] * tempArrY[i] * tempArrY[i];
// 		MAG_X1Y3 += tempArrX[i] * tempArrY[i] * tempArrY[i] * tempArrY[i];
// 		MAG_X2Y2 += tempArrX[i] * tempArrX[i] * tempArrY[i] * tempArrY[i];
// 		MAG_X3Y1 += tempArrX[i] * tempArrX[i] * tempArrX[i] * tempArrY[i];
// 	}

// 	MagArr[0][0] = MAG_X2Y2;
// 	MagArr[0][1] = MAG_X1Y3;
// 	MagArr[0][2] = MAG_X2Y1;
// 	MagArr[0][3] = MAG_X1Y2;
// 	MagArr[0][4] = MAG_X1Y1;
// 	MagArr[0][5] = -MAG_X3Y1;

// 	MagArr[1][0] = MAG_X1Y3;
// 	MagArr[1][1] = MAG_X0Y4;
// 	MagArr[1][2] = MAG_X1Y2;
// 	MagArr[1][3] = MAG_X0Y3;
// 	MagArr[1][4] = MAG_X0Y2;
// 	MagArr[1][5] = -MAG_X2Y2;

// 	MagArr[2][0] = MAG_X2Y1;
// 	MagArr[2][1] = MAG_X1Y2;
// 	MagArr[2][2] = MAG_X2Y0;
// 	MagArr[2][3] = MAG_X1Y1;
// 	MagArr[2][4] = MAG_X1Y0;
// 	MagArr[2][5] = -MAG_X3Y0;

// 	MagArr[3][0] = MAG_X1Y2;
// 	MagArr[3][1] = MAG_X0Y3;
// 	MagArr[3][2] = MAG_X1Y1;
// 	MagArr[3][3] = MAG_X0Y2;
// 	MagArr[3][4] = MAG_X0Y1;
// 	MagArr[3][5] = -MAG_X2Y1;

// 	MagArr[4][0] = MAG_X1Y1;
// 	MagArr[4][1] = MAG_X0Y2;
// 	MagArr[4][2] = MAG_X1Y0;
// 	MagArr[4][3] = MAG_X0Y1;
// 	MagArr[4][4] = Num;
// 	MagArr[4][5] = -MAG_X2Y0;

// 	for (i = 0; i < 5; i++)
// 		for (j = i + 1; j < 6; j++)
// 			for (k = 5; k > i - 1; k--)
// 				MagArr[j][k] = MagArr[j][k] - MagArr[j][i] / MagArr[i][i] * MagArr[i][k];

// 	for (i = 0; i < 5; i++) {
// 		temp = MagArr[i][i];

// 		for (j = i; j < 6; j++)
// 			MagArr[i][j] = MagArr[i][j] / temp;
// 	}

// 	for (j = 4; j > 0; j--)
// 		for (i = 0; i < j; i++)
// 			MagArr[i][5] = MagArr[i][5] - MagArr[i][j] * MagArr[j][5];

// 	temp = (1.0f - MagArr[1][5]) / MagArr[0][5];
// 	temp1 = temp + sqrtf(temp * temp + 1.0f);
// 	Ans[0] = atanf(temp1);	// Theta

// 	temp = MagArr[0][5] * MagArr[0][5] - 4 * MagArr[1][5];
// 	Ans[1] = (2.0f * MagArr[1][5] * MagArr[2][5] - MagArr[0][5] * MagArr[3][5]) / temp;	// X0
// 	Ans[2] = (2.0f * MagArr[3][5] - MagArr[0][5] * MagArr[2][5]) / temp;	    // Y0

// 	temp = arm_cos_f32(Ans[0]);
// 	temp2  = (Ans[1] * Ans[1] + MagArr[0][5] * Ans[1] * Ans[2] + MagArr[1][5] * Ans[2] * Ans[2] - MagArr[4][5]) * (temp1 * temp1 * temp1 * temp1 - 1.0f);
// 	Ans[3] = temp / sqrtf((MagArr[1][5] * temp1 * temp1 - 1) / temp2);	// a
// 	Ans[4] = temp / sqrtf((temp1 * temp1 - MagArr[1][5]) / temp2); // b

// 	Ans[1] = Ans[1] * 1000.0f;
// 	Ans[2] = Ans[2] * 1000.0f;
// 	Ans[3] = Ans[3] * 1000.0f;
// 	Ans[4] = Ans[4] * 1000.0f;
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : CompassDir
// **功能 : Compass Direction
// **輸入 : Angle
// **輸出 : Direction
// **使用 : CompDir = CompassDir(Angle);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// u8 CompassDir(float Angle)
// {
// 	if ((Angle > 22.5f) && (Angle <= 67.5f))
// 		return 1;
// 	else if ((Angle > 67.5f) && (Angle <= 112.5f))
// 		return 2;
// 	else if ((Angle > 112.5f) && (Angle <= 157.5f))
// 		return 3;
// 	else if ((Angle > 157.5f) && (Angle <= 202.5f))
// 		return 4;
// 	else if ((Angle > 202.5f) && (Angle <= 247.5f))
// 		return 5;
// 	else if ((Angle > 247.5f) && (Angle <= 292.5f))
// 		return 6;
// 	else if ((Angle > 292.5f) && (Angle <= 337.5f))
// 		return 7;
// 	else
// 		return 0;
// }




// /////////////////////////////////////////////////////////////
// ///////////////////////// MPU9150 ///////////////////////
// /////////////////////////////////////////////////////////////




// u8 MPU9150_Init(void)
// {
// 	u8 i = 0;
// 	u8 ReadBuf[3] = {0};
// 	u8 MPU6050_Init_Data[6][2] = {
// 		{0x01, MPU6050_PWR_MGMT_1},   // Reset device
// 		{0x03, MPU6050_CONFIG},       //
// 		{0x18, MPU6050_GYRO_CONFIG},  // +-2000dps
// 		{0x08, MPU6050_ACCEL_CONFIG}, // +-4G
// 		{0x32, MPU6050_INT_PIN_CFG},  //
// 		{0x00, MPU6050_USER_CTRL}     //
// 	};
// 	u8 AK8975_Init_Data[3][2] = {
// 		{0x0F, AK8975_CNTL},          // Set Fuse ROM access mode
// 		{0x00, AK8975_CNTL},          // Set Power Down Mode
// 		{0x01, AK8975_CNTL}           // Set Single Measurement Mode
// 	};

// 	/* MPU6050 */
// 	I2C_DMA_ReadReg(MPU6050_I2C_ADDR, MPU6050_WHO_AM_I, ReadBuf, 1);

// 	if (ReadBuf[0] != MPU6050_Device_ID)
// 		return ERROR;

// 	Delay_1ms(10);

// 	for (i = 0; i < 6; i++) {
// 		I2C_DMA_WriteReg(MPU6050_I2C_ADDR, MPU6050_Init_Data[i][1], MPU6050_Init_Data[i], 1);
// 		Delay_1ms(10);
// 	}

// 	Mag.AdjustX = 1;
// 	Mag.AdjustY = 1;
// 	Mag.AdjustZ = 1;

// 	return SUCCESS;
// }
// /*==============================================================================================*/
// /*==============================================================================================*
// **函數 : MPU9150_Read
// **功能 : 初始化 MPU9150
// **輸入 : None
// **輸出 : None
// **使用 : MPU9150_Read(IMU_Buf);
// **==============================================================================================*/
// /*==============================================================================================*/
// void MPU9150_Read(u8 *ReadBuf)
// {
// 	u8 ReadData = 0x00;
// 	u8 WriteData = 0x01;

// 	I2C_DMA_ReadReg(MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, ReadBuf, 14);
// }




// /////////////////////////////////////////////////////////////
// ///////////////////////// MS5611 ///////////////////////////
// /////////////////////////////////////////////////////////////






// static void MS5611_ReadPROM(MS5611_ST *COEFF);
// static void MS5611_ReadADC(MS5611_ST *COEFF, u8 ADC_Sel);
// static void MS5611_Calculate(MS5611_ST *COEFF);
// static void MS5611_ConvADC(u8 ADC_ConvMode);
// /*=====================================================================================================*/
// /*=====================================================================================================*/
// MS5611_ST Baro;
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_Init
// **功能 : 初始化MS5611
// **輸入 : COEFF
// **輸出 : None
// **使用 : MS5611_Init(&Baro);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void MS5611_Init(MS5611_ST *COEFF)
// {
// 	u8 WriteCMD = MS5611_RESET;

// 	/* Reset */
// 	I2C_WriteBytes(MS5611_I2C_ADDR, &WriteCMD, 1);
// //  I2C_MS5611_WriteByte(MS5611_I2C_ADDR, MS5611_RESET);
// 	Delay_1ms(10);

// 	/* Read PROM */
// 	MS5611_ReadPROM(COEFF);
// 	Delay_1ms(10);

// 	/* D1, D2 Conversion */
// 	MS5611_ConvADC(MS5611_D1_OSR_4096);
// 	Delay_1ms(10);
// 	MS5611_ReadADC(COEFF, MS5611_ADC_D1);

// 	MS5611_ConvADC(MS5611_D2_OSR_4096);
// 	Delay_1ms(10);
// 	MS5611_ReadADC(COEFF, MS5611_ADC_D2);

// 	/* Calculate */
// 	MS5611_Calculate(COEFF);
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_ReadPROM
// **功能 : Read PROM
// **輸入 : COEFF
// **輸出 : None
// **使用 : MS5611_ReadPROM(&Baro);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// static void MS5611_ReadPROM(MS5611_ST *COEFF)
// {
// 	u8 i = 0;
// 	u8 WriteCMD = 0;
// 	u8 ReadC[6][2] = {{0}};

// 	for (i = 0; i < 6; i++) {
// 		WriteCMD = MS5611_PROM_COEFF_1 + i;
// 		I2C_WriteBytes(MS5611_I2C_ADDR, &WriteCMD, 1);
// //    I2C_MS5611_WriteByte(MS5611_I2C_ADDR, MS5611_PROM_COEFF_1+i);
// 		I2C_ReadBytes(MS5611_I2C_ADDR, ReadC[i], 2);
// //    I2C_MS5611_ReadData(ReadC[i], MS5611_I2C_ADDR, 2);
// 		COEFF->C[i + 1] = (u16)((ReadC[i][0] << 8) | ReadC[i][1]);
// 	}
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_ConvADC
// **功能 : Convert ADC
// **輸入 : ADC_ConvMode
// **輸出 : None
// **使用 : MS5611_ConvADC(MS5611_D1_OSR_4096);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// static void MS5611_ConvADC(u8 ADC_ConvMode)
// {
// 	u8 WriteCMD = ADC_ConvMode;

// 	I2C_WriteBytes(MS5611_I2C_ADDR, &WriteCMD, 1);
// //  I2C_MS5611_WriteByte(MS5611_I2C_ADDR, ADC_ConvMode);
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_ReadADC
// **功能 : Read ADC
// **輸入 : COEFF, ADC_Sel
// **輸出 : None
// **使用 : MS5611_ReadADC(&Baro, ADC_Sel);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// static void MS5611_ReadADC(MS5611_ST *COEFF, u8 ADC_Sel)
// {
// 	u8 ReadADC[3] = {0};
// 	u8 WriteCMD = MS5611_ADC;

// 	I2C_WriteBytes(MS5611_I2C_ADDR, &WriteCMD, 1);
// //  I2C_MS5611_WriteByte(MS5611_I2C_ADDR, MS5611_ADC);
// 	I2C_ReadBytes(MS5611_I2C_ADDR, ReadADC, 3);
// //  I2C_MS5611_ReadData(ReadADC, MS5611_I2C_ADDR, 3);

// 	COEFF->D[ADC_Sel] = (u16)(ReadADC[0] << 16 | ReadADC[1] << 8 | ReadADC[2]);
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_Calculate
// **功能 : MS5611 Calculate
// **輸入 : COEFF
// **輸出 : None
// **使用 : MS5611_Calculate(&Baro);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// static void MS5611_Calculate(MS5611_ST *COEFF)
// {
// 	COEFF->dT = (s32)(COEFF->D[2] - (COEFF->C[5] * POW_2_8));        // dT = D2 - C5*2^8
// 	COEFF->rTemp = (s32)(2000 + (COEFF->dT * COEFF->C[5]) / POW_2_23); // Temp = 2000 + dT*C5/2^23

// 	COEFF->OFF = (int64_t)(COEFF->C[2] * POW_2_16 + (COEFF->C[4] * COEFF->dT) / POW_2_7); // OFF = C2*2^16 + (C4*dT)/2^7
// 	COEFF->SENS = (int64_t)(COEFF->C[1] * POW_2_15 + (COEFF->C[3] * COEFF->dT) / POW_2_8); // SENS = C1*2^15 + (C3*dT)/2^8
// 	COEFF->rPress = (s32)((COEFF->D[1] * COEFF->SENS) / POW_2_21 - COEFF->OFF) / POW_2_15; // Press = (D1*SENS/2^21 - OFF)/2^15

// 	COEFF->Temp  = (float)(COEFF->rTemp / 100.0f);
// 	COEFF->Press = (float)(COEFF->rPress / 100.0f);
// 	COEFF->High  = (float)((COEFF->rPress - 101333) * 9.5238f);
// }
// /*=====================================================================================================*/
// /*=====================================================================================================*
// **函數 : MS5611_Read
// **功能 : MS5611 Read
// **輸入 : Baro, ADC_ConvMode
// **輸出 : None
// **使用 : MS5611_Read(&Baro, MS5611_D1_OSR_4096);
// **=====================================================================================================*/
// /*=====================================================================================================*/
// void MS5611_Read(MS5611_ST *COEFF, u8 ADC_ConvMode)
// {
// 	u8 ReadFlag = ERROR;
// 	static MS5611_STATUS ReadSta = MS5611_ConvInit;

// 	switch (ReadSta) {
// 	case MS5611_ConvInit:
// 		MS5611_ConvADC(ADC_ConvMode);
// 		ReadSta = MS5611_ConvD1;
// 		break;

// 	case MS5611_ConvD1:
// 		MS5611_ReadADC(COEFF, MS5611_ADC_D1);
// 		MS5611_ConvADC(ADC_ConvMode | 0x10);
// 		ReadSta = MS5611_ConvD2;
// 		break;

// 	case MS5611_ConvD2:
// 		MS5611_ReadADC(COEFF, MS5611_ADC_D2);
// 		MS5611_ConvADC(ADC_ConvMode);
// 		ReadFlag = SUCCESS;
// 		ReadSta = MS5611_ConvD1;
// 		break;
// 	}

// 	if (ReadFlag == SUCCESS)
// 		MS5611_Calculate(COEFF);
// }