/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v8.0 - FLASH VERSION (ISP)
 *  Architecture ATmega328P-AU @ 8MHz / 3.3V avec Écran OLED
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 *  🔧 VERSION FLASH : Adaptée pour ATmega328P-AU standalone via ISP
 *    - Cristal 8MHz externe (Y1)
 *    - Alimentation 3.3V (LDO MCP1700T depuis batterie Li-Ion)
 *    - ADC VREF = 3.3V (VCC)
 *    - Serial 57600 baud (fiable à 8MHz)
 *    - Diagnostics breadboard retirés
 * 
 *  📋 FRÉQUENCES DE NOGIER (1956) :
 *    A : 2.28 Hz  │ B : 4.56 Hz  │ C : 9.12 Hz  │ D : 18.25 Hz
 *    E : 36.50 Hz │ F : 73.00 Hz │ G : 146.00 Hz
 * 
 *  🎨 MODES :
 *    - DÉTECTION  : LED faible (5mA, 25% duty) + modulation optionnelle
 *    - TRAITEMENT : LED forte (20mA, 92% duty) + modulation optionnelle
 *    - BLANC      : LED moyenne (10mA, 50% duty) sans modulation
 * 
 *  🔌 CONNEXIONS :
 *    PD2 (D2)  : Bouton FREQ (INT0, pull-up interne)
 *    PD4 (D4)  : Bouton MODE (pull-up interne)
 *    PB0 (D8)  : Bouton MENU (pull-up interne)
 *    PD3 (D3)  : LED principale PWM (Timer2)
 *    PC4 (SDA) : I2C Data (OLED)
 *    PC5 (SCL) : I2C Clock (OLED)
 *    PC0 (A0)  : Mesure tension batterie (diviseur 100kΩ/33kΩ)
 * 
 *  📦 COMPOSANTS I2C :
 *    - OLED SSD1306 @ 0x3C (128×32)
 * 
 *  ⚡ PROGRAMMATION :
 *    - Via ISP (USBasp ou Arduino as ISP)
 *    - Fuses : 8MHz external crystal, BOD 2.7V
 * 
 *  Auteur  : Fabrice Deconynck
 *  Date    : Mars 2026
 *  Version : 8.0 (Flash)
 *  License : MIT
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// ═══ Configuration OLED ═══
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ═══ Pins ═══
const uint8_t PIN_LED_MAIN = 3;     // PWM Timer2 (PD3/D3)
const uint8_t PIN_BTN_FREQ = 2;     // INT0 (PD2/D2)
const uint8_t PIN_BTN_MODE = 4;     // (PD4/D4)
const uint8_t PIN_BTN_MENU = 8;     // (PB0/D8)
const uint8_t PIN_BATTERY  = A0;    // ADC0 (PC0/A0)

// ═══ PWM ═══
#define PWM_MAX 255                    // 8-bit PWM maximum

