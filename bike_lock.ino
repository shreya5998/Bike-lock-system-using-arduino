#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0,1);

#define buzzer 15
#include <Servo.h> 
#define out 16          //wrong passsword buzzer
int servoPin = 14; 
Servo Servo1; 
LiquidCrystal lcd(13,12,11,10,9,8);

void ser();
void beep();
void wrong();
void tempwrong();
void thief();

char password[4];
char pass[4],pass1[4];
int i=0;
int change_count=0;
char customKey=0;

const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','4','7'},
  {'2','5','8'},
  {'3','6','#'}
  
};

byte rowPins[ROWS] = {5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup()
{

  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);

  
  Servo1.attach(servoPin); 
  lcd.begin(16,2);

  pinMode(buzzer, OUTPUT);

  lcd.print("  Electronic ");
  lcd.setCursor(0,1);
  lcd.print("  Keypad Lock ");
  delay(1000);
  lcd.clear();
  lcd.print("Enter Ur Passkey:");
  lcd.setCursor(0,1);
 
  for(int j=0;j<4;j++)
       EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
      pass[j]=EEPROM.read(j);
}

int count=0;
void loop()
{
  
  customKey = customKeypad.getKey();

  if(customKey=='#')
  {
        change();
  }
  
  if (customKey)
  {
     count++;
     password[i++]=customKey;
     lcd.print(customKey);
     beep();
  }

  if(count==14)
     thief();

     
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
        pass[j]=EEPROM.read(j);
    if(!(strncmp(password, pass,4)))
    {
       
        beep();
        lcd.clear();
        lcd.print("Passkey Accepted");
        delay(100);
        ser();
        lcd.setCursor(0,1);
        lcd.print("#.Change Passkey");
        delay(100);
        lcd.clear();
        lcd.print("Enter Passkey:");
        lcd.setCursor(0,1);
        i=0;
        count=0;
    }
    
    else
    {
          tempwrong();
          
          lcd.clear();
          lcd.print("Access Denied...");
          lcd.setCursor(0,1);
          lcd.print("#.Change Passkey");
          delay(1000);
          lcd.clear();
          lcd.print("Enter Passkey:");
          lcd.setCursor(0,1);
          i=0;
          
          count++;
    }
  }
}


void tempwrong()
{
  for(int i=0;i<50;i++)
  {
       digitalWrite (buzzer, HIGH);                                                         // This will turn the buzzer ON
       delay (20) ;                                                                                      // Giving a Delay of 1ms will set frequency 1
       digitalWrite (buzzer, LOW);                                                          // This will turn the buzzer OFF
       delay (20) ; 
  }
}       

void thief()
{
    i=0;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("****BLOCKED****");
    lcd.setCursor(0,1);
    SendMessage();
                                                                          // Declaring variables
     while (1)
     {
       for (i = 0; i <80; i++)
       {                                                                     // 100 cycles of sound
          digitalWrite (buzzer, HIGH);                                                         // This will turn the buzzer ON
          delay (1) ;                                                                                      // Giving a Delay of 1ms will set frequency 1
          digitalWrite (buzzer, LOW);                                                          // This will turn the buzzer OFF
          delay (1) ;                                                                                      // Giving a delay ms
       }
       for (i = 0; i <100; i++)
       {                                                                       // 100 cycles of sound
           digitalWrite (buzzer, HIGH);                                                         // This will turn the buzzer ON
           delay (2) ;                                                                                      // Giving a delay of 2ms will set frequency 2
           digitalWrite (buzzer, LOW);                                                          // This will turn the buzzer OFF
           delay (2) ;                                                                                      // Giving a delay of 2ms
       }
     }
    
}
void change()
{
  
  int j=0;
  lcd.clear();
  lcd.print("UR CURRENT PASSk");
  lcd.setCursor(0,1);
  
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      beep();
    }
    key=0;
  }
  delay(200);
  
  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("Wrong Passkey...");
    lcd.setCursor(0,1);
    lcd.print("Better Luck Again");
    delay(500);

    if(change_count<2)
    {
        change_count++;
        change();
    }
    else
        thief();

    
  }
  else
  {
    j=0;
    
    lcd.clear();
    lcd.print("Enter New Passk:");
    lcd.setCursor(0,1);
    while(j<4)
    {
        char key=customKeypad.getKey();
        if(key)
        {
          pass[j]=key;
          lcd.print(key);
          EEPROM.write(j,key);
          j++;
          beep();
        }
   }
    lcd.print("  Done......");
    change_count=0;
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Ur Passk:");
  lcd.setCursor(0,1);
  i=0;
  customKey=0;
}


void beep()
{
  
    for(int k=0;k<5;k++)
    {
      digitalWrite(buzzer, HIGH);
      delay(30);
      digitalWrite(buzzer, LOW);
      delay(30);
    } 
}



void ser(){ 
   // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
   Servo1.write(180); 
   delay(1000); 
   ser();
}

 void SendMessage()
{
  Serial.println("Sending SMS");
  mySerial.begin(9600);
  delay(100);
  mySerial.println("AT+CMGS=\"+917022308599\"\r");
   delay(100);
  mySerial.println("I am SMS from GSM Module");
    delay(1000);
   Serial.println("SENT");
}

