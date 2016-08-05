# chibios_mp3

## Explanation 

1. The project references http://www.chibios.com/forum/viewtopic.php?t=351, which is compiled from Eclipse. Then I modify the file path and rewrite makefile for building on Linux.

2. This is a MP3 player, using STM32F4-Discovery board providing a audio speaker. The project uses RTOS named ChibiOS, and music are from SD card. In addition, the communication from SD card to STM32F4 uses SPI interface instead of SDIO.

##The pin connection 
        [SD Card]      [STM32F4]
          MISO  ----->    PA6
          MOSI  ----->    PA7
          SCK   ----->    PA5
          NSS   ----->    PC4
     
