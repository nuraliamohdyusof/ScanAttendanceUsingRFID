/*

 Lecturer Availablity Attendance (Using RFID and Arduino Nano)
 -Muhammad Afiq bin Mohd Yusof

  The circuit:
 * LCD RS pin to digital pin A0
 * LCD Enable pin to digital A1
 * LCD D4 pin to digital pin A2
 * LCD D5 pin to digital pin A3
 * LCD D6 pin to digital pin A4
 * LCD D7 pin to digital pin A5
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/

// include the library code:
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(2,0); // Sets the location at which subsequent text written to the LCD will be displayed 
  lcd.print("List of Lecturer");
  lcd.setCursor(0,1); 
  lcd.print("--------------------");
  lcd.setCursor(2,2);
  lcd.print("Encik Nazri: ");
  lcd.setCursor(2,3);
  lcd.print("Encik Yahya: ");
}

/*void clearLCDLine(int line)
{               
    lcd.setCursor(0,line);
    for(int n = 0; n < 24; n++) // 20 indicates symbols in line. For 2x20 LCD write - 20
    {
            lcd.print(" ");
    }
}*/

int CounterKad1;
int CounterKad2;
void loop() 
{
  lcd.display();
  delay(100);
  int count = 0;
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  if (content.substring(1) == "89 A2 00 5D")   //yahya(button)
  {
    CounterKad1++;
    if (CounterKad1 % 2 != 0 ) {         //odd(ganjil)
        lcd.setCursor(15,2); 
        lcd.print("IN ");
        delay(1000);
    }
    else{
      lcd.setCursor(15,2);
      lcd.print("OUT");
      delay(1000);
    }
  }
  else{   
    if (content.substring(1) == "4C 8E C7 B3")  //nazri(kad)
    {
      CounterKad2++;
      if (CounterKad2 % 2 != 0 ) {         
          lcd.setCursor(15,3);  //odd(ganjil)
          lcd.print("IN ");
          delay(1000);
      }
      else{
        lcd.setCursor(15,3);
        lcd.print("OUT");
        delay(1000);
      }
    }
  }
} 
