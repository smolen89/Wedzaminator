#include <Wire.h>               // Standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h>  // Biblioteki I2C dla LCD

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

  // Zakończenie przygotowania sterownika
  // Znaczy że wszystko jest ok
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

}
