/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(47, 46, 42, 43, 44, 45);

double rpm = 0;
double rpmPerAmp = 336; // Calibre-me!
double rpmTarget = 1600; // Velocidade desejada

int entradaRPMPin = 0;
int entradaCorrentePin = 1; // Nao Implementado ainda

int saidaCorrentePin = 2;

void setup() {
  // Init LCD
  lcd.begin(20, 4);
  // Init out Pins
  pinMode(saidaCorrentePin, OUTPUT);
}

double algor(double rpm, double rpmTarget, double rpmPerAmp = 336)
{
  double result = 0;
  // se dentro do dominio valido da funcao
  if(rpm<rpmTarget){
    result = (rpmTarget * log(rpm))/(rpmPerAmp * log(rpmTarget));
  } else {
    result = 0;
  }

  return result;
}

void loop() {
  rpm = analogRead(entradaRPMPin);
  // Mapeia a entrada. Calibrar!
  rpm = map(rpm, 0,1022,0,2000);
  
  lcd.setCursor(0, 0);
  lcd.print("RPM entrada");
  lcd.setCursor(0, 1);
  lcd.print(rpm);
  lcd.setCursor(0, 2);
  lcd.print("Saida A");
  lcd.setCursor(0, 3);
  double outpAmp = algor(rpm,rpmTarget,rpmPerAmp);
  lcd.print(outpAmp);

  //outpAmp = map(outpAmp,etc,etc,etc,etc);

  //analogWrite(saidaCorrentePin, outpAmp);
  
  //lcd.print(algor(input,1600,336));
  //input += 0.001;
}
