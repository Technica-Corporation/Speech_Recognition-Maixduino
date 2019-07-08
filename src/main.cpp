#include <Arduino.h>
#include <Sipeed_ST7789.h>

/*
    I added input parameters to Maix_Speech_Recognition.h's begin(int pin0, int pin1, int, pin2)
    This allows you to specify the pins to use as I was trying to get the one dock's built-in mic to work
    Additionally, the default data0 pin in the code did not work for me with the mic array
 */
#include "Maix_Speech_Recognition_demo.h"
#include "model_wake.h"
#include "model_primaryc.h"
#include "model_secondaryc.h"
#include "model_invalidc.h"
#include "mic_array.h"

#define LABEL_TEXT_SIZE 2 

// This is labeled as a red squiggle for some reason; it still compiles though... 
SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_, SIPEED_ST7789_DCX_PIN, SIPEED_ST7789_RST_PIN, DMAC_CHANNEL2);
SpeechRecognizer rec;

void printCenterOnLCD(Sipeed_ST7789 &lcd_, const char *msg, uint8_t textSize = LABEL_TEXT_SIZE) 
{
    lcd_.setCursor((lcd_.width() - (6 * textSize * strlen(msg))) / 2, (lcd_.height() - (8*textSize)) / 2);
    lcd_.print(msg);
}

void setup() {
  lcd.setTextSize(LABEL_TEXT_SIZE);
  lcd.setTextColor(COLOR_WHITE);

/*
    Turn off/down Mic array lights
    If you just plug in the array and go, they lights blink very fast and are VERY bright
 */
  lights_out(0, 3, 4, MIC_LED_DAT, SPI0_CS1);

/*
    Pin inputs for Mic:
    rec.begin(I2S0_IN_D0, I2S0_SCLK, I2S0_WS)

    Mic Array data input pins: d0=23, d1=22, d2=20
      d0 doesn't seem to work for me
      d2 kind of works, but not well for me
      d1 I have not tested

    Built-in mic pin inputs: rec.begin(31, 30, 32);
      I could not get the built-in mic to work with Maixduino
      It did work with just the standalone-sdk though
 */
  rec.begin(23, 18, 19);

  delay(1000);
  Serial.begin(115200);
  
  if (!lcd.begin(15000000, COLOR_BLACK)) {
    Serial.println("");
    while(1); 
  }
    
  Serial.println("Initializing...");
  printCenterOnLCD(lcd, "Initializing...");

  // *Moving the mic array across a desk*
  rec.addVoiceModel(0, 0, move_0, fram_num_move_0); 
  rec.addVoiceModel(0, 1, move_1, fram_num_move_1); 
  rec.addVoiceModel(0, 2, move_2, fram_num_move_2); 
  rec.addVoiceModel(0, 3, move_3, fram_num_move_3);  

  // *clearing my throat*
  rec.addVoiceModel(1, 0, clearthroat_0, fram_num_clearthroat_0); 
  rec.addVoiceModel(1, 1, clearthroat_1, fram_num_clearthroat_1); 
  rec.addVoiceModel(1, 2, clearthroat_2, fram_num_clearthroat_2); 
  rec.addVoiceModel(1, 3, clearthroat_3, fram_num_clearthroat_3);   

  // *clap* and *clap clap*
  rec.addVoiceModel(2, 0, clap_0, fram_num_clap_0);   
  rec.addVoiceModel(2, 1, clap_1, fram_num_clap_1);   
  rec.addVoiceModel(2, 2, clap_2, fram_num_clap_2);   
  rec.addVoiceModel(2, 3, clap_3, fram_num_clap_3);   

  // "red" and "red on"
  rec.addVoiceModel(3, 0, red_0, fram_num_red_0); 
  rec.addVoiceModel(3, 1, red_1, fram_num_red_1); 
  rec.addVoiceModel(3, 2, red_2, fram_num_red_2); 
  rec.addVoiceModel(3, 3, red_3, fram_num_red_3); 

  // "green" and "green on"
  rec.addVoiceModel(4, 0, green_0, fram_num_green_0);     
  rec.addVoiceModel(4, 1, green_1, fram_num_green_1);     
  rec.addVoiceModel(4, 2, green_2, fram_num_green_2);     
  rec.addVoiceModel(4, 3, green_3, fram_num_green_3); 

  // "blue" and "blue on"   
  rec.addVoiceModel(5, 0, blue_0, fram_num_blue_0);   
  rec.addVoiceModel(5, 1, blue_1, fram_num_blue_1);   
  rec.addVoiceModel(5, 2, blue_2, fram_num_blue_2);   
  rec.addVoiceModel(5, 3, blue_3, fram_num_blue_3);   

  // "cyan" and "cyan on"
  rec.addVoiceModel(6, 0, cyan_0, fram_num_cyan_0); 
  rec.addVoiceModel(6, 1, cyan_1, fram_num_cyan_1); 
  rec.addVoiceModel(6, 2, cyan_2, fram_num_cyan_2); 
  rec.addVoiceModel(6, 3, cyan_3, fram_num_cyan_3); 

  // "magenta" and "magenta on"
  rec.addVoiceModel(7, 0, magenta_0, fram_num_magenta_0);     
  rec.addVoiceModel(7, 1, magenta_1, fram_num_magenta_1);     
  rec.addVoiceModel(7, 2, magenta_2, fram_num_magenta_2);     
  rec.addVoiceModel(7, 3, magenta_3, fram_num_magenta_3); 

  // "yellow" and "yellow on"  
  rec.addVoiceModel(8, 0, yellow_0, fram_num_yellow_0);   
  rec.addVoiceModel(8, 1, yellow_1, fram_num_yellow_1);   
  rec.addVoiceModel(8, 2, yellow_2, fram_num_yellow_2);   
  rec.addVoiceModel(8, 3, yellow_3, fram_num_yellow_3);  

  Serial.println("Model init OK!");
  lcd.fillScreen(COLOR_BLACK);
  printCenterOnLCD(lcd, "Initialization complete!");
  delay(1000);
}

