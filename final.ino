#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


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
// color565(0, 0, 0); --> RGB color code. Use the "color picker" to use or find another color code.
uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
//----------------------------------------

/********************************************************************************/
String payload;
const char* ssid = "CKEMFG-L2";
const char* password = "CheongKongT65";
String GOOGLE_SCRIPT_ID = "AKfycbz8AHh6OA9OKkI7GPkKYwLEYunzcvDLvwPfdbmGMNFK3BHFfeieegBqcs5neH2Xib3QvA";

const int sendInterval = 5000;

struct DataPoint {
  String date;
  int days;
  int hours;
  int minutes;
  int seconds;
};
/********************************************************************************/


void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Started");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

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
  
  dma_display->clearScreen();
  
  dma_display->fillScreen(myWHITE);
  delay(1000);
  dma_display->fillScreen(myRED);
  delay(1000);
  dma_display->fillScreen(myGREEN);
  delay(1000);
  dma_display->fillScreen(myBLUE);
  delay(1000);
  
  dma_display->clearScreen();
  delay(1000);

  Serial.println("Ready to go");
}

void loop() {
  
  spreadsheet_comm();
    dma_display->clearScreen();

    dma_display->setTextSize(1);    
  dma_display->setTextWrap(false);

  dma_display->setCursor(0, 0);
  dma_display->setTextColor(dma_display->color565(255, 255, 255));
  dma_display->print(payload);


  delay(sendInterval);

}

void spreadsheet_comm(void) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?read";
    Serial.print("Making a request: ");
    Serial.println(url);
    
    http.begin(url.c_str());  // Specify the URL
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);  // Follow redirects

    int httpCode = http.GET();

    if (httpCode > 0) { // Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();  // End HTTP connection
  } else {
    Serial.println("WiFi not connected");
  }
}
