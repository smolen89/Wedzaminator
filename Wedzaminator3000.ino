#include <Wire.h>               // Standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h>  // Biblioteki I2C dla LCD

// Definicja Pinów
#define RELAY_PIN_HOT1 6
#define RELAY_PIN_HOT2 7
#define RELAY_PIN_SMOKE 8
#define RELAY_PIN_FAN 9


LiquidCrystal_I2C lcd(0x27, 16, 2);  // Ustawienie adresu ukladu na 0x27 oraz ustawienie rozmiaru wyświetlacza na 16x2

void setup() {
  // Initializacja Portu Debugowania
  Serial.begin(9600);                                // Otwarcie portu 9600
  delay(1500);                                       // Dodanie opóźnienia 1,5s by debuger mógł zdążyć odebrać sygnał z Arduino
  Serial.println("Wedzaminator 3000 Mark IV v0.1");  // Potwierdzenie otwarcia portu.

  // Initializacja Wyświetlacza
  lcd.init();                       // Initializacja LCD
  lcd.backlight();                  // Włączenie wyświetlacza
  lcd.setCursor(0,0);               // Ustawienie kursora
  lcd.print("Wedzaminator3000");
  lcd.setCursor(0, 1);
  lcd.print(" v0.1   Mark IV ");


  RelaysSetup();

  // Zakończenie przygotowania sterownika
  // Znaczy że wszystko jest ok
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void RelaysSetup(){
  // Inicjacja Przełączników Relay
  pinMode(RELAY_PIN_FAN, OUTPUT);
  pinMode(RELAY_PIN_SMOKE, OUTPUT);
  pinMode(RELAY_PIN_HOT1, OUTPUT);
  pinMode(RELAY_PIN_HOT2, OUTPUT);
  // Wyłączene Relaysów
  HotSwitch(false);
  SmokeSwitch(false);
  FanSwitch(false);
}
void HotSwitch(bool on) {
  digitalWrite(RELAY_PIN_HOT1, !on);
  digitalWrite(RELAY_PIN_HOT2, !on);
}
void SmokeSwitch(bool on) {
  digitalWrite(RELAY_PIN_SMOKE, !on);
}
void FanSwitch(bool on) {
  digitalWrite(RELAY_PIN_FAN, !on);
}