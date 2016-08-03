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
 * statname.h - name mangling macros for static linking
 **************************************************************************************/

#ifndef _STATNAME_H
#define _STATNAME_H

#include "stm32f4xx.h"

/* define STAT_PREFIX to a unique name for static linking 
 * all the C functions and global variables will be mangled by the preprocessor
 *   e.g. void FFT(int *fftbuf) becomes void cook_FFT(int *fftbuf)
 */
#define STAT_PREFIX		xmp3fixpt

#define STATCC1(x,y,z)	STATCC2(x,y,z)
#define STATCC2(x,y,z)	x##y##z  

#ifdef STAT_PREFIX
#define STATNAME(func)	STATCC1(STAT_PREFIX, _, func)
#else
#define STATNAME(func)	func
#endif

/* these symbols are common to all implementations */
#define	CheckPadBit			STATNAME(CheckPadBit)
#define	UnpackFrameHeader	STATNAME(UnpackFrameHeader)
#define	UnpackSideInfo		STATNAME(UnpackSideInfo)
#define	AllocateBuffers		STATNAME(AllocateBuffers)
#define	FreeBuffers			STATNAME(FreeBuffers)
#define	DecodeHuffman		STATNAME(DecodeHuffman)
#define	Dequantize			STATNAME(Dequantize)
#define	IMDCT				STATNAME(IMDCT)
#define	UnpackScaleFactors	STATNAME(UnpackScaleFactors)
#define	Subband				STATNAME(Subband)

#define	samplerateTab		STATNAME(samplerateTab)
#define	bitrateTab			STATNAME(bitrateTab)
#define	samplesPerFrameTab	STATNAME(samplesPerFrameTab)
#define	bitsPerSlotTab		STATNAME(bitsPerSlotTab)
#define	sideBytesTab		STATNAME(sideBytesTab)
#define	slotTab				STATNAME(slotTab)
#define	sfBandTable			STATNAME(sfBandTable)

/////////////////////////////////////////////////////////////
///////////////////////// Delay ////////////////////////////
/////////////////////////////////////////////////////////////
void Delay_nop(vu32);
void Delay_1us(vu32);
void Delay_10us(vu32);
void Delay_100us(vu32);
void Delay_1ms(vu32);
void Delay_10ms(vu32);
void Delay_100ms(vu32);


/////////////////////////////////////////////////////////////
///////////////////////// I2C ///////////////////////////////
/////////////////////////////////////////////////////////////
void I2C_Config(void);

u32 I2C_ReadBytes(u8 SlaveAddr, u8 *ReadBuf, u8 NumByte);
u32 I2C_WriteBytes(u8 SlaveAddr, u8 *WriteBuf, u8 NumByte);
u32 I2C_DMA_ReadReg(u8 SlaveAddr, u8 ReadAddr, u8 *ReadBuf, u8 NumByte);
u32 I2C_DMA_WriteReg(u8 SlaveAddr, u8 WriteAddr, u8 *WriteBuf, u8 NumByte);

u32 I2C_TimeOut(void);

void I2C_TX_DMA_IRQ(void);
void I2C_RX_DMA_IRQ(void);


/////////////////////////////////////////////////////////////
///////////////////////// mpu9150 ////////////////////////
/////////////////////////////////////////////////////////////

#define MPU9150A_2g       ((float)0.0000610352f)  // 0.0000610352 g/LSB
#define MPU9150A_4g       ((float)0.0001220703f)  // 0.0001220703 g/LSB
#define MPU9150A_8g       ((float)0.0002441406f)  // 0.0002441406 g/LSB
#define MPU9150A_16g      ((float)0.0004882813f)  // 0.0004882813 g/LSB

#define MPU9150G_250dps   ((float)0.007633587786f)  // 0.007633587786 dps/LSB
#define MPU9150G_500dps   ((float)0.015267175572f)  // 0.015267175572 dps/LSB
#define MPU9150G_1000dps  ((float)0.030487804878f)  // 0.030487804878 dps/LSB
#define MPU9150G_2000dps  ((float)0.060975609756f)  // 0.060975609756 dps/LSB

#define MPU9150M_1200uT   ((float)0.3f)       // 0.3 uT/LSB

#define MPU9150T_85degC   ((float)0.00294f)   // 0.00294 degC/LSB
/*=====================================================================================================*/
#define MPU6050_I2C_ADDR            ((u8)0xD0)
#define MPU6050_Device_ID           ((u8)0x68)

