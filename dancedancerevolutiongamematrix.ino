#include "LedControl.h" 

#include <LiquidCrystal.h>

#include<EEPROM.h>

#include "pitches.h"

#include <ezBuzzer.h> // ezBuzzer library

// notes in the melody for game:
int melody1[] = {
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
  NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
  NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,
  
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
  NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
  NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5
};


int noteDurations[] = {
  4, 4, 4, 4,
  8, 4, 2, 4,
  4, 4, 4, 8, 4, 2,
  
  4, 4, 4, 4, 8,
  4, 2, 8, 8, 8,
  8, 8, 8, 8, 8, 4, 2
};


int noteLength;

#define	NOTE_G3	196
#define NOTE_A2 110
#define DELAY_TIME 250

const int choice = 1;
const int menu = 2;//stare menu
const int game = 6; //stare joc
const int settings = 4;
const int setBright = 9;
const int setDifficulty = 10;
const int highScore = 5;
const int about = 7;
const int howPlay = 8;
const int exitt = 11;
const int soundOnOf = 12;
const int resetHighsc = 13;
const int exits = 14;
const int restart = 15;
const int setName = 16;
int menugame[3] = {game, menu,restart}; // to switch from menu state to game state
int menuList[5] = {settings, highScore, howPlay, about, exitt}; // actual menu states
int settingslist[6] ={ setName, setBright, setDifficulty, soundOnOf, resetHighsc, exits};// settings options

// lcd pins
const byte pinRs = 9;

const byte pinEn = 8;

const byte pinD4 = 7;

const byte pinD5 = 6;

const byte pinD6 = 5;

const byte pinD7 = 4;


//matrix pins
const byte dinPin = 12;

const byte clockPin = 11;

const byte loadPin = 10;

const byte matrixSize = 8;

// buttons pins
const int buttonLeft = A5;
const int buttonRight = A4;
const int debounceTime = 100;
bool buttonLeftState = LOW;
bool lastLeftButtonPinState = LOW;
bool buttonRightState = LOW;
bool lastRightButtonPinState = LOW;
bool readLeft = LOW;
bool lastReadLeft = LOW;
bool readRight = LOW;
bool lastReadRight = LOW;
int lastDebounceTime = 0;
bool readSW = LOW;
bool readSW2 = LOW;
bool readSW22 =LOW;
bool readSWS = LOW;
bool readSWSS = LOW;
bool readSWSTN = LOW;
bool readSWH = LOW;
bool readSWB = LOW;
bool readSWD = LOW;
bool readSWSD = LOW;
bool readSWRH = LOW;
bool readSWStr = LOW;
bool buttonSWState = LOW;
bool lastSWButtonPinState = LOW;
// joystick pins
const int joyXPin = A2;
const int joyYPin = A1;
const int pinSW = A3;

// variables for joystick
const int joyLeftThreshold = 350;//in the game
const int joyRightThreshold = 650;//in the game
const int joyDownThreshold = 350;//for menu
const int joyUpThreshold = 650;//for menu


//buzzer pin 
const int buzzerPin = A0;
ezBuzzer buzzer(buzzerPin); // create ezBuzzer object that attach to a pin;


// matrix and lcd initializations

LiquidCrystal lcd(pinRs, pinEn, pinD4, pinD5, pinD6, pinD7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER



// images for 8x8 matrix used for menu
const byte images[][8] {
  {
  0b00111100,
  0b01000010,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b01000010,
  0b00111100
  },
  {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
  },
  {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111
  },
  {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
  },
  {
  0b00010000,
  0b00111000,
  0b10010000,
  0b01111100,
  0b00010010,
  0b00101000,
  0b00100100,
  0b00100010
  },
  {
  0b00010000,
  0b00111000,
  0b00010010,
  0b01111100,
  0b10010000,
  0b00101000,
  0b01001000,
  0b10001000
},
{ 0b00111100,
  0b01111110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100
  },
  {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
  },
  {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11011011
  },
  {
  0b00000011, 
  0b00000011,
  0b00001111,
  0b00001111,
  0b00111111,
  0b00111111,
  0b11111111,
  0b11111111
  },
  {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011000,
  0b00011000,
  0b01111000,
  0b01111110
  },
  {
  0b00111100,
  0b01000010,
  0b00000010,
  0b00001100,
  0b00010000,
  0b00010000,
  0b00000000,
  0b00010000
  },
  {
  0b00011000,
  0b00011000,
  0b00000000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000
  },
  {
  0b00000000,
  0b00100000,
  0b00110000,
  0b00111000,
  0b00111100,
  0b00111000,
  0b00110000,
  0b00100000
  },
  {
  0b00111100,
  0b01000010,
  0b01000010,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00011000
  },
  {
  0b00000000,
  0b01111110,
  0b10000001,
  0b10000001,
  0b10111001,
  0b10110001,
  0b10101001,
  0b10000101
  },
  {
  0b10000000,
  0b10100000,
  0b10101000,
  0b10101000,
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010
  },
  {
  0b10000010,
  0b11000110,
  0b10101010,
  0b10010010,
  0b10000010,
  0b10000010,
  0b10000010,
  0b10000010
  },
  {
  0b00000000,
  0b00011000,
  0b00111100,
  0b00111100,
  0b00011000,
  0b01111110,
  0b11111111,
  0b11111111
  },
  {
  0b00000000,
  0b01011100,
  0b01100010,
  0b01110010,
  0b00000010,
  0b00000010,
  0b00111100,
  0b00000000
  },
  {
  0b00001000,
  0b00011000,
  0b00101000,
  0b01001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000
},
{
  0b00111000,
  0b01000100,
  0b01000100,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01111110
},
{
  0b00111000,
  0b01000100,
  0b00000100,
  0b00011000,
  0b00011000,
  0b00000100,
  0b01000100,
  0b00111000
},
{
  0b00000000,
  0b00010000,
  0b00110100,
  0b11110010,
  0b11110010,
  0b00110100,
  0b00010000,
  0b00000000
},
{
  0b00000000,
  0b00010000,
  0b00110000,
  0b11110101,
  0b11110010,
  0b00110101,
  0b00010000,
  0b00000000
},
{ 0b00000000,
  0b00000000,
  0b00000000,   //nimic 25
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11011011
 },  
 {
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11011011  //stanga buton 26
},
{
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b11011011 //stanga joystick 27
},
{
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b11011011 //dreapta joystick 28
},
{
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b11011011  //dreapta button 29
},
{
  0b11000011,
  0b11000011,
  0b11000011,  //dreapta si stanga button 30
  0b11000011,
  0b11000011,
  0b11000011,
  0b11000011,
  0b11011011
},
{
  0b11010000,
  0b11010000,
  0b11010000,
  0b11010000,  //stg button stg joy 31
  0b11010000,
  0b11010000,
  0b11010000,
  0b11011011
},
{
  0b00010011,
  0b00010011,
  0b00010011,
  0b00010011,
  0b00010011, //dreapta button stg joy 32
  0b00010011,
  0b00010011,
  0b11011011
},
{
  0b00001011,
  0b00001011,
  0b00001011,
  0b00001011,
  0b00001011,  //drpt b drpt joy 33
  0b00001011,
  0b00001011,
  0b11011011
},
{
  0b11001000,
  0b11001000,
  0b11001000,
  0b11001000,  //stg b drt j 34
  0b11001000,
  0b11001000,
  0b11001000,
  0b11011011
},
{
  0b11001011,
  0b11001011,
  0b11001011,
  0b11001011,
  0b11001011,
  0b11001011,  //drt j b L b R 35
  0b11001011,
  0b11011011
},
{
  0b11010011,
  0b11010011,
  0b11010011,
  0b11010011, //stg j b L b R  36
  0b11010011,
  0b11010011,
  0b11010011,
  0b11011011
},
{
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00011000,
  0b00111100
},
{
  0b00010110,
  0b00011001,
  0b00011101,
  0b00000001,
  0b00011010,
  0b00011000,
  0b01111000,
  0b01111110
}


};

const int nameLenght = 3;
const int maxDifficulty = 3;
const int minDifficulty = 1;
const int ON = 1;
const int OFF = 0;

// custom lcd characters
byte arrow[]  = {
  B00000,
  B00100,
  B00110,
  B00111,
  B00110,
  B00100,
  B00000,
  B00000
};

byte arrowUp[] = {
	0b00000,
	0b00000,
	0b00100,
	0b01110,
	0b11111,
	0b00000,
	0b00000,
	0b00000
};

byte arrowDown[] = {
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b01110,
	0b00100,
	0b00000,
	0b00000
};

byte clearCell[] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

const int gol = 25;
const int colstg = 26;
const int mijstg = 27;
const int mijdr = 28;
const int coldr = 29;
const int colstgdr = 30;
const int colstgmijstg = 31;
const int coldrmijstg = 32;
const int coldrmijdr = 33;
const int colstgmijdr = 34;
const int colstgdrmijdr = 35;
const int colstgdrmijstg = 36;



String strr = "HHH";
const int adressScoreNames[5] {11, 15, 19, 23, 27};
const int adressName = 35;
char name[4] ="HHH";
int lenMsg = 180;
int lenMsg3 = 240;
int iii = 0;
int ii;
int scadress = 1;
int adress = 11;
int nmm = adress;
int tempo = 210;
const long interval = 70;
unsigned long lastMiillis = 0;
int lastState = 0;
int lastPoz = 0;
int lastPoz2 = 0;
const int melody[]  = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5,
  NOTE_D5, NOTE_G5, NOTE_E5,
  NOTE_D5, NOTE_G5
};


