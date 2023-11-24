# X2Kbd-ps2

Use the Daewoo MSX2 X-2 (CPC-400) Keyboard as PS/2 Keyboard using Arduino Pro Micro

> Original USB output version is https://github.com/cloudree/X2Kbd

# copyright
GNU GPL

# Usage :
  * See documents at here (Korean) : https://cafe.naver.com/msx/19349

1. prepare Daewoo MSX X-2 (CPC-400) Keyboard
  * disassemble it and remove cable (DIN 13 pin ~ Unknown 14 pin cable)

2. set up a arduino pro micro with source (from this github)

3. connect betwwen the arduino and CPC-300 Keyboard's 14 pin connector of inner PCB
  a. Arconnection map
    * Arduino = 14pin connector of PCB in keyboard
    * D2 = 1 (CAPS)
    * D3 = 2 (CODE/HAN)
    * D4 = 3 (CLK)
    * D5 = 4 (D2)
    * D6 = 5 (D1)
    * D7 = 6 (D0)
    * D8 = 7 (D3)
    * D9 = 8 (D4)
    * D10 = 9 (D5)
    * D16 = 10 (D6)
    * D14 = 11 (D7)
    * VCC = 12 (VCC)
    * GND = 13 (GND)
    * GND = 14 (GND)
    * A0 = PS/2 CLOCK
    * A1 = PS/2 DATA
    * GND = PS/2 GND

4. assemble keyboard and plug into PS/2 port on PC, and pray...

5. Key Mapping
    * Keep the printed keys on X2 keycap as possible as I can.
    * Some PS/2 only keys are not supported. (ex. DEL, END, PGUP, PGDN, ...)
    * Shift+F1..F5 keys = F6..F10. So Shift+F1..F5 keys are not supported.
    * Graph = LEFT_ALT
    * Korean = RIGHT_ALT
    * At this time, you could define X -> X, X -> Y, Shift+X -> X or X -> Shift+Y, ShiftX -> Shift+Y
    * see [Translate.cpp](Arduino_source/X2Kbd/Translate.cpp#L32)

---
May the **SOURECE** be with you...