#define MPU6050_SELF_TEST_X         ((u8)0x0D)
#define MPU6050_SELF_TEST_Y         ((u8)0x0E)
#define MPU6050_SELF_TEST_Z         ((u8)0x0F)
#define MPU6050_SELF_TEST_A         ((u8)0x10)
#define MPU6050_SMPLRT_DIV          ((u8)0x19)
#define MPU6050_CONFIG              ((u8)0x1A)
#define MPU6050_GYRO_CONFIG         ((u8)0x1B)
#define MPU6050_ACCEL_CONFIG        ((u8)0x1C)
#define MPU6050_MOT_THR             ((u8)0x1F)
#define MPU6050_FIFO_EN             ((u8)0x23)
#define MPU6050_I2C_MST_CTRL        ((u8)0x24)
#define MPU6050_I2C_SLV0_ADDR       ((u8)0x25)
#define MPU6050_I2C_SLV0_REG        ((u8)0x26)
#define MPU6050_I2C_SLV0_CTRL       ((u8)0x27)
#define MPU6050_I2C_SLV1_ADDR       ((u8)0x28)
#define MPU6050_I2C_SLV1_REG        ((u8)0x29)
#define MPU6050_I2C_SLV1_CTRL       ((u8)0x2A)
#define MPU6050_I2C_SLV2_ADDR       ((u8)0x2B)
#define MPU6050_I2C_SLV2_REG        ((u8)0x2C)
#define MPU6050_I2C_SLV2_CTRL       ((u8)0x2D)
#define MPU6050_I2C_SLV3_ADDR       ((u8)0x2E)
#define MPU6050_I2C_SLV3_REG        ((u8)0x2F)
#define MPU6050_I2C_SLV3_CTRL       ((u8)0x30)
#define MPU6050_I2C_SLV4_ADDR       ((u8)0x31)
#define MPU6050_I2C_SLV4_REG        ((u8)0x32)
#define MPU6050_I2C_SLV4_DO         ((u8)0x33)
#define MPU6050_I2C_SLV4_CTRL       ((u8)0x34)
#define MPU6050_I2C_SLV4_DI         ((u8)0x35)
#define MPU6050_I2C_MST_STATUS      ((u8)0x36)
#define MPU6050_INT_PIN_CFG         ((u8)0x37)
#define MPU6050_INT_ENABLE          ((u8)0x38)
#define MPU6050_INT_STATUS          ((u8)0x3A)
#define MPU6050_ACCEL_XOUT_H        ((u8)0x3B)
#define MPU6050_ACCEL_XOUT_L        ((u8)0x3C)
#define MPU6050_ACCEL_YOUT_H        ((u8)0x3D)
#define MPU6050_ACCEL_YOUT_L        ((u8)0x3E)
#define MPU6050_ACCEL_ZOUT_H        ((u8)0x3F)
#define MPU6050_ACCEL_ZOUT_L        ((u8)0x40)
#define MPU6050_TEMP_OUT_H          ((u8)0x41)
#define MPU6050_TEMP_OUT_L          ((u8)0x42)
#define MPU6050_GYRO_XOUT_H         ((u8)0x43)
#define MPU6050_GYRO_XOUT_L         ((u8)0x44)
#define MPU6050_GYRO_YOUT_H         ((u8)0x45)
#define MPU6050_GYRO_YOUT_L         ((u8)0x46)
#define MPU6050_GYRO_ZOUT_H         ((u8)0x47)
#define MPU6050_GYRO_ZOUT_L         ((u8)0x48)
#define MPU6050_EXT_SENS_DATA_00    ((u8)0x49)
#define MPU6050_EXT_SENS_DATA_01    ((u8)0x4A)
#define MPU6050_EXT_SENS_DATA_02    ((u8)0x4B)
#define MPU6050_EXT_SENS_DATA_03    ((u8)0x4C)
#define MPU6050_EXT_SENS_DATA_04    ((u8)0x4D)
#define MPU6050_EXT_SENS_DATA_05    ((u8)0x4E)
#define MPU6050_EXT_SENS_DATA_06    ((u8)0x4F)
#define MPU6050_EXT_SENS_DATA_07    ((u8)0x50)
#define MPU6050_EXT_SENS_DATA_08    ((u8)0x51)
#define MPU6050_EXT_SENS_DATA_09    ((u8)0x52)
#define MPU6050_EXT_SENS_DATA_10    ((u8)0x53)
#define MPU6050_EXT_SENS_DATA_11    ((u8)0x54)
#define MPU6050_EXT_SENS_DATA_12    ((u8)0x55)
#define MPU6050_EXT_SENS_DATA_13    ((u8)0x56)
#define MPU6050_EXT_SENS_DATA_14    ((u8)0x57)
#define MPU6050_EXT_SENS_DATA_15    ((u8)0x58)
#define MPU6050_EXT_SENS_DATA_16    ((u8)0x59)
#define MPU6050_EXT_SENS_DATA_17    ((u8)0x5A)
#define MPU6050_EXT_SENS_DATA_18    ((u8)0x5B)
#define MPU6050_EXT_SENS_DATA_19    ((u8)0x5C)
#define MPU6050_EXT_SENS_DATA_20    ((u8)0x5D)
#define MPU6050_EXT_SENS_DATA_21    ((u8)0x5E)
#define MPU6050_EXT_SENS_DATA_22    ((u8)0x5F)
#define MPU6050_EXT_SENS_DATA_23    ((u8)0x60)
#define MPU6050_I2C_SLV0_DO         ((u8)0x63)
#define MPU6050_I2C_SLV1_DO         ((u8)0x64)
#define MPU6050_I2C_SLV2_DO         ((u8)0x65)
#define MPU6050_I2C_SLV3_DO         ((u8)0x66)
#define MPU6050_I2C_MST_DELAY_CTRL  ((u8)0x67)
#define MPU6050_SIGNAL_PATH_RESET   ((u8)0x68)
#define MPU6050_MOT_DETECT_CTRL     ((u8)0x69)
#define MPU6050_USER_CTRL           ((u8)0x6A)
#define MPU6050_PWR_MGMT_1          ((u8)0x6B)
#define MPU6050_PWR_MGMT_2          ((u8)0x6C)
#define MPU6050_FIFO_COUNTH         ((u8)0x72)
#define MPU6050_FIFO_COUNTL         ((u8)0x73)
#define MPU6050_FIFO_R_W            ((u8)0x74)
#define MPU6050_WHO_AM_I            ((u8)0x75)	// 0x68