int durations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4,
  8, 8 ,4, 4 ,4
};

 void displayImageSetup(const byte* image) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          lc.setLed(0, i, j, bitRead(image[i], 7 - j));
        }
      }
    }

    
const char  messagePadded[] PROGMEM = "  Project by Zaharia Diana Cristiana, link github : https://github.com/ZahariaDiana132/Dance-dance-revolution-kind-of-arduino-matrix-game , Nume: Dance Dance Revolution                                                                ";
const char  messagePadded3[] PROGMEM = " For the left/right column press left/right buttons when the respective column lights up, For the middle column depending on what half of the column (left/right) lights up move the joystick to the left/right side          Good luck!                 ";
int letter = 0;
int letter3 = 0;
int poz = 0;
int poz2 = 0;
int poz22 = 0;
int note1 = 0;
int intro = 0;
int nrlives = 4;
int score = 0;
int livesLeft = 3;
int laststart = -1;
int joyX = 0;
int joyY = 0;
int joy2X = 0;
int joy2Y = 0;
int joy22X = 0;
int joy22Y = 0;
int joyGX = 0;
int joyGY = 0;
int joyGY1 = 0;
int joyGY2 = 0;
int joyYSNC = 0;
int joyYB =  0;
int joyYD = 0;
int joyXH = 0;
int joyYSD = 0;
int joyYRH = 0;
int joyXSS = 0;
int joyYLR = 0;
bool joyIsNeutral = true;
bool joyIsNeutrall = true;
unsigned long lastExecutedMillis;
int state = 0; 
int repeatSound = 0 ;
int noteLength2;
int n1,n2,n3,n4,n5; //adrese nume highscore
int scoreStat = 0; // nu e highscore
int notHighS = -1;
int pozChr = 0;
int ok = 0;

//SUUS

// top 5 highscore names

char scoreNames[5][4];
 String st1;
 String st2;
 String st3;
 String st4;
 String st5;

// top 5 scores
int scores[5];

int s1 = 1, s2 = 2, s3 = 3, s4 =4, s5 = 5;

int currentPoz = 0;
int lastCurrentPoz = 0;
int matrixBrightness;
int activeDifficulty;
int chosenDifficulty;
byte sound;
byte opt = OFF;
#define SOUND_ADDR 10
#define DIFFIC_ADDR 9
#define BRIGHT_ADDR 8

int buttonLeftStatus = 0 ; //buttonleft
int buttonRightStatus = 0 ; //buttonright
int joystickStatus = 0;  //joystick
long randNumber = 25;
byte okcdr = 0;
byte okcst = 0;
byte okjdr = 0;
byte okjst = 0;
unsigned long time1 = 0;
unsigned long speed = 2000;
const unsigned long speed0Time = 3000;
const unsigned long speed1Time = 2000;
const unsigned long speed2Time = 1500;
const unsigned long speed3Time = 1300;