// ═══ Fréquences de Nogier (Hz) ═══
const float frequencies[] = {
  2.28,   // A - Fréquence cellulaire
  4.56,   // B - Fréquence nutritive
  9.12,   // C - Fréquence nerveuse
  18.25,  // D - Fréquence circulatoire
  36.50,  // E - Fréquence psychique
  73.00,  // F - Fréquence dégénérescence
  146.00  // G - Fréquence séquentielle
};
const char freqNames[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

// ═══ Mesure batterie (standalone : ATmega328P-AU @ 3.3V, batterie Li-Ion) ═══
// VREF = VCC = 3.3V (sortie LDO MCP1700T)
const float VREF = 3.3;               // Référence ADC = VCC 3.3V
const float R_HIGH = 100000.0;         // R8 = 100kΩ (diviseur haut)
const float R_LOW  = 33000.0;          // R9 = 33kΩ  (diviseur bas)
const float VBAT_MAX = 4.2;            // Tension max Li-Ion chargée
const float VBAT_MIN = 3.0;            // Tension min cutoff Li-Ion

// ═══ Modes de fonctionnement ═══
enum Mode {
  MODE_DETECTION = 0,
  MODE_TREATMENT,
  MODE_WHITE,
  MODE_COUNT
};

const char* modeNames[MODE_COUNT] = {
  "Detection",
  "Traitement",
  "Full Spectre"
};

// Duty cycles moyens par mode (pour 8-bit PWM : 0-255)
const uint8_t modeDutyCycles[MODE_COUNT] = {
  64,   // DETECTION  : 25% de 255 ≈ 64  (5mA)
  235,  // TREATMENT  : 92% de 255 ≈ 235 (20mA)
  128   // WHITE      : 50% de 255 = 128 (10mA)
};

// ═══ État du système ═══
volatile uint8_t currentFreq = 0;       // Index fréquence actuelle (0-6)
volatile uint8_t currentMode = MODE_DETECTION;
volatile bool btnFreqPressed = false;
volatile bool btnModePressed = false;
volatile bool btnMenuPressed = false;

// ⭐ Modulation
bool modulationEnabled = true;          // Modulation ON/OFF
const float MODULATION_DEPTH = 0.70;    // Profondeur 70%

// ⭐ Menu OLED
enum MenuState {
  MENU_MAIN_DISPLAY = 0,
  MENU_CONFIG,
  MENU_TIMER_SET,
  MENU_ABOUT,
  MENU_COUNT
};
MenuState menuState = MENU_MAIN_DISPLAY;
uint8_t menuSelection = 0;

// ⭐ Timer (en secondes, pas de 10s)
bool timerEnabled = false;
uint16_t timerSeconds = 0;              // Secondes restantes
uint16_t timerSetSeconds = 30;          // Durée configurée (secondes, défaut 30s)
unsigned long timerLastUpdate = 0;

// Gestion écran
unsigned long lastInteraction = 0;
bool displayOn = true;
const unsigned long DISPLAY_TIMEOUT = 20000; // 20s avant extinction

// Anti-rebond boutons (un timer par bouton)
const uint16_t DEBOUNCE_DELAY = 200;
unsigned long lastDebounceFreq = 0;
unsigned long lastDebounceMode = 0;
unsigned long lastDebounceMenu = 0;

// Mesure batterie
float batteryVoltage = 4.2;
uint8_t batteryPercent = 100;
unsigned long lastBatteryCheck = 0;
const uint16_t BATTERY_CHECK_INTERVAL = 5000;

// EEPROM addresses
const uint8_t EEPROM_ADDR_MODULATION = 0;
const uint8_t EEPROM_ADDR_TIMER_TENS = 1;   // Stocké en dizaines de secondes (3=30s, 6=60s...)

// ═══════════════════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  // Initialisation série (debug via ISP/FTDI si connecté)
  Serial.begin(57600);
  Serial.println(F("\n=== Stylet Auriculotherapie v8.0 (Flash) ==="));
  Serial.println(F("ATmega328P-AU @ 8MHz / 3.3V\n"));

  // Configuration des pins
  pinMode(PIN_LED_MAIN, OUTPUT);
  pinMode(PIN_BTN_FREQ, INPUT_PULLUP);
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_MENU, INPUT_PULLUP);
  pinMode(PIN_BATTERY, INPUT);

  // Configuration PWM Timer2 pour LED principale
  setupPWM();

  // Initialisation I2C
  Wire.begin();
  Wire.setClock(100000);  // 100kHz (safe pour OLED)

  // Initialisation OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED non detecte !"));
  } else {
    Serial.println(F("OLED initialise"));
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Splash screen
    display.setCursor(10, 8);
    display.println(F("Stylet Auriculo"));
    display.setCursor(35, 20);
    display.println(F("v8.0"));
    display.display();
    delay(2000);
  }

  // Charger configuration depuis EEPROM
  loadConfig();

  // Attachement interruptions boutons
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_FREQ), isrBtnFreq, FALLING);

  // Affichage initial
  updateDisplay();

  Serial.println(F("Systeme initialise"));
  Serial.print(F("Freq: ")); Serial.print(freqNames[currentFreq]);
  Serial.print(F(" (")); Serial.print(frequencies[currentFreq]); Serial.println(F(" Hz)"));
  Serial.print(F("Mode: ")); Serial.println(modeNames[currentMode]);
  Serial.print(F("Modulation: ")); Serial.println(modulationEnabled ? F("ON") : F("OFF"));
}

