#include <LiquidCrystal.h>
#include <Adafruit_RGBLCDShield.h>

// Declare lcd object
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// enum of Backlight colors
enum BackLightColor { RED=0x1, YELLOW=0x3, GREEN=0x2, TEAL=0x6, BLUE=0x4, VIOLET=0x5, WHITE=0x7 };

// current button clicked
uint8_t button;

/**
 * setup code that runs once
 */
void setup() {
  lcd.begin(16, 2);
  opening_display();
}

/**
 * loop continuously while program is running
 */
void loop() {
  // put your main code here, to run repeatedly:
  read_button();
}

/**
 * homescreen lcd display
 */
void opening_display() {
  lcd.clear();
  lcd.setBacklight(GREEN);
  lcd.print(F("WELCOME TO THE"));
  lcd.print(F("                          "));
  lcd.print(F("GREENHOUSE!"));
  
}

/**
 * photoresistor sensor reads and displays light intensity data 
 * max val for the sensor is 1023
 * https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-photoresistor-46c5eb
 */ 
void photo_resistor() {
  lcd.clear();
  lcd.setCursor(0,0);
  int value = analogRead(A0);
  lcd.print("RESISTANCE: ");
  lcd.print(value);
  
  lcd.setCursor(0,1);

  if (value < 20) {
    lcd.print(F("DARK LIGHT"));
  }
  else if (value < 50) {
    lcd.print(F("DIM LIGHT"));
  }
  else if (value < 150) {
    lcd.print(F("PARTIAL SHADE"));
  }
  else if (value < 300) {
    lcd.print(F("PARTIAL SUN"));
  }
  else {
    lcd.print(F("DIRECT SUN"));
  }
}

/**
 * temperature sensor reads and displays temperature in degrees fahrenheit
 * https://learn.adafruit.com/tmp36-temperature-sensor
 */
void temperature() {
  lcd.clear();
  lcd.setCursor(0,0);
  float voltage = analogRead(A1) * ((float)5000/1024); // voltage in millivolts
  float tempC = (voltage - 500) / 10;  // millivolts to celcius
  float tempF = tempC * 1.8 + 32; // millivolts to farhenheit
  lcd.print("TEMP: ");     // TEMP: degree *F
  lcd.print(tempF);
  lcd.print(" ");
  lcd.write(0xDF);
  lcd.print("F");
}

/**
 * moisture sensor reads and displays analog data
 */
void moisture() {
  lcd.setCursor(0,0);
  int analog = analogRead(A2);
  double decimal = (1000 - analog) / ((double)715);
  int percent = decimal * 100;
  lcd.print("Moisture:");
  lcd.print(percent);
  lcd.print("%  ");
}

/**
 * button behaviors
 * change background colors based on button pressed
 */
void read_button() {
  button = lcd.readButtons();
   if (button) {
    if (button & BUTTON_UP) {   // if up is pressed, display temperature
      lcd.setBacklight(VIOLET);
      temperature();
    }
    if (button & BUTTON_DOWN) {  // if down is pressed, display soil moisture
      lcd.setBacklight(RED);
      moisture();
    }
    if (button & BUTTON_LEFT) {  // if left button is pressed, print photoresistor data
      lcd.setBacklight(YELLOW);
      photo_resistor();
    }
    if (button & BUTTON_RIGHT) {
      lcd.setBacklight(TEAL);
    }
    if (button & BUTTON_SELECT) {  // if select button is pressed, return to home screen
      lcd.setBacklight(GREEN);
      opening_display();
    }
   }
   
}