void setup() {

  pinMode(pinSW,INPUT_PULLUP);// activate pull-upresistor on the// push-button pin
  lcd.begin(16,2);
  lcd.clear();
  lcd.createChar(0, arrow);
  lcd.createChar(3, arrowUp);
  lcd.createChar(4, arrowDown);
  lcd.createChar(5, clearCell);
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.clearDisplay(0);// clear screen
  sound =  EEPROM.read(SOUND_ADDR);
  matrixBrightness = EEPROM.read(BRIGHT_ADDR);
  activeDifficulty = EEPROM.read(DIFFIC_ADDR);
  
  readStringFromEEPROM(adressName, &strr);

  for (int i = 0; i < 5; i++)
    scores[i] = EEPROM.read((scadress + i));


  int nume1 = readStringFromEEPROM(adress, &st1); 
  int nume2 = readStringFromEEPROM(nume1, &st2);
  int nume3 = readStringFromEEPROM(nume2, &st3);
  int nume4 = readStringFromEEPROM(nume3, &st4);  
  readStringFromEEPROM(nume4, &st5);


  st1.toCharArray(scoreNames[0],4);
  st2.toCharArray(scoreNames[1],4);
  st3.toCharArray(scoreNames[2],4);
  st4.toCharArray(scoreNames[3],4);
  st5.toCharArray(scoreNames[4],4);

  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)

  pinMode(buzzerPin, OUTPUT);
  //analogWrite(pinV0, contrast);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  
   lcd.setCursor(0, 0);
   lcd.print(" Press joystick ");
   lcd.setCursor(0, 1);
   lcd.print(" button to start");

   noteLength = sizeof(noteDurations) / sizeof(int);

}

