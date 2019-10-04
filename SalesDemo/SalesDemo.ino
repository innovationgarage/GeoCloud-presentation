#include <U8g2lib.h>
#include <Wire.h>
#include "OneButton.h"
#include "elcheapo.h"

#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

OneButton button_left(BTN_LEFT, true), button_middle(BTN_MID, true), button_right(BTN_RIGHT, true);

#define LCD_CLK 5
#define LCD_SDA 4
#define WW 128
#define HH 64

void setup(void) {

  // put your setup code here, to run once:
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(LCD_CLK, LCD_SDA);
  u8g2.begin();

  Serial.begin(9600);
  u8g2.enableUTF8Print();
}

int step = 0, anim = 0;
unsigned long next = 0;

void main_screen()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_11_mr ); // choose a suitable font

  const int s = 10, s2 = 6, h = 68;
  u8g2.setCursor(0, h - s * 6);
  u8g2.print("MMSI  :     261536002");
  u8g2.setCursor(0, h - s * 5);
  u8g2.print("IMO:          9048902");
  u8g2.setCursor(0, h - s * 4);
  u8g2.print("IP  :  200.192.18.177");
  u8g2.setCursor(0, h - s * 3);
  u8g2.print("Lat:        69.6492 N");
  u8g2.setCursor(0, h - s * 2);
  u8g2.print("Long:       18.9553 E");
  u8g2.setFont(u8g2_font_5x8_mf      ); // choose a suitable font
  u8g2.setCursor(0, 64 - s2);
  int m = anim <10?0:(anim<20?1:(anim<30?2:3));
  switch (m)
  {
    case 0: u8g2.print("Updating."); break;
    case 1: u8g2.print("Updating.."); break;
    case 2: u8g2.print("Updating..."); break;
    case 3: u8g2.print("Updated a few seconds ago"); break;
  }

  u8g2.setFont(bdf_font2); // choose a suitable font

  // next to mmsi
  u8g2.drawGlyph(25, 51, (anim > 48 && anim < 62) ? anim : 49);

  // next to ip
  u8g2.drawGlyph(13, 71, (anim > 48 && anim < 62) ? anim : 49);

  if (anim++ > 200)
    anim = 0;


  u8g2.sendBuffer();
  next = millis() + 10;
}

void button_left_click()
{
  Serial.println("left");
  step = 10;
  next = 0;
}

void button_middle_click()
{
  Serial.println("middle");
  step = 0;
  next = 0;
}

void button_right_click()
{
  Serial.println("right");
  step = 20;
  next = 0;
}

void register_screen(int animstep)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_22_mn   );
  u8g2.setCursor(0, 64);
  u8g2.print(random(100, 999));
  u8g2.setCursor(45, 64);
  u8g2.print(random(100, 999));
  u8g2.setCursor(90, 64);
  u8g2.print(random(100, 999));

  if (anim++ < 10)
    next = millis();
  else
  {
    anim = 0;
    next = millis() + 10000;

    u8g2.setFont(bdf_font2); // choose a suitable font
    u8g2.drawGlyph(64 - 25, 50, 48);

  }

  u8g2.sendBuffer();
}

void settings_screen(int animstep)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_11_mr ); // choose a suitable font

  const int s = 12, s2 = 6, h = 68;

  u8g2.setFontMode(0);
  u8g2.setDrawColor(0);
  u8g2.drawStr(0,h-s*5,"Ping");
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setCursor(0, h - s * 4);
  u8g2.print("Configure Wifi");
  u8g2.setCursor(0, h - s * 3);
  u8g2.print("Message stats");
  u8g2.setCursor(0, h - s * 2);
  u8g2.print("Reboot");
  u8g2.setCursor(0, h - s * 1);
  u8g2.print("Shutdown");
  u8g2.setCursor(0, h - s * 0);

  u8g2.sendBuffer();
}

void main_loop()
{
  button_left.attachClick(button_left_click);
  //button_left.attachLongPressStart(button_left_hold);
  button_middle.attachClick(button_middle_click);
  //button_middle.attachLongPressStart(button_middle_hold);
  button_right.attachClick(button_right_click);
  //button_right.attachLongPressStart(button_right_hold);

  while (true)
  {
    if (millis() > next)
    {
      switch (step)
      {
        case 0:
          main_screen();
          break;


        case 10:
          anim = 0;
          step++;
        case 11:
          // Register
          register_screen(anim);
          break;

        case 20:
          anim = 0;
          step++;
        case 21:

          // Settings
          settings_screen(anim);
          break;
      }
    }

    button_left.tick();
    button_middle.tick();
    button_right.tick();
    delay(10);
  }
}

void loop(void) {
  delay(100);
  //smain_loop();
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
        u8g2.print("Vessel Link       OK");
      case 87:
        u8g2.setFont(u8g2_font_t0_12_mf ); // choose a suitable font

        u8g2.setCursor(x, 64 - s - h);
        u8g2.print("Dualog Servers    OK");
      case 85: case 86:
        u8g2.setCursor(x, 64 - s - h * 2);
        u8g2.print("GeoCloud Service  OK");
      case 84:
        u8g2.setCursor(x, 64 - s - h * 3);
        u8g2.print("System start      OK");
    }

    u8g2.sendBuffer();        // transfer internal memory to the display
    delay(random(800, 3000));
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

  // Main screen
  main_loop();
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
