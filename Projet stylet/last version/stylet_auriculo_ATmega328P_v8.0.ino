/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v8.0
 *  Architecture ATmega328P-AU avec Écran OLED + Modulation Réimplémentée
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 *  🎉 NOUVEAUTÉS v8.0 :
 *    ✅ Écran OLED SSD1306 128×32 I2C
 *    ✅ Modulation fréquences Nogier réactivée (porteuse 976Hz)
 *    ✅ Timer de soin avec chronomètre
 *    ✅ Menu configuration via OLED
 *    ✅ Bouton MENU pour navigation
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
 *    PB0 (D8)  : Bouton MENU (pull-up interne) - NOUVEAU v8.0
 *    PD3 (D3)  : LED principale PWM (Timer2)
 *    PC4 (SDA) : I2C Data (PCF8574 + OLED)
 *    PC5 (SCL) : I2C Clock (PCF8574 + OLED)
 *    PC0 (A0)  : Mesure tension batterie (diviseur 100kΩ/33kΩ)
 * 
 *  📦 COMPOSANTS I2C :
 *    - PCF8574T @ 0x20 (LEDs RGB)
 *    - SSD1306 @ 0x3C (OLED 128×32)
 * 
 *  Auteur  : Fabrice Deconynck
 *  Date    : Février 2026
 *  Version : 8.0
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
const uint8_t PIN_BTN_MENU = 8;     // NOUVEAU v8.0 (PB0/D8)
const uint8_t PIN_BATTERY  = A0;    // ADC0 (PC0/A0)

// ═══ I2C Addresses ═══
const uint8_t PCF8574_ADDR = 0x20;  // PCF8574 pour LEDs RGB

// ═══ Constantes physiques ═══
const float VREF = 3.3;             // Tension de référence ADC
const float R1 = 100000.0;          // Résistance diviseur haut (100kΩ)
const float R2 = 33000.0;           // Résistance diviseur bas (33kΩ)
const float VBAT_MAX = 4.2;         // Tension batterie pleine
const float VBAT_MIN = 3.0;         // Tension batterie vide

// ═══ Configuration PWM ═══
const uint16_t PWM_FREQUENCY = 976; // Hz (porteuse)
const uint8_t PWM_MAX = 255;

// ═══ Fréquences de Nogier ═══
const float frequencies[7] = {2.28, 4.56, 9.12, 18.25, 36.50, 73.00, 146.00};
const char* freqNames[7] = {"A", "B", "C", "D", "E", "F", "G"};

// ═══ Configuration LEDs RGB (PCF8574) ═══
const uint8_t LED_RED   = 0b00000001; // P0
const uint8_t LED_GREEN = 0b00000010; // P1
const uint8_t LED_BLUE  = 0b00000100; // P2

// Codage couleur par fréquence
const uint8_t freqColors[7] = {
  LED_RED,                    // A (2.28 Hz)  : Rouge
  LED_GREEN,                  // B (4.56 Hz)  : Vert
  LED_RED | LED_GREEN,        // C (9.12 Hz)  : Jaune
  LED_BLUE,                   // D (18.25 Hz) : Bleu
  LED_RED | LED_BLUE,         // E (36.50 Hz) : Magenta
  LED_GREEN | LED_BLUE,       // F (73.00 Hz) : Cyan
  LED_RED | LED_GREEN | LED_BLUE // G (146.00 Hz): Blanc
};

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

// ⭐ NOUVEAU v8.0 : Modulation
bool modulationEnabled = true;          // Modulation ON/OFF
const float MODULATION_DEPTH = 0.70;    // Profondeur 70%

// ⭐ NOUVEAU v8.0 : Menu OLED
enum MenuState {
  MENU_MAIN_DISPLAY = 0,
  MENU_CONFIG,
  MENU_TIMER_SET,
  MENU_COUNT
};
MenuState menuState = MENU_MAIN_DISPLAY;
uint8_t menuSelection = 0;

// ⭐ NOUVEAU v8.0 : Timer
bool timerEnabled = false;
uint16_t timerSeconds = 0;              // Secondes restantes
uint16_t timerSetMinutes = 10;          // Durée configurée (minutes)
unsigned long timerLastUpdate = 0;

