#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int val = 0;  
int button = 2;  // Pin for the first button
int button1 = 3; // Pin for the second button
int red = 4;
int yellow = 6;
int green = 5;
int motor = 10;
int strom = 0;

int buttonout = LOW;
int button1out = LOW;

volatile bool buttonPressed = false;
volatile bool button1Pressed = false;

byte Battery_empty[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B00000
};

byte  Battery_low[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B00000
};

byte Battery_mid[] = {
  B01110,
  B11011,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

byte Battery_full[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

// Interrupt service routines
void buttonPressISR() {
  buttonPressed = !buttonPressed;
}

void button1PressISR() {
  button1Pressed = !button1Pressed;
}

void setup() {
  Serial.begin(9600);           // Setup serial communication
  pinMode(A0, INPUT);           // Analog input for strom
  pinMode(button, INPUT_PULLUP);  // Set button pin as input with pull-up
  pinMode(button1, INPUT_PULLUP); // Set button1 pin as input with pull-up
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.createChar(0, Battery_full);
  lcd.createChar(1, Battery_mid);
  lcd.createChar(2, Battery_low);
  lcd.createChar(3, Battery_empty);

  lcd.home();
  lcd.write(0);
  pinMode(red, OUTPUT);  // LED pin as output
  pinMode(yellow, OUTPUT);  // LED pin as output
  pinMode(green, OUTPUT);  // LED pin as output
  pinMode(motor, OUTPUT);  // Motor pin as output

  attachInterrupt(digitalPinToInterrupt(button), buttonPressISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(button1), button1PressISR, FALLING);
  lcd.setCursor(6, 1);
  lcd.write(3);
}

void loop() {
  val = analogRead(A0);  // Read the input pin
  strom = map(val, 0, 1023, 0, 500);
  //Serial.println(val);
  Serial.println(strom * 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(val);
  lcd.print("0");
  lcd.print("mV");
  delay(200);
  lcd.setCursor(6, 1);
  lcd.write(3);
  
  //stufe eins
  if (strom * 2 <= 100) {
    lcd.setCursor(6, 1);
    lcd.write(0);
    digitalWrite(red, LOW);    // Turn off green LED
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);    // Turn off green LED
    digitalWrite(motor, LOW);    // Stop motor
    // No action needed for this range
  }

  //stufe zwei
  else if (strom * 2 >= 101 && strom * 2 <= 300) {
    lcd.setCursor(6, 1);
    lcd.write(1);
    if (buttonPressed == true) {
      //delay(50);
      digitalWrite(red, HIGH);     // Turn on red LED
      digitalWrite(green, LOW);    // Turn off green LED
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);    // Stop motor
    } else {
      //delay(50);
      digitalWrite(red, LOW);      // Turn off red LED
      digitalWrite(green, LOW);    // Turn off green LED
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);    // Stop motor
    }
  }

 //stufe drei
  else if (strom * 2 >= 301 && strom * 2 <= 500) {
    lcd.setCursor(6, 1);
    lcd.write(3);
    if (buttonPressed == true) {
      //delay(50);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      digitalWrite(yellow, HIGH);
      lcd.setCursor(6, 1);
      lcd.write(3);
    } else{
        //delay(50);
        digitalWrite(green, LOW);    // Turn off green LED
        digitalWrite(red, LOW);    // Turn off green LED
        digitalWrite(yellow, LOW);    // Turn off green LED
        digitalWrite(motor, LOW);   // Start motor
      }
    
     if (strom * 2 >= 301 && strom * 2 <= 500) {
      //delay(50);
      if (button1Pressed == true) {
        digitalWrite(green, LOW);   // Turn on green LED
        digitalWrite(motor, HIGH);   // Start motor
        digitalWrite(red, LOW);    // Turn off green LED
      } else {
        //delay(50);
        digitalWrite(green, LOW);    // Turn off green LED
        digitalWrite(red, LOW);    // Turn off green LED
        digitalWrite(yellow, LOW);    // Turn off green LED
        digitalWrite(motor, LOW);   // Start motor
      }
    }
  }

   else if (strom * 2 >= 501) {
    lcd.setCursor(6, 1);                                                                         
    lcd.write(1);
    if (buttonPressed == true) {
      //delay(50);
      digitalWrite(red, LOW);     // Turn on red LED
      digitalWrite(green, HIGH);    // Turn off green LED
      digitalWrite(yellow, LOW);
      digitalWrite(motor, HIGH);    // Stop motor
    } else {    //delay(50);
      digitalWrite(red, LOW);      // Turn off red LED
      digitalWrite(green, LOW);    // Turn off green LED
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);    // Stop motor
    }
   }
   }
  /*else if (strom * 2 >= 201 && strom * 2 <= 400) {
    lcd.setCursor(6, 1);
    lcd.write(2);
    if (buttonPressed) {
      digitalWrite(yellow, HIGH);  // Turn on yellow LED
      digitalWrite(red, LOW);    // Turn off green LED
      digitalWrite(green, LOW);
      digitalWrite(motor, LOW);    // Stop motor
      //delay(50);
    } else {
      digitalWrite(yellow, LOW);   // Turn off yellow LED
      digitalWrite(red, LOW);    // Turn off green LED
      digitalWrite(green, LOW);
      digitalWrite(motor, LOW);    // Stop motor
      // delay(50);
    }
  }
  else {
    //delay(50);
    digitalWrite(green, LOW);    // Turn off green LED
    digitalWrite(motor, LOW);    // Stop motor
    digitalWrite(red, LOW);    // Turn off green LED
    digitalWrite(yellow, LOW);    // Turn off green LED
  }
}*/
