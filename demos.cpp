#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>

// Definitions for our display.
#include "CFA10109_defines.h"
#include "CFA240320E0_024Sx.h"


#if BUILD_SD
#include <SD.h>
#endif

//the logo
#include "logo_ARGB2.h"
#include "logo1.h"

#include "EVE_defines.h"
#include "EVE_base.h"
#include "EVE_draw.h"
//Demonstrations of various EVE functions
#include "demos.h"

uint8_t r;
uint8_t g;
uint8_t b;
uint8_t transparency;
int8_t fontsize;

uint8_t state = 0;
const long interval = 2000;
unsigned long previousMillis = 0;

unsigned long currentTime;

const long interval1 = 200;
unsigned long previousMillis1 = 0;

unsigned long currentTime1;
unsigned long sec = 0;

int8_t fontsize1;

//===========================================================================
// #if (0!=rainbow_demo)

// uint8_t red;
// uint8_t green;
// uint8_t blue;
// uint32_t color;

// uint16_t HAUTEUR_ARC;
// double H;
// double S;
// uint32_t h;


// uint16_t Initialize_Rainbow(uint16_t FWol){
// red = 255;
// green = 0;
// blue = 0;
// color = red << 16;

// h=0;
// H=0;

// HAUTEUR_ARC = 40;

// return(FWol);
// }

// uint16_t Add_Rainbow_To_Display_List(uint16_t FWol){

//   for (uint16_t q=0;q<HAUTEUR_ARC;q++) {

//     S = 1 - (double)(q) / (double)(HAUTEUR_ARC);

//     for (uint16_t j=0;j<319;j++) {

//       double m = 255 * (1 - S);

//       H += 360. / 319.; //l'axe de rotation et la taille de l'ecran

//       if (H > 360) H = 0; // on s'arret à 360°

//         h = (unsigned int)(H);
//         //h = (uint32_t)(H);

//         double y = (255 - m) * (1 - fabs(fmod((double)h / 60., 2) - 1));

//         // byte z31 = (byte)(map(y, 0, 255, 0, 31));
//         // byte z63 = (byte)(map(y, 0, 255, 0, 63));
//         // byte m31 = (byte)(map(m, 0, 255, 0, 31));
//         // byte m63 = (byte)(map(m, 0, 255, 0, 63));


//         if (0 <= h && h < 60) {
//           red = 255;
//           green = y + m;
//           blue = m;
//           }
//         if (60 <= h && h < 120) {
//           red = y + m;
//           green = 255;
//           blue = m;

//           }
//         if (120 <= h && h < 180) {
//           red = m;
//           green = 255;
//           blue = y + m;
//         }
//         if (180 <= h && h < 240) {
//           red = m;
//           green = y + m;
//           blue = 255;
//         }
//         if (240 <= h && h < 300) {
//           red = y + m;
//           green = m;
//           blue = 255;
//         }
//         if (300 <= h && h < 360) {
//           red = 255;
//           green = m;
//           blue = y + m;
//         }
//         color = red << 16 | green << 8 | blue << 0;

//       FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR(color));
//       FWol=EVE_Filled_Rectangle(FWol,j,200,319,240);

//         }


//     }
//   return (FWol);
// }
// #endif

void Initialize_BACK(void) {
  r = 0xFF;
  g = 0;
  b = 0;
  transparency = 0xFF;
  fontsize = 30;
  currentTime = millis();
}

uint16_t Add_BACK_To_Display_List(uint16_t FWol) {

  if (currentTime - previousMillis >= interval) {
    previousMillis = currentTime;
    if (state == 0) {
      DBG_STAT("\nON");
      FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_RGB(r, g, b));
      FWol = EVE_PrintF(FWol, LCD_WIDTH / 2, 0, fontsize, EVE_OPT_FLAT, "ULTRO");
      // FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_RGB(r, g, b));
      // FWol = EVE_PrintF(FWol,LCD_WIDTH/2,0,fontsize,EVE_OPT_FLAT,"ULTRO");
      state = 1;
    }    
    else state = 0;
  }
  return FWol;
}


// void Initialize_Number(void) {

//   fontsize1 = 22;

// }

// uint16_t Add_Number_To_Display_List(uint16_t FWol) {

//   FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_RGB(0xFF,0xFF,0xFF));

//   currentTime1 = millis();

//   if (currentTime1 - previousMillis1 >= interval1) {
//       previousMillis1 = currentTime1;

//       DBG_STAT("\nON_Number");
//       sec+=1;
//       FWol = EVE_PrintF(FWol,0,0,fontsize1,EVE_OPT_FLAT,"%u",sec);
//       if (sec>=100){
//         sec = 0;
//       }
//   }

// }

#if (0 != logo_demo)

uint32_t Logo_Width;
uint32_t Logo_Height;

uint32_t Logo_RAM_G_Address;

#if (0 != DEBUG_COPROCESSOR_RESET)
uint8_t
  first = 0;
#endif  // (0!=DEBUG_COPROCESSOR_RESET)

