#include <Wire.h>              // Standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h> // Biblioteki I2C dla LiquidCrystal
#include <Keypad.h>            // Biblioteka obsługi Keypada
#include "Relays.h"            // Biblioteka Relays
#include "TemperatureLib.h"    // Biblioteka DallasTemperature
#include "Timers.h"            // Biblioteka Timerów

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
byte KEYPAD_COL_PIN[4] = {16, 10, 4, 5};   // connect to the row pinouts of the keypad
byte KEYPAD_ROW_PIN[4] = {19, 18, 15, 14}; // connect to the column pinouts of the keypad
char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
Keypad keypad = Keypad(makeKeymap(keys), KEYPAD_ROW_PIN, KEYPAD_COL_PIN, 4, 4);

const int numOfScreens = 27;
int currentScreen = 0;

String screens[numOfScreens][2] =
    {
        /* 00 */ {"Prog. automat.  ", "-> Rozpocznij <-"}, // Wszystkie programy (Drying, Smoking, Baking)
        /* 01 */ {"Prog. polautomat", "-> Rozpocznij <-"}, // Jeden program (pytanie czy robić go na timer odliczający czy poprostu żeby sobie leciał)
        /* 02 */ {"Prog. reczny    ", "-> Rozpocznij <-"}, // Typowy manual, wszystko sterowane ręcznie

        /* 03 */ {"Polauto temper", "\337C"},        // Temperatura docelowa
        /* 04 */ {"Polauto nadmuch", ""},              // Stan Nadmuchu
        /* 05 */ {"Polauto nadmuch", "min time"},
        /* 06 */ {"Polauto nadmuch", "min interval"},
        /* 07 */ {"Polauto Dym", ""},

        /* 08 */ {"Suszenie temper", "min"},
        /* 09 */ {"Suszenie temp", "\337C"},
        /* 10 */ {"Suszenie Fan", ""},
        /* 11 */ {"Suszenie Fan", "min time"},
        /* 12 */ {"Suszenie Fan", "min interval"},
        /* 13 */ {"Suszenie Smoke", ""},

        /* 14 */ {"Smoking time", "min"},
        /* 15 */ {"Smoking temp", "\337C"},
        /* 16 */ {"Smoking Fan", ""},
        /* 17 */ {"Smoking Fan", "min time"},
        /* 18 */ {"Smoking Fan", "min interval"},
        /* 19 */ {"Smoking Smoke", ""},

        /* 20 */ {"Baking time", "min"},
        /* 21 */ {"Baking temp", "\337C"},
        /* 22 */ {"Baking Fan", ""},
        /* 23 */ {"Baking Fan", "min time"},
        /* 24 */ {"Baking Fan", "min interval"},
        /* 25 */ {"Baking Smoke", ""},

        /* 26 */ {"Hysteresis", "\337C tolerance"}
};
long parameters[numOfScreens] = {
    LOW, LOW, LOW, // Start Programs

    50, LOW, 0, 0, LOW, // Realtime Settings

    60 * 1, 40, LOW, 0, 0, LOW,    // Drying Settings
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

char menuPrevChar = 'A';
char menuNextChar = 'B';
char menuBackChar = 'C';
char menuOkChar = 'D';
char menuPlusChar = '#';
char menuMinusChar = '*';

uint32_t processingTime = 0;

Timer sensorsRefreshTimer;
Timer lcdRefreshTimer;
int sensorsRefreshTime = 5;
int lcdRefreshTime = 1;

void setup()
{
  // Initializacja Portu Debugowania
  Serial.begin(9600);                               // Otwarcie portu 9600
  delay(1500);                                      // Dodanie opóźnienia 1,5s by debuger mógł zdążyć odebrać sygnał z Arduino
  Serial.println("Wedzaminator 3000 Mark IV v0.1"); // Potwierdzenie otwarcia portu.

  LCD_Setup();
  TemperatureSensorsSetup();
  TimersSetup();
  
  displayMainMenu();
}

void LCD_Setup()
{
  // Initializacja Wyświetlacza
  lcd.init();          // Initializacja LCD
  lcd.backlight();     // Włączenie wyświetlacza
  lcd.setCursor(0, 0); // Ustawienie kursora
  lcd.print("Wedzaminator3000");
  lcd.setCursor(0, 1);
  lcd.print(" v0.1   Mark IV ");
  delay(3000);
  lcd.clear();
}

void TemperatureSensorsSetup()
{
  sensorTemp.Initialization();
  lcd.setCursor(0, 0);
  lcd.print("Temp. Res.:   ");
  lcd.println(sensorTemp.GetDT().getResolution());
  lcd.setCursor(0, 1);
  lcd.print("Temp. Count:  ");
  lcd.println(sensorTemp.GetDT().getDeviceCount());
  delay(3000);
  lcd.clear();
}

void TimersSetup()
{
  lcdRefreshTimer.begin(SECS(lcdRefreshTime));
  sensorsRefreshTimer.begin(SECS(sensorsRefreshTime));
}

void loop()
{
  char key = keypad.getKey();

  switch (menuState)
  {
    case menuStateEnum::MainMenu:
      // Główne Menu
      if (key)
      {
        mainMenu(key);        // Operacje w głównym menu
        displayMainMenu(); // Wyświetlanie głównego menu
      }
      break;

    case menuStateEnum::Manual:
      // Program Manualny
      break;

    case menuStateEnum::AutoProgram:
      // Program Automatyczny w pełni (Suszenie, wędzenie i pieczenie)
      break;

    case menuStateEnum::RealtimeProgram:
      // Program półautomatyczny (znaczy jeden automat)
      break;

    default:
      // W tym wypadku jest jakiś problem z 'menuState' bo nie powinno dość do tego miejsca.
      break;
  }
}

void mainMenu(char key)
{
    // Processing Menu
    if (key == menuPrevChar)
    {
      // currentScreen--
      
      // Jeśli menu jest w pozcji pierwszej trzeba je cofnąć do ostatniej,
      // a jeśli nie to do poprzedniej.
      if (currentScreen == 0)
      {
        currentScreen = numOfScreens - 1;
      }
      else
      {
        currentScreen--;
      }
    }
    else if (key == menuNextChar)
    {
      // currentScreen++

      // Jeśli menu jest w pozycji ostatniej, trzeba je przewinąć na początek,
      // a jeśli nie to do następnej pozycji.
      if (currentScreen == numOfScreens -1)
      {
        currentScreen = 0;
      }
      else
      {
        currentScreen++;
      }
    }
    else if (key == menuMinusChar)
    {
        switch (currentScreen)
        {
        case 0:
        case 1:
        case 2:
          // Start programów, tu nie ma czego przestawiać!!
          break;
        case 4:   // ON/OFF
        case 7:
        case 10:
        case 13:
        case 16:
        case 19:
        case 22:
        case 25:
          parameters[currentScreen] = LOW;
          break;

        default:
          // jeśli jakiś parametr jest już na minimalnej wartości 
          // to nie można go zmienić na mniejszy
          if (parameters[currentScreen] > 0)
            parameters[currentScreen]--;

          break;
        }
    }
    else if (key == menuPlusChar)
    {
      switch (currentScreen)
        {
        case 0:
        case 1:
        case 2:
          // Start programów, tu nie ma czego przestawiać!!
          break;
        case 4:   // ON/OFF
        case 7:
        case 10:
        case 13:
        case 16:
        case 19:
        case 22:
        case 25:
          parameters[currentScreen] = HIGH;
          break;

        default:
          // Tu nie ma górnej granicy
          parameters[currentScreen]++;
            
          break;
        }
    }
    else if (key == menuOkChar)
    {
      switch (currentScreen)
      {
        case 0:
          /* START ProgramRunning */
          break;

        case 1:
          /* START RealTimeProgram */
          break;

        case 2:
          /* START Manual */
          break;

        default:
          break;
      }
    }
    else if (key == menuBackChar)
    {
      // TODO: Bonus
    }
}

void displayMainMenu()
{
  lcd.clear();
  // Wyświetlenie pierwszej lini tj. nazwa parametru
  // todo: zrobić przewijający tekst
  lcd.print(screens[currentScreen][0]);

  lcd.setCursor(0,1);   // czas na drugą linijkę
  // wyświetlenie drugiej linijki

  // Wyświetlenie nazwy (pierwszego człona) parametru
  switch (currentScreen)
  {
    case 0:
    case 1:
    case 2:
      break;
    
      case 4:   // ON/OFF
      case 7:
      case 10:
      case 13:
      case 16:
      case 19:
      case 22:
      case 25:
        // parametr to włącznik/wyłącznik
        if (parameters[currentScreen] == 0)
        {
          // parametr jest wyłączony
          lcd.print("> Off");
        }
        else
        {
          // Parametr jest włączony
          lcd.print("> On");
        }
      break;

    default:
      // parametr to wartość liczbowa
      lcd.print("> ");
      lcd.print(parameters[currentScreen]);
      break;
  }

  // Wyświetlenie typu wartości parametru
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}

