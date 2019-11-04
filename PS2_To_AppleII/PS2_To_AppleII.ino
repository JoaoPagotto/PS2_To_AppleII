//-----------------------------------------------------------------------------
//  
//  Keyboard PS/2 to Apple II v1.00
//  
//  Date: 2019-11-03 23:00am
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

#define APII_KEYS_ROWS      64
#define APII_KEYS_COLS       8

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

void setup() 
{
  pinMode(APII_STROBE, OUTPUT);
  digitalWrite(APII_STROBE, LOW);
  
  pinMode(APII_RESET, OUTPUT);
  digitalWrite(APII_RESET, HIGH);

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
  
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  //-------------------------

  pinMode(LED_BUILTIN, OUTPUT);

  //-------------------------

  keyboard.begin(PS2_DATA, PS2_IRQ); // Setup keyboard pins
  keyboard.setNoBreak(1);            // No break codes for keys (when key released)
  keyboard.setNoRepeat(1);           // Don't repeat shift ctrl etc
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

  if ( data & PS2_BREAK ) Serial.print("BREAK, ");
  if ( data & PS2_SHIFT ) Serial.print("SHIFT, ");
  if ( data & PS2_CTRL ) Serial.print("CTRL, ");
  if ( data & PS2_CAPS ) Serial.print("CAPS, ");
  if ( data & PS2_ALT ) Serial.print("ALT, ");
  if ( data & PS2_ALT_GR ) Serial.print("ALTGR, ");
  if ( data & PS2_GUI ) Serial.print("GUI, ");
  if ( data & PS2_FUNCTION ) Serial.print("FUNCTION, ");
      
  Serial.print(chr, DEC);      
  Serial.print("\n\r");

  //---------------------------

  if ( data & PS2_FUNCTION ) 
  {
    if ( data & PS2_CTRL ) 
    {
      switch ( chr ) 
      {
        case PS2_KEY_BREAK: { apII_send_reset(); break; }
      }
      
      return;
    }
    
    switch ( chr ) 
    {    
      case PS2_KEY_ENTER:
      case PS2_KEY_KP_ENTER:
        apII_send_key(0x8D); break;

      case PS2_KEY_ESC:
        apII_send_key(0x9B); break;

      case PS2_KEY_SPACE:
        apII_send_key(0xA0); break;

      case PS2_KEY_BS:
      case PS2_KEY_L_ARROW:
        apII_send_key(0x88); break;
      
      case PS2_KEY_R_ARROW:
        apII_send_key(0x95); break;
    }
    
    return;
  }
  else
  {
    if ( data & PS2_CTRL ) 
    {
      switch ( chr ) 
      {
        case 'C': { apII_send_key(0x83); break; }
      }
           
      return;
    }

    if ( data & PS2_SHIFT ) 
    {
      switch ( chr ) 
      {
        case 64: { apII_send_key(0xA2); break; } // "     
        case 49: { apII_send_key(0xA1); break; } // !
        case 50: { apII_send_key(0xC0); break; } // @      
        case 51: { apII_send_key(0xA3); break; } // #
        case 52: { apII_send_key(0xA4); break; } // $
        case 53: { apII_send_key(0xA5); break; } // %
        case 55: { apII_send_key(0xA6); break; } // &
        case 56: { apII_send_key(0xAA); break; } // *
        case 57: { apII_send_key(0xA8); break; } // (
        case 48: { apII_send_key(0xA9); break; } // )
        // case 60: { break; }  // _
        case 95: { apII_send_key(0xBB); break; } // +
        // case 93: { break; }  // `
        // case 94: { break; }  // {
        case 58: { apII_send_key(0xCE); break; } // ^
        // case 92: { break; }  // }
        case 59: { apII_send_key(0xBC); break; } // <
        case 61: { apII_send_key(0xBE); break; } // >
        case 62: { apII_send_key(0xBA); break; } // :
        // case 145: { break; } // ?
      }
           
      return;
    }
    
    if ( data < 256 )
    {
      switch ( chr ) 
      {
        case PS2_KEY_KP0:
        case '0': { apII_send_key(0xB0); break; }
        case PS2_KEY_KP1:
        case '1': { apII_send_key(0xB1); break; }
        case PS2_KEY_KP2:
        case '2': { apII_send_key(0xB2); break; }
        case PS2_KEY_KP3:
        case '3': { apII_send_key(0xB3); break; }
        case PS2_KEY_KP4:
        case '4': { apII_send_key(0xB4); break; }
        case PS2_KEY_KP5:
        case '5': { apII_send_key(0xB5); break; }
        case PS2_KEY_KP6:
        case '6': { apII_send_key(0xB6); break; }
        case PS2_KEY_KP7:
        case '7': { apII_send_key(0xB7); break; }
        case PS2_KEY_KP8:
        case '8': { apII_send_key(0xB8); break; }
        case PS2_KEY_KP9:
        case '9': { apII_send_key(0xB9); break; }

        case PS2_KEY_KP_DIV    : { break; }
        case PS2_KEY_KP_TIMES  : { apII_send_key(0xAA); break; }
        case PS2_KEY_KP_MINUS  : { apII_send_key(0xAD); break; }
        case PS2_KEY_KP_PLUS   : { apII_send_key(0xBB); break; }
        case PS2_KEY_KP_DOT    : { apII_send_key(0xAC); break; }
        case PS2_KEY_KP_ENTER  : { apII_send_key(0x8D); break; }
        case PS2_KEY_KP_COMMA  : { apII_send_key(0xAE); break; }
      
        case 'A': { apII_send_key(0xC1); break; }
        case 'B': { apII_send_key(0xC2); break; }
        case 'C': { apII_send_key(0xC3); break; }
        case 'D': { apII_send_key(0xC4); break; }
        case 'E': { apII_send_key(0xC5); break; }
        case 'F': { apII_send_key(0xC6); break; }
        case 'G': { apII_send_key(0xC7); break; }
        case 'H': { apII_send_key(0xC8); break; }
        case 'I': { apII_send_key(0xC9); break; }
        case 'J': { apII_send_key(0xCA); break; }
        case 'K': { apII_send_key(0xCB); break; }
        case 'L': { apII_send_key(0xCC); break; }
        case 'M': { apII_send_key(0xCD); break; }
        case 'N': { apII_send_key(0xCE); break; }
        case 'O': { apII_send_key(0xCF); break; }
        case 'P': { apII_send_key(0xD0); break; }
        case 'Q': { apII_send_key(0xD1); break; }
        case 'R': { apII_send_key(0xD2); break; }
        case 'S': { apII_send_key(0xD3); break; }
        case 'T': { apII_send_key(0xD4); break; }
        case 'U': { apII_send_key(0xD5); break; }
        case 'V': { apII_send_key(0xD6); break; }
        case 'W': { apII_send_key(0xD7); break; }
        case 'X': { apII_send_key(0xD8); break; }
        case 'Y': { apII_send_key(0xD9); break; }
        case 'Z': { apII_send_key(0xDA); break; }      

        case 64: { apII_send_key(0xA7); break; } // '      
        case 60: { apII_send_key(0xAD); break; } // -
        case 95: { apII_send_key(0xBD); break; } // =      
        // case 93: { break; } // ´
        // case 94: { break; } // [
        // case 58: { break; } // ~
        // case 92: { break; } // ]
        case 59: { apII_send_key(0xAC); break; }      // ,
        case 61: { apII_send_key(0xAE); break; }      // .
        case 62: { apII_send_key(0xBB); break; }      // ;
        // case 145: { break; } // /
      }
    }
  }
  
}

//-----------------------------------------------------------------------------
