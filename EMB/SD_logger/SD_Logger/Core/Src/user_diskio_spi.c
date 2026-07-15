#include "user_diskio_spi.h"
#include "main.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

#define CS_LOW() HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET)

#define CT_MMC (0x01U)
#define CT_SD1 (0x02U)
#define CT_SD2 (0x04U)
#define CT_SDC (CT_SD1 | CT_SD2)
#define CT_BLOCK (0x08U)

#define CMD0 (0)
#define CMD1 (1)
#define CMD8 (8)
#define CMD9 (9)
#define CMD12 (12)
#define CMD16 (16)
#define CMD17 (17)
#define CMD18 (18)
#define CMD24 (24)
#define CMD25 (25)
#define CMD55 (55)
#define CMD58 (58)
#define ACMD23 (23 | 0x80)
#define ACMD41 (41 | 0x80)

#define TOKEN_SINGLE (0xFE)
#define TOKEN_MULTI (0xFC)
#define TOKEN_STOP (0xFD)

static volatile DSTATUS Stat = STA_NOINIT;
static BYTE CardType = 0;

/* ---------------------------------------------------------------------- */
/* Byte / block level SPI transfers                                       */
/* ---------------------------------------------------------------------- */

static void SPI_TxByte(BYTE data)
{
    HAL_SPI_Transmit(HSPI_SDCARD, &data, 1, SPI_TIMEOUT);
}

static BYTE SPI_RxByte(void)
{
    BYTE data;

    data = 0xFF;
    HAL_SPI_TransmitReceive(HSPI_SDCARD, &data, &data, 1, SPI_TIMEOUT);

    return (data);
}

static void SPI_TxBuffer(const BYTE *buff, UINT len)
{
    HAL_SPI_Transmit(HSPI_SDCARD, (BYTE *)buff, (uint16_t)len, SPI_TIMEOUT);
}

static void SPI_RxBuffer(BYTE *buff, UINT len)
{
    /* fill with 0xFF first: this doubles as both the dummy TX data and the
       RX destination in one blocking full-duplex call */
    memset(buff, 0xFF, len);
    HAL_SPI_TransmitReceive(HSPI_SDCARD, buff, buff, (uint16_t)len,
                            SPI_TIMEOUT);
}

/* ---------------------------------------------------------------------- */
/* Timeout helper (HAL_GetTick based)                                     */
/* ---------------------------------------------------------------------- */

static BYTE SD_WaitReady(uint32_t timeout_ms)
{
    uint32_t start;
    BYTE res;

    start = HAL_GetTick();

    do
    {
        res = SPI_RxByte();
    } while ((0xFF != res) && ((HAL_GetTick() - start) < timeout_ms));

    return (res);
}

/* ---------------------------------------------------------------------- */
/* Data block transfer                                                    */
/* ---------------------------------------------------------------------- */

static BYTE SD_RxDataBlock(BYTE *buff, UINT len)
{
    BYTE token;
    uint32_t start;

    start = HAL_GetTick();

    do
    {
        token = SPI_RxByte();
    } while ((0xFF == token) && ((HAL_GetTick() - start) < SPI_TIMEOUT));

    if (TOKEN_SINGLE != token)
    {
        return (0);
    }

    SPI_RxBuffer(buff, len);

    /* CRC is always disabled in SPI mode, discard the 2 trailing bytes */
    (void)SPI_RxByte();
    (void)SPI_RxByte();

    return (1);
}

static BYTE SD_TxDataBlock(const BYTE *buff, BYTE token)
{
    BYTE resp;

    if (0xFF != SD_WaitReady(SPI_TIMEOUT))
    {
        return (0);
    }

    SPI_TxByte(token);

    if (TOKEN_STOP == token)
    {
        return (1);
    }

    SPI_TxBuffer(buff, 512);

    /* dummy CRC, ignored by the card in SPI mode */
    SPI_TxByte(0xFF);
    SPI_TxByte(0xFF);

    resp = SPI_RxByte();

    if (0x05 != (resp & 0x1F))
    {
        return (0);
    }

    if (0xFF != SD_WaitReady(500))
    {
        return (0);
    }

    return (1);
}