// ═══════════════════════════════════════════════════════════════════════════
//  LOOP PRINCIPAL
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  static unsigned long lastModulationUpdate = 0;
  
  // Gestion boutons
  handleButtons();

  // Calcul PWM avec modulation
  // modulationEnabled=true → "ON" à l'écran → lumière stable (lissage actif)
  // modulationEnabled=false → "OFF" à l'écran → variation visuelle des fréquences
  if (currentMode != MODE_WHITE && !modulationEnabled) {
    // Modulation OFF → on voit les fréquences varier
    if (millis() - lastModulationUpdate >= 1) {  // Update 1ms
      lastModulationUpdate = millis();
      uint8_t pwmValue = calculateModulatedPWM();
      analogWrite(PIN_LED_MAIN, pwmValue);
    }
  } else {
    // Modulation ON ou mode BLANC → duty cycle fixe (lumière stable)
    analogWrite(PIN_LED_MAIN, modeDutyCycles[currentMode]);
  }

  // Mesure batterie périodique
  if (millis() - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
    lastBatteryCheck = millis();
    updateBatteryStatus();
  }

  // Gestion timer
  if (timerEnabled) {
    updateTimer();
  }

  // Extinction auto écran
  if (displayOn && (millis() - lastInteraction > DISPLAY_TIMEOUT)) {
    displayOn = false;
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  // Mise à jour affichage si besoin
  static unsigned long lastDisplayUpdate = 0;
  if (displayOn && (millis() - lastDisplayUpdate > 500)) {
    lastDisplayUpdate = millis();
    updateDisplay();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  CALCUL PWM AVEC MODULATION
// ═══════════════════════════════════════════════════════════════════════════

uint8_t calculateModulatedPWM() {
  // Formule : PWM = duty_moyen ± (amplitude × sin(2π × freq_nogier × t))
  
  float timeSeconds = millis() / 1000.0;
  float freqNogier = frequencies[currentFreq];
  uint8_t baseDuty = modeDutyCycles[currentMode];
  
  // Amplitude de modulation (70% de la plage disponible)
  float amplitude = (PWM_MAX / 2.0) * MODULATION_DEPTH;
  
  // Calcul sinusoïdal
  float modulationFactor = sin(2.0 * PI * freqNogier * timeSeconds);
  
  // Application
  int16_t pwmValue = baseDuty + (amplitude * modulationFactor);
  
  // Saturation
  if (pwmValue < 0) pwmValue = 0;
  if (pwmValue > PWM_MAX) pwmValue = PWM_MAX;
  
  return (uint8_t)pwmValue;
}

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION PWM TIMER2
// ═══════════════════════════════════════════════════════════════════════════

void setupPWM() {
  // Timer2 : Fast PWM, non-inverting, prescaler /32
  // Fréquence = 8MHz / (32 × 256) = 976 Hz
  
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  // Fast PWM, Clear OC2B
  TCCR2B = _BV(CS21) | _BV(CS20);                  // Prescaler /32
  OCR2B = 0;                                       // Duty cycle initial 0%
}

// ═══════════════════════════════════════════════════════════════════════════
//  AFFICHAGE OLED
// ═══════════════════════════════════════════════════════════════════════════

void updateDisplay() {
  if (!displayOn) return;
  
  display.clearDisplay();
  
  switch (menuState) {
    case MENU_MAIN_DISPLAY:
      drawMainScreen();
      break;
    case MENU_CONFIG:
      drawConfigMenu();
      break;
    case MENU_TIMER_SET:
      drawTimerSetScreen();
      break;
    case MENU_ABOUT:
      drawAboutScreen();
      break;
  }
  
  display.display();
}

void drawMainScreen() {
  display.setTextSize(1);
  
  // Ligne 1 (Y=0) : Statut + Batterie
  display.setCursor(0, 0);
  display.print(F("ON | Bat: "));
  display.print(batteryPercent);
  display.print(F("%"));
  
  // Ligne 2 (Y=8) : Mode + Modulation
  display.setCursor(0, 8);
  display.print(F("Mode: "));
  display.print(modeNames[currentMode]);
  if (!modulationEnabled && currentMode != MODE_WHITE) {
    display.print(F(" [MOD]"));
  }
  
  // Ligne 3 (Y=16) : Fréquence
  display.setCursor(0, 16);
  display.print(F("Freq: "));
  display.print(freqNames[currentFreq]);
  display.print(F(" "));
  display.print(frequencies[currentFreq], 1);
  display.print(F("Hz"));
  
  // Ligne 4 (Y=24) : Timer si actif
  if (timerEnabled && timerSeconds > 0) {
    display.setCursor(0, 24);
    display.print(F("Timer: "));
    uint16_t mins = timerSeconds / 60;
    uint16_t secs = timerSeconds % 60;
    if (mins < 10) display.print(F("0"));
    display.print(mins);
    display.print(F(":"));
    if (secs < 10) display.print(F("0"));
    display.print(secs);
  }
}

void drawConfigMenu() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Option 1 : Modulation
  if (menuSelection == 0) display.print(F(">"));
  else display.print(F(" "));
  display.print(F(" Modulation: "));
  display.println(modulationEnabled ? F("ON") : F("OFF"));
  
  // Option 2 : Timer
  display.setCursor(0, 8);
  if (menuSelection == 1) display.print(F(">"));
  else display.print(F(" "));
  display.print(F(" Timer: "));
  display.print(timerSetSeconds);
  display.println(F("s"));
  
  // Option 3 : About
  display.setCursor(0, 16);
  if (menuSelection == 2) display.print(F(">"));
  else display.print(F(" "));
  display.println(F(" About"));
  
  // Option 4 : Retour
  display.setCursor(0, 24);
  if (menuSelection == 3) display.print(F(">"));
  else display.print(F(" "));
  display.print(F(" Retour"));
}

void drawTimerSetScreen() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Timer Configuration"));
  
  display.setCursor(0, 12);
  display.print(F("Duree: "));
  display.print(timerSetSeconds);
  display.println(F(" sec"));
  
  display.setCursor(0, 24);
  display.println(F("FREQ:+10s MODE:Start"));
}

