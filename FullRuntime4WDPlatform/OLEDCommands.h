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
    display.display();

    display.clearDisplay();
}

void DoOLEDTxtCommand()
{
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(40, 32);        // Start in the center
    Serial.print(OLEDTXT_TEXTDATA);
    display.println(OLEDTXT_TEXTDATA);
    // display.println(F("GOOD EVENING"))
    display.display();
}