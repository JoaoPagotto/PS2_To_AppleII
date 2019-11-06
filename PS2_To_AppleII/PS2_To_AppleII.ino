//-----------------------------------------------------------------------------
//
//  Keyboard PS/2 to Apple II v1.10
//
//  Date: 2019-11-05 20:00pm
//
//  João Ricardo Pagotto
//  https://github.com/joaopagotto
//
//  This is free open-source software, distributed under a very liberal license
//  (the well-known MIT license). It may be used for any purpose, including
//  commercial purposes, at absolutely no cost. Just download it and use it.
//
//-----------------------------------------------------------------------------

#include <PS2KeyAdvanced.h>

PS2KeyAdvanced keyboard;

//---------------------------

const int PS2_DATA = 3;
const int PS2_IRQ  = 2;

//---------------------------

#define DEBUG 1

#define APII_TIME_STROBE    10 // microsecs
#define APII_TIME_INTERVAL 500 // microsecs
#define APII_TIME_RESET     10 // msecs

const int APII_STROBE = 4;
const int APII_RESET  = 5;
const int APII_B0     = 6;
const int APII_B1     = 7;
const int APII_B2     = 8;
const int APII_B3     = 9;
const int APII_B4     = 10;
const int APII_B5     = 11;
const int APII_B6     = 12;

//-----------------------------------------------------------------------------

void apII_send_key(unsigned char data);
void apII_send_reset();

//-----------------------------------------------------------------------------

void setup() 
{
  pinMode(APII_RESET, OUTPUT);
  digitalWrite(APII_RESET, HIGH);
  
  pinMode(APII_STROBE, OUTPUT);
  digitalWrite(APII_STROBE, LOW);
  
  pinMode(APII_B0, OUTPUT);
  digitalWrite(APII_B0, LOW);

  pinMode(APII_B1, OUTPUT);
  digitalWrite(APII_B1, LOW);

  pinMode(APII_B2, OUTPUT);
  digitalWrite(APII_B2, LOW);

  pinMode(APII_B3, OUTPUT);
  digitalWrite(APII_B3, LOW);

  pinMode(APII_B4, OUTPUT);
  digitalWrite(APII_B4, LOW);

  pinMode(APII_B5, OUTPUT);
  digitalWrite(APII_B5, LOW);

  pinMode(APII_B6, OUTPUT);
  digitalWrite(APII_B6, LOW);

  //-------------------------  

  #if defined(DEBUG)
    Serial.begin(9600);
    Serial.println("Keyboard PS/2 to Apple II v1.00 (DEBUG)\n\r");
  #endif

  //-------------------------

  pinMode(LED_BUILTIN, OUTPUT);

  //-------------------------

  keyboard.begin(PS2_DATA, PS2_IRQ); // Setup keyboard pins
  keyboard.setNoBreak(1);            // No break codes for keys (when key released)
  keyboard.setNoRepeat(1);           // Don't repeat shift ctrl etc  
  keyboard.setLock(PS2_LOCK_NUM);    // Sets the current status of Locks and LEDs
  
}

//-----------------------------------------------------------------------------