void drawAboutScreen() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Stylet"));
  display.println(F("Auriculotherapie"));
  display.println(F("Version: 8.0"));
  display.println(F("Created by: Fabrice D."));
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION BOUTONS
// ═══════════════════════════════════════════════════════════════════════════

void handleButtons() {
  unsigned long currentTime = millis();
  
  // ═══ Bouton FREQ (interruption + anti-rebond propre) ═══
  if (btnFreqPressed && (currentTime - lastDebounceFreq >= DEBOUNCE_DELAY)) {
    btnFreqPressed = false;
    lastDebounceFreq = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    if (menuState == MENU_MAIN_DISPLAY) {
      currentFreq = (currentFreq + 1) % 7;
      Serial.print(F("Freq: ")); Serial.println(freqNames[currentFreq]);
    } else if (menuState == MENU_TIMER_SET) {
      timerSetSeconds += 10;
      if (timerSetSeconds > 300) timerSetSeconds = 10;  // 10s à 300s (5min)
      saveConfig();
    }
    updateDisplay();
  }
  
  // ═══ Bouton MODE (polling + anti-rebond propre) ═══
  bool modeState = (digitalRead(PIN_BTN_MODE) == LOW);
  if (modeState && !btnModePressed && (currentTime - lastDebounceMode >= DEBOUNCE_DELAY)) {
    btnModePressed = true;
    lastDebounceMode = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    if (menuState == MENU_MAIN_DISPLAY) {
      currentMode = (currentMode + 1) % MODE_COUNT;
      Serial.print(F("Mode: ")); Serial.println(modeNames[currentMode]);
    } else if (menuState == MENU_CONFIG) {
      menuSelection = (menuSelection + 1) % 4;  // 4 options: Modulation, Timer, About, Retour
    } else if (menuState == MENU_TIMER_SET) {
      timerSeconds = timerSetSeconds;
      timerEnabled = true;
      timerLastUpdate = millis();
      menuState = MENU_MAIN_DISPLAY;
      Serial.print(F("Timer: ")); Serial.print(timerSetSeconds); Serial.println(F("s"));
    }
    updateDisplay();
  }
  if (!modeState) {
    btnModePressed = false;
  }
  
  // ═══ Bouton MENU (polling + anti-rebond propre) ═══
  bool menuBtnState = (digitalRead(PIN_BTN_MENU) == LOW);
  if (menuBtnState && !btnMenuPressed && (currentTime - lastDebounceMenu >= DEBOUNCE_DELAY)) {
    btnMenuPressed = true;
    lastDebounceMenu = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    if (menuState == MENU_MAIN_DISPLAY) {
      menuState = MENU_CONFIG;
      menuSelection = 0;
    } else if (menuState == MENU_CONFIG) {
      if (menuSelection == 0) {
        // Modulation: toggle
        modulationEnabled = !modulationEnabled;
        saveConfig();
      } else if (menuSelection == 1) {
        // Timer: ouvrir config
        menuState = MENU_TIMER_SET;
      } else if (menuSelection == 2) {
        // About: afficher
        menuState = MENU_ABOUT;
      } else if (menuSelection == 3) {
        // Retour: ecran principal
        menuState = MENU_MAIN_DISPLAY;
      }
    } else if (menuState == MENU_TIMER_SET) {
      menuState = MENU_CONFIG;
    } else if (menuState == MENU_ABOUT) {
      menuState = MENU_CONFIG;
    }
    updateDisplay();
  }
  if (!menuBtnState) {
    btnMenuPressed = false;
  }
}

