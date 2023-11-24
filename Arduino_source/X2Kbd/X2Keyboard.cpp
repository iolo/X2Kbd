//
// X2 Keyboard for PC
// by cloudree@naver.com
//

#include <Arduino.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"

static CX2Keyboard    CX2Keyboard::m_instance;

void CX2Keyboard::init() 
{
    pinMode( m_pinCAPS, OUTPUT );
    pinMode( m_pinCODE, OUTPUT );
    pinMode( m_pinCLK, OUTPUT );

    digitalWrite( m_pinCAPS, HIGH );
    digitalWrite( m_pinCODE, HIGH );
    digitalWrite( m_pinCLK, LOW );
}

void CX2Keyboard::process() 
{
// uncomment the next line to use emulation mode!
// #define NO_X2_KEYBOARD 1
#if NO_X2_KEYBOARD
//====================================================================
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case '0': // release all
        DBGLN("* release all");
        memset(m_readVal, 0, sizeof(m_readVal));
        break;
      case '1': 
        DBGLN("* press Shift+F1 -> F6: 0x0b");
        m_readVal[6] |= 0x80 | 0x04;
        break;
      case '2':
        DBGLN("* press @ -> Shift+2: 0x12 0x1e");
        m_readVal[1] |= 0x04;
        // FIXME: missing *unfake* press shift
        break;
      case '3': 
        DBGLN("* press Caps -> Caps: 0x58");
        m_readVal[6] |= 0x10;
        break;
      case '4': 
        DBGLN("* press Graph -> LeftAlt: 0x11");
        m_readVal[6] |= 0x20;
        break;
      case '5': 
        DBGLN("* press Ctrl -> LeftCtrl: 0x14");
        m_readVal[6] |= 0x40;
        break;
      case '6': 
        DBGLN("* press Shift+6(&) -> Shift+7: 0x12 0x3d");
        m_readVal[6] |= 0x80;
        m_readVal[0] |= 0x02;
        break;
      case '7': 
        DBGLN("* press Shift+7(') -> QUOTE('): 0x52");
        m_readVal[6] |= 0x80;
        m_readVal[0] |= 0x01;
        // FIXME: missing unfake press shift
        break;
      case '?': 
        for(int row = 0; row < sizeX; row++) {
          DBG("* ");
          for (int bit = 0; bit < sizeY; bit++) {
            DBG(m_readVal[row] & (1 << bit) ? "1 " : "0 ");
          }
          DBG(": ");DBGLN(row);
        }
        break;
    }
  }
//====================================================================
#else
    for( int XS = 0; XS < sizeX; XS ++ )
    {
        digitalWrite( m_pinCLK, LOW );
        
        for( int i = 0; i < sizeS; i ++ )
        {
            pinMode( m_pinS[i], OUTPUT );
        }
         
        digitalWrite( m_pinS[0], ((XS & 1) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[1], ((XS & 2) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[2], ((XS & 4) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[3], ((XS & 8) > 0) ? HIGH : LOW );
        
        delay(1);
        digitalWrite( m_pinCLK, HIGH );   //trigger to get result
        
        m_readVal[ XS ] = 0;
        for( int i = 0; i < sizeY; i ++ )
        {
            pinMode( m_pinY[i], INPUT_PULLUP );
            m_readVal[ XS ] |= ( digitalRead( m_pinY[i] ) == HIGH ? 0 : 1 );
            if( i != 7 )
                m_readVal[ XS ] <<= 1;      // 0 MSB ... 7 LSB
        }
        DBG(">x2:");
        Dump( sizeX, m_readVal );
    }
#endif
}

void CX2Keyboard::SetLed_CAPS(bool isOn)
{
    digitalWrite( m_pinCAPS, isOn ? LOW : HIGH );
}
void CX2Keyboard::SetLed_CODE(bool isOn)
{
    digitalWrite( m_pinCODE, isOn ? LOW : HIGH );
}
