#include <LiquidCrystal.h>
#include <Servo.h>
//servo
Servo myservo;

//RBG LEDs
//led1
const int RED_PIN1 = 9;
const int GREEN_PIN1 = 10;
const int BLUE_PIN1 = 13;
//led1 alt 
//const int[3] RGB1 = [9,10,13]

//led2
const int RED_PIN2 = 6;
const int GREEN_PIN2 = 7;
const int BLUE_PIN2 = 8;
//led1 alt
//const int[3] RGB2 = [6,7,8]

//this is in millisecond, right?
int DISPLAY_TIME = 100;

//want to turn these into array values are well just rgb?
//int[3] rgbIntensity
int redIntensity;
int greenIntensity;
int blueIntensity;

//Photo Resistor
//naming suggestion photosensorPin
const int sensorPin = 1;

//want to set the levels to constants instead?
int lightLevel, high = 0,med = 511, low = 1023;

//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int temperaturePin = 0;
int val;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  lcd.begin(16, 2);
  lcd.print("Temp in C: ");
  lcd.setCursor(0, 1);
  lcd.print("Temp in F: ");

//we could make this a loop with the arrays set up or keep as is
  pinMode (RED_PIN1, OUTPUT);
  pinMode (GREEN_PIN1, OUTPUT);
  pinMode (BLUE_PIN1, OUTPUT);
  pinMode (RED_PIN2, OUTPUT);
  pinMode (GREEN_PIN2, OUTPUT);
  pinMode (BLUE_PIN2, OUTPUT);
}
float getVoltage(int pin){
  return(analogRead(pin) * 0.004882814);
}
void loop() {
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin);

  degreesC = (voltage - 0.5) * 100.0;
  
  degreesF = degreesC * (9.0/5.0) + 32.0;

  val = degreesC;
  val = map(val, 30, 25, 0, 180);
  myservo.write(val);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(10, 0);
  lcd.print(degreesC);
  lcd.setCursor(10, 1);
  lcd.print(degreesF);

  lightLevel = analogRead(sensorPin);
  autoTune();
  
  if(degreesF > 60){
        //redIntensity = 255; 
         
        digitalWrite(RED_PIN1, HIGH);
        digitalWrite(GREEN_PIN1, LOW);
        digitalWrite(BLUE_PIN1, LOW);
        
        digitalWrite(RED_PIN2, HIGH);
        digitalWrite(GREEN_PIN2, LOW);
        digitalWrite(BLUE_PIN2, LOW);
      }
   
   if(60 > degreesF > 50){
        redIntensity = 255;  
        greenIntensity = 255;
      
        digitalWrite(RED_PIN1, HIGH);
        digitalWrite(GREEN_PIN1, HIGH);
        digitalWrite(BLUE_PIN1, LOW);
        
        digitalWrite(RED_PIN2, HIGH);
        digitalWrite(GREEN_PIN2, HIGH);
        digitalWrite(BLUE_PIN2, LOW);
    }

    if(50 > degreesF > 40){
        greenIntensity = 255;  
        
        digitalWrite(RED_PIN1, LOW);
        digitalWrite(GREEN_PIN1, HIGH);
        digitalWrite(BLUE_PIN1, LOW);
        
        digitalWrite(RED_PIN2, LOW);
        digitalWrite(GREEN_PIN2, HIGH);
        digitalWrite(BLUE_PIN2, LOW);
    }

   if(40 > degreesF ){
    
        blueIntensity = 255;  
        
        digitalWrite(RED_PIN1, LOW);
        digitalWrite(GREEN_PIN1, LOW);
        digitalWrite(BLUE_PIN1, HIGH);
        
        digitalWrite(RED_PIN2, LOW);
        digitalWrite(GREEN_PIN2, LOW);
        digitalWrite(BLUE_PIN2, HIGH);
    }
   
  Serial.print("val: ");
  Serial.print(val);
  Serial.print("  voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.println(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);

  delay(1000);
  
}

void autoTune()
{  
  if (lightLevel < low)
  {
    low = lightLevel;
    redIntensity = 255;  
    greenIntensity = 255;
    blueIntensity = 255;
  }

  if (high > lightLevel > low)
  {
    med = lightLevel;
    redIntensity = 511;  
    greenIntensity = 511;
    blueIntensity = 511;
  }
  
  if (lightLevel > high)
  {
    high = lightLevel;
    redIntensity = 768;  
    greenIntensity = 768;
    blueIntensity = 768;
  }
  
  lightLevel = map(lightLevel, low+30, high-30, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
}
