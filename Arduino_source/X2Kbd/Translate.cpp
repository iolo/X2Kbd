//
// PC Keyboard
// by cloudree@naver.com
//

#include <Arduino.h>
//#include <Keyboard.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"
//#include "PCKeyboard.h"
#include "ps2scancode.h"
#include "ps2dev.h"
#include "Translate.h"

static CTranslate    CTranslate::m_instance;

enum Translate_Code {
    TRANS_NORMAL  = 0,
    TRANS_SHIFT   = 1,
    TRANS_MAX,
};

#define KEY_PRTSCR    (0x46+136)
#define KEY_SCRLK     (0x47+136)
#define KEY_PAUSE     (0x48+136)

#define MOD_SHIFT_VAL   0x4000
#define MS              MOD_SHIFT_VAL + 

// some PS2 scancode has special key prefix(0xe0) as hi-byte (b15..b8 = 0xe0)
// custom translation flag is overlapped on lo-nibble of the hi-byte of scan code (b11..b8)
//
//  1 1 1 0 0 0 0 0    0 0 0 0 0 0 0 0
// +-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+ 
// ps2 special key prefix   ps2 scan code
//         +-+-+-+-+
//     custom translation flag(overlapped!)
//          |  |  |  |  |
//          +--+--+  | no shift
//             |   shift
//          reserved
//  
#define _CUSTOM_TRANSLATION_MASK 0x0f00
#define _NOSHIFT_MASK 0x0100
#define _SHIFT_MASK 0x0200
#define _NOSHIFT(x) ((x)|_NOSHIFT_MASK)
#define _SHIFT(x) ((x)|_SHIFT_MASK)
// mapping table from msx keyboard matrix to ps/2 scan code.
// https://www.msx.org/wiki/Keyboard_Matrices#Korean_matrix_for_Daewoo_CPC-300.2C_CPC-300E.2C_CPC-400_and_CPC-400S
// NOTE: see with horizontal flip!
// ps/2 keyboard send same scan code whether caps lock is on or not.
// bypass press and release if possible!
// TODO: MSX combination keys into PS2 multiple scan codes
// ...
static int msx_keyboard_matrix[TRANS_MAX][sizeX][sizeY] = {
  // Translate MSX <X> -> PS2 <Y> or Shift+<Y>
  // ex. MSX A..Z,0..9,F1..F5 -> PS2 A-Z,0..9,F1..F5
  // ex. MSX ^ -> PS2 Shift+6
  {
    // 0               1!                       2"                3#              4$              5%              6&                      7'
    { PS2_0,           PS2_1,                   PS2_2,            PS2_3,          PS2_4,          PS2_5,          PS2_6,                  PS2_7           },
    // 8(              9)                       -=                ^~              \|              @`              [{                      ;+
    { PS2_8,           PS2_9,                   PS2_MINUS,        _SHIFT(PS2_6),  PS2_BACKSLASH,  _SHIFT(PS2_2),  PS2_LEFT_ANGLE_BRACKET, PS2_SEMICOLON   },
    // :*              ]}                       ,<                .<              /?              N/A             aA                      bB
    { _SHIFT(PS2_SEMICOLON), PS2_RIGHT_ANGLE_BRACKET, PS2_COMMA,  PS2_PERIOD,     PS2_SLASH,      0,              PS2_A,                  PS2_B           },
    // cC              dD                       eE                fF              gG              hH              iI                      jJ
    { PS2_C,           PS2_D,                   PS2_E,            PS2_F,          PS2_G,          PS2_H,          PS2_I,                  PS2_J           },
    // kK              lL                       mM                nN              oO              pP              qQ                      rR
    { PS2_K,           PS2_L,                   PS2_M,            PS2_N,          PS2_O,          PS2_P,          PS2_Q,                  PS2_R           },
    // sS              tT                       uU                vV              wW              xX              yY                      zZ
    { PS2_S,           PS2_T,                   PS2_U,            PS2_V,          PS2_W,          PS2_X,          PS2_Y,                  PS2_Z           },
    // Shift           Ctrl                     Graph             Caps            Korean          F1              F2                      F3
    { PS2_LEFT_SHIFT,  PS2_LEFT_CTRL,           PS2_LEFT_ALT,     PS2_CAPS_LOCK,  PS2_RIGHT_ALT,  PS2_F1,         PS2_F2,                 PS2_F3          },
    // F4              F5                       Esc               Tab             Stop            Backspace       Select                  Return
    { PS2_F4,          PS2_F5,                  PS2_ESC,          PS2_TAB,        PS2_F11,        PS2_BACKSPACE,  PS2_F12,                PS2_ENTER       },
    // Space           Home                     Ins               Sup             Left            Up              Down                    Right
    { PS2_SPACE,       PS2_HOME,                PS2_INSERT,       PS2_DELETE,     PS2_LEFT_ARROW, PS2_UP_ARROW,   PS2_DOWN_ARROW,         PS2_RIGHT_ARROW },
    // *               +                        /                 0               1               2               3                       4
    { PS2_KP_ASTERISK, PS2_KP_PLUS,             PS2_KP_SLASH,     PS2_KP_0,       PS2_KP_1,       PS2_KP_2,       PS2_KP_3,               PS2_KP_4        },
    // 5               6                        7                 8               9               -               ,                       .
    { PS2_KP_5,        PS2_KP_6,                PS2_KP_7,         PS2_KP_8,       PS2_KP_9,       PS2_KP_MINUS,   PS2_COMMA,              PS2_PERIOD      }
  },
  // Translate MSX Shift+X -> PS2 Y or Shift+Y
  // ex. MSX Shift+F1..F5 -> PS2 F6..F10  (TODO: how to PS2 Shift+F6..F10?)
  // ex. MAX Shift+7(') -> PS2 '
  // ex. MSX Shift+6(&) -> PS2 Shift+7
  {
    // 0               1!                       2"                3#              4$              5%              6&                      7'
    { PS2_0,           PS2_1,                   _SHIFT(PS2_QUOTE),PS2_3,          PS2_4,          PS2_5,          _SHIFT(PS2_7),          _NOSHIFT(PS2_QUOTE)},
    // 8(              9)                       -=                ^~              \|              @`              [{                      ;+
    { _SHIFT(PS2_9), _SHIFT(PS2_0), _NOSHIFT(PS2_EQUAL), _SHIFT(PS2_BACKTICK), PS2_BACKSLASH, _NOSHIFT(PS2_BACKTICK), PS2_LEFT_ANGLE_BRACKET, PS2_EQUAL       },
    // :*              ]}                       ,<                .<              /?              N/A             aA                      bB
    { _SHIFT(PS2_8),   PS2_RIGHT_ANGLE_BRACKET, PS2_COMMA,        PS2_PERIOD,     PS2_SLASH,      0,              PS2_A,                  PS2_B           },
    // cC              dD                       eE                fF              gG              hH              iI                      jJ
    { PS2_C,           PS2_D,                   PS2_E,            PS2_F,          PS2_G,          PS2_H,          PS2_I,                  PS2_J           },
    // kK              lL                       mM                nN              oO              pP              qQ                      rR
    { PS2_K,           PS2_L,                   PS2_M,            PS2_N,          PS2_O,          PS2_P,          PS2_Q,                  PS2_R           },
    // sS              tT                       uU                vV              wW              xX              yY                      zZ
    { PS2_S,           PS2_T,                   PS2_U,            PS2_V,          PS2_W,          PS2_X,          PS2_Y,                  PS2_Z           },
    // Shift           Ctrl                     Graph             Caps            Korean          F1              F2                      F3
    { PS2_LEFT_SHIFT,  PS2_LEFT_CTRL,           PS2_LEFT_ALT,     PS2_CAPS_LOCK,  PS2_RIGHT_ALT,  _NOSHIFT(PS2_F6), _NOSHIFT(PS2_F7),     _NOSHIFT(PS2_F8)},
    // F4              F5                       Esc               Tab             Stop            Backspace       Select                  Return
    { _NOSHIFT(PS2_F9), _NOSHIFT(PS2_F10),      PS2_ESC,          PS2_TAB,        PS2_F11,        PS2_BACKSPACE,  PS2_F12,                PS2_ENTER       },
    // Space           Home                     Ins               Sup             Left            Up              Down                    Right
    { PS2_SPACE,       PS2_HOME,                PS2_INSERT,       PS2_DELETE,     PS2_LEFT_ARROW, PS2_UP_ARROW,   PS2_DOWN_ARROW,         PS2_RIGHT_ARROW },
    // *               +                        /                 0               1               2               3                       4
    { PS2_KP_ASTERISK, PS2_KP_PLUS,             PS2_KP_SLASH,     PS2_KP_0,       PS2_KP_1,       PS2_KP_2,       PS2_KP_3,               PS2_KP_4        },
    // 5               6                        7                 8               9               -               ,                       .
    { PS2_KP_5,        PS2_KP_6,                PS2_KP_7,         PS2_KP_8,       PS2_KP_9,       PS2_KP_MINUS,   PS2_COMMA,              PS2_PERIOD      }
  }
};

