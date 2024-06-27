#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include "DHT.h"
#include <SimpleTimer.h>
#include <wifi_provisioning/manager.h>
#include <ArduinoJson.h>
#include "realtime.h"

// BLE Credentials
const char *service_name = "PROV_1234566";
const char *pop = "123456756";

// GPIO
static uint8_t gpio_reset = 0;
static uint8_t DHTPIN = 19;

// Variables
bool wifi_connected = false;
const unsigned long sensorInterval = 30000;  // 3 seconds
StaticJsonDocument<200> doc;
String data;
int temp = 0, humi = 0;
short cnt = 0;
int last_temp = 0, last_humi = 0;
const int significantChange = 1;  // Threshold for significant change
int check_but = 0;
bool stt_old_but = 0, stt_new_but = 0;
SimpleTimer Timer;
// Declaring Devices
static TemperatureSensor temperature("Temperature");
static TemperatureSensor humidity("Humidity");

// Wi-Fi Provisioning Event Handling
void sysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("\nConnected to Wi-Fi!");
      wifi_connected = true;
      delay(500);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("\tSSID : ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("\tPassword : ");
      Serial.println((char const *)sys_event->event_info.prov_cred_recv.password);
      break;
    case ARDUINO_EVENT_PROV_INIT:
      wifi_prov_mgr_disable_auto_stop(10000);
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("Stopping Provisioning!!!");
      wifi_prov_mgr_stop_provisioning();
      break;
  }
}

void setup() {

  Serial1.begin(115200, SERIAL_8N1, 18, 17);
  Serial.begin(115200);

  // Configure the input GPIOs
  pinMode(gpio_reset, INPUT);

  // Declaring Node
  Node my_node;
  my_node = RMaker.initNode("AIoT");

  // Adding Devices in Node
  my_node.addDevice(temperature);
  my_node.addDevice(humidity);

  // Optional features
  RMaker.enableOTA(OTA_USING_PARAMS);
  RMaker.enableTZService();
  RMaker.enableSchedule();

  Serial.println("\nStarting ESP-RainMaker");
  RMaker.start();
  Timer.setInterval(30000);
  WiFi.onEvent(sysProvEvent);

#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif
  //Setup_Realtime();
}

void loop() {

  if (Timer.isReady() && wifi_connected) {
    if (abs(temp - last_temp) >= significantChange || abs(humi - last_humi) >= significantChange) {
      //Send_Sensor();
      last_temp = temp;
      last_humi = humi;
    }
    ++cnt;
    Timer.reset();
  }
  if (cnt == 2) {
    Serial.println("Sending Time");
    sendJSONTime(1);
    cnt = 0;
  }

  if (digitalRead(gpio_reset) == HIGH) {
    handleResetButton();
  }

  if (check_but == 1) {  // Khi số lần nhấn là lẻ
    Serial.println("Calling sendJSONTime with 0");
    sendJSONTime(0);
    check_but++;
    // Đặt lại sau khi gọi hàm
  } else if (check_but == 3) {  // Khi số lần nhấn là chẵn
    Serial.println("Calling sendJSONTime with 1");
    sendJSONTime(1);
    check_but = 0;  // Đặt lại sau khi gọi hàm
  }
  //Receive_Data();
}

void handleResetButton() {
  Serial.println("Reset Button Pressed!");
  delay(100);  // Debounce delay
  int startTime = millis();
  while (digitalRead(gpio_reset) == HIGH) delay(50);
  int endTime = millis();
  
  if ((endTime - startTime) > 10000) {
    Serial.println("Reset to factory.");
    wifi_connected = false;
    RMakerFactoryReset(2);
  }
  else if ((endTime - startTime) > 3000) {
    Serial.println("Reset Wi-Fi.");
    wifi_connected = false;
    RMakerWiFiReset(2);
  } 
  else if ((endTime - startTime) > 500) {
    if (Check_But()) {
      ++check_but;
    }
  }
}

void Receive_Data() {
  if (Serial1.available() > 0) {
    data = Serial1.readString();
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
    }

    temp = doc["Temperature"].as<int>();
    humi = doc["Humidity"].as<int>();
  }
}

void Send_Sensor() {
  float h = humi;
  float t = temp;

  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);

  temperature.updateAndReportParam("Temperature", t);
  humidity.updateAndReportParam("Temperature", h);
}
bool Check_But() {
  stt_old_but = stt_new_but;
  stt_new_but = digitalRead(gpio_reset);
  if (stt_old_but == 0 && stt_new_but == 1) {
    return 1;
  }
}