/* ---------------------------------------------------------------------- */
/* Command layer                                                          */
/* ---------------------------------------------------------------------- */

static BYTE SD_SendCmd(BYTE cmd, DWORD arg)
{
    BYTE n;
    BYTE res;

    if (0x80 == (cmd & 0x80))
    {
        /* ACMD: send the CMD55 prefix first */
        cmd = (BYTE)(cmd & 0x7F);
        res = SD_SendCmd(CMD55, 0);

        if (1 < res)
        {
            return (res);
        }
    }

    if (CMD12 != cmd)
    {
        CS_HIGH();
        (void)SPI_RxByte();
        CS_LOW();

        if (0xFF != SD_WaitReady(SPI_TIMEOUT))
        {
            return (0xFF);
        }
    }

    SPI_TxByte((BYTE)(0x40 | cmd));
    SPI_TxByte((BYTE)(arg >> 24));
    SPI_TxByte((BYTE)(arg >> 16));
    SPI_TxByte((BYTE)(arg >> 8));
    SPI_TxByte((BYTE)arg);

    n = 0x01;

    if (CMD0 == cmd)
    {
        n = 0x95;
    }

    if (CMD8 == cmd)
    {
        n = 0x87;
    }

    SPI_TxByte(n);

    if (CMD12 == cmd)
    {
        (void)SPI_RxByte();
    }

    n = 10;

    do
    {
        res = SPI_RxByte();
    } while ((0x80 == (res & 0x80)) && (0U != --n));

    return (res);
}

/* ---------------------------------------------------------------------- */
/* Public diskio API                                                      */
/* ---------------------------------------------------------------------- */

DSTATUS USER_SPI_initialize(BYTE pdrv)
{
    BYTE n;
    BYTE cmd;
    BYTE ocr[4];
    uint32_t start;

    if (0 != pdrv)
    {
        return (STA_NOINIT);
    }

    CS_HIGH();

    /* >= 74 dummy clocks with CS held high before addressing the card */
    for (n = 0; n < 10; n++)
    {
        SPI_TxByte(0xFF);
    }

    CardType = 0;

    if (1 == SD_SendCmd(CMD0, 0))
    {
        start = HAL_GetTick();

        if (1 == SD_SendCmd(CMD8, 0x1AA))
        {
            /* SD ver.2+: read the trailing R7 payload */
            for (n = 0; n < 4; n++)
            {
                ocr[n] = SPI_RxByte();
            }

            if ((0x01 == ocr[2]) && (0xAA == ocr[3]))
            {
                while (((HAL_GetTick() - start) < 1000) &&
                       (0 != SD_SendCmd(ACMD41, 0x40000000)))
                {
                    /* poll until the card leaves idle state */
                }

                if (((HAL_GetTick() - start) < 1000) &&
                    (0 == SD_SendCmd(CMD58, 0)))
                {
                    for (n = 0; n < 4; n++)
                    {
                        ocr[n] = SPI_RxByte();
                    }

                    /* bit 30 of the OCR reports block (SDHC/SDXC) addressing */
                    CardType = (0x40 == (ocr[0] & 0x40))
                                   ? (BYTE)(CT_SD2 | CT_BLOCK)
                                   : (BYTE)CT_SD2;
                }
            }
        }
        else
        {
            /* card did not answer CMD8: SD ver.1 or MMC */
            if (1 >= SD_SendCmd(ACMD41, 0))
            {
                CardType = CT_SD1;
                cmd = ACMD41;
            }
            else
            {
                CardType = CT_MMC;
                cmd = CMD1;
            }

            while (((HAL_GetTick() - start) < 1000) &&
                   (0 != SD_SendCmd(cmd, 0)))
            {
                /* poll until the card leaves idle state */
            }

            if (((HAL_GetTick() - start) >= 1000) ||
                (0 != SD_SendCmd(CMD16, 512)))
            {
                CardType = 0;
            }
        }
    }

    CS_HIGH();
    (void)SPI_RxByte();

    if (0 != CardType)
    {
        Stat &= (DSTATUS)(~STA_NOINIT);
    }

    return (Stat);
}

DSTATUS USER_SPI_status(BYTE pdrv)
{
    if (0 != pdrv)
    {
        return (STA_NOINIT);
    }

    return (Stat);
}

