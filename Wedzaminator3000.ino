#include <Wire.h>               // Standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h>  // Biblioteki I2C dla LiquidCrystal
#include <Keypad.h>             // Biblioteka obsługi Keypada
#include "Relays.h"             // Biblioteka Relays
#include "TemperatureLib.h"     // Biblioteka DallasTemperature
#include "Timers.h"             // Biblioteka Timerów

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

const int numOfScreens = 27;
int currentScreen = 0;

String screens[numOfScreens][2] =
{
  /* 00 */ {"Auto Program", "Start"},           // Wszystkie programy (Drying, Smoking, Baking)
  /* 01 */ {"Realtime Program", "Start"},       // Jeden program (pytanie czy robić go na timer odliczający czy poprostu żeby sobie leciał)
  /* 02 */ {"Manual Program", "Start"},         // Typowy manual, wszystko sterowane ręcznie

  /* 03 */ {"Realtime temp", "\337C"},          // Temperatura docelowa
  /* 04 */ {"Realtime Fan", "state"},           // Stan Nadmuchu
  /* 05 */ {"Realtime Fan", "sec time"},  
  /* 06 */ {"Realtime Fan", "sec interval"}, 
  /* 07 */ {"Realtime Smoke", "state"},

  /* 08 */ {"Drying time", "min"},  
  /* 09 */ {"Drying temp", "\337C"},  
  /* 10 */ {"Drying Fan", "state"},  
  /* 11 */ {"Drying Fan", "sec time"},  
  /* 12 */ {"Drying Fan", "sec interval"},  
  /* 13 */ {"Drying Smoke", "state"},

  /* 14 */ {"Smoking time", "min"},  
  /* 15 */ {"Smoking temp", "\337C"},  
  /* 16 */ {"Smoking Fan", "state"},  
  /* 17 */ {"Smoking Fan", "sec time"},  
  /* 18 */ {"Smoking Fan", "sec interval"},  
  /* 19 */ {"Smoking Smoke", "state"},

  /* 20 */ {"Baking time", "min"},  
  /* 21 */ {"Baking temp", "\337C"},  
  /* 22 */ {"Baking Fan", "state"},  
  /* 23 */ {"Baking Fan", "sec time"},  
  /* 24 */ {"Baking Fan", "sec interval"},  
  /* 25 */ {"Baking Smoke", "state"},

  /* 26 */ {"Hysteresis", "\337C tolerance"},
};
long parameters[numOfScreens] = {
  LOW, LOW, LOW,            // Start Programs

  50, LOW, 0, 0, LOW,       // Realtime Settings

  60 * 1, 40, LOW,  0,  0, LOW,  // Drying Settings
  60 * 5, 50, LOW, 10, 10, HIGH, // Smoking Settings
  60 * 1, 80, LOW, 10, 10, LOW,  // Baking Settings
  3                              // Others
  };

enum menuStateEnum : byte
{
  MainMenu,
  AutoProgram,
  RealtimeProgram,
  Manual
};
byte menuState = menuStateEnum::MainMenu;

uint32_t processingTime = 0;

Timer sensorsRefreshTimer;
Timer lcdRefreshTimer;
int sensorsRefreshTime = 5;
int lcdRefreshTime = 1;


void setup() {
  // Initializacja Portu Debugowania
  Serial.begin(9600);                                // Otwarcie portu 9600
  delay(1500);                                       // Dodanie opóźnienia 1,5s by debuger mógł zdążyć odebrać sygnał z Arduino
  Serial.println("Wedzaminator 3000 Mark IV v0.1");  // Potwierdzenie otwarcia portu.

  LCD_Setup();
  TemperatureSensorsSetup();
  TimersSetup();
}

void loop() {
  char key = keypad.getKey();

  switch (menuState)
  {
  case menuStateEnum::MainMenu:
    // Processing Main Menu

    // Keys:
    // A - currentScreen--
    // B - currentScreen++
    if (key == 'A')
    {
      if (currentScreen == 0)
        currentScreen = numOfScreens - 1;
      else
        currentScreen--;
    }
    else if (key == 'B')
    {
      if (currentScreen == numOfScreens - 1)
        currentScreen = 0;
      else
        currentScreen++;
    }
    // Display Menu
    if (key) 
      DisplayMenu();
      
    break;
  
  default:
    break;
  }

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

void TimersSetup(){
  lcdRefreshTimer.begin(SECS(lcdRefreshTime));
  sensorsRefreshTimer.begin(SECS(sensorsRefreshTime));
}

void DisplayMenu(){
  lcd.clear();
  // Display name of a parameter
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0, 1);
  
  // display type of the parameter type
  switch (currentScreen)
  {
    case 0:
    case 1:
    case 2:
      // We not have a parameter
      break;

    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 22:
    case 25:
      // parameter is a State type
      if (parameters[currentScreen] == 0)
      {
        // parameter is OFF state
        lcd.print(">");
        lcd.print("Off");
      }
      else
      {
        // parameter is ON state
        lcd.print(">");
        lcd.print("On");
      }
      break;

    default:
      // parameter has a value like temperature or time
      lcd.print(">");
      lcd.print(parameters[currentScreen]);
      break;
  }

  // Display type of the parameter
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}