#include "realtime.h"

const char* ssid = "AIoT Lab 2G";
const char* password = "ptitlab@123";
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 21600;
const int daylightOffset_sec = 3600;

void Setup_Realtime() {
  sntp_set_time_sync_notification_cb(timeavailable);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  char timeString[50];
  strftime(timeString, sizeof(timeString), "%a, %b %d %Y %H:%M:%S", &timeinfo);  // Thêm giây
  Serial.println(timeString);
}

void sendJSONTime(bool a) {
  struct tm timeinfo;
  DynamicJsonDocument doc(1024);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  // Định dạng chuỗi thời gian
  char timeString[50];
  strftime(timeString, sizeof(timeString), "%a %b %d %Y %H:%M:%S", &timeinfo);  // Thêm giây

  // Tách từng phần tử của chuỗi thời gian
  char dayOfWeek[4];
  char month[4];
  int day;
  int year;
  int hour;
  int minutes;
  int seconds;
  sscanf(timeString, "%s %s %d %d %d:%d:%d", dayOfWeek, month, &day, &year, &hour, &minutes, &seconds);

  // Đóng gói vào JSON
  doc["Button"] = String(a);
  doc["Signal"] = "1";
  doc["Weekday"] = String(dayOfWeek);
  doc["Date"] = String(day);
  doc["Month"] = String(month);
  doc["Year"] = String(year);
  doc["Hour"] = String(hour);
  doc["Minutes"] = String(minutes);
  doc["Seconds"] = String(seconds);

  // In JSON ra serial
  serializeJson(doc,Serial1);
  Serial.println();
  serializeJson(doc,Serial);
  Serial.println();
}

// Callback function (get's called when time adjusts via NTP) 
void timeavailable(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}