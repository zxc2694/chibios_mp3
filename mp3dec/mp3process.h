#ifndef _MP3PROCESS_H
#define _MP3PROCESS_H

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include <ff.h>
#include <string.h>
#include "mp3dec.h"
#include "codec.h"

#define MP3_INBUF_SIZE 4096
#define MP3_DMA_BUFFER_SIZE (4 * MAX_NCHAN * MAX_NGRAN * MAX_NSAMP)

FATFS MMC_FS;
MMCDriver MMCD1;

extern Thread* g_pMp3DecoderThread;

bool_t mmc_is_inserted(void);
bool_t mmc_is_protected(void);
FRESULT Mp3PlayAllFiles(char* path);
uint32_t Mp3ReadId3V2Text(FIL* pInFile, uint32_t unDataLen, char* pszBuffer, uint32_t unBufferSize);
uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);
UINT Mp3FillReadBuffer(BYTE* pInData, UINT unInDataLeft, FIL* pInFile);
int Mp3Decode(const char* pszFile);


#endif	