// Gestion écran
unsigned long lastInteraction = 0;
bool displayOn = true;
const uint16_t DISPLAY_TIMEOUT = 10000; // 10s avant extinction

// Anti-rebond boutons
unsigned long lastDebounceTime = 0;
const uint16_t DEBOUNCE_DELAY = 50;

// Mesure batterie
float batteryVoltage = 4.2;
uint8_t batteryPercent = 100;
unsigned long lastBatteryCheck = 0;
const uint16_t BATTERY_CHECK_INTERVAL = 5000;

// EEPROM addresses
const uint8_t EEPROM_ADDR_MODULATION = 0;
const uint8_t EEPROM_ADDR_TIMER_MINUTES = 1;

// ═══════════════════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  // Initialisation série (debug)
  Serial.begin(115200);
  Serial.println(F("\n╔═══════════════════════════════════════╗"));
  Serial.println(F("║  Stylet Auriculothérapie v8.0        ║"));
  Serial.println(F("║  OLED + Modulation Réactivée         ║"));
  Serial.println(F("╚═══════════════════════════════════════╝\n"));

  // Configuration des pins
  pinMode(PIN_LED_MAIN, OUTPUT);
  pinMode(PIN_BTN_FREQ, INPUT_PULLUP);
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_MENU, INPUT_PULLUP);  // NOUVEAU v8.0
  pinMode(PIN_BATTERY, INPUT);

  // Configuration PWM Timer2 pour LED principale
  setupPWM();

  // Initialisation I2C
  Wire.begin();
  Wire.setClock(100000);  // 100kHz (safe pour PCF8574 + OLED)

  // Initialisation PCF8574
  initPCF8574();

  // ⭐ Initialisation OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("❌ OLED non détecté !"));
    // Continuer sans OLED si échec
  } else {
    Serial.println(F("✅ OLED initialisé"));
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
  updateLEDs();
  updateDisplay();

  Serial.println(F("✅ Système initialisé\n"));
  Serial.print(F("Fréquence: ")); Serial.print(freqNames[currentFreq]);
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

  // ⭐ Calcul PWM avec modulation
  if (currentMode != MODE_WHITE && modulationEnabled) {
    // Modulation active (sauf en mode BLANC)
    if (millis() - lastModulationUpdate >= 1) {  // Update 1ms
      lastModulationUpdate = millis();
      uint8_t pwmValue = calculateModulatedPWM();
      analogWrite(PIN_LED_MAIN, pwmValue);
    }
  } else {
    // Pas de modulation : duty cycle fixe
    analogWrite(PIN_LED_MAIN, modeDutyCycles[currentMode]);
  }

  // Mesure batterie périodique
  if (millis() - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
    lastBatteryCheck = millis();
    updateBatteryStatus();
  }

  // ⭐ Gestion timer
  if (timerEnabled) {
    updateTimer();
  }

  // ⭐ Extinction auto écran
  if (displayOn && (millis() - lastInteraction > DISPLAY_TIMEOUT)) {
    displayOn = false;
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    Serial.println(F("💤 OLED éteint (économie énergie)"));
  }

  // Mise à jour affichage si besoin
  static unsigned long lastDisplayUpdate = 0;
  if (displayOn && (millis() - lastDisplayUpdate > 500)) {
    lastDisplayUpdate = millis();
    updateDisplay();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  ⭐ NOUVEAU v8.0 : CALCUL PWM AVEC MODULATION
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
  // Fréquence = 8MHz / (32 × 256) ≈ 976 Hz
  
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  // Fast PWM, Clear OC2B
  TCCR2B = _BV(CS21) | _BV(CS20);                  // Prescaler /32
  OCR2B = 0;                                       // Duty cycle initial 0%
  
  Serial.println(F("✅ PWM Timer2 configuré (976 Hz)"));
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION I2C
// ═══════════════════════════════════════════════════════════════════════════

void initPCF8574() {
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(0x00);  // Toutes LEDs éteintes
  if (Wire.endTransmission() == 0) {
    Serial.println(F("✅ PCF8574 détecté"));
  } else {
    Serial.println(F("❌ PCF8574 non détecté !"));
  }
}

void updateLEDs() {
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(freqColors[currentFreq]);
  Wire.endTransmission();
}

// ═══════════════════════════════════════════════════════════════════════════
//  ⭐ NOUVEAU v8.0 : AFFICHAGE OLED
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
  }
  
  display.display();
}

