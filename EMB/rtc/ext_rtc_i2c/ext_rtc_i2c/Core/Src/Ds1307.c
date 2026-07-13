#include "Ds1307.h"
#include "main.h"

#define DS1307_REG_POINTER_ADDRESS (0x00U)
#define DS1307_SET_TIME_BUFFER_SIZE (9U)
#define DS1307_GET_TIME_BUFFER_SIZE (9U)
#define DS1307_I2C_TIMEOUT_MS (100U)

extern I2C_HandleTypeDef hi2c3;

static uint8_t dec_to_bcd(uint8_t dec);
static uint8_t bcd_to_dec(uint8_t bcd);

static uint8_t dec_to_bcd(uint8_t dec)
{
    return (uint8_t)(((dec / 10U) << 4) | (dec % 10U));
}

static uint8_t bcd_to_dec(uint8_t bcd)
{
    return (uint8_t)(((bcd >> 4) * 10U) + (bcd & 0x0FU));
}

Ds1307_Status_t Ds1307_SetTime(const Ds1307_Time_t *time)
{
    uint8_t tx_buffer[DS1307_SET_TIME_BUFFER_SIZE];
    HAL_StatusTypeDef hal_status;

    tx_buffer[0] = DS1307_REG_POINTER_ADDRESS;
    tx_buffer[1] = dec_to_bcd(time->seconds);
    tx_buffer[2] = dec_to_bcd(time->minutes);
    tx_buffer[3] = dec_to_bcd(time->hours);
    tx_buffer[4] = dec_to_bcd(time->day_of_week);
    tx_buffer[5] = dec_to_bcd(time->date);
    tx_buffer[6] = dec_to_bcd(time->month);
    tx_buffer[7] = dec_to_bcd(time->year);
    tx_buffer[8] = 0x00U;

    hal_status = HAL_I2C_Master_Transmit(&hi2c3, DS1307_I2C_ADDRESS, tx_buffer,
                                         DS1307_SET_TIME_BUFFER_SIZE,
                                         DS1307_I2C_TIMEOUT_MS);

    if (HAL_OK != hal_status)
    {
        return DS1307_ERROR;
    }

    return DS1307_OK;
}

Ds1307_Status_t Ds1307_GetTime(Ds1307_Time_t *time)
{
    uint8_t reg_pointer;
    uint8_t rx_buffer[DS1307_GET_TIME_BUFFER_SIZE];
    HAL_StatusTypeDef hal_status;

    reg_pointer = DS1307_REG_POINTER_ADDRESS;

    hal_status = HAL_I2C_Master_Transmit(
        &hi2c3, DS1307_I2C_ADDRESS, &reg_pointer, 1U, DS1307_I2C_TIMEOUT_MS);

    if (HAL_OK != hal_status)
    {
        return DS1307_ERROR;
    }

    hal_status = HAL_I2C_Master_Receive(&hi2c3, DS1307_I2C_ADDRESS, rx_buffer,
                                        DS1307_GET_TIME_BUFFER_SIZE,
                                        DS1307_I2C_TIMEOUT_MS);

    if (HAL_OK != hal_status)
    {
        return DS1307_ERROR;
    }

    time->seconds = bcd_to_dec((uint8_t)(rx_buffer[0] & 0x7FU));
    time->minutes = bcd_to_dec(rx_buffer[1]);
    time->hours = bcd_to_dec((uint8_t)(rx_buffer[2] & 0x3FU));
    time->day_of_week = bcd_to_dec(rx_buffer[3]);
    time->date = bcd_to_dec(rx_buffer[4]);
    time->month = bcd_to_dec(rx_buffer[5]);
    time->year = bcd_to_dec(rx_buffer[6]);

    return DS1307_OK;
}

Ds1307_Status_t Ds1307_IsTimeSet(uint8_t *is_set)
{
    uint8_t reg_pointer;
    uint8_t seconds_register;
    HAL_StatusTypeDef hal_status;

    reg_pointer = DS1307_REG_POINTER_ADDRESS;

    hal_status = HAL_I2C_Master_Transmit(
        &hi2c3, DS1307_I2C_ADDRESS, &reg_pointer, 1U, DS1307_I2C_TIMEOUT_MS);

    if (HAL_OK != hal_status)
    {
        return DS1307_ERROR;
    }

    hal_status =
        HAL_I2C_Master_Receive(&hi2c3, DS1307_I2C_ADDRESS, &seconds_register,
                               1U, DS1307_I2C_TIMEOUT_MS);

    if (HAL_OK != hal_status)
    {
        return DS1307_ERROR;
    }

    if (0U == (seconds_register & 0x80U))
    {
        *is_set = 1U;
    }
    else
    {
        *is_set = 0U;
    }

    return DS1307_OK;
}