int shiftX = 6;
int shiftY_ = 0x80;

int capsX = 6;
int capsY = 4;

int hangulX = 6;
int hangulY = 3;

static unsigned char lastVal[ sizeX ]; 

void CTranslate::init()
{
    for( int i = 0; i < sizeX; i ++ )
        lastVal [i] = 0;
        
    m_isCAPS = false;
    m_isHANGUL = false;
    m_oldShifted = false;
    
    CX2Keyboard::GetInstance()->SetLed_CAPS( m_isCAPS );
    CX2Keyboard::GetInstance()->SetLed_CODE( m_isHANGUL );
}

void CTranslate::CheckToggleCAPS( int x, int y )
{
    if( x == capsX && y == capsY )
    {
        m_isCAPS = ! m_isCAPS;
        CX2Keyboard::GetInstance()->SetLed_CAPS( m_isCAPS );
    }
}

void CTranslate::CheckToggleHANGUL( int x, int y )
{
    if( x == hangulX && y == hangulY )
    {
        m_isHANGUL = ! m_isHANGUL;
        CX2Keyboard::GetInstance()->SetLed_CODE( m_isHANGUL );
    }
}

bool CTranslate::IsPressed( int x, int y_ )
{
    BYTE val = CX2Keyboard::GetInstance()->Get( x );
    return ( (val & y_) != 0 );
}