void loop() {
 

  if(state == 0){
    readSW = digitalRead(pinSW);
  
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSW != buttonSWState)
    {
      buttonSWState = readSW;
      if (buttonSWState == LOW)
      {
             
            lcd.begin(16,2);
            lcd.setCursor(2, 0);
            // Print a message to the LCD.
            lcd.print(" LOADING: ");
            lcd.setCursor(0, 1); 
            lcd.print(" please wait... ");


  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
     if(repeatSound % 2 == 0)
      displayImageSetup(images[4]);
     else 
       displayImageSetup(images[5]);
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
     if(sound == 1){
    tone(buzzerPin, melody[note], duration);}

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(buzzerPin);
    repeatSound++;
  }
  
  displayImageSetup(images[1]);
  lcd.clear();
  lcd.setCursor(2, 0);
  // Print a message to the LCD.
  lcd.print("DANCE  DANCE");
  // set the cursor to column 0, line 1

  // (note: line 1 is the second row, since counting begins with 0):

  lcd.setCursor(3, 1);
  lcd.print("REVOLUTION!");
  delay(2000);
  state = choice;
  lcd.clear();
  repeatSound = 0;
      }
    }
  }
}


  if(state == choice){

   poz=joyStickMenu(menugame[3],3,poz);
  if( poz == 0){
   lcd.setCursor(0, 0);
   lcd.write(byte(0)); 
   lcd.print(" PLAY GAME");
   lcd.setCursor(15, 0);
   lcd.write(byte(5)); 
   lcd.setCursor(0, 1);
   lcd.print("  MENU     ");
   lcd.setCursor(15, 1);
   lcd.write(byte(4)); 
   displayImageSetup(images[13]);
   }
  else if(poz == 1)
  {
   lcd.setCursor(0, 0);
   lcd.write(byte(0)); 
   lcd.print(" MENU     ");
   lcd.setCursor(15, 0);
   lcd.write(byte(3)); 
   lcd.setCursor(0, 1);
   lcd.print("  RESTART    ");
   lcd.setCursor(15, 1);
   lcd.write(byte(4)); 
   displayImageSetup(images[17]);
 
  }
  else {
    
   lcd.setCursor(0, 0);
   lcd.print("  MENU     ");
   lcd.setCursor(15, 0);
   lcd.write(byte(3));
   lcd.setCursor(0, 1);
   lcd.write(byte(0)); 
   lcd.print(" RESTART     ");
   lcd.setCursor(15, 1);
   lcd.write(byte(5)); 
   displayImageSetup(images[19]); 

  }

   readSW = digitalRead(pinSW);
   if (readSW != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSW;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSW != buttonSWState)
    {
      buttonSWState = readSW;
      if (buttonSWState == LOW)
      {
        state = menugame[poz];
        lcd.clear();
        lastState = 0;
      }
    }
  }
   lastState = readSW;
}
 
 if(state == restart){

   lc.clearDisplay(0);
   lcd.setCursor(0, 0);
   lcd.print(" Press joystick ");
   lcd.setCursor(0, 1);
   lcd.print(" button to start");
   state = 0;
 }


  if(state == menu)
  {  
 
    poz2=joyStickMenuList(menuList[5],5,poz2);
     if(lastPoz != poz2)
     lcd.clear();
    lastPoz=poz2;
    if(poz2 == 0){
      displayImageSetup(images[9]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0)); 
      lcd.print(" SETTINGS");
      lcd.setCursor(15, 0);
      lcd.write(byte(5)); 
      lcd.setCursor(0, 1);
      lcd.print("  HIGHSCORE");
      lcd.setCursor(15, 1);
      lcd.write(byte(4)); 
    }
    else if(poz2 == 1){
      displayImageSetup(images[10]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0)); 
      lcd.print(" HIGHSCORE");
      lcd.setCursor(15, 0);
      lcd.write(byte(3)); 
      lcd.setCursor(0, 1);
      lcd.print("  HOW TO PLAY");
      lcd.setCursor(15, 1);
      lcd.write(byte(4)); 
    }
    else if(poz2 == 2){
      displayImageSetup(images[11]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0)); 
      lcd.print(" HOW TO PLAY");
      lcd.setCursor(15, 0);
      lcd.write(byte(3)); 
      lcd.setCursor(0, 1);
      lcd.print("  ABOUT");
      lcd.setCursor(15, 1);
      lcd.write(byte(4)); 
    }
    else if(poz2 == 3) {
      displayImageSetup(images[12]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print(" ABOUT");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1); 
      lcd.print("  EXIT");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));
    }
    else {
      displayImageSetup(images[7]);
      lcd.setCursor(0, 0);
      lcd.print("  ABOUT");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1);
      lcd.write(byte(0)); 
      lcd.print(" EXIT");
      lcd.setCursor(15, 1);
      lcd.write(byte(5));
    }
  readSW2 = digitalRead(pinSW);
   if (readSW2 != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSW2;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSW2 != buttonSWState)
    {
      buttonSWState = readSW2;
      if (buttonSWState == LOW)
      {
        state = menuList[poz2];
        lcd.clear();
        lastState = 0;
        letter = 0;
        letter3 = 0;
      }
    }
  }
  lastState = readSW2;


 }
 
 if(state == exitt){
   state = choice;
 }
  if(state == settings){
      
     poz22 =joyStickSettingsList(settingslist[6],6,poz22);
     if(lastPoz2 != poz22)
     lcd.clear();
    lastPoz2=poz22;



   if(poz22 == 0){
       displayImageSetup(images[18]);
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(15, 0);
      lcd.write(byte(5));
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" SET NAME");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));
   }
    else if(poz22 == 1){
      displayImageSetup(images[14]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0)); 
      lcd.print(" SET BRIGHTNESS");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1);
      lcd.print("  DIFFICULTY");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));
    }
    else if(poz22 == 2){
      displayImageSetup(images[15]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0)); 
      lcd.print(" DIFFICULTY");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1);
      lcd.print("  SOUND ON/OFF");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));
    }
    else if( poz22 == 3 ){
      displayImageSetup(images[16]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print(" SOUND ON/OFF");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1); 
      lcd.print("  RESET HIGH");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));

    }
    else if( poz22 == 4 ){
      displayImageSetup(images[38]);
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print(" RESET HIGH");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1); 
      lcd.print("  EXIT->MENU");
      lcd.setCursor(15, 1);
      lcd.write(byte(4));

    }
    else {
      displayImageSetup(images[7]);
      lcd.setCursor(0, 0);
      lcd.print("  RESET HIGH");
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(0, 1); 
      lcd.write(byte(0));
      lcd.print(" EXIT->MENU");
      lcd.setCursor(15, 1);
      lcd.write(byte(5));

    }

  readSW22 = digitalRead(pinSW);
   if (readSW22 != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSW22;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSW22 != buttonSWState)
    {
      buttonSWState = readSW22;
      if (buttonSWState == LOW)
      {
        state = settingslist[poz22];
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSW22;

  }

 
  if(state == howPlay){
   displayImageSetup(images[8]);
    lcd.setCursor(0, 0);
    lcd.print("INSTRUCTIONS: ");
    letter3 = joyStickScroll2(letter3);
    if(letter3 <= lenMsg3 - 16)
  {
    showLetters3(0, letter3);
  }

   readSWS = digitalRead(pinSW);
   if (readSWS != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWS;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWS != buttonSWState)
    {
      buttonSWState = readSWS;
      if (buttonSWState == LOW)
      {
        state = menu;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWS;

  }

 
  if(state == setName)
  {
    lcd.setCursor(0, 0);
    lcd.print("YOUR NAME: ");
    pozChr = joyStickSetName(pozChr);
  
    lcd.setCursor(pozChr, 1);
    lcd.blink();
    name[pozChr] = joyStickStrCh(name, pozChr);  
    lcd.setCursor(0, 1);
    lcd.print(name);

    

    readSWSTN = digitalRead(pinSW);
   if (readSWSTN != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWSTN;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWSTN != buttonSWState)
    {
      buttonSWState = readSWSTN;
      if (buttonSWState == LOW)
      {
        state = settings;
        lcd.clear();
        lastState = 0;
        strr = (String)name;
        writeStringToEEPROM(adressName, strr);
        lcd.noBlink();
      }
    }
  }
  lastState = readSWSTN;

  }
   

  if(state == about){
    displayImageSetup(images[3]);
    lcd.setCursor(0, 0);
    lcd.print("INFO: ");
    letter = joyStickScroll(letter);
   //From 0 to upto n-16 characters supply to below function
  if( letter <= (lenMsg - 16) )
    showLetters(0, letter);
   
   readSWSS = digitalRead(pinSW);
   if (readSWSS != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWSS;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWSS != buttonSWState)
    {
      buttonSWState = readSWSS;
      if (buttonSWState == LOW)
      {
        state = menu;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWSS;

  }


  if(state == highScore){
    displayImageSetup(images[37]);

    currentPoz = joyStickHigh(currentPoz);

    if(currentPoz != lastCurrentPoz)
       lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print((currentPoz+1));
      lcd.print("  ");
      lcd.print(scoreNames[currentPoz]);
      lcd.print(" : ");
      lcd.print(scores[currentPoz]);
      lcd.setCursor(0, 1);
      lcd.print((currentPoz+2));
      lcd.print("  ");
      lcd.print(scoreNames[currentPoz + 1]);
      lcd.print(" : ");
      lcd.print(scores[currentPoz + 1]);

      lastCurrentPoz = currentPoz;

       readSWH = digitalRead(pinSW);
   if (readSWH != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWH;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWH != buttonSWState)
    {
      buttonSWState = readSWH;
      if (buttonSWState == LOW)
      {
        state = menu;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWH;



  }//EEPROM

  if(state == setBright){
    displayImageSetup(images[2]);
  matrixBrightness = joyStickBright(matrixBrightness);
  EEPROM.update( BRIGHT_ADDR , matrixBrightness );
  lc.setIntensity(0, matrixBrightness);
     readSWB = digitalRead(pinSW);
   if (readSWB != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWB;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWB != buttonSWState)
    {
      buttonSWState = readSWB;
      if (buttonSWState == LOW)
      {
        state = settings;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWB;


    //matrixBrightness creste sau scade -> joystick 

  } //EEPROM

  if(state == setDifficulty){

    activeDifficulty = joyStickDif(activeDifficulty);
    if(activeDifficulty == 1)
        displayImageSetup(images[20]);
    else if(activeDifficulty == 2)
        displayImageSetup(images[21]);
    else 
        displayImageSetup(images[22]);

   EEPROM.update( DIFFIC_ADDR , activeDifficulty);
 readSWD = digitalRead(pinSW);
   if (readSWD != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWD;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWD != buttonSWState)
    {
      buttonSWState = readSWD;
      if (buttonSWState == LOW)
      {
        state = settings;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWD;


  } //EEPROM

  if(state == soundOnOf){

  if(sound == ON)
   displayImageSetup(images[23]);
  else
   displayImageSetup(images[24]);

   lcd.setCursor(0, 0);
   lcd.print("js L/R -> ON/OFF");

   sound = joyStickSound(sound);
   EEPROM.update( SOUND_ADDR , sound );

   readSWSD = digitalRead(pinSW);
   if (readSWSD != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWSD;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWSD != buttonSWState)
    {
      buttonSWState = readSWSD;
      if (buttonSWState == LOW)
      {
        state = settings;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWSD;

  }

 if( state == resetHighsc){
  
  lcd.setCursor(0,0);
  lcd.print(" RIGHT=RESET ");

  opt = joyStickOPT(opt);

  if(opt == ON){
  
   for (int i = 0; i < 5; i++)
    EEPROM.update((scadress + i) , 0);

  for (int i = 0; i < 5; i++)
    scores[i] = EEPROM.read((scadress + i));

  String str1 = "   ";
  int nm1 = writeStringToEEPROM(adress, str1);
  int nm2 = writeStringToEEPROM(nm1, str1);
  int nm3 = writeStringToEEPROM(nm2, str1);
  int nm4 = writeStringToEEPROM(nm3, str1);
  writeStringToEEPROM(nm4, str1);


  int nume1 = readStringFromEEPROM(adress, &st1); 
  int nume2 = readStringFromEEPROM(nume1, &st2);
  int nume3 = readStringFromEEPROM(nume2, &st3);
  int nume4 = readStringFromEEPROM(nume3, &st4);  
  readStringFromEEPROM(nume4, &st5);

  st1.toCharArray(scoreNames[0],4);
  st2.toCharArray(scoreNames[1],4);
  st3.toCharArray(scoreNames[2],4);
  st4.toCharArray(scoreNames[3],4);
  st5.toCharArray(scoreNames[4],4);

  }

     readSWRH = digitalRead(pinSW);
   if (readSWRH != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastState = readSWRH;
  }
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readSWRH != buttonSWState)
    {
      buttonSWState = readSWRH;
      if (buttonSWState == LOW)
      {
        state = settings;
        lcd.clear();
        lastState = 0;
      }
    }
  }
  lastState = readSWRH;

 }

  if (state == exits){
     state = menu;
  }

  if (state == game){

  buzzer.loop(); // MUST call the buzzer.loop() function in loop()
 if(sound == ON ){
  if (buzzer.getState() == BUZZER_IDLE) { // if stopped
    buzzer.playMelody(melody1, noteDurations, noteLength); // playing
  }
}

   if(intro == 0){
   randomSeed(analogRead(0));
   lastMiillis++;
   displayImageSetup(images[1]);
   lcd.setCursor(1, 0);
   lcd.print(strr);
   lcd.print(" get ready ");
   lcd.setCursor(3, 1);
   lcd.print("to dance!");
   if(lastMiillis == interval)
    {lcd.clear(); intro = 1; lastMiillis = 0; score = 0;}

   }
   if(intro == 1) { 
     lastMiillis++;
     displayImageSetup(images[8]);
     lcd.setCursor(0, 0);
     lcd.print("   React fast   ");
  if(lastMiillis == interval) {
      intro = 2;
      iii = 0;
      randNumber = gol;  
      okcdr = 0;
      okcst = 0;
      okjdr = 0;
      okjst = 0; 
      time1 = 0;
      chosenDifficulty = activeDifficulty;
      lcd.clear();
      }
   }

   if(intro == 2){ 

   lcd.setCursor(0, 0);
   lcd.print("lives:");
   lcd.print(nrlives);
   lcd.setCursor(9, 0);
   if(activeDifficulty == 1)
    lcd.print("   EASY");
   else if(activeDifficulty == 2)
    lcd.print(" MEDIUM");
   else if(activeDifficulty == 3)
    lcd.print("   HARD");

  if(activeDifficulty == 1){
    if(score <= 6)
    speed = speed0Time;
    else if(score > 6 && score < 15)
    speed = speed1Time;
    else if(score >= 15 && score <= 30)
    speed = speed2Time;
    else if(score > 30) {
    activeDifficulty = 2;
    nrlives++;

    }
  }
    
   else if(activeDifficulty == 2){
    
    if(score <= 55)
    speed = speed0Time;
    if(score >55 && score <= 95)
    speed = speed1Time;
    if(score > 95 && score <= 125)
    speed = speed2Time;
    else if(score > 125){
      nrlives++;
    activeDifficulty = 3;
    }
    
   }
   
   else {
    if(score <= 55)
    speed = speed1Time; 
    if(score >55 && score < 90)
    speed = speed2Time;
    else
    speed = speed3Time;
   }
 


   lcd.setCursor(0, 1);
   lcd.print("score: ");
   lcd.print(score);
   displayImageSetup(images[randNumber]);
   //joyStickGame(nrlives,score); // for testing 
   joyStickLeftRight(joystickStatus);
   buttonLeftPress(buttonLeftStatus);
   buttonRightPress(buttonRightStatus);

   if( randNumber == coldr && okcdr != 1){
     if (buttonRightStatus == 2){
       if(activeDifficulty == 1){
       score++;
       }
       else if(activeDifficulty == 2)
       {
       score = score + 2;
       }
       else
        score= score + 3;
      okcdr = 1;
     }
      else if (buttonLeftStatus == 1){
       nrlives--;
       okcdr = 1;
     }
     else if(joystickStatus == 4){
       nrlives--;
       okcdr = 1;
     }
      else if(joystickStatus == 3){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }
 
  }

    if( randNumber == colstg && okcst != 1){
     if (buttonLeftStatus == 1){
        if(activeDifficulty == 1){
           score++;
       }
       else if(activeDifficulty == 2)
       {
           score = score + 2;
       }
       else
        score= score + 3;
         okcst = 1;
     }
     else if (buttonRightStatus == 2){
       nrlives--;
       okcst = 1;
     }
     else if(joystickStatus == 4){
       nrlives--;
       okcst = 1;
     }
      else if(joystickStatus == 3){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }
   
  }

    if( randNumber == mijstg && okjst != 1){
     if (joystickStatus == 3){
          if(activeDifficulty == 1){
           score++;
       }
       else if(activeDifficulty == 2)
       {
           score = score + 2;
       }
       else
        score= score + 3;
         okjst = 1;
     }
     else if(joystickStatus == 4){
       nrlives--;
       okjst = 1;
     }
      else if (buttonRightStatus == 2){
       nrlives--;
       okjst = 1;
     }
      else if (buttonLeftStatus == 1){
       nrlives--;
       okjst = 1;
     }
      else {
       okjst = 2;
     }
  
  }

  if( randNumber == mijdr && okjdr != 1){
     if (joystickStatus == 4){
          if(activeDifficulty == 1){
           score++;
       }
       else if(activeDifficulty == 2)
       {
           score = score + 2;
       }
       else
        score= score + 3;

      okjdr = 1;
     }
      else if(joystickStatus == 3){
       nrlives--;
       okjdr = 1;
     }
      else if (buttonRightStatus == 2){
       nrlives--;
       okjdr = 1;
     }
      else if (buttonLeftStatus == 1){
       nrlives--;
       okjdr = 1;
     }
     else  {
       okjdr = 2;
     }
    
  }

  if(randNumber == colstgmijstg && (okcst != 1 || okjst != 1)){
  if(okcst != 1){
     if (buttonLeftStatus == 1){
        okcst = 1;
        score = score + 2;
     }
     else if(buttonRightStatus == 2){
       nrlives--;
       okcst = 1;
     }
       else if(joystickStatus == 4){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }

  }

   if(okjst != 1){

      if (joystickStatus == 3 ){
         okjst = 1;
        score = score + 2;
        if(nrlives <= 2)
           nrlives++;
     }
    else if(joystickStatus == 4){
       nrlives--;
       okjst = 1;
     }
      else if(buttonRightStatus == 2){
       nrlives--;
       okjst = 1;
     }
      else {
       okjst = 2;
     }
   }

  }

 if(randNumber == coldrmijdr && (okcdr != 1 || okjdr != 1)){
  if(okcdr != 1){
     if (buttonRightStatus == 2){
        okcdr = 1;
        score = score + 2;
     }
     else if(buttonLeftStatus == 1){
       nrlives--;
       okcdr = 1;
     }
       else if(joystickStatus == 3){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }

  }

   if(okjdr != 1){

      if (joystickStatus == 4 ){
         okjdr = 1;
         score = score + 2;
         if(nrlives <= 2)
           nrlives++;
     }
    else if(joystickStatus == 3){
       nrlives--;
       okjdr = 1;
     }
      else if(buttonRightStatus == 2){
       nrlives--;
       okjdr = 1;
     }
      else {
       okjdr = 2;
     }
   }

  }

  if(randNumber == colstgmijdr && (okcst != 1 || okjdr != 1)){
  if(okcst != 1){
     if (buttonLeftStatus == 1){
        okcst = 1;
        score = score + 2;
     }
     else if(buttonRightStatus == 2){
       nrlives--;
       okcst = 1;
     }
       else if(joystickStatus == 3){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }

  }

   if(okjdr != 1){

      if (joystickStatus == 4 ){
         okjdr = 1;
         score = score + 2;
         if(nrlives <= 2)
           nrlives++;
     }
    else if(joystickStatus == 3){
       nrlives--;
       okjdr = 1;
     }
      else if(buttonRightStatus == 2){
       nrlives--;
       okjdr = 1;
     }
      else {
       okjdr = 2;
     }
   }

  }

  if(randNumber == coldrmijstg && (okcdr != 1 || okjst != 1)){
  if(okcdr != 1){
     if (buttonRightStatus == 2){
        okcdr = 1;
        score = score + 2;
     }
     else if(buttonLeftStatus == 1){
       nrlives--;
       okcdr = 1;
     }
       else if(joystickStatus == 4){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }

  }

   if(okjst != 1){

      if (joystickStatus == 3 ){
         okjst = 1;
         score = score + 2;
         if(nrlives <= 2)
           nrlives++;
     }
    else if(joystickStatus == 4){
       nrlives--;
       okjst = 1;
     }
      else if(buttonRightStatus == 2){
       nrlives--;
       okjst = 1;
     }
      else {
       okjst = 2;
     }
   }

  }



  if(randNumber == colstgdr && (okcdr != 1 || okcst != 1)){
    
    if(okcst != 1){
     if (buttonLeftStatus == 1){
        okcst = 1;
        score++;
     }
     else if(joystickStatus == 3){
       nrlives--;
       okcst = 1;
     }
       else if(joystickStatus == 4){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }
  }
  if(okcdr != 1){

      if (buttonRightStatus == 2 ){
         okcdr = 1;
         score++;
     }
    else if(joystickStatus == 4){
       nrlives--;
       okcdr = 1;
     }
      else if(joystickStatus == 3){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }
   }
 }


  if(randNumber == colstgdrmijdr && (okcdr != 1 || okcst != 1 || okjdr != 1)){
    
    if(okcst != 1){
     if (buttonLeftStatus == 1){
        okcst = 1;
        score++;
     }
     else if(joystickStatus == 3){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }
  }
  if(okcdr != 1){

      if (buttonRightStatus == 2 ){
         okcdr = 1;
         score++;
     }
      else if(joystickStatus == 3){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }
   }

  if(okjdr != 1){
   if(joystickStatus == 4 ){
       score= score + 3;
       if(nrlives <= 2)
        nrlives++;
       okjdr = 1;
     }
    else if(joystickStatus == 3){
       nrlives--;
       okjdr = 1;
     }

  }

 }

   if(randNumber == colstgdrmijstg && (okcdr != 1 || okcst != 1 || okjst != 1)){
    
    if(okcst != 1){
     if (buttonLeftStatus == 1){
        okcst = 1;
        score++;
     }
     else if(joystickStatus == 4){
       nrlives--;
       okcst = 1;
     }
      else {
       okcst = 2;
     }
  }
  if(okcdr != 1){

      if (buttonRightStatus == 2 ){
         okcdr = 1;
         score++;
     }
      else if(joystickStatus == 4){
       nrlives--;
       okcdr = 1;
     }
      else {
       okcdr = 2;
     }
   }

  if(okjst != 1){
   if(joystickStatus == 3 ){
       score= score + 3;
       if(nrlives <= 2)
        nrlives++;
       okjst = 1;
     }
    else if(joystickStatus == 4){
       nrlives--;
       okjst = 1;
     }

  }

 }



  if(millis() > time1 + speed){    
   time1 = millis();
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   displayImageSetup(images[gol]);
   if(okjdr == 2)
     nrlives--;
   if(okjst == 2)
     nrlives--;
   if(okcdr == 2)
     nrlives--;
   if(okcst == 2)
     nrlives--;
   if(activeDifficulty == 1){
   randNumber = random(colstg, colstgdr);
   }
   else if(activeDifficulty == 2){
   randNumber = random(colstg, colstgdrmijdr);
   }
   else {
      randNumber = random(colstg, colstgdrmijstg + 1);
   }
   okcdr = 0;
   okcst = 0;
   okjdr = 0;
   okjst = 0;
  
  }

   
   if(nrlives < 1){
     
    if(iii == 0){
   ii = isItHigh(score, iii); 
   lcd.clear();
    } 
     if( ii == notHighS)
    {  
      lcd.setCursor(4, 0);
      lcd.print("GAME OVER");
      lcd.setCursor(4, 1);
      lcd.print("score: ");
      lcd.print(score);
      noTone(buzzerPin);
      delay(1000);
       lcd.clear();
      intro = 0;
      lastMiillis = 0;
      nrlives = 4;
      ok = 0;
      state = choice;
      activeDifficulty = chosenDifficulty;
    }
    else
    { 
      lcd.setCursor(2, 0);
      lcd.print("New Highscore");
      lcd.setCursor(3, 1);
      lcd.print("score: ");
      lcd.print(score);
      for(int i = 4; i > ii; i--){
      EEPROM.update((scadress + i) , scores[i - 1]);
      scores[i] = EEPROM.read((scadress + i));
       writeStringToEEPROM(adressScoreNames[i], (String)scoreNames[i-1]);
       String stttt;
       readStringFromEEPROM(adressScoreNames[i], &stttt);
      stttt.toCharArray(scoreNames[i],4);
       }
    
      EEPROM.update((scadress + ii) , score);
      scores[ii] = EEPROM.read((scadress + ii));

    writeStringToEEPROM(adressScoreNames[ii],strr);
    strr.toCharArray(scoreNames[ii],4);
    
    noTone(buzzerPin);
     delay(2000);
     lcd.clear();
     intro = 0;
     lastMiillis = 0;
     nrlives = 4;
     ok = 0;
     state = choice;
     activeDifficulty = chosenDifficulty;
    
      }

     
     }

   }

  }


}

int  isItHigh(int score, int &iii)
{ 
  int ok;
   while(iii < 5 && ok == 0){
    if(score > scores[iii]){ 
      ok = 1;
      return iii;
    }
    iii++;
   }
   return -1;

}




char joyStickStrCh(char name[4], int pozChr){
  joyXSS = analogRead(joyXPin); 
   if(joyXSS < joyDownThreshold && joyIsNeutral) { 

     joyStkSound();
     joyIsNeutral = false;
     if(name[pozChr] < 'Z')
      name[pozChr] ++;
     return name[pozChr];
   }
    if(joyXSS > joyUpThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      if(name[pozChr] > 'A')
       name[pozChr] --;
      return name[pozChr];

    }
  
     if(joyDownThreshold <= joyXSS && joyXSS <= joyUpThreshold) {
       joyIsNeutral = true; 
       return name[pozChr];
    }

}
void showLetters(int print, int start1)
{  
    lcd.setCursor(print, 1);
   for (int letter = start1; letter <= start1 + 15; letter++) 
  {
    lcd.write(pgm_read_byte_near(messagePadded + letter));
  }
    lcd.print(" ");

  
}

void showLetters3(int printStart, int startLetter)
{
  lcd.setCursor(printStart, 1);
  for (int letter3 = startLetter; letter3 <= startLetter + 15; letter3++) 
  {
    lcd.write(pgm_read_byte_near(messagePadded3 + letter3));
  }

  lcd.print(" ");
 

}

void buttonRightPress(int &status2){
 readRight= digitalRead(buttonRight);
  
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readRight != buttonRightState)
    {
      buttonRightState = readRight;
      if (buttonRightState == LOW)
      {
       status2 = 2;
      }
      else
       status2 = 0;
    }
  }
  lastReadRight = readRight;

}

void buttonLeftPress( int &status1){
   readLeft= digitalRead(buttonLeft);
  
  if (millis() - lastDebounceTime > debounceTime)
  {
    if (readLeft != buttonLeftState)
    {
      buttonLeftState = readLeft;
      if (buttonLeftState == LOW)
      {

       status1 = 1;
      }
      else 
       status1 = 0;
    }
  }
  lastReadLeft = readLeft;

}

void joyStickLeftRight( int &status3){ 
     
     joyYLR = analogRead(joyYPin);

   if(joyYLR > joyRightThreshold && joyIsNeutrall) { 
      joyIsNeutrall = false;
      status3 = 3;
    }
    if(joyYLR < joyLeftThreshold && joyIsNeutrall) { 
      joyIsNeutrall = false;
      status3 = 4;
    }
    
     if(joyLeftThreshold <= joyYLR && joyYLR <= joyRightThreshold) {
       joyIsNeutrall = true; 
       status3 = 0;
    }

}


byte joyStickOPT(byte opt){ 
     
     joyYRH = analogRead(joyYPin);

   if(joyYRH > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print("No thanks      ");
      opt = OFF;
      return opt;
   
    }
    if(joyYRH < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print("RESET PENDING  ");
      opt = ON;
      return opt;
    
    }
    
     if(joyLeftThreshold <= joyYRH && joyYRH <= joyRightThreshold) {
       joyIsNeutral = true; 
       lcd.setCursor(0, 1);
       if(opt == OFF)
       lcd.print("NOPE          ");
       else
       lcd.print("RESET         ");
       return opt;
     
  }
}


int joyStickMenu(int aleg[], int n, int poz){ //for menu choices
     joyX = analogRead(joyXPin);
     joyY = analogRead(joyYPin);
   if(joyX < joyDownThreshold && joyIsNeutral) { 
     joyStkSound();
     joyIsNeutral = false;
     if(poz > 0)
     {
       poz--; 
       return poz;}
     else {
        
         return poz;
      }
   
   
   }
    if(joyX > joyUpThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      if(poz < n-1)
        {poz++; 
      
        return poz;
        }
     else {
        
        return poz;
     }

    }
   if(joyY > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      displayImageSetup(images[7]);
      return poz;

    }
    if(joyY < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      displayImageSetup(images[7]);
      joyStkSound();
      return poz;

    }
    
     if(joyLeftThreshold <= joyY && joyY <= joyRightThreshold && joyDownThreshold <= joyX && joyX <= joyUpThreshold) {
       joyIsNeutral = true; 
       return poz;
     
       
  }
}

int joyStickHigh(int currentPoz){
     joyXH = analogRead(joyXPin);
   if(joyXH < joyDownThreshold && joyIsNeutral) { 
     joyIsNeutral = false;
     if(currentPoz > 0)
     {joyStkSound();
       currentPoz--; 
       return currentPoz;}
     else {
         joyStkSound();
         return currentPoz;
      }
   }
    if(joyXH > joyUpThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      if(currentPoz < 3)
        {currentPoz++; 
        joyStkSound();
        return currentPoz;
        }
     else {
        joyStkSound();
        return currentPoz;
     }

    }
    
     if(joyDownThreshold <= joyXH && joyXH <= joyUpThreshold) {
       joyIsNeutral = true; 
       return currentPoz;
  }
}


int joyStickMenuList(int choices[], int n, int poz2){ //for menu list choices
      joy2X = analogRead(joyXPin);
      joy2Y = analogRead(joyYPin);
   if(joy2X < joyDownThreshold && joyIsNeutral) { 
     joyIsNeutral = false;
     if(poz2 > 0)
     {joyStkSound();
       poz2--; 
       return poz2;}
     else {
         joyStkSound();
         return poz2;
      }
   
   
   }
    if(joy2X > joyUpThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      if(poz2 < n-1)
        {poz2++; 
        joyStkSound();
        return poz2;
        }
     else {
        joyStkSound();
        return poz2;
     }

    }
   if(joy2Y > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      displayImageSetup(images[7]);
      return poz2;

    }
    if(joy2Y < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
       joyStkSound();
      displayImageSetup(images[7]);
      return poz2;

    }
    
     if(joyLeftThreshold <= joy2Y && joy2Y <= joyRightThreshold && joyDownThreshold <= joy2X && joy2X <= joyUpThreshold) {
       joyIsNeutral = true; 
       return poz2;
     
       
  }
}

int joyStickSettingsList(int choices[], int n, int poz22){ //for menu list choices
     joy22X = analogRead(joyXPin);
     joy22Y = analogRead(joyYPin);
   if(joy22X < joyDownThreshold && joyIsNeutral) { 
     joyIsNeutral = false;
     if(poz22 > 0)
     {joyStkSound();
       poz22--; 
       return poz22;}
     else {
         joyStkSound();
         return poz22;
      }
   
   
   }
    if(joy22X > joyUpThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      if(poz22 < n-1)
        {poz22++; 
        joyStkSound();
        return poz22;
        }
     else {
        joyStkSound();
        return poz22;
     }

    }
   if(joy22Y > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      displayImageSetup(images[7]);
      return poz22;

    }
    if(joy22Y < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
       joyStkSound();
      displayImageSetup(images[7]);
      return poz22;

    }
    
     if(joyLeftThreshold <= joy22Y && joy22Y <= joyRightThreshold && joyDownThreshold <= joy22X && joy22X <= joyUpThreshold) {
       joyIsNeutral = true; 
       return poz22;
     
       
  }
}

int joyStickSetName(int pozChr){

 joyYSNC = analogRead(joyYPin);

   if(joyYSNC > joyRightThreshold && joyIsNeutrall) { 
      joyIsNeutrall = false;
      joyStkSound();
        if(pozChr > 0)
      pozChr --;
      return pozChr;

    }
    if(joyYSNC < joyLeftThreshold && joyIsNeutrall) { 
      joyIsNeutrall = false;
      joyStkSound();
       if(pozChr < 2)
       pozChr++;
      return pozChr;
    }
    
     if(joyLeftThreshold <= joyYSNC && joyYSNC <= joyRightThreshold) {
       joyIsNeutrall = true; 
       return pozChr;
     
  }

}
void joyStickGame(int &nr, int &score){ //for game
     
     joyGX = analogRead(joyXPin);
   if(joyGX < joyDownThreshold && joyIsNeutral) { 
     lcd.clear();
      nr--;
     joyIsNeutral = false;
     joyStkSound();   //pause
   }
    if(joyGX > joyUpThreshold && joyIsNeutral) {
      lcd.clear(); 
      joyIsNeutral = false;
      score++;
      joyStkSound();   //get out of pause
    }
 
     if(joyDownThreshold <= joyGX && joyGX <= joyUpThreshold) {
       joyIsNeutral = true; 
     
       
  }
}

byte joyStickSound(byte sound){ 
     
     joyYSD = analogRead(joyYPin);

   if(joyYSD > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print("ON ");
      sound = ON;
      return sound;
   
    }
    if(joyYSD < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print("OFF");
      sound = OFF;
      return sound;
    
    }
    
     if(joyLeftThreshold <= joyYSD && joyYSD <= joyRightThreshold) {
       joyIsNeutral = true; 
       lcd.setCursor(0, 1);
       if(sound == OFF)
       lcd.print("OFF");
       else
       lcd.print("ON ");
       return sound;
     
  }
}


int joyStickDif(int activeDifficulty){ 
     
     joyYD = analogRead(joyYPin);

   if(joyYD > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print(activeDifficulty);
      if(activeDifficulty > minDifficulty)
      activeDifficulty--;
      return activeDifficulty;
   
    }
    if(joyYD < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      lcd.print(activeDifficulty);

      if(activeDifficulty < maxDifficulty)
      activeDifficulty++;

      return activeDifficulty;
    
    }
    
     if(joyLeftThreshold <= joyYD && joyYD <= joyRightThreshold) {
       joyIsNeutral = true; 
       lcd.setCursor(0, 1);
       lcd.print(activeDifficulty);
       return activeDifficulty;
     
  }
}

int joyStickBright(int matrixBrightness){ 
     
     joyYB = analogRead(joyYPin);

   if(joyYB > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      if(matrixBrightness < 10)
      lcd.print(" ");
      lcd.print(matrixBrightness);
      if(matrixBrightness >= 2)
      matrixBrightness = matrixBrightness - 2 ;
      return matrixBrightness;
   
    }
    if(joyYB < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      lcd.setCursor(0, 1);
      if(matrixBrightness < 10)
      lcd.print(" ");
      lcd.print(matrixBrightness);
      if(matrixBrightness <= 13)
      matrixBrightness = matrixBrightness + 2;
      return matrixBrightness;
    
    }
    
     if(joyLeftThreshold <= joyYB && joyYB <= joyRightThreshold) {
       joyIsNeutral = true; 
       lcd.setCursor(0, 1);
       if(matrixBrightness < 10)
       lcd.print(" ");
       lcd.print(matrixBrightness);
       return matrixBrightness;
     
  }
}

int joyStickScroll(int start){ 
     
     joyGY1 = analogRead(joyYPin);

   if(joyGY1 > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      if(start <=  (lenMsg - 16 - 11))
      start=start + 11;
      return start;
   
    }
    if(joyGY1 < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      if(start>=11)
     {start = start - 11;}
      joyStkSound();
      return start;
    
    }
    
     if(joyLeftThreshold <= joyGY1 && joyGY1 <= joyRightThreshold) {
       joyIsNeutral = true; 
       return start;
     
  }
}

int joyStickScroll2(int start2){ 
     
     joyGY2 = analogRead(joyYPin);

   if(joyGY2 > joyRightThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      if(start2 <= lenMsg3 - 16 - 11)
      start2 = start2 + 11;
      return start2;

    }
    if(joyGY2 < joyLeftThreshold && joyIsNeutral) { 
      joyIsNeutral = false;
      joyStkSound();
      if(start2 >= 11 )
      start2 = start2 - 11;
      return start2;
    }
    
     if(joyLeftThreshold <= joyGY2 && joyGY2 <= joyRightThreshold) {
       joyIsNeutral = true; 
       return start2;
     
  }
}

void joyStkSound()
{ 
  if(sound == 1)
  tone(buzzerPin, NOTE_G3, 10);
  else
  noTone(buzzerPin);
  
}

int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}

int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; 
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

