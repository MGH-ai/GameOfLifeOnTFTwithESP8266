#include <Adafruit_GFX.h>       // Core graphics library for the display
#include <Adafruit_ST7735.h>    // ST7735 display driver
#include <SPI.h>                // SPI library for communication

// Define the grid size (gridSize x gridSize) for the Game of Life
const int gridSize = 50;  

// Function declarations
void printGrid(bool gridOne[gridSize+1][gridSize+1]);
void determineState(bool gridOne[gridSize+1][gridSize+1]);
void clearScreen(void);

// Define grid arrays to hold cell states
bool gridOne[gridSize+1][gridSize+1] = {};  // Current state
bool gridOld[gridSize+1][gridSize+1] = {};  // Temporary storage for the previous state
int x, y, n;  // Variables for loop counters

// TFT display pins on Wemos D1 mini
#define TFT_CS   D8  // Chip select pin
#define TFT_DC   D2  // Data/command pin
#define TFT_RST  D6  // Reset pin

int width, height;            // Dimensions of the display
int random_height, random_width;  // Random positions for testing
uint random_color;            // Random color for testing

// Initialize the ST7735 display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  tft.initR(INITR_BLACKTAB);    // Initialize display with a black background
  tft.fillScreen(ST77XX_BLACK); // Fill screen with black
  width = tft.width();          // Get display width
  height = tft.height();        // Get display height

  // Initialize starting pattern for the Game of Life
  initialState();
}

void loop() {
  // Display the current grid on the screen
  printGrid(gridOne);
  delayMicroseconds(200000);    // Short delay to control animation speed

  // Clear screen for the next frame
  clearScreen();

  // Calculate next generation of cells based on Game of Life rules
  determineState(gridOne);
}

// Function to set an initial pattern of live cells
void initialState(){
  gridOne[5][4] = true;  // Example pattern: setting cells to be "alive"
  gridOne[7][4] = true;
  gridOne[5][5] = true;
  gridOne[6][5] = true;
  gridOne[5][6] = true;
}

// Function to clear "alive" cells from the screen by turning them black
void clearScreen() {
  for(int a = 1; a < gridSize; a++) {
    for(int b = 1; b < gridSize; b++) {
      if(gridOne[a][b] == true) {
        tft.drawPixel(a, b, ST7735_BLACK); // Set pixel to black
      }
    }
  }
}

// Function to print the current grid state on the display
void printGrid(bool gridOne[gridSize+1][gridSize+1]){
  for(int a = 1; a < gridSize; a++) {
    for(int b = 1; b < gridSize; b++) {
      if(gridOne[a][b] == true) {
        tft.drawPixel(a, b, ST7735_CYAN);  // Draw live cells in cyan
      }
    }
  }
}

// Copy gridOne (current state) into gridTwo (previous state) for comparison
void compareGrid (bool gridOne[gridSize+1][gridSize+1], bool gridTwo[gridSize+1][gridSize+1]){
  for(int a = 0; a < gridSize; a++) {
    for(int b = 0; b < gridSize; b++) {
      gridTwo[a][b] = gridOne[a][b];
    }
  }
}

// Calculate the next generation of cells
void determineState(bool gridOne[gridSize+1][gridSize+1]){
  bool gridTwo[gridSize+1][gridSize+1] = {};  // Temporary grid to hold previous state
  compareGrid(gridOne, gridTwo);              // Copy current state into gridTwo

  // Apply Conway's Game of Life rules
  for(int a = 1; a < gridSize; a++) {
    for(int b = 1; b < gridSize; b++) {
      int alive = 0;  // Count of live neighbors

      // Check all 8 neighbors
      for(int c = -1; c < 2; c++) {
        for(int d = -1; d < 2; d++) {
          if(!(c == 0 && d == 0)) {       // Exclude the cell itself
            if(gridTwo[a+c][b+d]) {       // If neighbor is alive
              ++alive;                    // Increment live neighbor count
            }
          }
        }
      }

      // Game of Life rules
      if(alive < 2) {
        gridOne[a][b] = false;            // Cell dies from loneliness
      } else if(alive == 3) {
        gridOne[a][b] = true;             // Cell becomes or remains alive
      } else if(alive > 3) {
        gridOne[a][b] = false;            // Cell dies from overpopulation
      }
    }
  }
}
