#ifndef __MSXSCANCODE_H__
#define __MSXSCANCODE_H__

// my own msx scan code based on MSX keyboard matrix
// https://www.msx.org/wiki/Keyboard_Matrices
// <scancode> ::= <row> <bit>
// <row> ::= b7..b4
// <bit> ::= b3..b0

#define MSX_ROW(code) (((code)&0xf0)>>4)
#define MSX_BIT(code) ((code)&0x0f)
#define MSX_CODE(row,bit) (((row)<<4)|(bit))

#define MSX_7   MSX_CODE(0,7)
#define MSX_6   MSX_CODE(0,6)
#define MSX_5   MSX_CODE(0,5)
#define MSX_4   MSX_CODE(0,4)
#define MSX_3   MSX_CODE(0,3)
#define MSX_2   MSX_CODE(0,2)
#define MSX_1   MSX_CODE(0,1)
#define MSX_0   MSX_CODE(0,0)

#define MSX_SEMICOLON           MSX_CODE(1,7)
#define MSX_LEFT_ANGLE_BRAKET   MSX_CODE(1,6)
#define MSX_AT                  MSX_CODE(1,5)
#define MSX_KRW                 MSX_CODE(1,4)
#define MSX_CARET               MSX_CODE(1,3)
#define MSX_MINUS               MSX_CODE(1,2)
#define MSX_9                   MSX_CODE(1,1)
#define MSX_8                   MSX_CODE(1,0)

#define MSX_B                   MSX_CODE(2,7)
#define MSX_A                   MSX_CODE(2,6)
#define MSX_UNDERSCORE          MSX_CODE(2,5)
#define MSX_SLASH               MSX_CODE(2,4)
#define MSX_PERIOD              MSX_CODE(2,3)
#define MSX_COMMA               MSX_CODE(2,2)
#define MSX_RIGHT_ANGLE_BRACKET MSX_CODE(2,1)
#define MSX_COLON               MSX_CODE(2,0)

#define MSX_J   MSX_CODE(3,7)
#define MSX_I   MSX_CODE(3,6)
#define MSX_H   MSX_CODE(3,5)
#define MSX_G   MSX_CODE(3,4)
#define MSX_F   MSX_CODE(3,3)
#define MSX_E   MSX_CODE(3,2)
#define MSX_D   MSX_CODE(3,1)
#define MSX_C   MSX_CODE(3,0)

#define MSX_R   MSX_CODE(4,7)
#define MSX_Q   MSX_CODE(4,6)
#define MSX_P   MSX_CODE(4,5)
#define MSX_O   MSX_CODE(4,4)
#define MSX_N   MSX_CODE(4,4)
#define MSX_M   MSX_CODE(4,3)
#define MSX_L   MSX_CODE(4,2)
#define MSX_K   MSX_CODE(4,0)

#define MSX_Z   MSX_CODE(5,7)
#define MSX_Y   MSX_CODE(5,6)
#define MSX_X   MSX_CODE(5,5)
#define MSX_W   MSX_CODE(5,4)
#define MSX_V   MSX_CODE(5,3)
#define MSX_U   MSX_CODE(5,2)
#define MSX_T   MSX_CODE(5,1)
#define MSX_S   MSX_CODE(5,0)

#define MSX_F3          MSX_CODE(6,7)
#define MSX_F2          MSX_CODE(6,6)
#define MSX_F1          MSX_CODE(6,5)
#define MSX_HANGUL      MSX_CODE(6,4)
#define MSX_CAPS        MSX_CODE(6,3)
#define MSX_GRAPH       MSX_CODE(6,2)
#define MSX_CTRL        MSX_CODE(6,1)
#define MSX_SHIFT       MSX_CODE(6,0)

#define MSX_RETURN      MSX_CODE(7,7)
#define MSX_SELECT      MSX_CODE(7,6)
#define MSX_BACKSPACE   MSX_CODE(7,5)
#define MSX_STOP        MSX_CODE(7,4)
#define MSX_TAB         MSX_CODE(7,3)
#define MSX_ESC         MSX_CODE(7,2)
#define MSX_F5          MSX_CODE(7,1)
#define MSX_F4          MSX_CODE(7,0)

#define MSX_RIGHT_ARROW MSX_CODE(8,7)
#define MSX_DOWN_ARROW  MSX_CODE(8,6)
#define MSX_UP_ARROW    MSX_CODE(8,5)
#define MSX_LEFT_ARROW  MSX_CODE(8,4)
#define MSX_SUP         MSX_CODE(8,3)
#define MSX_INS         MSX_CODE(8,2)
#define MSX_HOME        MSX_CODE(8,1)
#define MSX_SPACE       MSX_CODE(8,0)

#define MSX_KP_4        MSX_CODE(9,7)
#define MSX_KP_3        MSX_CODE(9,6)
#define MSX_KP_2        MSX_CODE(9,5)
#define MSX_KP_1        MSX_CODE(9,4)
#define MSX_KP_0        MSX_CODE(9,3)
#define MSX_KP_SLASH    MSX_CODE(9,2)
#define MSX_KP_PLUS     MSX_CODE(9,1)
#define MSX_KP_ASTERISK MSX_CODE(9,0)

#define MSX_KP_PERIOD   MSX_CODE(10,7)
#define MSX_KP_COMMA    MSX_CODE(10,6)
#define MSX_KP_MINUS    MSX_CODE(10,5)
#define MSX_KP_9        MSX_CODE(10,4)
#define MSX_KP_8        MSX_CODE(10,3)
#define MSX_KP_7        MSX_CODE(10,2)
#define MSX_KP_6        MSX_CODE(10,1)
#define MSX_KP_5        MSX_CODE(10,0)

#endif