void drawMainScreen() {
  // Ligne 1 : Statut + Batterie
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("ON | Bat: "));
  display.print(batteryPercent);
  display.println(F("%"));
  
  // Ligne 2 : Mode
  display.setCursor(0, 10);
  display.print(F("Mode: "));
  display.println(modeNames[currentMode]);
  
  // Ligne 3 : Fréquence + Modulation
  display.setCursor(0, 20);
  display.print(F("Freq: "));
  display.print(freqNames[currentFreq]);
  display.print(F(" "));
  display.print(frequencies[currentFreq], 2);
  display.print(F("Hz"));
  
  if (modulationEnabled && currentMode != MODE_WHITE) {
    display.print(F(" [MOD]"));
  }
  
  // Ligne 4 : Timer si actif
  if (timerEnabled && timerSeconds > 0) {
    display.setCursor(80, 20);
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
  display.println(F("=== MENU ==="));
  
  // Option 1 : Modulation
  display.setCursor(0, 12);
  if (menuSelection == 0) display.print(F(">"));
  else display.print(F(" "));
  display.print(F(" Modulation: "));
  display.println(modulationEnabled ? F("ON") : F("OFF"));
  
  // Option 2 : Timer
  display.setCursor(0, 22);
  if (menuSelection == 1) display.print(F(">"));
  else display.print(F(" "));
  display.print(F(" Timer: "));
  display.print(timerSetMinutes);
  display.println(F(" min"));
}

void drawTimerSetScreen() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Timer Configuration"));
  
  display.setCursor(0, 12);
  display.print(F("Duree: "));
  display.print(timerSetMinutes);
  display.println(F(" min"));
  
  display.setCursor(0, 24);
  display.println(F("FREQ:+1  MODE:Start"));
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION BOUTONS
// ═══════════════════════════════════════════════════════════════════════════

void handleButtons() {
  unsigned long currentTime = millis();
  
  // Anti-rebond global
  if (currentTime - lastDebounceTime < DEBOUNCE_DELAY) {
    return;
  }
  
  // ⭐ Bouton FREQ (interruption)
  if (btnFreqPressed) {
    btnFreqPressed = false;
    lastDebounceTime = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    if (menuState == MENU_MAIN_DISPLAY) {
      // Mode normal : change fréquence
      currentFreq = (currentFreq + 1) % 7;
      updateLEDs();
      Serial.print(F("Fréquence: ")); Serial.println(freqNames[currentFreq]);
    } else if (menuState == MENU_CONFIG) {
      // Menu config : toggle modulation si sélectionné
      if (menuSelection == 0) {
        modulationEnabled = !modulationEnabled;
        saveConfig();
        Serial.print(F("Modulation: ")); Serial.println(modulationEnabled ? F("ON") : F("OFF"));
      }
    } else if (menuState == MENU_TIMER_SET) {
      // Timer set : +1 minute
      timerSetMinutes++;
      if (timerSetMinutes > 60) timerSetMinutes = 1;
      saveConfig();
    }
    updateDisplay();
  }
  
  // Bouton MODE (polling)
  if (digitalRead(PIN_BTN_MODE) == LOW && !btnModePressed) {
    btnModePressed = true;
    lastDebounceTime = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    if (menuState == MENU_MAIN_DISPLAY) {
      // Mode normal : change mode
      currentMode = (currentMode + 1) % MODE_COUNT;
      Serial.print(F("Mode: ")); Serial.println(modeNames[currentMode]);
    } else if (menuState == MENU_CONFIG) {
      // Menu config : change sélection
      menuSelection = (menuSelection + 1) % 2;
    } else if (menuState == MENU_TIMER_SET) {
      // Timer set : START timer
      timerSeconds = timerSetMinutes * 60;
      timerEnabled = true;
      timerLastUpdate = millis();
      menuState = MENU_MAIN_DISPLAY;
      Serial.print(F("Timer démarré: ")); Serial.print(timerSetMinutes); Serial.println(F(" min"));
    }
    updateDisplay();
  }
  if (digitalRead(PIN_BTN_MODE) == HIGH) {
    btnModePressed = false;
  }
  
  // ⭐ Bouton MENU (polling) - NOUVEAU v8.0
  if (digitalRead(PIN_BTN_MENU) == LOW && !btnMenuPressed) {
    btnMenuPressed = true;
    lastDebounceTime = currentTime;
    lastInteraction = currentTime;
    wakeDisplay();
    
    // Cycle entre états menu
    if (menuState == MENU_MAIN_DISPLAY) {
      menuState = MENU_CONFIG;
      menuSelection = 0;
      Serial.println(F("📋 Menu config ouvert"));
    } else if (menuState == MENU_CONFIG) {
      if (menuSelection == 1) {
        // Si Timer sélectionné : ouvrir config timer
        menuState = MENU_TIMER_SET;
        Serial.println(F("⏱️ Configuration timer"));
      } else {
        // Sinon : retour
        menuState = MENU_MAIN_DISPLAY;
        Serial.println(F("🔙 Retour écran principal"));
      }
    } else if (menuState == MENU_TIMER_SET) {
      // Annuler config timer
      menuState = MENU_CONFIG;
      Serial.println(F("🔙 Retour menu config"));
    }
    updateDisplay();
  }
  if (digitalRead(PIN_BTN_MENU) == HIGH) {
    btnMenuPressed = false;
  }
}

