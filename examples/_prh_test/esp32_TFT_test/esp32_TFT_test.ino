//--------------------------------------
// pins
//---------------------------------------
// ESP32 pin  definitions are in libraries/TFT_eSPI/prh_setup.h
//
//      VCC         5V
//      GND         GND
//      CS          17          prh_setup.h: was 15
//      RESET       3.3V
//      D/C         16          prh_setup.h: was 2
//      SDI         23          MOSI
//      SCK         18          CLK
//      LED         5V          use 100 ohm resistor
//      SDO         19          MISO
//
//      T_CLK       18          CLK
//      T_CS        5
//      T_DIN       23          MOSI
//      T_DO        19          MISO
//
//      T_IRQ       -1          Not Used




#include "SPI.h"
#include "TFT_eSPI.h"



TFT_eSPI tft = TFT_eSPI();


#define WITH_TOUCH  1

#if WITH_TOUCH

    #include "XPT2046_Touchscreen.h"

    XPT2046_Touchscreen ts(21);

    // const int TOUCH_IRQ = 35;// to identify the pin to watch.
    // TS_Point rawLocation;
    //
    // void IRAM_ATTR ScreenPressed()
    // {
    //     // halt further interrupts
    //     detachInterrupt(TOUCH_IRQ);
    //     if ( touch.touched() )
    //     {
    //       rawLocation = touch.getPoint();
    //       TouchX = rawLocation.x;
    //       TouchY = rawLocation.y;
    //       PAD_DOWN = true;
    //     }
    //     digitalWrite(BUILTIN_LED, HIGH);
    //     attachInterrupt(digitalPinToInterrupt(TOUCH_IRQ), ScreenPressed, FALLING);
    // }
    //
    // void AttachTouch()
    // {
    //   PAD_DOWN = false;
    //   attachInterrupt(digitalPinToInterrupt(TOUCH_IRQ), ScreenPressed, FALLING);
    // }
#endif


void setup()
{
    Serial.begin(115200);
    while (!Serial);

    Serial.println("");
    Serial.println("");
    Serial.println("TFT_eSPI library test!");

    #if 0
        SPI.begin(alt_sclk,alt_miso,alt_miso);
        SPI.setClockDivider(1);
        SPI.beginTransaction(SPISettings(60000000, MSBFIRST, SPI_MODE0));
        SPI.endTransaction();
        touch.begin();
        Serial.println("Touch Screen is ready!");
    #endif

    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.fillScreen(TFT_RED);
    tft.fillScreen(TFT_GREEN);
    tft.fillScreen(TFT_BLUE);
    tft.fillScreen(TFT_BLACK);

    #if WITH_TOUCH
        tft.setRotation(1);
        ts.begin();
        ts.setRotation(1);
    #endif


    testText();

}

void loop(void)
{
    #if WITH_TOUCH
        static boolean wastouched = true;
        boolean istouched = ts.touched();
        if (istouched)
        {
            TS_Point p = ts.getPoint();
            if (!wastouched)
            {
                tft.fillScreen(TFT_BLACK);
                tft.setTextColor(TFT_YELLOW);
                //  tft.setFont(Arial_60);
                tft.setTextSize(5);
                tft.setCursor(60, 80);
                tft.print("Touch");
            }
            tft.fillRect(100, 150, 140, 60, TFT_BLACK);
            tft.setTextColor(TFT_GREEN);
            // tft.setFont(Arial_24);
            tft.setTextSize(2);
            tft.setCursor(100, 150);
            tft.print("X = ");
            tft.print(p.x);
            tft.setCursor(100, 180);
            tft.print("Y = ");
            tft.print(p.y);
            Serial.print(", x = ");
            Serial.print(p.x);
            Serial.print(", y = ");
            Serial.println(p.y);
        }
        else
        {
            if (wastouched)
            {
                tft.fillScreen(TFT_BLACK);
                tft.setTextColor(TFT_RED);
                // tft.setFont(Arial_48);
                tft.setTextSize(3);
                tft.setCursor(120, 50);
                tft.print("No");
                tft.setCursor(80, 120);
                tft.print("Touch");
            }
            Serial.println("no touch");
        }
        wastouched = istouched;
        delay(100);
    #else
        for (uint8_t rotation = 0; rotation < 4; rotation++)
        {
            tft.setRotation(rotation);
            testText();
            delay(2000);
        }
    #endif
}


void testText()
{
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(TFT_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
}
