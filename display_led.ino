//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01_Showing_Text
//----------------------------------------Including the libraries.
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
//----------------------------------------

//----------------------------------------Defines the connected PIN between P5 and ESP32.
#define R1_PIN 19
#define G1_PIN 13
#define B1_PIN 18
#define R2_PIN 5
#define G2_PIN 12
#define B2_PIN 17

#define A_PIN 16
#define B_PIN 14
#define C_PIN 4
#define D_PIN 27
#define E_PIN -1  //--> required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32.

#define LAT_PIN 26
#define OE_PIN 15
#define CLK_PIN 2
//----------------------------------------

//----------------------------------------Defines the P5 Panel configuration.
#define PANEL_RES_X 64  //--> Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32  //--> Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1   //--> Total number of panels chained one to another
//----------------------------------------

// Initialize MatrixPanel_I2S_DMA as "dma_display".
MatrixPanel_I2S_DMA *dma_display = nullptr;

//----------------------------------------Variable for color.
uint16_t myBLACK, myWHITE, myRED, myGREEN, myBLUE;

//----------------------------------------Number counter
int counter = 0;

//________________________________________________________________________________VOID SETUP()
void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  delay(1000);

  // Initialize the connected PIN between Panel P5 and ESP32.
  HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
  delay(10);

  //----------------------------------------Module configuration.
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   //--> module width.
    PANEL_RES_Y,   //--> module height.
    PANEL_CHAIN,   //--> Chain length.
    _pins          //--> pin mapping.
  );
  delay(10);
  //----------------------------------------

  // Set I2S clock speed.
  mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;  // I2S clock speed, better leave as-is unless you want to experiment.
  delay(10);

  //----------------------------------------Display Setup.
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(35); //--> 0-255.
  //----------------------------------------

  myBLACK = dma_display->color565(0, 0, 0);
  myWHITE = dma_display->color565(255, 255, 255);
  myRED = dma_display->color565(255, 0, 0);
  myGREEN = dma_display->color565(0, 255, 0);
  myBLUE = dma_display->color565(0, 0, 255);

  dma_display->clearScreen();
  delay(1000);
}

//________________________________________________________________________________VOID LOOP()
void loop() {
  dma_display->clearScreen();  // Clear the screen at the beginning of the loop

  // Set the text properties
  dma_display->setTextSize(1);    
  dma_display->setTextWrap(false);

  //----------------------------------------
  dma_display->setCursor(10, 0);
  dma_display->setTextColor(dma_display->color565(255, 153, 0));
  dma_display->println("SHELF");

  dma_display->setCursor(36, 0);
  dma_display->setTextColor(dma_display->color565(255, 0, 255));
  dma_display->print("DATA");
  //----------------------------------------

  //----------------------------------------
  dma_display->setCursor(11, 8);
  dma_display->setTextColor(dma_display->color565(0, 152, 158));
  dma_display->println("DISPLAY");
  //----------------------------------------

  //----------------------------------------
  dma_display->setCursor(16, 17);
  dma_display->setTextColor(dma_display->color565(255, 255, 255));
  dma_display->print("P5");

  dma_display->setCursor(30, 17);
  dma_display->setTextColor(dma_display->color565(255, 0, 0));
  dma_display->print("R");
  
  dma_display->setTextColor(dma_display->color565(0, 255, 0));
  dma_display->print("G");

  dma_display->setTextColor(dma_display->color565(0, 0, 255));
  dma_display->print("B");
  //----------------------------------------

  //----------------------------------------
  dma_display->setCursor(16, 25);
  dma_display->setTextColor(dma_display->color565(255, 0, 102));
  dma_display->print("NUMBER");

  dma_display->setCursor(36, 25);
  dma_display->setTextColor(dma_display->color565(241, 197, 7));

  // Display the counter
  dma_display->print(counter);
  //----------------------------------------

  // Update the counter
  counter++;
  if (counter > 9) {
    counter = 0; // Reset the counter after 9
  }

  delay(1000); // Wait for 1 second before updating the display
}
//________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