// ISR bouton FREQ
void isrBtnFreq() {
  btnFreqPressed = true;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ⭐ NOUVEAU v8.0 : GESTION TIMER
// ═══════════════════════════════════════════════════════════════════════════

void updateTimer() {
  if (millis() - timerLastUpdate >= 1000) {
    timerLastUpdate = millis();
    
    if (timerSeconds > 0) {
      timerSeconds--;
      
      if (timerSeconds == 0) {
        // Timer terminé : alarme
        Serial.println(F("⏰ TIMER TERMINÉ !"));
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
  
  // Conversion en tension
  float vMeasured = (adcValue / 1023.0) * VREF;
  
  // Tension batterie réelle (diviseur résistif)
  batteryVoltage = vMeasured * ((R1 + R2) / R2);
  
  // Calcul pourcentage (linéaire entre VBAT_MIN et VBAT_MAX)
  batteryPercent = (uint8_t)(((batteryVoltage - VBAT_MIN) / (VBAT_MAX - VBAT_MIN)) * 100.0);
  
  // Saturation
  if (batteryPercent > 100) batteryPercent = 100;
  if (batteryPercent < 0) batteryPercent = 0;
  
  // Debug série
  if (batteryPercent < 20) {
    Serial.print(F("⚠️ Batterie faible: "));
    Serial.print(batteryPercent);
    Serial.println(F("%"));
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  ⭐ NOUVEAU v8.0 : GESTION ÉCRAN
// ═══════════════════════════════════════════════════════════════════════════

void wakeDisplay() {
  if (!displayOn) {
    displayOn = true;
    display.ssd1306_command(SSD1306_DISPLAYON);
    Serial.println(F("💡 OLED rallumé"));
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  SAUVEGARDE/CHARGEMENT EEPROM
// ═══════════════════════════════════════════════════════════════════════════

void saveConfig() {
  EEPROM.write(EEPROM_ADDR_MODULATION, modulationEnabled ? 1 : 0);
  EEPROM.write(EEPROM_ADDR_TIMER_MINUTES, timerSetMinutes);
  Serial.println(F("💾 Configuration sauvegardée"));
}

void loadConfig() {
  uint8_t modValue = EEPROM.read(EEPROM_ADDR_MODULATION);
  if (modValue <= 1) {
    modulationEnabled = (modValue == 1);
  }
  
  uint8_t timerValue = EEPROM.read(EEPROM_ADDR_TIMER_MINUTES);
  if (timerValue > 0 && timerValue <= 60) {
    timerSetMinutes = timerValue;
  }
  
  Serial.println(F("📂 Configuration chargée"));
}
