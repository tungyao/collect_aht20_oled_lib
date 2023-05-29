#include "stm32f4xx_hal.h"

#define I2C_TIMEOUT 1000

GPIO_InitTypeDef GPIO_InitStruct;
TIM_HandleTypeDef htim;

void I2C_Init(void) {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_TIM_Base_Start(&htim);
}

void I2C_Start(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_Delay(1);
}

void I2C_Stop(void) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_Delay(1);
}

void I2C_WriteBit(uint8_t bit) {
  if (bit) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
  }
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

uint8_t I2C_ReadBit(void) {
  uint8_t bit;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_Delay(1);
  bit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_Delay(1);
  return bit;
}

uint8_t I2C_WriteByte(uint8_t byte) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    I2C_WriteBit((byte & (1 << (7 - i))));
  }
  return I2C_ReadBit();
}

uint8_t I2C_ReadByte(uint8_t ack) {
  uint8_t i;
  uint8_t byte = 0;
  for (i = 0; i < 8; i++) {
    byte |= (I2C_ReadBit() << (7 - i));
  }
  I2C_WriteBit(ack);
  return byte;
}
