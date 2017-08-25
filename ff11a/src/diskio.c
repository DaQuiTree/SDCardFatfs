/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdio_sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

#define Block_Size 512
extern SD_CardInfo SDCardInfo;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	SD_Error Status;

	Status=SD_Init();

	if(Status!=SD_OK)
	{
		return STA_NOINIT;
	}
	else
	{
		return RES_OK;
	}	
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if(count>1)
	{
	   SD_ReadMultiBlocks(buff,sector*Block_Size,Block_Size,count);
	   //等待DMA传输结束
	   SD_WaitReadOperation();
	   //等待SDIO到SD卡传输结束
	   while(SD_GetStatus()!=SD_TRANSFER_OK);
	}
	else
	{
	   SD_ReadBlock(buff,sector*Block_Size,Block_Size);
	   //等待DMA传输结束
	   SD_WaitReadOperation();
	   //等待SDIO到SD卡传输结束
	   while(SD_GetStatus()!=SD_TRANSFER_OK);
	}

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{	
	if(count>1)
	{
	   SD_WriteMultiBlocks((uint8_t *)buff,sector*Block_Size,Block_Size,count);
	   //等待DMA传输结束
	   SD_WaitWriteOperation();
	   //等待SDIO到SD卡传输结束
	   while(SD_GetStatus()!=SD_TRANSFER_OK);
	}
	else
	{
	   SD_WriteBlock((uint8_t *)buff,sector*Block_Size,Block_Size);
	   //等待DMA传输结束
	   SD_WaitWriteOperation();
	   //等待SDIO到SD卡传输结束
	   while(SD_GetStatus()!=SD_TRANSFER_OK);
	}

	return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch(cmd)
	{
		case CTRL_SYNC:
			res=RES_OK;
		break;
		case GET_SECTOR_SIZE:
		{
			*(DWORD*)buff=512;
			res=RES_OK;
		}
		break;
		case GET_SECTOR_COUNT:
		{
			 *(DWORD*)buff=SDCardInfo.CardCapacity/512;
			 res=RES_OK;
		}
		break;
		case GET_BLOCK_SIZE:
		{
		   *(DWORD*)buff=SDCardInfo.CardBlockSize;
		   res=RES_OK;
		}
		break;
		default:
		   res=RES_PARERR;
		break;
	}

	return res;
}
#endif

DWORD get_fattime(void)
{
	return 0;
}
