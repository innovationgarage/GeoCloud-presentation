#include <U8g2lib.h>
#include <Wire.h>
#include "elcheapo.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define LCD_CLK 5
#define LCD_SDA 4
#define WW 128
#define HH 64
void setup(void) {

  // put your setup code here, to run once:
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(LCD_CLK, LCD_SDA);
  u8g2.begin();
  u8g2.enableUTF8Print();
}


void loop(void) {
  delay(100);

  for (int i = 48; i < 59; i++)
  {
    u8g2.clearBuffer();        // transfer internal memory to the display

    /*u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.setCursor(0, 20);
      u8g2.print(i);*/
    u8g2.setFont(progress); // choose a suitable font
    u8g2.setCursor(0, 64);
    u8g2.print((char)i); // write something to the internal memory*/

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(20);
  }
  for (int i = 59; i < 65; i++)
  {
    u8g2.clearBuffer();        // transfer internal memory to the display
    u8g2.setFont(progress); // choose a suitable font
    u8g2.setCursor(0, 64);
    u8g2.print((char)i); // write something to the internal memory*/

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(100);
  }
  delay(1000);

  for (int i = 65; i < 83; i++)
  {
    u8g2.clearBuffer();        // transfer internal memory to the display
    u8g2.setFont(progress); // choose a suitable font
    u8g2.setCursor(0, 64);
    u8g2.print((char)i); // write something to the internal memory*/

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(20);
  }

  delay(1000);

  for (int i = 83; i < 90; i++)
  {
    u8g2.clearBuffer();        // transfer internal memory to the display
    u8g2.setFont(progress); // choose a suitable font
    u8g2.setCursor(0, 64);
    u8g2.print((char)i); // write something to the internal memory*/

    u8g2.setFont(u8g2_font_t0_12_mf ); // choose a suitable font
    const int h = 12, s = 4, x = 3;
    switch (i)
    {
      case 88:      case 89:
        u8g2.setFont(u8g2_font_t0_11b_mf ); // choose a suitable font

        u8g2.setCursor(x, 64 - s);
        u8g2.print("Initialising UI...");
      case 87:
        u8g2.setFont(u8g2_font_t0_12_mf ); // choose a suitable font

        u8g2.setCursor(x, 64 - s - h);
        u8g2.print("Network interface OK");
      case 85: case 86:
        u8g2.setCursor(x, 64 - s - h * 2);
        u8g2.print("Serial port       OK");
      case 84:
        u8g2.setCursor(x, 64 - s - h * 3);
        u8g2.print("System boot       OK");
    }

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(random(300, 1000));
  }
  delay(1000);
  for (int i = 90; i < 100; i++)
  {
    u8g2.clearBuffer();        // transfer internal memory to the display
    u8g2.setFont(progress); // choose a suitable font
    u8g2.setCursor(0, 64);
    u8g2.print((char)i); // write something to the internal memory*/

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(20);
  }

  delay(1000);
}

/*void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
  {
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic

  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 69);
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 65);
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 64);
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 67);
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
      u8g2.drawGlyph(x, y, 67);
      break;
  }
  }*/

/*void drawWeather(uint8_t symbol, int degree)
  {
  drawWeatherSymbol(0, 48, symbol);
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setCursor(48+3, 42);
  u8g2.print(degree);
  u8g2.print("°C");   // requires enableUTF8Print()
  }*/

/*
  Draw a string with specified pixel offset.
  The offset can be negative.
  Limitation: The monochrome font with 8 pixel per glyph
*/
/*void drawScrollString(int16_t offset, const char *s)
  {
  static char buf[36];  // should for screen with up to 256 pixel width
  size_t len;
  size_t char_offset = 0;
  u8g2_uint_t dx = 0;
  size_t visible = 0;


  u8g2.setDrawColor(0);   // clear the scrolling area
  u8g2.drawBox(0, 49, u8g2.getDisplayWidth()-1, u8g2.getDisplayHeight()-1);
  u8g2.setDrawColor(1);   // set the color for the text


  len = strlen(s);
  if ( offset < 0 )
  {
    char_offset = (-offset)/8;
    dx = offset + char_offset*8;
    if ( char_offset >= u8g2.getDisplayWidth()/8 )
      return;
    visible = u8g2.getDisplayWidth()/8-char_offset+1;
    strncpy(buf, s, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(char_offset*8-dx, 62, buf);
  }
  else
  {
    char_offset = offset / 8;
    if ( char_offset >= len )
      return; // nothing visible
    dx = offset - char_offset*8;
    visible = len - char_offset;
    if ( visible > u8g2.getDisplayWidth()/8+1 )
      visible = u8g2.getDisplayWidth()/8+1;
    strncpy(buf, s+char_offset, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(-dx, 62, buf);
  }

  }*/

/*void draw(const char *s, uint8_t symbol, int degree)
  {
  int16_t offset = -(int16_t)u8g2.getDisplayWidth();
  int16_t len = strlen(s);

  u8g2.clearBuffer();         // clear the internal memory
  drawWeather(symbol, degree);    // draw the icon and degree only once
  for(;;)             // then do the scrolling
  {

    drawScrollString(offset, s);        // no clearBuffer required, screen will be partially cleared here
    u8g2.sendBuffer();        // transfer internal memory to the display

    delay(20);
    offset+=2;
    if ( offset > len*8+1 )
      break;
  }
  }*/
