// Reference: http://www.chibios.com/forum/viewtopic.php?t=351
// Connection:
// SD card (SPI mode) --> PC4 ((CS), PA5 (SCK), PA6 (MISO), PA7 (MOSI)
// UART --> PA2 (TX), PA3 (RX) [38400 baudrate]

#include "mp3process.h"
#include "MPU60X0.h"
#include "adc_lld.h"

bool_t fs_ready = FALSE;
SPIConfig hs_spicfg = { NULL, GPIOC, 4, 0 };
SPIConfig ls_spicfg = { NULL, GPIOC, 4, SPI_CR1_BR_2 | SPI_CR1_BR_1 };
extern Thread* g_pMp3DecoderThread;

#define SOUND_ADC_GRP1_NUM_CHANNELS 1
#define SOUND_ADC_GRP1_BUF_DEPTH 1
static adcsample_t irSamples[SOUND_ADC_GRP1_NUM_CHANNELS * SOUND_ADC_GRP1_BUF_DEPTH];
static const ADCConversionGroup adcgrpcfg1 = {
  FALSE, // circular buffer mode
  SOUND_ADC_GRP1_NUM_CHANNELS, // number of the analog channels
  NULL, // callback function
  NULL, // error callback
  0, // CR1
  ADC_CR2_SWSTART, // CR2
  ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN14(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN15(ADC_SAMPLE_3),// sample times for channel 10-18
  0,// sample times for channel 0-9
  ADC_SQR1_NUM_CH(SOUND_ADC_GRP1_NUM_CHANNELS),// ADC SQR1 Conversion group sequence 13-16 + sequence length.
  0, // ADC SQR2 Conversion group sequence 7-12
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN14) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN15) // ADC SQR3 Conversion group sequence 1-6
};

void SoundADCinit(void){
   palSetGroupMode(GPIOC, PAL_PORT_BIT(1), 0, PAL_MODE_INPUT_ANALOG); //Pin PC1
   adcStart(&ADCD1, NULL);
   adcConvert(&ADCD1, &adcgrpcfg1, irSamples, SOUND_ADC_GRP1_BUF_DEPTH);
}

void int2str(int n, char *str){
   char buf[10] = "";
   int i = 0;
   int len = 0;
   int temp = n < 0 ? -n: n;
   if(str == NULL){
       return;
   }

   while(temp){
   buf[i++] = (temp % 10) + '0';
   temp = temp / 10;
   }
   len = n < 0 ? ++i: i;
   str[i] = 0;
   while(1){
       i--;
       if(buf[len-i-1] ==0){
           break;
       }
   str[i] = buf[len-i-1];
   }
   if (i == 0){
       str[i] = '-';
   }
}

void chprintf_int(BaseChannel *chp, int data)
{
  char p[100];
  int2str(data, p);
  chprintf(chp, p);
  chprintf((BaseChannel *) &SD2, "\r\n");
}

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
    chThdSleepMilliseconds(1);
    adcConvert(&ADCD1, &adcgrpcfg1, irSamples, SOUND_ADC_GRP1_BUF_DEPTH);
    //chprintf_int((BaseChannel *) &SD2, (uint8_t)irSamples[0]);
    if((uint8_t)irSamples[0] > 150) // sound detected 
    {
      chprintf_int((BaseChannel *) &SD2, (uint8_t)irSamples[0]);
      palSetPad(GPIOD, GPIOD_LED3);
      chThdSleepMilliseconds(200);
      palClearPad(GPIOD, GPIOD_LED3);
    }
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
  SoundADCinit();

  // initialize MMC driver
  mmcObjectInit(&MMCD1, &SPID1, &ls_spicfg, &hs_spicfg, mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, NULL);

  // it create the DMA stream for SPI3 because ChibiOS has no I2S support yet
  dmaStreamAllocate(SPID3.dmatx, STM32_SPI_SPI3_IRQ_PRIORITY, (stm32_dmaisr_t)I2SDmaTxInterrupt, &SPID3);

  // blink thread; also checks the user button
  chThdCreateStatic(waBlinkThread, sizeof(waBlinkThread), NORMALPRIO, BlinkThread, NULL);

  chEvtRegister(&MMCD1.inserted_event, &el0, 0);
  chEvtRegister(&MMCD1.removed_event, &el1, 1);
 
  while(TRUE)
  {
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  }
}
