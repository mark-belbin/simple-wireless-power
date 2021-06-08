const int HBridge_PWM = 2; //Add pins here
const int HBridge_EN = 5;
const int ISEN = A4;

float current = 0;

void setup() {
  pinMode(HBridge_PWM, OUTPUT);
  pinMode(HBridge_EN, OUTPUT);
  
  delay(1000);  //Wait after powering on.

  /*//Optional Printing
  Serial.println("Starting WTX in 3");
  delay(1);  
  Serial.print("...2");
  delay(1);  
  Serial.print("...1");
  delay(1);  
  Serial.print("....POW");*/

  digitalWrite(HBridge_EN, HIGH);
  tone(HBridge_PWM, 100000); // 100kHz default
}

void loop() {
 //Do Nothing
}
