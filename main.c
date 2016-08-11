// Reference: http://www.chibios.com/forum/viewtopic.php?t=351
// Connection:
// SD card (SPI mode) --> PC4 ((CS), PA5 (SCK), PA6 (MISO), PA7 (MOSI)
// UART --> PA2 (TX), PA3 (RX) [38400 baudrate]

#include "mp3process.h"
#include "MPU60X0.h"

bool_t fs_ready = FALSE;
SPIConfig hs_spicfg = { NULL, GPIOC, 4, 0 };
SPIConfig ls_spicfg = { NULL, GPIOC, 4, SPI_CR1_BR_2 | SPI_CR1_BR_1 };
extern Thread* g_pMp3DecoderThread;

// Configure I2C for sensors
static const I2CConfig i2cfg1 = {OPMODE_I2C, 200000, FAST_DUTY_CYCLE_2};

// Periodic thread 1 
static WORKING_AREA(waBlinkThread, 256);
static msg_t BlinkThread(void *arg)
{
  (void) arg;
  chRegSetThreadName("BlinkThread");
  while(TRUE)
  {
    if(palReadPad(GPIOA, GPIOA_BUTTON))
    {
      if(g_pMp3DecoderThread != NULL)
      {
        chEvtSignalFlags(g_pMp3DecoderThread, (eventmask_t)8);
      }
    }
    palSetPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(80);
    palClearPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(80);
  }

  return (msg_t)0;
}

// Periodic thread 2
static WORKING_AREA(waSensorRecordThread, 128);
static msg_t SensorRecordThread(void *arg){
    (void)arg;
    chRegSetThreadName("SensorRecordThread");
   while (TRUE) {
     mpu_i2c_read_data(0x3B, 14); /* Read accelerometer, temperature and gyro data */
     chThdSleepMilliseconds(50);
  }
 
  return (msg_t)0;
}


static void I2SDmaTxInterrupt(SPIDriver* spip, uint32_t flags)
{
  if(spip == &SPID3)
  {
    if(flags & STM32_DMA_ISR_HTIF)
    {
      // let the decoder thread play the next samples
      chSysLockFromIsr();
      chEvtSignalFlagsI(g_pMp3DecoderThread, (eventmask_t)2);
      chSysUnlockFromIsr();
    }
    if(flags & STM32_DMA_ISR_TCIF)
    {
      // let the decoder thread play the next samples
      chSysLockFromIsr();
      chEvtSignalFlagsI(g_pMp3DecoderThread, (eventmask_t)4);
      chSysUnlockFromIsr();
    }
  }
}

// MMC card insertion event
static void InsertHandler(eventid_t id)
{
  FRESULT err;
  (void) id;
  if(mmcConnect(&MMCD1))
  {
    chprintf((BaseChannel *) &SD2, "SD: Failed to connect to card\r\n");
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: Connected to card\r\n");
  }

  err = f_mount(0, &MMC_FS);
  if(err != FR_OK)
  {
    chprintf((BaseChannel *) &SD2, "SD: f_mount() failed %d\r\n", err);
    mmcDisconnect(&MMCD1);
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: File system mounted\r\n");
  }
  fs_ready = TRUE;

  while(1)
  {
    ////////// Read mp3 processing //////////
    Mp3PlayAllFiles("/");
  }
}

// MMC card removal event
static void RemoveHandler(eventid_t id)
{
  (void) id;
  fs_ready = FALSE;
}

// turn on blue LED after called on kernel painc
void port_halt(void)
{
  port_disable();
  palSetPad(GPIOD, 15); 
  while(TRUE);
}

// called on I2C communication failures with the DAC
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  port_halt();
  return 0;
}

static void pinModeInit()
{
    // setup LED pads
  palSetPadMode(GPIOD, 12, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST); // green LED
  palClearPad(GPIOD, 12); 
  palSetPadMode(GPIOD, 15, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST); // blue LED
  palClearPad(GPIOD, 15); 

  // setup pads to USART2 function
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7) | PAL_STM32_OSPEED_HIGHEST); // TX
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));                                                       // RX

  // setup pads to SPI1 function (connect these pads to your SD card accordingly)
  palSetPadMode(GPIOC, 4, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST); // NSS
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);          // SCK
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));                                                                // MISO
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);          // MOSI
  palSetPad(GPIOC, 4); // set NSS high

  // setup pads to I2C1
  i2cStart(&I2CD1, &i2cfg1);
  palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);
  palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);

  chThdSleepMilliseconds(50);



  set_mpu_sample_rate(9);
  set_mpu_config_regsiter(EXT_SYNC_SET0, DLPF_CFG0);
  set_mpu_gyro(XG_ST_DIS, YG_ST_DIS, ZG_ST_DIS, FS_SEL_250);
  set_mpu_accel(XA_ST_DIS, YA_ST_DIS, ZA_ST_DIS, AFS_SEL_2g, ACCEL_HPF0);
  set_mpu_power_mgmt1(DEVICE_RESET_DIS, SLEEP_DIS, CYCLE_DIS, TEMPERATURE_EN, CLKSEL_XG);
  set_mpu_user_control(USER_FIFO_DIS, I2C_MST_DIS, I2C_IF_DIS, FIFO_RESET_DIS, I2C_MST_RESET_DIS, SIG_COND_RESET_DIS);

  write_mpu_power_mgmt1();
  write_mpu_gyro();
  write_mpu_accel();
  write_mpu_sample_rate();
}

int main(void)
{
  static const evhandler_t evhndl[] = {InsertHandler, RemoveHandler};
  struct EventListener el0, el1;

  // os init
  halInit();
  chSysInit();

  // setup pin mode init
  pinModeInit();

  // initialize MMC driver
  mmcObjectInit(&MMCD1, &SPID1, &ls_spicfg, &hs_spicfg, mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, NULL);

  // it create the DMA stream for SPI3 because ChibiOS has no I2S support yet
  dmaStreamAllocate(SPID3.dmatx, STM32_SPI_SPI3_IRQ_PRIORITY, (stm32_dmaisr_t)I2SDmaTxInterrupt, &SPID3);

  // blink thread; also checks the user button
  chThdCreateStatic(waBlinkThread, sizeof(waBlinkThread), NORMALPRIO, BlinkThread, NULL);

  // Sensor data record
  chThdCreateStatic(waSensorRecordThread, sizeof(waSensorRecordThread), NORMALPRIO, SensorRecordThread, NULL);

  chEvtRegister(&MMCD1.inserted_event, &el0, 0);
  chEvtRegister(&MMCD1.removed_event, &el1, 1);
  while(TRUE)
  {
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  }
}
