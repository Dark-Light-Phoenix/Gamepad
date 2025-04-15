#include "ADC.h"
#include "HID.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;
extern GamepadReport_TypeDef gamepad_report;

volatile uint16_t min_even1_val = 4096, max_even1_val = 0;
volatile uint16_t min_odd1_val = 4096, max_odd1_val = 0;

volatile uint16_t min_even2_val = 4096, max_even2_val = 0;
volatile uint16_t min_odd2_val = 4096, max_odd2_val = 0;

volatile uint16_t min_central_zone = MID_ZONE - DEAD_ZONE;
volatile uint16_t max_central_zone = MID_ZONE + DEAD_ZONE;

static uint16_t adc_buffer1[2];
static uint16_t adc_buffer2[2];

int16_t adc_x1, adc_x2;
int16_t adc_y1, adc_y2;

volatile bool calibration_done = false;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, 2);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, 2);

	HAL_TIM_Base_Start (&htim6);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1 || hadc->Instance == ADC2)
	{
		if(!calibration_done)
		{
			Update_Range();
		}
	}
	ADC_Scale();
}

void Update_Range (void)
{
    while (max_even1_val - min_even1_val < TRESHOLD)
    {
        uint16_t x1_value = adc_buffer1[0];

        if (x1_value < min_even1_val) min_even1_val = x1_value;
        if (x1_value > max_even1_val) max_even1_val = x1_value;
    }

    while (max_odd1_val - min_odd1_val < TRESHOLD)
    {
    	uint16_t y1_value = adc_buffer1[1];

        if (y1_value < min_odd1_val)  min_odd1_val  = y1_value;
        if (y1_value > max_odd1_val)  max_odd1_val  = y1_value;
    }

    while (max_even2_val - min_even2_val < TRESHOLD)
    {
        uint16_t x2_value = adc_buffer2[0];

        if (x2_value < min_even2_val) min_even2_val = x2_value;
        if (x2_value > max_even2_val) max_even2_val = x2_value;
    }

    while (max_odd2_val - min_odd2_val < TRESHOLD)
    {
        uint16_t y2_value = adc_buffer2[1];

        if (y2_value < min_odd2_val)  min_odd2_val  = y2_value;
        if (y2_value > max_odd2_val)  max_odd2_val  = y2_value;
    }
}

void ADC_Filtering (uint16_t value, volatile uint16_t min_value, volatile uint16_t max_value, int16_t *adc_value)
{
	if (value >= min_central_zone && value <= max_central_zone)
	{
		*adc_value = 0;
		return;
	}

	if (max_value == min_value)
	{
		*adc_value = 0;
	} else {
		if ((value >= min_value) && (value < min_central_zone))
		{
			*adc_value = (((int32_t)(value - min_value) * 2048) / (max_value - min_value)) - 1024;
			if (*adc_value > 1024)
			{
				*adc_value = 1024;
			} else if (*adc_value < -1024)
			{
				*adc_value = -1024;
			}
			return;
		}

		if ((value > max_central_zone) && (value <= max_value))
		{
			*adc_value = ((((int32_t)(value - min_value) * 2048) / (max_value - min_value))) - 1024;
			if (*adc_value > 1024)
			{
				*adc_value = 1024;
			} else if (*adc_value < -1024)
			{
				*adc_value = -1024;
			}
			return;
		}

		if (value < min_value)
		{
			*adc_value = ((int32_t)(min_value * 2048) / (max_value - min_value)) - 1024;
			if (*adc_value > 1024)
			{
				*adc_value = 1024;
			} else if (*adc_value < -1024)
			{
				*adc_value = -1024;
			}
			return;
		}

		if (value > max_value)
		{
			*adc_value = ((int32_t)(max_value * 2048) / (max_value - min_value)) - 1024;
			if (*adc_value > 1024)
			{
				*adc_value = 1024;
			} else if (*adc_value < -1024)
			{
				*adc_value = -1024;
			}
			return;
		}
	}
}

void ADC_Scale (void)
{
	uint16_t x1_value = adc_buffer1[0];
	uint16_t y1_value = adc_buffer1[1];
	uint16_t x2_value = adc_buffer2[0];
	uint16_t y2_value = adc_buffer2[1];

	ADC_Filtering (x1_value, min_even1_val, max_even1_val, &adc_x1);
	ADC_Filtering (y1_value, min_odd1_val, max_odd1_val, &adc_y1);
	ADC_Filtering (x2_value, min_even2_val, max_even2_val, &adc_x2);
	ADC_Filtering (y2_value, min_odd2_val, max_odd2_val, &adc_y2);

	adc_x1 = -adc_x1;
	adc_y2 = -adc_y2;

	gamepad_report.x1 = adc_x1;
	gamepad_report.y1 = adc_y1;
	gamepad_report.x2 = adc_x2;
	gamepad_report.y2 = adc_y2;
}
