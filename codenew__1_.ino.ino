#include <Wire.h>
#include <LiquidCrystal_I2C.h>       // Bibliothek für das LCD-Display einbinden
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD-Objekt initialisieren (Adresse: 0x27, 20 Zeichen, 4 Zeilen)

// Variable für Strommessung und Tasten
int val = 0;        // Wert für den analogen Eingang (Spannungswert)
int button = 2;     // Pin für den ersten Button
int button1 = 3;    // Pin für den zweiten Button
int red = 4;        // Pin für die rote LED
int yellow = 6;     // Pin für die gelbe LED
int green = 5;      // Pin für die grüne LED
int motor = 10;     // Pin für den Motor
int stromtest = 7;  // TEST-Pin (möglicherweise zum Strom messen)
int strom = 0;      // Stromwert initialisieren

volatile bool buttonPressed = false;   // Status des ersten Buttons (Interrupt-gesteuert)
volatile bool button1Pressed = false;  // Status des zweiten Buttons (Interrupt-gesteuert)

// Interrupt-Service-Routine für Button 1
void buttonPressISR() {
  buttonPressed = !buttonPressed;  // Toggle Button-Status, wenn der Button gedrückt wird
}

// Interrupt-Service-Routine für Button 2
void button1PressISR() {
  button1Pressed = !button1Pressed;  // Toggle Button-Status, wenn der Button gedrückt wird
}

void setup() {
  // Serial.begin(9600);  // Seriellen Monitor für Debugging (optional auskommentiert)

  pinMode(A0, INPUT);              // Pin A0 als Eingang für die Spannungsmessung festlegen
  pinMode(button, INPUT_PULLUP);   // Button 1 als Eingang mit internem Pullup-Widerstand
  pinMode(button1, INPUT_PULLUP);  // Button 2 als Eingang mit internem Pullup-Widerstand

  lcd.init();       // LCD-Display initialisieren
  lcd.backlight();  // Hintergrundbeleuchtung des LCD einschalten

  pinMode(red, OUTPUT);     // Pin für rote LED als Ausgang setzen
  pinMode(yellow, OUTPUT);  // Pin für gelbe LED als Ausgang setzen
  pinMode(green, OUTPUT);   // Pin für grüne LED als Ausgang setzen
  pinMode(motor, OUTPUT);   // Pin für den Motor als Ausgang setzen

  // Interrupts für die Buttons einrichten
  attachInterrupt(digitalPinToInterrupt(button), buttonPressISR, FALLING);    // Button 1 interrupt (Falling Edge)
  attachInterrupt(digitalPinToInterrupt(button1), button1PressISR, FALLING);  // Button 2 interrupt (Falling Edge)
}

void loop() {
  digitalWrite(stromtest, LOW);  // Stromtest-Pin auf LOW setzen (möglicherweise zum Vermeiden von Rauschen oder Interferenzen)

  val = analogRead(A0);                // Analogwert (Spannung) von Pin A0 einlesen
  strom = map(val, 0, 1023, 0, 5000);  // Umrechnung des analog eingelesenen Wertes in mA (Stromstärke)
  strom = strom / 12;                  // Anpassung der Berechnung (kann von der Schaltung abhängen)
  Serial.println(strom);               // Ausgabe des Stromwertes auf dem seriellen Monitor
  delay(20);                           // Kurze Pause, um die Messung zu stabilisieren

  digitalWrite(stromtest, HIGH);  // Stromtest-Pin auf HIGH setzen (möglicherweise für spätere Messung oder Aktivierung)

  lcd.clear();          // LCD-Bildschirm löschen
  lcd.setCursor(0, 0);  // Cursor auf Zeile 0, Spalte 0 setzen
  lcd.print(strom);     // Den berechneten Stromwert auf dem LCD anzeigen
  lcd.print("mA");      // Einheit "mA" nach dem Wert anzeigen
  delay(200);           // Kurze Pause, um das Display nicht zu überfluten

  // Stufe 1: Strom <= 100mA
  if (strom <= 100) {
    digitalWrite(red, LOW);     // Rote LED ausschalten
    digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
    digitalWrite(green, LOW);   // Grüne LED ausschalten
    digitalWrite(motor, LOW);   // Motor ausschalten
  }

  // Stufe 2: Strom zwischen 101mA und 300mA
  else if (strom >= 101 && strom <= 300) {
    if (buttonPressed == true) {  // Wenn Button 1 gedrückt wurde
      digitalWrite(red, HIGH);    // Rote LED einschalten
      digitalWrite(green, LOW);   // Grüne LED ausschalten
      digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
      digitalWrite(motor, LOW);   // Motor ausschalten
    } else {
      digitalWrite(red, LOW);     // Rote LED ausschalten
      digitalWrite(green, LOW);   // Grüne LED ausschalten
      digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
      digitalWrite(motor, LOW);   // Motor ausschalten
    }
  }

  // Stufe 3: Strom zwischen 301mA und 500mA
  else if (strom >= 301 && strom <= 500) {
    if (buttonPressed == true) {  // Wenn Button 1 gedrückt wurde
      // LED-Status setzen
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      digitalWrite(yellow, HIGH);  // Gelbe LED einschalten
    } else {
      digitalWrite(green, LOW);   // Grüne LED ausschalten
      digitalWrite(red, LOW);     // Rote LED ausschalten
      digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
      digitalWrite(motor, LOW);   // Motor ausschalten
    }

    if (strom >= 301 && strom <= 500) {
      if (button1Pressed == true) {  // Wenn Button 2 gedrückt wurde
        digitalWrite(green, LOW);    // Grüne LED ausschalten
        digitalWrite(motor, HIGH);   // Motor einschalten
        digitalWrite(red, LOW);      // Rote LED ausschalten
      } else {
        digitalWrite(green, LOW);   // Grüne LED ausschalten
        digitalWrite(red, LOW);     // Rote LED ausschalten
        digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
        digitalWrite(motor, LOW);   // Motor ausschalten
      }
    }
  }

  // Stufe 4: Strom > 500mA
  else if (strom >= 501) {
    if (buttonPressed == true) {  // Wenn Button 1 gedrückt wurde
      digitalWrite(red, LOW);     // Rote LED ausschalten
      digitalWrite(green, HIGH);  // Grüne LED einschalten
      digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
      digitalWrite(motor, HIGH);  // Motor einschalten
    } else {
      digitalWrite(red, LOW);     // Rote LED ausschalten
      digitalWrite(green, LOW);   // Grüne LED ausschalten
      digitalWrite(yellow, LOW);  // Gelbe LED ausschalten
      digitalWrite(motor, LOW);   // Motor ausschalten
    }
  }
}
