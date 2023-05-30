#include "stm32f4xx_hal.h"
#include <stdio.h>

#define AHT20_ADDRESS 0x38
#define AHT20_CMD_INIT 0b1110001
#define AHT20_CMD_TRIGGER 0b10101100
#define AHT20_CMD_SOFTRESET 0b10110110

// I2C_HandleTypeDef hi2c;

// void I2C_Init(void) {
//   hi2c.Instance = I2C1;
//   hi2c.Init.ClockSpeed = 100000;
//   hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
//   hi2c.Init.OwnAddress1 = 0;
//   hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c.Init.OwnAddress2 = 0;
//   hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   if (HAL_I2C_Init(&hi2c) != HAL_OK) {
//     Error_Handler();
//   }
// }

void AHT20_Init(void) {
  uint8_t cmd[] = {AHT20_CMD_INIT, 0x08, 0x00};
  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS << 1, cmd, sizeof(cmd), HAL_MAX_DELAY);
}

void AHT20_TriggerMeasurement(void) {
  uint8_t cmd[] = {AHT20_CMD_TRIGGER, 0x33, 0x00};
  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS << 1, cmd, sizeof(cmd), HAL_MAX_DELAY);
}

float AHT20_ReadTemperature(void) {
  uint8_t data[6];
  float temp;

  AHT20_TriggerMeasurement();
  HAL_Delay(100);

  HAL_I2C_Master_Receive(&hi2c, AHT20_ADDRESS << 1, data, sizeof(data), HAL_MAX_DELAY);

  temp = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5];
  temp /= (1 << 20);
  temp *= 200;
  temp -= 50;

  return temp;
}

float AHT20_ReadHumidity(void) {
  uint8_t data[6];
  float humidity;

  AHT20_TriggerMeasurement();
  HAL_Delay(100);

  HAL_I2C_Master_Receive(&hi2c, AHT20_ADDRESS << 1, data, sizeof(data), HAL_MAX_DELAY);

  humidity = (data[1] << 12) | (data[2] << 4) | (data[3] >> 4);
  humidity /= (1 << 20);
  humidity *= 100;

  return humidity;
}

int main(void) {
  float temperature, humidity;

  HAL_Init();
  I2C_Init();
  
  AHT20_Init();

  while (1) {
    temperature = AHT20_ReadTemperature();
    humidity = AHT20_ReadHumidity();
    char Temperature[20];
    sprintf(Temperature, "%.2f", temperature);
    char Humid[20];
    sprintf(Humid, "%.2f", humidity);

    HAL_Delay(1000);
  }
}
