#ifndef __DEMOS_H__
#define __DEMOS_H__
//============================================================================
//
// Some demo routines for Crystalfontz CFA10109 EVE accelerated displays.
//
// 2020-08-05 Brent A. Crosby / Crystalfontz America, Inc.
// https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//============================================================================

void Initialize_BACK(void);
uint16_t Add_BACK_To_Display_List(uint16_t FWol);

void Initialize_Number(void);
uint16_t Add_Number_To_Display_List(uint16_t FWol);

#if (0!=rainbow_demo)
uint16_t Initialize_Rainbow(uint16_t FWol);
uint16_t Add_Rainbow_To_Display_List(uint16_t FWol);
#endif

#if (0!=logo_demo)
uint16_t Initialize_LOGO(uint16_t FWol, uint32_t *RAM_G_Unused_Start);
uint16_t Add_LOGO_To_Display_List(uint16_t FWol);
#endif

#if (0!=logo_demo1)
uint16_t Initialize_LOGO1(uint16_t FWol, uint32_t *RAM_G_Unused_Start);
uint16_t Add_LOGO1_To_Display_List(uint16_t FWol);
#endif

//============================================================================
#endif // __DEMOS_H__
