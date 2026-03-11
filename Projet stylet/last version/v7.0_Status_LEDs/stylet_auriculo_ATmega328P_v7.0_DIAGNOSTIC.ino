/*
 * DIAGNOSTIC VERSION - Tests hardware without Serial
 * This version uses LED blink patterns to diagnose issues
 * 
 * BLINK CODES:
 * - Setup start: 1 long blink (500ms)
 * - I2C OK: 2 short blinks (100ms each)
 * - I2C FAIL: 5 fast blinks (50ms each)
 * - Loop running: 1 quick blink every 2 seconds
 */

#include <Wire.h>

// Pin definitions
#define LED_PWM_PIN     3
#define BTN_FREQ_PIN    2
#define BTN_MODE_PIN    4
#define PCF8574_ADDRESS 0x20

// Variables
volatile uint8_t currentFreq = 0;
volatile uint8_t currentMode = 0;
uint8_t loopCounter = 0;

void setup() {
  // Setup start indicator: 1 long blink
  pinMode(LED_PWM_PIN, OUTPUT);
  digitalWrite(LED_PWM_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PWM_PIN, LOW);
  delay(500);
  
  // Configure pins
  pinMode(BTN_FREQ_PIN, INPUT_PULLUP);
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);
  
  // Initialize I2C
  Wire.begin();
  Wire.setClock(100000);
  delay(10);
  
  // Test I2C communication
  Wire.beginTransmission(PCF8574_ADDRESS);
  uint8_t error = Wire.endTransmission();
  
  if (error == 0) {
    // I2C OK: 2 short blinks
    for (uint8_t i = 0; i < 2; i++) {
      digitalWrite(LED_PWM_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PWM_PIN, LOW);
      delay(100);
    }
    
    // Set initial LED pattern (frequency A = red)
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0b11111000);  // Red LED
    Wire.endTransmission();
  } else {
    // I2C FAIL: 5 fast blinks
    for (uint8_t i = 0; i < 5; i++) {
      digitalWrite(LED_PWM_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PWM_PIN, LOW);
      delay(50);
    }
  }
  
  // Setup Timer2 PWM
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= (1 << WGM21) | (1 << WGM20);  // Fast PWM
  TCCR2A |= (1 << COM2B1);                 // Non-inverting
  #if F_CPU == 16000000L
    TCCR2B |= (1 << CS22);                 // Prescaler 64
  #elif F_CPU == 8000000L
    TCCR2B |= (1 << CS21) | (1 << CS20);   // Prescaler 32
  #else
    TCCR2B |= (1 << CS22);                 // Default 64
  #endif
  OCR2B = 0;
  
  delay(1000);
}

void loop() {
  static uint32_t lastBlink = 0;
  static uint32_t lastButtonCheck = 0;
  static bool lastBtnFreq = HIGH;
  static bool lastBtnMode = HIGH;
  
  uint32_t now = millis();
  
  // Heartbeat: blink every 2 seconds to show loop is running
  if (now - lastBlink > 2000) {
    lastBlink = now;
    loopCounter++;
    
    // Quick blink
    OCR2B = 255;
    delay(50);
    OCR2B = 0;
  }
  
  // Check buttons every 100ms
  if (now - lastButtonCheck > 100) {
    lastButtonCheck = now;
    
    bool btnFreq = digitalRead(BTN_FREQ_PIN);
    bool btnMode = digitalRead(BTN_MODE_PIN);
    
    // FREQ button pressed (LOW when pressed)
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      currentFreq = (currentFreq + 1) % 7;
      
      // Update I2C LEDs
      const uint8_t freqIndicators[7] = {
        0b11111000,  // A: Red
        0b11110100,  // B: Green
        0b11110000,  // C: Yellow
        0b11111010,  // D: Blue
        0b11111001,  // E: Magenta
        0b11110101,  // F: Cyan
        0b11110000   // G: White
      };
      
      Wire.beginTransmission(PCF8574_ADDRESS);
      Wire.write(freqIndicators[currentFreq]);
      Wire.endTransmission();
      
      // Feedback: 2 quick blinks
      for (uint8_t i = 0; i < 2; i++) {
        OCR2B = 255;
        delay(80);
        OCR2B = 0;
        delay(80);
      }
      delay(200);
    }
    
    // MODE button pressed
    if (btnMode == LOW && lastBtnMode == HIGH) {
      currentMode = (currentMode + 1) % 3;
      
      // Feedback: blinks according to mode
      uint8_t blinks = currentMode + 1;
      for (uint8_t i = 0; i < blinks; i++) {
        OCR2B = 255;
        delay(100);
        OCR2B = 0;
        delay(100);
      }
      delay(200);
    }
    
    lastBtnFreq = btnFreq;
    lastBtnMode = btnMode;
  }
  
  // Simple LED control based on mode
  if (currentMode == 0) {
    OCR2B = 64;   // Low intensity (Detection)
  } else if (currentMode == 1) {
    OCR2B = 235;  // High intensity (Treatment)
  } else {
    OCR2B = 128;  // Medium intensity (White)
  }
}
