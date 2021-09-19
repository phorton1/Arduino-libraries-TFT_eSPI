
#define ILI9341_DRIVER
// #define ILI9488_DRIVER
// #define ILI9486_DRIVER

#define CNC3018


#define TFT_MISO    19
#define TFT_MOSI    23
#define TFT_SCLK    18

#ifdef CNC3018
    #define TFT_CS      22
    #define TFT_DC      21
    #define TOUCH_CS    5
#else
    #define TFT_CS      17  // was 15 // Chip select control pin
    #define TFT_DC      16  // was 2  // Data Command control pin
    #define TOUCH_CS    5   // Chip select pin (T_CS) of touch screen
#endif

// #define TFT_RST     4  // Reset pin (could connect to RST pin)
#define TFT_RST     -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST


// prh - undef'd these after I created my own fonts
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
    // prh - turned back on for test of old touchUI with SDCard turned off




// #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
// #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters

// prh undefined the following 3 because they don't seem to work with setTextFont(n)
//      #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
//      #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
//      #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.

//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
    // prh - misleading comment ... there is no #defeine FF1..FF48 ... I guess you can pass thru define them
    // to become "standard" with an ancient version of Adafruit's GFX library

// Comment out the #define below to stop the SPIFFS filing system
// and smooth font code being loaded
// this will save ~20kbytes of FLASH

// #define SMOOTH_FONT
    // prh - removed.  As far as I can tell it is for "VLW" fonts that
    // can be loaded from the SPIFFS at runtime


#define SPI_FREQUENCY           27000000
#define SPI_READ_FREQUENCY      20000000
#define SPI_TOUCH_FREQUENCY     2500000

// #define USE_HSPI_PORT

// #define SUPPORT_TRANSACTIONS
