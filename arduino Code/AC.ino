#include <Servo.h>

                                                                                                                                                                                                                                                                                                                                     int temp_value;
float cel = 0;
int tempPin = 1;

int light_value;
int light = 0;
int lightPin = 0;

int on_off_value;
int on_off_pin = 9;

int measurePin = 3;
int ledPower = 13;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity_temp = 0;
float dustDensity = 0;

int loop_cnt = 0;

Servo servo1;
Servo servo2;
int servoStatus;

void setup() {
  Serial.begin(9600);
  pinMode(on_off_pin, INPUT);
  pinMode(ledPower, OUTPUT);
  servo1.attach(11);
  servo2.attach(10);
}

void loop()
{
   if(Serial.available()){
    String str = Serial.readStringUntil('\n');
    int j = 0;
    if(str.startsWith("OFF") && on_off_value == 0){
      for(int i = 90; i>0;i--){
        j = 90- i; 
        servo1.write(i);
        servo2.write(j);
        delay(10);  
      }
    }else if(str.startsWith("ON") && on_off_value == 1){
      for(int i = 0; i<91;i++){
        j = 90 - i;
        servo1.write(i);
        servo2.write(j);
        delay(10);
      }
    }
   }
   
  temp_value = analogRead(tempPin);
  float mv = ( temp_value / 1024.0) * 5000;
  cel += mv / 10;

  
  light_value = analogRead(lightPin);
  light += light_value * 0.0976;

  
  on_off_value = digitalRead(on_off_pin);
//  if (on_off_value == HIGH) {
//    servo.write(0);
//  } else {
//    servo.write(120);
//  }

  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity_temp = (0.17 * calcVoltage - 0.1) * 1000;
  if(dustDensity_temp < 20) dustDensity_temp = 0;
  dustDensity += dustDensity_temp;


  loop_cnt++;
  if(loop_cnt == 10){
    cel = cel / 10;
    light = light / 10;
    dustDensity = dustDensity/ 10;
    loop_cnt = 0;

    Serial.print("/TEMPRATURE:");
    Serial.print(cel, 1);
    Serial.print(" ");
    Serial.print("/LIGHT:");
    Serial.print(light);
    Serial.print(" ");
    Serial.print("/WINDOWSTATUS:");
    Serial.print(on_off_value);
    Serial.print(" ");
    Serial.print("/DUSTDENSITY:");
    Serial.print(dustDensity);
    Serial.print(" ");
    Serial.println();
  }
  delay(900);
 
}
