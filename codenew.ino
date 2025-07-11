#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LCD-Display Bibliotheken
LiquidCrystal_I2C lcd(0x27, 20, 4); //Formatierung LCD-Display
//WIR MÜSSEN NOCH AMPERE AUSRECHNEN-> DISCO FIX mit formel i=u/r (fester wiederstand einbauen, strom messen und berechnen)
int val = 0;  //Wert von val auf 0 gesetzt
int button = 2;  // Pin for the firsact button
int button1 = 3; // Pin for the second button
int red = 4; //Pin für die rote LED
int yellow = 6; //Pin für die gelbe LED
int green = 5; //Pin für die grüne LED
int motor = 10; //Pin für den Motor
int stromtest = 7; //TESTTTTT
int strom = 0; //Wert von strom auf 0 gesetzt

volatile bool buttonPressed = false; //Anfangsstatus von Button auf falsch gesetzt
volatile bool button1Pressed = false; //Anfangsstatus von Button1 auf falsch gesetzt

void buttonPressISR() {
  buttonPressed = !buttonPressed; //Interrupt, um den Button-Status zu verändern
}

void button1PressISR() {
  button1Pressed = !button1Pressed; //Interrupt, um den Button1-Status zu verändern
}

void setup() { 
 // Serial.begin(9600);          
  pinMode(A0, INPUT); // Pin A0 als Eingang (zum Messen der Spannung) gesetzt        
  pinMode(button, INPUT_PULLUP);  
  pinMode(button1, INPUT_PULLUP);
  lcd.init();
  lcd.backlight(); 

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(motor, OUTPUT); 
  attachInterrupt(digitalPinToInterrupt(button), buttonPressISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(button1), button1PressISR, FALLING);
}

void loop() {
  digitalWrite(stromtest, HIGH);
  val = analogRead(A0);
  strom = map(val, 0, 1023, 0, 5000);
  strom = strom / 12;
  Serial.println(strom);
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(strom);
  lcd.print("mA");
  delay(100);
  digitalWrite(stromtest, LOW);
  delay(200);
  //stufe eins
  if (strom <= 10) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    digitalWrite(motor, LOW);
  }

  //stufe zwei
  else if (strom >= 10 && strom <= 20) {
    if (buttonPressed == true) {
      digitalWrite(red, HIGH);  
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);
    } else {
      digitalWrite(red, LOW); 
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);
    }
  }

 //stufe drei
  else if (strom >= 20 && strom <= 22) {
    if (buttonPressed == true) {
      //delay(50);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      digitalWrite(yellow, HIGH);
    } else{
        //delay(50);
        digitalWrite(green, LOW); 
        digitalWrite(red, LOW);    
        digitalWrite(yellow, LOW);  
        digitalWrite(motor, LOW);
      }
    
     if (strom >= 20 && strom <= 22) {
      if (button1Pressed == true) {
        digitalWrite(green, LOW);  
        digitalWrite(motor, HIGH);  
        digitalWrite(red, LOW); 
      } else {
        digitalWrite(green, LOW);   
        digitalWrite(red, LOW);    
        digitalWrite(yellow, LOW);
        digitalWrite(motor, LOW); 
      }
    }
  }

   else if (strom >= 22) {
    if (buttonPressed == true) {
      digitalWrite(red, LOW);     
      digitalWrite(green, HIGH);   
      digitalWrite(yellow, LOW);
      digitalWrite(motor, HIGH);  
    } else {    //delay(50);
      digitalWrite(red, LOW);   
      digitalWrite(green, LOW); 
      digitalWrite(yellow, LOW);
      digitalWrite(motor, LOW);  
    }
   }
   }