/* ---- AK8975 Reg ---------------------------------------------------------- */

#define AK8975_I2C_ADDR             ((u8)0x18)
#define AK8975_Device_ID            ((u8)0x48)

#define AK8975_WIA                  ((u8)0x00)
#define AK8975_INFO                 ((u8)0x01)
#define AK8975_ST1                  ((u8)0x02)
#define AK8975_HXL                  ((u8)0x03)
#define AK8975_HXH                  ((u8)0x04)
#define AK8975_HYL                  ((u8)0x05)
#define AK8975_HYH                  ((u8)0x06)
#define AK8975_HZL                  ((u8)0x07)
#define AK8975_HZH                  ((u8)0x08)
#define AK8975_ST2                  ((u8)0x09)
#define AK8975_CNTL                 ((u8)0x0A)
#define AK8975_RSV                  ((u8)0x0B)
#define AK8975_ASTC                 ((u8)0x0C)
#define AK8975_TS1                  ((u8)0x0D)
#define AK8975_TS2                  ((u8)0x0E)
#define AK8975_I2CDIS               ((u8)0x0F)
#define AK8975_ASAX                 ((u8)0x10)
#define AK8975_ASAY                 ((u8)0x11)
#define AK8975_ASAZ                 ((u8)0x12)
/*=====================================================================================================*/
u8 MPU9150_Init(void);
void MPU9150_Read(u8 *ReadBuf);
void MPU9150_Bypass(u8 Bypass);



/////////////////////////////////////////////////////////////
///////////////////////// MS5611 ///////////////////////////
/////////////////////////////////////////////////////////////





#define MS5611_I2C_ADDR       ((u8)0xEE)

#define MS5611_ADC            ((u8)0x00)  // ADC Read
#define MS5611_RESET          ((u8)0x1E)  // RESET

#define MS5611_D1_OSR_256     ((u8)0x40)  // 3 bytes
#define MS5611_D1_OSR_512     ((u8)0x42)  // 3 bytes
#define MS5611_D1_OSR_1024    ((u8)0x44)  // 3 bytes
#define MS5611_D1_OSR_2048    ((u8)0x46)  // 3 bytes
#define MS5611_D1_OSR_4096    ((u8)0x48)  // 3 bytes
#define MS5611_D2_OSR_256     ((u8)0x50)  // 3 bytes
#define MS5611_D2_OSR_512     ((u8)0x52)  // 3 bytes
#define MS5611_D2_OSR_1024    ((u8)0x54)  // 3 bytes
#define MS5611_D2_OSR_2048    ((u8)0x56)  // 3 bytes
#define MS5611_D2_OSR_4096    ((u8)0x58)  // 3 bytes

