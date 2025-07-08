#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>

// Definitions for our display.
#include "CFA10109_defines.h"
#include "CFA240320E0_024Sx.h"

#if BUILD_SD
#include <SD.h>
#endif

// The very simple EVE library files
#include "EVE_defines.h"
#include "EVE_base.h"
#include "EVE_draw.h"

// Our demonstrations of various EVE functions
#include "demos.h"
#include "logo_ARGB2.h"
#include "logo1.h"

//=========================================================================
void setup()
  {
#if (DEBUG_LEVEL != DEBUG_NONE)
  // Initialize UART for debugging messages
  Serial.begin(115200);
#endif // (DEBUG_LEVEL != DEBUG_NONE)
  //Initialize GPIO port states
  // Set CS# high to start - SPI inactive
  SET_EVE_CS_NOT;
  // Set PD# high to start
  SET_EVE_PD_NOT;
  SET_SD_CS_NOT;

  //Initialize port directions
  // EVE interrupt output (not used in this example)
  pinMode(EVE_INT, INPUT_PULLUP);
  // EVE Power Down (reset) input
  pinMode(EVE_PD_NOT, OUTPUT);
  // EVE SPI bus CS# input
  pinMode(EVE_CS_NOT, OUTPUT);
  // USD card CS
  pinMode(SD_CS, OUTPUT);
  // Optional pin used for LED or oscilloscope debugging.
  pinMode(DEBUG_LED, OUTPUT);

  // Initialize SPI
  SPI.begin();
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  DBG_GEEK("SPI initialzed to: 8MHz\n");


  //See if we can find the FTDI/BridgeTek EVE processor
  if(0 != EVE_Initialize())
    {
    //DBG_STAT("Failed to initialize %s8%02X. Stopping.\n",EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    while(1);
    }
  else
    {
    //DBG_STAT("%s8%02X initialized.\n",EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    }

  } //  setup()
//===========================================================================
void loop()
  {
  DBG_GEEK("Loop initialization.\n");

  uint16_t FWo;
  FWo = EVE_REG_Read_16(EVE_REG_CMD_WRITE);
  DBG_GEEK("Initial Offest Read: 0x%04X = %u\n",FWo ,FWo);

  //Keep track of the RAM_G memory allocation
  uint32_t RAM_G_Unused_Start;
  RAM_G_Unused_Start=0;
  DBG_GEEK("Initial RAM_G: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);

  //Initialize_Number();

  Initialize_BACK();

  #if (0!=rainbow_demo)
    FWo = Initialize_Rainbow(FWo);
#endif
  

  #if (0!=logo_demo)
  FWo=Initialize_LOGO(FWo,&RAM_G_Unused_Start);
  DBG_GEEK("RAM_G after logo: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
  #endif

  #if (0!=logo_demo1)
  FWo=Initialize_LOGO1(FWo,&RAM_G_Unused_Start);
  DBG_GEEK("RAM_G after logo: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
  #endif

  //while(1)
    //{
    FWo=Wait_for_EVE_Execution_Complete(FWo);

    // //========== START THE DISPLAY LIST ==========
    FWo=EVE_Cmd_Dat_0(FWo,(EVE_ENC_CMD_DLSTART));

    // // Set the default clear color to black
    // FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_CLEAR_COLOR_RGB(0,0,0));
    // // Clear the screen - this and the previous prevent artifacts between lists
    FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_CLEAR(1 /*CLR_COL*/,1 /*CLR_STN*/,1 /*CLR_TAG*/));

    for (int i=10;i<=90;i++){
      if (i<50){
        FWo=EVE_Cmd_Dat_0(FWo,EVE_ENC_COLOR_RGB(0,0,0));
        FWo=EVE_Filled_Rectangle(FWo,i,113,48,76);

        FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_COLOR_RGB(0xFF,0xFF,0xFF));
        FWo=EVE_Filled_Rectangle(FWo,i,150,48,113);
    }
      else{

        FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_COLOR_RGB(0xFF,0xFF,0xFF));
        FWo=EVE_Filled_Rectangle(FWo,i,113,48,76);

        FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_COLOR_RGB(0,0,0));
        FWo=EVE_Filled_Rectangle(FWo,i,150,90,113);
      }
    }

    //========== ADD GRAPHIC ITEMS TO THE DISPLAY LIST ==========
  
// #if (0!=DEBUG_COPROCESSOR_RESET)
//             //Test code to crash coprocessor ever other time it is called --
//             //for testing Reset_EVE_Coprocessor()
//             //DBG_STAT("Initialize_Logo_Demo() . . .");
//             FWo=Initialize_LOGO(FWo,&RAM_G_Unused_Start);
//             //DBG_STAT("  done.\n");
// #endif // (0!=DEBUG_COPROCESSOR_RESET)

#if (0!=DEBUG_COPROCESSOR_RESET)
            //Test code to crash coprocessor ever other time it is called --
            //for testing Reset_EVE_Coprocessor()
            //DBG_STAT("Initialize_Logo_Demo() . . .");
            FWo=Initialize_LOGO1(FWo,&RAM_G_Unused_Start);
            //DBG_STAT("  done.\n");
#endif // (0!=DEBUG_COPROCESSOR_RESET)

 //FWo=Add_Number_To_Display_List(FWo);

 FWo=Add_BACK_To_Display_List(FWo);


#if (0!=logo_demo)
    FWo=Add_LOGO_To_Display_List(FWo);
#endif

#if (0!=logo_demo1)
    FWo=Add_LOGO1_To_Display_List(FWo);
#endif

#if (0!=rainbow_demo)
    FWo=Add_Rainbow_To_Display_List(FWo);
#endif

    // //========== FINSH AND SHOW THE DISPLAY LIST ==========
    // // Instruct the graphics processor to show the list
    FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_DISPLAY());
    // Make this list active
    FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_CMD_SWAP);
    // Update the ring buffer pointer so the graphics processor starts executing
    EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWo));

   //}  // while(1)
  } // loop()
//===========================================================================
