void setup() {
  Serial.begin(115200);
  pinMode(0,INPUT_PULLUP);

}

void loop() {
  Serial.println(digitalRead(0));
  delay(500);

}
// chao moi nguoi