#define MS5611_PROM_COEFF_1   ((u8)0xA2)  // 2 bytes
#define MS5611_PROM_COEFF_2   ((u8)0xA4)  // 2 bytes
#define MS5611_PROM_COEFF_3   ((u8)0xA6)  // 2 bytes
#define MS5611_PROM_COEFF_4   ((u8)0xA8)  // 2 bytes
#define MS5611_PROM_COEFF_5   ((u8)0xAA)  // 2 bytes
#define MS5611_PROM_COEFF_6   ((u8)0xAC)  // 2 bytes

#define MS5611_ADC_D1         ((u8)0x01)
#define MS5611_ADC_D2         ((u8)0x02)

#define POW_2_7   (128)
#define POW_2_8   (256)
#define POW_2_15  (32768)
#define POW_2_16  (65536)
#define POW_2_21  (2097152)
#define POW_2_23  (8388608)

// 1010~1000  上升100m 下降10.5mbar
// 1000~900   上升100m 下降11.2mbar
// 900~800    上升100m 下降12.2mbar
// 800~700    上升100m 下降13.3mbar
// 700~600    上升100m 下降15mbar
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	u16 C[7];
	u32 D[3];

	s32 dT;
	s32 rTemp;

	uint64_t OFF;
	uint64_t SENS;
	s32 rPress;

	float Temp;   // -4000~8500, 0.01'C/LSB
	float Press;  // 1000~120000, 0.01mbar/LSB
	float High;   // 0.01mbar ~ 9.5238cm(10.5mbar/100m) or 8.92857(11.2mbar/100m)
} MS5611_ST;

typedef __IO enum {
	MS5611_ConvInit,
	MS5611_ConvD1,
	MS5611_ConvD2
} MS5611_STATUS;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern MS5611_ST Baro;
/*=====================================================================================================*/
/*=====================================================================================================*/
void MS5611_Init(MS5611_ST *COEFF);
void MS5611_Read(MS5611_ST *COEFF, u8 ADC_ConvMode);

/////////////////////////////////////////////////////////////
///////////////////////// sensor ///////////////////////////
/////////////////////////////////////////////////////////////


#define ACC_X_OFFSET ((s16)0)
#define ACC_Y_OFFSET ((s16)0)
#define ACC_Z_OFFSET ((s16)8192)
#define GYR_X_OFFSET ((s16)0)
#define GYR_Y_OFFSET ((s16)0)
#define GYR_Z_OFFSET ((s16)0)
#define MAG_X_OFFSET ((s16)0)
#define MAG_Y_OFFSET ((s16)0)
#define MAG_Z_OFFSET ((s16)0)
#define TEMP_OFFSET  ((s16)(-12421))  // (340*35)+521
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	s16 X;
	s16 Y;
	s16 Z;
	s16 OffsetX;
	s16 OffsetY;
	s16 OffsetZ;
	float TrueX;
	float TrueY;
	float TrueZ;
} SensorAcc;
typedef struct {
	s16 X;
	s16 Y;
	s16 Z;
	s16 OffsetX;
	s16 OffsetY;
	s16 OffsetZ;
	float TrueX;
	float TrueY;
	float TrueZ;
} SensorGyr;
typedef struct {
	s16 X;
	s16 Y;
	s16 Z;
	u16 AdjustX;
	u16 AdjustY;
	u16 AdjustZ;
	float TrueX;
	float TrueY;
	float TrueZ;
	float EllipseSita;
	float EllipseX0;
	float EllipseY0;
	float EllipseA;
	float EllipseB;
} SensorMag;
typedef struct {
	s16 T;
	s16 OffsetT;
	float TrueT;
} SensorTemp;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern SensorAcc Acc;
extern SensorGyr Gyr;
extern SensorMag Mag;
extern SensorTemp Temp;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Config(void);
u8 Sensor_Init(void);
void EllipseFitting(float *Ans, volatile s16 *MagDataX, volatile s16 *MagDataY, u8 Num);
u8 CompassDir(float Angle);



#endif /* _STATNAME_H */
