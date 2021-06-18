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


#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();



#define KEY_X 40            // Centre of key
#define KEY_Y 96
#define KEY_W 62            // Width and height
#define KEY_H 30
#define KEY_SPACING_X 18    // X and Y gap
#define KEY_SPACING_Y 20
#define KEY_TEXTSIZE 1      // Font size multiplier
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2
#define DISP_X 1
#define DISP_Y 10
#define DISP_W 238
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN
#define STATUS_X 120
#define STATUS_Y 65
#define NUM_LEN 12

uint8_t numberIndex = 0;
char numberBuffer[NUM_LEN + 1] = "";

char keyLabel[15][5] = {
    "New", "Del", "Send",
    "1", "2", "3",
    "4", "5", "6",
    "7", "8", "9",
    ".", "0", "#" };
uint16_t keyColor[15] = {
    TFT_RED, TFT_DARKGREY, TFT_DARKGREEN,
    TFT_BLUE, TFT_BLUE, TFT_BLUE,
    TFT_BLUE, TFT_BLUE, TFT_BLUE,
    TFT_BLUE, TFT_BLUE, TFT_BLUE,
    TFT_BLUE, TFT_BLUE, TFT_BLUE };

TFT_eSPI_Button key[15];



//------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("esp32_Touch_test.ino started");

    tft.init();
    tft.setRotation(0);

    touch_calibrate();

    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 0, 240, 320, TFT_DARKGREY);
    tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
    tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
    drawKeypad();
}

//------------------------------------------------------------------------------------------

void loop(void)
{
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
    bool pressed = tft.getTouch(&t_x, &t_y);

    for (uint8_t b = 0; b < 15; b++)
    {
        if (pressed && key[b].contains(t_x, t_y))
        {
            key[b].press(true);  // tell the button it is pressed
        }
        else
        {
            key[b].press(false);  // tell the button it is NOT pressed
        }
    }

    // Check if any key has changed state

    for (uint8_t b = 0; b < 15; b++)
    {
        if (b < 3)
            tft.setFreeFont(LABEL1_FONT);
        else
            tft.setFreeFont(LABEL2_FONT);

        if (key[b].justReleased())
            key[b].drawButton();     // draw normal

        if (key[b].justPressed())
        {
            key[b].drawButton(true);  // draw invert

            // if a numberpad button, append the relevant # to the numberBuffer
            if (b >= 3)
            {
                if (numberIndex < NUM_LEN)
                {
                    numberBuffer[numberIndex] = keyLabel[b][0];
                    numberIndex++;
                    numberBuffer[numberIndex] = 0; // zero terminate
                }
                status(""); // Clear the old status
            }

            // Del button, so delete last char
            if (b == 1)
            {
                numberBuffer[numberIndex] = 0;
                if (numberIndex > 0)
                {
                    numberIndex--;
                    numberBuffer[numberIndex] = 0;//' ';
                }
                status(""); // Clear the old status
            }

            if (b == 2)
            {
                status("Sent value to serial port");
                Serial.println(numberBuffer);
            }

            if (b == 0)
            {
                status("Value cleared");
                numberIndex = 0; // Reset index to 0
                numberBuffer[numberIndex] = 0; // Place null in buffer
            }

            // Update the number display field
            tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
            tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
            tft.setTextColor(DISP_TCOLOR);     // Set the font colour

            // Draw the string, the value returned is the width in pixels
            int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

            // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
            // but it will not work with italic or oblique fonts due to character overlap.
            tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

            delay(10); // UI debouncing
        }
    }
}   // loop()




void drawKeypad()
{
    for (uint8_t row = 0; row < 5; row++)
    {
        for (uint8_t col = 0; col < 3; col++)
        {
            uint8_t b = col + row * 3;

            if (b < 3)
                tft.setFreeFont(LABEL1_FONT);
            else
                tft.setFreeFont(LABEL2_FONT);

            key[b].initButton(
                &tft,
                KEY_X + col * (KEY_W + KEY_SPACING_X),
                KEY_Y + row * (KEY_H + KEY_SPACING_Y),
                KEY_W,
                KEY_H,
                TFT_WHITE,
                keyColor[b],
                TFT_WHITE,
                keyLabel[b],
                KEY_TEXTSIZE);
            key[b].drawButton();
        }
    }
}


//------------------------------------------------------------------------------------------

void touch_calibrate()
{
    uint16_t calData[5];
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    Serial.println("Calibration data:");
    for (int i=0; i<5; i++)
        Serial.println(calData[i]);

    // typical:
    // 477,3365,206,3438,2
}


void status(const char *msg)
{
    tft.setTextPadding(240);
    //tft.setCursor(STATUS_X, STATUS_Y);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextFont(0);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    tft.drawString(msg, STATUS_X, STATUS_Y);
}