int ON = 0;
void loop() {
  int res;

  if (ON == 0){
      lcd.fillScreen(COLOR_BLACK);
      printCenterOnLCD(lcd, "Waiting for Wake Up...");
      res = rec.recognize();
      lcd.fillScreen(COLOR_BLACK);
      Serial.printf("Result: %d --> ", res);
      if (res > 0){
        switch (res)
        {                
            case 1:      
                printCenterOnLCD(lcd, "NOT AWAKE: MOVE");
                break;
            case 2:      
                printCenterOnLCD(lcd, "NOT AWAKE: Clear Throat");
                break;
            case 3:
                printCenterOnLCD(lcd, "AWAKE!");
                ON = 1;
                break;                                                           
            default: 
                printCenterOnLCD(lcd, "Not a Wake Up!"); 
                break;    
        }
      } else
      {
        printCenterOnLCD(lcd, "Not a Wake Up!");
      }
    delay(1000); // short delay here
  }
  if (ON == 1){
      lcd.fillScreen(COLOR_BLACK);
      lcd.setTextColor(COLOR_WHITE);
      printCenterOnLCD(lcd, "Color Command...");
      res = rec.recognize();
      Serial.printf("Result: %d --> ", res);
      if (res > 0){
        switch (res)
        {
            case 1:
                lcd.setTextColor(COLOR_WHITE);
                lcd.fillScreen(COLOR_BLACK);        
                printCenterOnLCD(lcd, "MOVE");
                break;     
            case 2:
                lcd.setTextColor(COLOR_WHITE);
                lcd.fillScreen(COLOR_BLACK);        
                printCenterOnLCD(lcd, "CLEAR THROAT");
                break;         
            case 3:
                lcd.setTextColor(COLOR_WHITE);
                lcd.fillScreen(COLOR_BLACK);        
                printCenterOnLCD(lcd, "Going back to sleep...");
                ON = 0;
                break;                
            case 4:
                lcd.setTextColor(COLOR_BLACK);
                lcd.fillScreen(COLOR_RED);        
                printCenterOnLCD(lcd, "RED");
                break;
            case 5:
                lcd.setTextColor(COLOR_BLACK);
                lcd.fillScreen(COLOR_GREEN);        
                printCenterOnLCD(lcd, "GREEN");
                break;
            case 6:
                lcd.setTextColor(COLOR_BLACK);          
                lcd.fillScreen(COLOR_BLUE);    
                printCenterOnLCD(lcd, "BLUE");
                break;                
            case 7:
                lcd.setTextColor(COLOR_BLACK);            
                lcd.fillScreen(COLOR_CYAN);        
                printCenterOnLCD(lcd, "CYAN");
                break;
            case 8:
                lcd.setTextColor(COLOR_BLACK);           
                lcd.fillScreen(COLOR_MAGENTA);        
                printCenterOnLCD(lcd, "MAGENTA");
                break;
            case 9:
                lcd.setTextColor(COLOR_BLACK);           
                lcd.fillScreen(COLOR_YELLOW);        
                printCenterOnLCD(lcd, "YELLOW");
                break;
            default:
                lcd.setTextColor(COLOR_BLACK);
                lcd.fillScreen(COLOR_LIGHTGREY);      
                printCenterOnLCD(lcd, "INVALID COMMAND");    
                break;          
        }
      } else
      {
        lcd.setTextColor(COLOR_BLACK);        
        lcd.fillScreen(COLOR_LIGHTGREY);        
        printCenterOnLCD(lcd, "INVALID COMMAND");
      }
    delay(3000); // longer delay to display color longer
  }
}