DRESULT USER_SPI_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    if ((0 != pdrv) || (0 == count))
    {
        return (RES_PARERR);
    }

    if (0 != (Stat & STA_NOINIT))
    {
        return (RES_NOTRDY);
    }

    if (0 == (CardType & CT_BLOCK))
    {
        /* byte addressing (SDSC): convert LBA to a byte offset */
        sector *= 512;
    }

    CS_LOW();

    if (1 == count)
    {
        if ((0 == SD_SendCmd(CMD17, sector)) &&
            (1 == SD_RxDataBlock(buff, 512)))
        {
            count = 0;
        }
    }
    else
    {
        if (0 == SD_SendCmd(CMD18, sector))
        {
            do
            {
                if (0 == SD_RxDataBlock(buff, 512))
                {
                    break;
                }

                buff += 512;
            } while (0U != --count);

            (void)SD_SendCmd(CMD12, 0);
        }
    }

    CS_HIGH();
    (void)SPI_RxByte();

    return (0 == count) ? RES_OK : RES_ERROR;
}

DRESULT USER_SPI_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    if ((0 != pdrv) || (0 == count))
    {
        return (RES_PARERR);
    }

    if (0 != (Stat & STA_NOINIT))
    {
        return (RES_NOTRDY);
    }

    if (0 == (CardType & CT_BLOCK))
    {
        sector *= 512;
    }

    CS_LOW();

    if (1 == count)
    {
        if ((0 == SD_SendCmd(CMD24, sector)) &&
            (1 == SD_TxDataBlock(buff, TOKEN_SINGLE)))
        {
            count = 0;
        }
    }
    else
    {
        if (0 != (CardType & CT_SDC))
        {
            (void)SD_SendCmd(ACMD23, count);
        }

        if (0 == SD_SendCmd(CMD25, sector))
        {
            do
            {
                if (0 == SD_TxDataBlock(buff, TOKEN_MULTI))
                {
                    break;
                }

                buff += 512;
            } while (0U != --count);

            if (0 == SD_TxDataBlock(NULL, TOKEN_STOP))
            {
                count = 1;
            }
        }
    }

    CS_HIGH();
    (void)SPI_RxByte();

    return (0 == count) ? RES_OK : RES_ERROR;
}

DRESULT USER_SPI_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT res;
    BYTE n;
    BYTE csd[16];
    DWORD csize;

    if (0 != pdrv)
    {
        return (RES_PARERR);
    }

    if (0 != (Stat & STA_NOINIT))
    {
        return (RES_NOTRDY);
    }

    res = RES_ERROR;

    switch (cmd)
    {
    case CTRL_SYNC:

        CS_LOW();

        if (0xFF == SD_WaitReady(SPI_TIMEOUT))
        {
            res = RES_OK;
        }

        CS_HIGH();
        (void)SPI_RxByte();
        break;

    case GET_SECTOR_COUNT:

        CS_LOW();

        if ((0 == SD_SendCmd(CMD9, 0)) && (1 == SD_RxDataBlock(csd, 16)))
        {
            if (1 == (csd[0] >> 6))
            {
                /* CSD version 2.0 (SDHC/SDXC) */
                csize = ((DWORD)(csd[7] & 0x3F) << 16) + ((DWORD)csd[8] << 8) +
                        csd[9] + 1;
                *(DWORD *)buff = csize << 10;
            }
            else
            {
                /* CSD version 1.0 (SDSC) */
                n = (BYTE)((csd[5] & 15) + ((csd[10] & 128) >> 7) +
                           ((csd[9] & 3) << 1) + 2);
                csize = ((DWORD)(csd[6] & 3) << 10) + ((DWORD)csd[7] << 2) +
                        ((csd[8] & 0xC0) >> 6) + 1;
                *(DWORD *)buff = csize << (n - 9);
            }

            res = RES_OK;
        }

        CS_HIGH();
        (void)SPI_RxByte();
        break;

    case GET_BLOCK_SIZE:

        *(DWORD *)buff = 512;
        res = RES_OK;
        break;

    default:

        res = RES_PARERR;
        break;
    }

    return (res);
}