// ISR bouton FREQ
void isrBtnFreq() {
  btnFreqPressed = true;
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION TIMER
// ═══════════════════════════════════════════════════════════════════════════

void updateTimer() {
  if (millis() - timerLastUpdate >= 1000) {
    timerLastUpdate = millis();
    
    if (timerSeconds > 0) {
      timerSeconds--;
      
      if (timerSeconds == 0) {
        // Timer terminé : alarme
        timerEnabled = false;
        
        // Clignotement LED pour alarme
        for (uint8_t i = 0; i < 6; i++) {
          analogWrite(PIN_LED_MAIN, 255);
          delay(200);
          analogWrite(PIN_LED_MAIN, 0);
          delay(200);
        }
      }
    }
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION BATTERIE
// ═══════════════════════════════════════════════════════════════════════════

void updateBatteryStatus() {
  // Lecture ADC
  int adcValue = analogRead(PIN_BATTERY);
  
  // Conversion en tension (VREF = VCC = 3.3V)
  float vMeasured = (adcValue / 1023.0) * VREF;
  
  // Tension batterie réelle (diviseur résistif R8/R9)
  batteryVoltage = vMeasured * ((R_HIGH + R_LOW) / R_LOW);
  
  // Calcul pourcentage (linéaire entre VBAT_MIN et VBAT_MAX)
  batteryPercent = (uint8_t)(((batteryVoltage - VBAT_MIN) / (VBAT_MAX - VBAT_MIN)) * 100.0);
  
  // Saturation
  if (batteryPercent > 100) batteryPercent = 100;
  if (batteryPercent < 0) batteryPercent = 0;
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION ÉCRAN
// ═══════════════════════════════════════════════════════════════════════════

void wakeDisplay() {
  if (!displayOn) {
    displayOn = true;
    display.ssd1306_command(SSD1306_DISPLAYON);
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  SAUVEGARDE/CHARGEMENT EEPROM
// ═══════════════════════════════════════════════════════════════════════════

void saveConfig() {
  EEPROM.write(EEPROM_ADDR_MODULATION, modulationEnabled ? 1 : 0);
  EEPROM.write(EEPROM_ADDR_TIMER_TENS, timerSetSeconds / 10);  // Stocké en dizaines de sec
}

void loadConfig() {
  uint8_t modValue = EEPROM.read(EEPROM_ADDR_MODULATION);
  if (modValue <= 1) {
    modulationEnabled = (modValue == 1);
  }
  
  uint8_t timerTens = EEPROM.read(EEPROM_ADDR_TIMER_TENS);
  if (timerTens >= 1 && timerTens <= 30) {  // 10s à 300s
    timerSetSeconds = timerTens * 10;
  }
}