uint16_t Initialize_LOGO(uint16_t FWol, uint32_t *RAM_G_Unused_Start) {
  Logo_RAM_G_Address = *RAM_G_Unused_Start;

#if (0 == logo_png_argb)

  FWol = EVE_Load_PNG_to_RAM_G(FWol, nexter1, logo_size_PNG, RAM_G_Unused_Start, &Logo_Width, &Logo_Height);

#endif

#if (1 == logo_png_argb)

  Logo_Height = logo_height;
  Logo_Width = logo_width;

#if (0 != DEBUG_COPROCESSOR_RESET)
  if (0 == first) {
    //good
    FWol = EVE_Inflate_to_RAM_G(FWol,
                                nexter,
                                logo_size_ARGB2,
                                RAM_G_Unused_Start);
    DBG_GEEK("Good inflate\n");
    first = 1;
  } else {
    //bad
    FWol = EVE_Inflate_to_RAM_G(FWol,
                                nexter + 20,
                                logo_size_ARGB2,
                                RAM_G_Unused_Start);
    DBG_GEEK("Bad inflate\n");
    first = 0;
  }
#else  //(0!=DEBUG_COPROCESSOR_RESET)

  FWol = EVE_Inflate_to_RAM_G(FWol,
                              nexter,
                              logo_size_ARGB2,
                              RAM_G_Unused_Start);

#endif  //(0!=DEBUG_COPROCESSOR_RESET)

#endif  // 1==LOGO_PNG_0_ARGB2_1
  //Pass our updated offset back to the caller

  return (FWol);
}

uint16_t Add_LOGO_To_Display_List(uint16_t FWol) {
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_RGB(0xFF, 0xFF, 0xFF));
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_A(0xFF));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_BITMAPS));
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_SOURCE(Logo_RAM_G_Address));

#if 0 == logo_png_argb

  FWol=EVE_Cmd_Dat_0(FWol,EVE_ENC_BITMAP_LAYOUT(EVE_FORMAT_ARGB4,Logo_Width,Logo_Height));

#endif

#if 1 == logo_png_argb

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_LAYOUT(EVE_FORMAT_ARGB2, Logo_Width, Logo_Height));

#endif
  //320*320
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_SIZE(EVE_FILTER_BILINEAR, EVE_WRAP_BORDER, EVE_WRAP_BORDER, Logo_Width, Logo_Height));

  //FWol=EVE_Cmd_Dat_2(FWol,EVE_ENC_CMD_TRANSLATE,to_16_16_fp(Logo_Width,0)/2,to_16_16_fp(Logo_Height,0)/2);

  //Undo the translation, and move to the center of the screen.
  //FWol=EVE_Cmd_Dat_2(FWol,EVE_ENC_CMD_TRANSLATE,(to_16_16_fp(-Logo_Width/2+0/2,0)),(to_16_16_fp(-Logo_Height/2+0/2,0)));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_CMD_SETMATRIX);
  //Render the bitmap to the current frame

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F((LCD_HEIGHT - 115) * (16 / 2), (LCD_WIDTH - Logo_Height) * (16 / 2)));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_END());

  return (FWol);
}
#endif

#if (0 != logo_demo1)

uint32_t Logo_Width1;
uint32_t Logo_Height1;

uint32_t Logo_RAM_G_Address1;

#if (0 != DEBUG_COPROCESSOR_RESET)
uint8_t
  first1 = 0;
#endif  // (0!=DEBUG_COPROCESSOR_RESET)

uint16_t Initialize_LOGO1(uint16_t FWol, uint32_t *RAM_G_Unused_Start) {
  Logo_RAM_G_Address1 = *RAM_G_Unused_Start;

#if (0 == logo1)

  FWol = EVE_Load_PNG_to_RAM_G1(FWol, wifi, logo1_size, RAM_G_Unused_Start, &Logo_Width1, &Logo_Height1);

#endif

#if (1 == logo1)

  Logo_Height1 = logo1_height;
  Logo_Width1 = logo1_width;

#if (0 != DEBUG_COPROCESSOR_RESET)
  if (0 == first1) {
    //good
    FWol = EVE_Inflate_to_RAM_G1(FWol,
                                 wifi,
                                 logo1_size,
                                 RAM_G_Unused_Start);
    DBG_GEEK("Good inflate\n");
    first1 = 1;
  } else {
    //bad
    FWol = EVE_Inflate_to_RAM_G1(FWol,
                                 wifi + 20,
                                 logo1_size,
                                 RAM_G_Unused_Start);
    DBG_GEEK("Bad inflate\n");
    first1 = 0;
  }
#else  //(0!=DEBUG_COPROCESSOR_RESET)

  FWol = EVE_Inflate_to_RAM_G1(FWol,
                               wifi,
                               logo1_size,
                               RAM_G_Unused_Start);

#endif  //(0!=DEBUG_COPROCESSOR_RESET)

#endif  // 1==LOGO_PNG_0_ARGB2_1
  //Pass our updated offset back to the caller

  return (FWol);
}

uint16_t Add_LOGO1_To_Display_List(uint16_t FWol) {
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_RGB(0xFF, 0xFF, 0xFF));
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_A(0xFF));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_BITMAPS));
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_SOURCE(Logo_RAM_G_Address1));

#if 0 == logo1

  FWol=EVE_Cmd_Dat_0(FWol,EVE_ENC_BITMAP_LAYOUT(EVE_FORMAT_ARGB4,Logo_Width1,Logo_Height1));

#endif

#if 1 == logo1

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_LAYOUT(EVE_FORMAT_ARGB2, Logo_Width1, Logo_Height1));

#endif
  //320*320
  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_BITMAP_SIZE(EVE_FILTER_BILINEAR, EVE_WRAP_BORDER, EVE_WRAP_BORDER, Logo_Width1, Logo_Height1));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_CMD_SETMATRIX);
  //Render the bitmap to the current frame

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(575 * (16 / 2), 0 * (16 / 2)));

  FWol = EVE_Cmd_Dat_0(FWol, EVE_ENC_END());

  return (FWol);
}
#endif