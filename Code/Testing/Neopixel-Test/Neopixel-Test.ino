#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    7
#define LED_PIN2   6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 400

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

uint32_t nvis_green_A = strip.ColorHSV(16384, 255, 102);
// uint32_t nvis_green_B = strip.Color(85, 138, 0);
// uint32_t nvis_yellow = strip.Color(255, 100, 0);
// uint32_t nvis_red = strip.Color(158, 4, 4);
// uint32_t nvis_white = strip.Color(176, 176, 176);
// uint32_t off = strip.Color(0,0,0);
int i = 0;

uint32_t color = strip.Color(0,0,0);

void setup() {
  strip.begin();
  strip2.begin();
  strip.setBrightness(255);
  strip2.setBrightness(255);
  strip.clear();
  strip2.clear();
  strip.fill(strip.gamma32(strip.ColorHSV(16384,255,190)));
  strip2.fill(strip.gamma32(strip.ColorHSV(16384,255,190)));
  strip.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // strip.setPixelColor(0, nvis_yellow);
  // strip.setPixelColor(1, nvis_yellow);
  // strip.show();
  // delay(2000);
  // for(i=2; i<LED_COUNT; i=i+2) 
  //   {
  //   strip.setPixelColor(i-2, off);
  //   strip.setPixelColor(i-1, off);
  //   strip.setPixelColor(i, nvis_yellow);
  //   strip.setPixelColor(i+1, nvis_yellow);
  //   strip.show();
  //   delay(2000);
  //   }
  // strip.setPixelColor(i-2, off);
  // strip.setPixelColor(i-1, off);
  // strip.show();
  // delay(2000);
//   strip.fill(strip.gamma32(strip.ColorHSV(16384,255,0)));
//   strip.show();
//   delay(3000);
//  strip.fill(strip.gamma32(strip.ColorHSV(16384,255,63)));
//   strip.show();
//   delay(3000); 
//   strip.fill(strip.gamma32(strip.ColorHSV(16384,255,127)));
//   strip.show();
//   delay (3000);
//   strip.fill(strip.gamma32(strip.ColorHSV(16384,255,190)));
//   strip.show();
//   delay (3000);
//   strip.fill(strip.gamma32(strip.ColorHSV(16384,255,255)));
//   strip.show();
//   delay(2000);
 
}