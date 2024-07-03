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
/*
This example code is in the Public Domain (or CC0 licensed, at your option.)

Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_POWER  true
extern esp_rmaker_device_t *switch_device;
void app_driver_init(void);
int app_driver_set_state(bool state);
bool app_driver_get_state(void);


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

