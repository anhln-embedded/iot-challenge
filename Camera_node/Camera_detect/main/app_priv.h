/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


/*
  ******************************************************************************
  * @file		abc.h                                                              *
  * @author		Luu Ngoc Anh                                                       *
  * @date		00/00/0000                                                         *
  ******************************************************************************
*/

#ifndef __APP_PRIV__
#define __APP_PRIV__

#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_POWER       true
#define DEFAULT_HUE         180
#define DEFAULT_SATURATION  100
#define DEFAULT_BRIGHTNESS  25

extern esp_rmaker_device_t *light_device;

void app_driver_init(void);
esp_err_t app_light_set(uint32_t hue, uint32_t saturation, uint32_t brightness);
esp_err_t app_light_set_power(bool power);
esp_err_t app_light_set_brightness(uint16_t brightness);
esp_err_t app_light_set_hue(uint16_t hue);
esp_err_t app_light_set_saturation(uint16_t saturation);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