void loop() 
{
  int data;
  unsigned char chr;

  if ( ! keyboard.available() )
    return;

  data = keyboard.read();
  chr = ( data & 0xFF );
  
  if ( data == 0 )
    return;

  //---------------------------
  // Debug

  #if defined(DEBUG)
    Serial.print("> ");
  
    if ( data & PS2_BREAK    ) Serial.print("BREAK, ");
    if ( data & PS2_SHIFT    ) Serial.print("SHIFT, ");
    if ( data & PS2_CTRL     ) Serial.print("CTRL, ");
    if ( data & PS2_CAPS     ) Serial.print("CAPS, ");
    if ( data & PS2_ALT      ) Serial.print("ALT, ");
    if ( data & PS2_ALT_GR   ) Serial.print("ALTGR, ");
    if ( data & PS2_GUI      ) Serial.print("GUI, ");
    if ( data & PS2_FUNCTION ) Serial.print("FUNCTION, ");
      
    Serial.print(chr, DEC);      
    Serial.print(" '");      
    Serial.print((char)chr);
    Serial.print("'");      
    Serial.print("\n\r");
  #endif

  //---------------------------

  if ( data & PS2_FUNCTION ) 
  {
    if ( data & PS2_CTRL ) 
    {
      switch ( chr ) 
      {
        case PS2_KEY_BREAK:
          apII_send_reset(); break; // Ctrl + Break
      }
      
      return;
    }

    if ( data & PS2_SHIFT ) 
    {
      switch ( chr ) 
      {
        case PS2_KEY_INTL1: 
          apII_send_key(0xBF); break; // ?
      }
      
      return;
    }
    
    switch ( chr ) 
    {    
      case PS2_KEY_ENTER:
      case PS2_KEY_KP_ENTER:
        apII_send_key(0x8D); break; // [Enter]

      case PS2_KEY_ESC:
        apII_send_key(0x9B); break; // [Esc]

      case PS2_KEY_SPACE:
        apII_send_key(0xA0); break; // [Space]

      case PS2_KEY_BS:
      case PS2_KEY_L_ARROW:
        apII_send_key(0x88); break; // [Left_Arrow]
      
      case PS2_KEY_R_ARROW:
        apII_send_key(0x95); break; // [Right_Arrow]

      case PS2_KEY_INTL1:
        apII_send_key(0xAF); break; // /
        
    }
    
    return;
  }
  else
  {
    if ( data & PS2_CTRL ) 
    {
      switch ( chr ) 
      {
        case PS2_KEY_C:
          apII_send_key(0x83); break; // Ctrl + C
      }
           
      return;
    }

    if ( data & PS2_SHIFT ) 
    {
      switch ( chr ) 
      {
        case PS2_KEY_SINGLE: 
          apII_send_key(0xA2); break; // "     

        case PS2_KEY_1:
          apII_send_key(0xA1); break; // !

        case PS2_KEY_2:
          apII_send_key(0xC0); break; // @      

        case PS2_KEY_3:
          apII_send_key(0xA3); break; // #

        case PS2_KEY_4:
          apII_send_key(0xA4); break; // $

        case PS2_KEY_5:
          apII_send_key(0xA5); break; // %

        case PS2_KEY_7:
          apII_send_key(0xA6); break; // &

        case PS2_KEY_8:
          apII_send_key(0xAA); break; // *

        case PS2_KEY_9:
          apII_send_key(0xA8); break; // (

        case PS2_KEY_0:
          apII_send_key(0xA9); break; // )

        case PS2_KEY_EQUAL:
          apII_send_key(0xAB); break; // +

        case PS2_KEY_APOS: 
          apII_send_key(0xDE); break; // ^

        case PS2_KEY_COMMA:
          apII_send_key(0xBC); break; // <

        case PS2_KEY_DOT:
          apII_send_key(0xBE); break; // >

        case PS2_KEY_DIV:
          apII_send_key(0xBA); break; // :
      }                                                  
           
      return;
    }
    
    if ( data < 256 )
    {
      switch ( chr ) 
      {
        case PS2_KEY_KP0:
        case PS2_KEY_0:
          apII_send_key(0xB0); break; // 0

        case PS2_KEY_KP1:
        case PS2_KEY_1:
          apII_send_key(0xB1); break; // 1

        case PS2_KEY_KP2:
        case PS2_KEY_2:
          apII_send_key(0xB2); break; // 2

        case PS2_KEY_KP3:
        case PS2_KEY_3:
          apII_send_key(0xB3); break; // 3

        case PS2_KEY_KP4:
        case PS2_KEY_4:
          apII_send_key(0xB4); break; // 4

        case PS2_KEY_KP5:
        case PS2_KEY_5:
          apII_send_key(0xB5); break; // 5

        case PS2_KEY_KP6:
        case PS2_KEY_6:
          apII_send_key(0xB6); break; // 6

        case PS2_KEY_KP7:
        case PS2_KEY_7:
          apII_send_key(0xB7); break; // 7

        case PS2_KEY_KP8:
        case PS2_KEY_8:
          apII_send_key(0xB8); break; // 8

        case PS2_KEY_KP9:
        case PS2_KEY_9:
          apII_send_key(0xB9); break; // 9

        case PS2_KEY_KP_TIMES: 
          apII_send_key(0xAA); break; // *
        
        case PS2_KEY_KP_MINUS: 
          apII_send_key(0xAD); break; // -

        case PS2_KEY_KP_PLUS:
          apII_send_key(0xBB); break; // +

        case PS2_KEY_KP_DOT: 
          apII_send_key(0xAC); break; // .

        case PS2_KEY_KP_ENTER: 
          apII_send_key(0x8D); break; // [ENTER]

        case PS2_KEY_KP_COMMA: 
          apII_send_key(0xAE); break; // ,
      
        case PS2_KEY_A: 
          apII_send_key(0xC1); break; // A

        case PS2_KEY_B: 
          apII_send_key(0xC2); break; // B

        case PS2_KEY_C: 
          apII_send_key(0xC3); break; // C

        case PS2_KEY_D: 
          apII_send_key(0xC4); break; // D

        case PS2_KEY_E: 
          apII_send_key(0xC5); break; // E

        case PS2_KEY_F: 
          apII_send_key(0xC6); break; // F

        case PS2_KEY_G: 
          apII_send_key(0xC7); break; // G

        case PS2_KEY_H: 
          apII_send_key(0xC8); break; // H

        case PS2_KEY_I: 
          apII_send_key(0xC9); break; // I

        case PS2_KEY_J: 
          apII_send_key(0xCA); break; // J

        case PS2_KEY_K: 
          apII_send_key(0xCB); break; // K

        case PS2_KEY_L: 
          apII_send_key(0xCC); break; // L

        case PS2_KEY_M: 
          apII_send_key(0xCD); break; // M

        case PS2_KEY_N: 
          apII_send_key(0xCE); break; // N

        case PS2_KEY_O: 
          apII_send_key(0xCF); break; // O

        case PS2_KEY_P: 
          apII_send_key(0xD0); break; // P

        case PS2_KEY_Q: 
          apII_send_key(0xD1); break; // Q

        case PS2_KEY_R: 
          apII_send_key(0xD2); break; // R

        case PS2_KEY_S: 
          apII_send_key(0xD3); break; // S

        case PS2_KEY_T: 
          apII_send_key(0xD4); break; // T

        case PS2_KEY_U: 
          apII_send_key(0xD5); break; // U

        case PS2_KEY_V: 
          apII_send_key(0xD6); break; // V

        case PS2_KEY_W: 
          apII_send_key(0xD7); break; // W

        case PS2_KEY_X: 
          apII_send_key(0xD8); break; // X

        case PS2_KEY_Y: 
          apII_send_key(0xD9); break; // Y

        case PS2_KEY_Z: 
          apII_send_key(0xDA); break; // Z     

        case PS2_KEY_SINGLE: 
          apII_send_key(0xA7); break; // '      
          
        case PS2_KEY_MINUS:
          apII_send_key(0xAD); break; // -
          
        case PS2_KEY_EQUAL:
          apII_send_key(0xBD); break; // =      
          
        case PS2_KEY_COMMA:
          apII_send_key(0xAC); break; // ,
          
        case PS2_KEY_DOT:
          apII_send_key(0xAE); break; // .
          
        case PS2_KEY_DIV:
          apII_send_key(0xBB); break; // ;

        case PS2_KEY_KP_DIV:
          apII_send_key(0xAF); break; // /
      }
    }
  }
  
}

//-----------------------------------------------------------------------------

void apII_send_key(unsigned char data)
{
    
  //-------------------------
  // Keyboard Data

  digitalWrite(APII_B0, bitRead(data, 0));
  digitalWrite(APII_B1, bitRead(data, 1));
  digitalWrite(APII_B2, bitRead(data, 2));
  digitalWrite(APII_B3, bitRead(data, 3));
  digitalWrite(APII_B4, bitRead(data, 4));
  digitalWrite(APII_B5, bitRead(data, 5));
  digitalWrite(APII_B6, bitRead(data, 6));
       
  //-------------------------
  // Keyboard Strobe
  
  digitalWrite(APII_STROBE, HIGH);
  delayMicroseconds(APII_TIME_STROBE);
  digitalWrite(APII_STROBE, LOW);

  //-------------------------
  // Keyboard Key Interval

  delayMicroseconds(APII_TIME_INTERVAL);
  
}

//-----------------------------------------------------------------------------

void apII_send_reset()
{
  digitalWrite(APII_RESET, LOW);
  delay(APII_TIME_RESET);
  digitalWrite(APII_RESET, HIGH);  
}

//-----------------------------------------------------------------------------
