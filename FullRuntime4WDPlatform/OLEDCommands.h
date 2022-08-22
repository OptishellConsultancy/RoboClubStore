#include "SensorDataUpdate.h"
#include "OLEDTestsAndUtil.h"

// To review:https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/

void EnabledOLED()
{
    if (!OLEDInitialised)
    {
        // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
        display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
        display.clearDisplay();
        OLEDInitialised = true;
    }
}
void DoOLEDImgCommand()
{
    Serial.print("DoOLEDImgCommand\r\n");

    display.clearDisplay();

    display.drawBitmap((display.width() - LOGO_WIDTH) / 2,
                       (display.height() - LOGO_HEIGHT) / 2,
                       logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
}

void DoOLEDTxtCommand()
{
    display.clearDisplay();
    OLEDTXT_VALIDCONFIG ? (display.setTextSize(OLEDTXT_S)) : display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);                                             // Draw white text
    // Set position
    OLEDTXT_VALIDCONFIG ? (display.setCursor(OLEDTXT_X, OLEDTXT_Y)) : (display.setCursor(OLEDTXT_X_DEF, OLEDTXT_Y_DEF));
    Serial.print(OLEDTXT_TEXTDATA);
    Serial.print(OLEDTXT_X);
    Serial.print(OLEDTXT_Y);
    
    display.println(OLEDTXT_TEXTDATA);
    // display.println(F("GOOD EVENING"))
    display.display();
}