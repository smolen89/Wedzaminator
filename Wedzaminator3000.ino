#include <Wire.h>               // Standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h>  // Biblioteki I2C dla LiquidCrystal
#include <Keypad.h>             // Biblioteka obsługi Keypada
#include "Relays.h"             // Biblioteka Relays
#include "TemperatureLib.h"     // Biblioteka DallasTemperature

// Definicja Pinów
#define RELAY_PIN_HOT1 6
#define RELAY_PIN_HOT2 7
#define RELAY_PIN_SMOKE 8
#define RELAY_PIN_FAN 9
Relay relay(RELAY_PIN_FAN, RELAY_PIN_SMOKE, RELAY_PIN_HOT1, RELAY_PIN_HOT2);

// Ustawienie adresu LCD na 0x27 oraz ustawienie rozmiaru wyświetlacza na 16x2
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);  

#define TEMPERATURE_SENSOR_PIN A2
TemperatureLib sensorTemp(TEMPERATURE_SENSOR_PIN);
byte KEYPAD_COL_PIN[4] = { 16, 10,  4,  5 }; //connect to the row pinouts of the keypad
byte KEYPAD_ROW_PIN[4] = { 19, 18, 15, 14 }; //connect to the column pinouts of the keypad
char keys[4][4] = {
  { '1', '2', '3', 'A' }, 
  { '4', '5', '6', 'B' }, 
  { '7', '8', '9', 'C' }, 
  { '*', '0', '#', 'D' }
};
Keypad keypad = Keypad( makeKeymap(keys), KEYPAD_ROW_PIN, KEYPAD_COL_PIN, 4, 4 );


void setup() {
  // Initializacja Portu Debugowania
  Serial.begin(9600);                                // Otwarcie portu 9600
  delay(1500);                                       // Dodanie opóźnienia 1,5s by debuger mógł zdążyć odebrać sygnał z Arduino
  Serial.println("Wedzaminator 3000 Mark IV v0.2");  // Potwierdzenie otwarcia portu.

  LCD_Setup();
  TemperatureSensorsSetup();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if (key)
  {
    Serial.print("Key pressed: ");
    Serial.println(key);
  }
  
}

void LCD_Setup(){
  // Initializacja Wyświetlacza
  lcd.init();                       // Initializacja LCD
  lcd.backlight();                  // Włączenie wyświetlacza
  lcd.setCursor(0,0);               // Ustawienie kursora
  lcd.print("Wedzaminator3000");
  lcd.setCursor(0, 1);
  lcd.print(" v0.1   Mark IV ");
  delay(3000);
  lcd.clear();
}

void TemperatureSensorsSetup(){
  sensorTemp.Initialization();
  lcd.setCursor(0,0); 
  lcd.print("Temp. Res.:   ");
  lcd.println(sensorTemp.GetDT().getResolution());
  lcd.setCursor(0, 1);
  lcd.print("Temp. Count:  ");
  lcd.println(sensorTemp.GetDT().getDeviceCount());
  delay(3000);
  lcd.clear();
}