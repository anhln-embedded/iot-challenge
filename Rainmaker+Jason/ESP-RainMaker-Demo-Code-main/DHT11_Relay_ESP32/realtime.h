#ifndef __REALTIME_H__
#define __REALTIME_H__

#include "WiFi.h"
#include "time.h"
#include "sntp.h"
#include "ArduinoJson.h"

extern const char* ssid;
extern const char* password;
extern const char* ntpServer1;
extern const char* ntpServer2;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

extern void Setup_Realtime();
extern void printLocalTime();
extern void sendJSONTime(bool a);
extern void timeavailable(struct timeval* t);



#endif