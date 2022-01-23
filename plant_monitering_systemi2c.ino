/**
 * Plant monitoring system using a 20x4 i2c LCD screen and custom characters
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

// custom characters
uint8_t tinyCactus[8] = {B01000,B11100,B11101,B11111,B11111,B11100,B11100,B11100};
uint8_t cactusL[8] = {B00010,B00110,B00110,B00111,B00111,0x0,0x0,0x0};
uint8_t cactusU[8] = {0x0,0x0,B01100,B11110,B11110,B11110,B11110,B11110};
uint8_t cactusM[8] = {B11110,B11111,B11111,B11110,B111110,B11110,B11110,B11110};
uint8_t cactusD[8] = {B11110,B11110,B11110,B11110,B11110,B11110,B11110,B11110};
uint8_t cactusUR[8] = {0x0,0x0,0x0,0x0,0x0,B01000,B11000,B11000};
uint8_t cactusLR[8] = {B11000,B11000,B10000,0x0,0x0,0x0,0x0,0x0};

// The current time in milliseconds since boot.
unsigned long time;

// The time since last read of photoresistor
unsigned long last_light_read_time;

// The time since last read of temp sensor
unsigned long last_temp_read_time;

// The time since last read of moisture sensor
unsigned long last_moisture_read_time;

void setup() {
  lcd.init(); // initialize LCD
  lcd.backlight();
  opening_display();
  delay(5000);
  lcd.clear();
  cactus(17,2);
  lcd.createChar(0,tinyCactus);
  lcd.setCursor(16,3);
  lcd.printByte(0);
}

void loop() {
  time = millis(); // measure number of milliseconds passed since starting program
  display_stats();
}

/**
 * homescreen lcd display
 */
void opening_display() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to Sophia's ");
  lcd.setCursor(4, 1);
  lcd.print("Greenhouse!!");
  cactus(0,1);
  cactus(17,1);
  cactus(2,2);
  cactus(15,2);
  
}

/**
 * continuously updates plant stats to lcd screen
 */
void display_stats() {
  lcd.home();
  lcd.print("Sophia's plant stats");
  
  // timers for each sensor
  // reanalyzes every 3 seconds
  if (time - last_light_read_time >= 3000) {
    last_light_read_time = time;
    photo_resistor();
  }
  if (time - last_temp_read_time >= 3000) {
    last_temp_read_time = time;
    temperature();
  }
  if (time - last_moisture_read_time >= 3000) {
    last_moisture_read_time = time;
    moisture();
  }
}

/**
 * photoresistor sensor reads and displays light intensity data in the form of a percent
 * max val for the sensor is 1023
 * https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-photoresistor-46c5eb
 */
void photo_resistor() {
  lcd.setCursor(0,1);
  int value = analogRead(A0);
  float percent = (value / (double)1023) * 100;
  lcd.print("Light:");
  lcd.print(percent);
  lcd.write(0x25); // percent sign %
  lcd.print(" ");

  if (value < 20) {
    lcd.print(F("Dark"));
  }
  else if (value < 50) {
    lcd.print(F("Dim light"));
  }
  else if (value < 150) {
    lcd.print(F("Shady"));
  }
  else if (value < 300) {
    lcd.print(F("Partial sun"));
  }
  else {
    lcd.print(F("Direct sun"));
  }
}

/**
 * temperature sensor reads and displays temperature in degrees fahrenheit
 * https://learn.adafruit.com/tmp36-temperature-sensor
 */
void temperature() {
  lcd.setCursor(0,2);
  float voltage = analogRead(A1) * ((float)5000/1024); // voltage in millivolts
  float tempC = (voltage - 525) / 9.5;  // millivolts to celcius
  float tempF = tempC * 1.8 + 32; // millivolts to farhenheit
  lcd.print("Temp:");    
  lcd.print(tempF);
  lcd.print(" ");
  lcd.write(0xDF);  // degree symbol °
  lcd.print("F");
}

/**
 * moisture sensor reads and displays analog data
 */
void moisture() {
  lcd.setCursor(0,3);
  int analog = analogRead(A2);
  lcd.print("Moisture:");
  lcd.print(analog);
}

/**
 * crete cactus custom character
 * x and y are the coordinates for top left corner of cactus. x rep columns, y rep rows
 * x must be between 0 and 17
 * y must be between 0 and 2
 * https://www.engineersgarage.com/making-custom-characters-on-lcd-using-arduino/
 */
void cactus(int x,int y) {
  // initialize custom characters 
  lcd.createChar(1,cactusL);
  lcd.createChar(2, cactusU);
  lcd.createChar(3, cactusM);
  lcd.createChar(4, cactusD);
  lcd.createChar(5, cactusUR);
  lcd.createChar(6, cactusLR);
  lcd.setCursor(x,y+1);
  lcd.printByte(1);
  lcd.setCursor(x+1,y);
  lcd.printByte(2);
  lcd.setCursor(x+1,y+1);
  lcd.printByte(3);
  lcd.setCursor(x+2,y);
  lcd.printByte(5);
  lcd.setCursor(x+2,y+1);
  lcd.printByte(6);
  
  if (y < 2) {
    lcd.setCursor(x+1,y+2);
    lcd.printByte(4);
  }

}