void CTranslate::process()
{
    int shifted = IsPressed( shiftX, shiftY_ ) ? TRANS_SHIFT : TRANS_NORMAL;
    if( shifted != m_oldShifted )
    {
        if (shifted) {
          DBG(">ps2: press shift:");DBGHEXLN(PS2_LEFT_SHIFT);
          PS2dev::GetInstance()->keyboard_press(PS2_LEFT_SHIFT);
        } else {
          DBG(">ps2: release shift:");DBGHEXLN(PS2_LEFT_SHIFT);
          PS2dev::GetInstance()->keyboard_release(PS2_LEFT_SHIFT);
        }
        m_oldShifted = shifted;
        return;
    }
    for( int x = 0; x < sizeX; x ++ )
    {
        int val = CX2Keyboard::GetInstance()->Get( x ); // 0..10
        if( lastVal [x] == val )
            continue;
            
        int b = 0x80;
        for( int y = 0; y < sizeY; y ++ ) // 0..7
        {
            int last = ( lastVal [x] & b ) > 0 ? 1 : 0;
            int curr = ( val & b ) > 0 ? 1 : 0;
            b >>= 1;
            if (last == curr) {
              continue;
            }

            int key = msx_keyboard_matrix[shifted][x][y];
            if (key == 0 || key == PS2_LEFT_SHIFT || key == PS2_RIGHT_SHIFT) {
              continue;
            }
            DBG("<x2: shift=");DBG(shifted);DBG("x=");DBG(x);DBG("y=");DBG(y);DBG(curr ? "press" : "release");DBG("-> key=");DBGHEXLN(key);

            int custom_translation_mask = key & _CUSTOM_TRANSLATION_MASK;
            if (custom_translation_mask) {
              // erase custom_translation_mask
              key &= ~_CUSTOM_TRANSLATION_MASK;
            }

            bool is_ps2_special_key = false;              
            if ((key & 0xff00) == 0xe000) {
              is_ps2_special_key = true;
              // erase special key prefix(0xe0)
              // and use low byte
              key &= 0xff;
            }

            if( last == 0 && curr == 1 )
            {
              if (custom_translation_mask) {
                if (shifted && custom_translation_mask == _NOSHIFT_MASK) {
                  DBGLN(">ps2: *fake* release shift");
                  PS2dev::GetInstance()->keyboard_release(PS2_LEFT_SHIFT);
                  m_oldShifted = false;
                }
                if (!shifted && custom_translation_mask == _SHIFT_MASK) {
                  DBGLN(">ps2: *fake* press shift");
                  PS2dev::GetInstance()->keyboard_press(PS2_LEFT_SHIFT);
                  m_oldShifted = true;
                }
                m_fakeKey = key;//msx_keyboard_matrix[TRANS_NORMAL][x][y];
              }

              CheckToggleCAPS( x, y );
              CheckToggleHANGUL( x, y );

              if (is_ps2_special_key) {
                DBG(">ps2: press special:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_press_special(key);
              } else {
                DBG(">ps2: press:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_press(key);
              }
            }
            else if( last == 1 && curr == 0 )
            {
              // send release the key was sent to ps2 instead of msx key
              if (m_fakeKey) {
                DBG("* release fake key instead of ");DBGHEXLN(key);
                key = m_fakeKey;
                m_fakeKey = 0;
              }
              if (is_ps2_special_key) {
                DBG(">ps2: release special:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_release_special(key);
              } else {
                DBG(">ps2: release:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_release(key);
              }
              // if (m_fakeShift != shifted) {
              //   if (shifted) {
              //     DBGLN(">ps2: *unfake* press shift");
              //     PS2dev::GetInstance()->keyboard_press(PS2_LEFT_SHIFT);
              //   } else {
              //     DBGLN(">ps2: *unfake* release shift");
              //     PS2dev::GetInstance()->keyboard_release(PS2_LEFT_SHIFT);
              //   }
              //   m_fakeShift = shifted;
              // }
            }
        }
        
        lastVal [x] = val;
    }
}
