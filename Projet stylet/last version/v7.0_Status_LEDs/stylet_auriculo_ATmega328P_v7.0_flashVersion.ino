/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v7.0 - FLASH VERSION (PCBA standalone)
 *  ATmega328P-AU avec LED NSPW500CS
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * VERSION : Flash PCBA - Programmation via USBasp sur ISP 6-pin
 * 
 * DESCRIPTION :
 * Code optimisé pour ATmega328P-AU standalone sur PCBA avec :
 *   - Cristal externe 8MHz
 *   - LDO MCP1700T 3.3V (VREF = 3.3V)
 *   - Batterie Li-Ion 3.7V (VBAT_MAX = 4.2V)
 *   - Debug Serial désactivé (pas d'UART sur PCBA)
 * 
 * DIFFÉRENCES vs version Breadboard :
 *   - VREF = 3.3V (au lieu de 5V Arduino Uno)
 *   - Calcul batterie adapté pour VREF 3.3V
 *   - Tous les Serial.print() supprimés (pas juste commentés)
 *   - Variable debug supprimée
 * 
 * ARCHITECTURE MATÉRIELLE (PCBA) :
 * - Microcontrôleur : ATmega328P-AU (TQFP-32) @ 8MHz cristal externe
 * - Alimentation : LDO MCP1700T 3.3V depuis Li-Ion 3.7V
 * - LED thérapeutique : Nichia NSPW500CS (CRI >90, 20mA, 5mm)
 * - Driver LED : Résistance série 47Ω
 * - Batterie : Li-Ion 3.7V (4.2V max, 3.0V min)
 * - LED indicateurs : Via PCF8574 I2C (RGB pour fréquence)
 * - Programmation : ISP 6-pin (USBasp)
 * 
 * FONCTIONNALITÉS :
 * ✓ 7 fréquences de Nogier (A=2.28Hz à G=146Hz)
 * ✓ 3 modes : Détection / Traitement / Lumière blanche
 * ✓ Modulation porteuse 976Hz pour confort visuel
 * ✓ Profondeur modulation 70%
 * ✓ Mesure batterie Li-Ion avec protection
 * ✓ Sélection fréquence/mode via boutons
 * 
 * PINS UTILISÉES (ATmega328P-AU TQFP-32) :
 * - PD3 (pin 1)  : PWM LED principale (Timer2, OC2B)
 * - PD2 (pin 32) : Bouton FREQ (INPUT_PULLUP, INT0)
 * - PD4 (pin 2)  : Bouton MODE (INPUT_PULLUP)
 * - PC4 (pin 27) : I2C SDA (PCF8574)
 * - PC5 (pin 28) : I2C SCL (PCF8574)
 * - PC0 (pin 23) : ADC0 Mesure batterie
 * - PB3 (pin 15) : MOSI  (ISP - ne pas utiliser en fonctionnement)
 * - PB4 (pin 16) : MISO  (ISP)
 * - PB5 (pin 17) : SCK   (ISP)
 * - PC6 (pin 29) : RESET (ISP)
 * 
 * FUSES RECOMMANDÉS (8MHz cristal externe, BOD 2.7V) :
 *   lfuse = 0xFF  (cristal externe full swing)
 *   hfuse = 0xD9  (no bootloader, EESAVE off)
 *   efuse = 0xFD  (BOD 2.7V)
 * 
 * COMMANDE AVRDUDE :
 *   avrdude -c usbasp -p m328p -U flash:w:stylet_auriculo_ATmega328P_v7.0_flashVersion.hex:i
 *   avrdude -c usbasp -p m328p -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xFD:m
 * 
 * AUTEUR : Fabrice Deconynck
 * VERSION : 7.0 Flash PCBA
 * DATE : Mars 2026
 * LICENCE : MIT
 * ═══════════════════════════════════════════════════════════════════════════
 */

// ═══════════════════════════════════════════════════════════════════════════
//  LIBRAIRIES
// ═══════════════════════════════════════════════════════════════════════════

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION PINS
// ═══════════════════════════════════════════════════════════════════════════

#define LED_PWM_PIN     3       // D3 (PD3) - PWM LED NSPW500CS (OC2B Timer2)
#define BTN_FREQ_PIN    2       // D2 (PD2) - Bouton fréquence (INT0)
#define BTN_MODE_PIN    4       // D4 (PD4) - Bouton mode
#define ADC_BATTERY_PIN A0      // A0 (PC0) - ADC0 Mesure batterie

// ═══════════════════════════════════════════════════════════════════════════
//  ADRESSE I2C PCF8574 (LED Indicateurs)
// ═══════════════════════════════════════════════════════════════════════════

#define PCF8574_ADDRESS 0x20    // A0=A1=A2=GND

// ═══════════════════════════════════════════════════════════════════════════
//  FRÉQUENCES DE NOGIER (Hz)
// ═══════════════════════════════════════════════════════════════════════════

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
const uint8_t numFreqs = 7;

// ═══════════════════════════════════════════════════════════════════════════
//  MODES DE FONCTIONNEMENT
// ═══════════════════════════════════════════════════════════════════════════

enum Mode {
  MODE_DETECTION,      // Basse intensité (~5mA)
  MODE_TRAITEMENT,     // Haute intensité (~20mA)
  MODE_BLANC           // Lumière continue (~10mA)
};

const char* modeNames[] = {"DETECTION", "TRAITEMENT", "BLANC"};
const uint8_t numModes = 3;

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION MODULATION
// ═══════════════════════════════════════════════════════════════════════════

#define CARRIER_FREQ        976.0     // Porteuse Timer2 (Hz)
#define MODULATION_DEPTH    0.70      // Profondeur 70%
#define MIN_INTENSITY       0.20      // Minimum 20%

// ═══════════════════════════════════════════════════════════════════════════
//  PARAMÈTRES LED NSPW500CS
// ═══════════════════════════════════════════════════════════════════════════

#define DUTY_DETECTION   64    // 25% → ~5mA
#define DUTY_TRAITEMENT  235   // 92% → ~20mA
#define DUTY_BLANC       128   // 50% → ~10mA

// ═══════════════════════════════════════════════════════════════════════════
//  PARAMÈTRES BATTERIE Li-Ion (PCBA avec LDO 3.3V)
// ═══════════════════════════════════════════════════════════════════════════

// VREF = AVCC = 3.3V (régulé par MCP1700T)
// Diviseur tension : R1=100kΩ (haut), R2=33kΩ (bas)
// Facteur = (R1+R2)/R2 = 133/33 = 4.03
// V_bat = ADC × (3300mV / 1023) × 4.03

#define VREF_MV          3300   // Tension référence ADC en mV (3.3V LDO)
#define VBAT_MAX         4200   // Li-Ion pleine charge (mV)
#define VBAT_WARNING     3500   // Seuil avertissement (mV)
#define VBAT_CUTOFF      3000   // Seuil coupure sécurité (mV)

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEURS LED RGB (PCF8574)
// ═══════════════════════════════════════════════════════════════════════════

const uint8_t freqIndicators[7] = {
  0b11111000,  // A (2.28Hz)  : Rouge
  0b11110100,  // B (4.56Hz)  : Vert
  0b11110000,  // C (9.12Hz)  : Jaune
  0b11111010,  // D (18.25Hz) : Bleu
  0b11111001,  // E (36.50Hz) : Magenta
  0b11110101,  // F (73Hz)    : Cyan
  0b11110000   // G (146Hz)   : Blanc
};

// ═══════════════════════════════════════════════════════════════════════════
//  VARIABLES GLOBALES
// ═══════════════════════════════════════════════════════════════════════════

volatile uint8_t currentFreq = 0;
volatile Mode currentMode = MODE_DETECTION;
volatile uint16_t batteryVoltage = 0;

// ═══════════════════════════════════════════════════════════════════════════
//  SETUP - INITIALISATION
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  
  // ═══ CONFIGURATION PINS ═══
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BTN_FREQ_PIN, INPUT_PULLUP);
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);
  pinMode(ADC_BATTERY_PIN, INPUT);
  
  // ═══ INITIALISATION I2C (LED Indicateurs) ═══
  Wire.begin();
  Wire.setClock(100000);
  delay(10);
  
  // Test communication PCF8574
  if (!testPCF8574()) {
    for (uint8_t i = 0; i < 5; i++) {
      digitalWrite(LED_PWM_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PWM_PIN, LOW);
      delay(100);
    }
  }
  
  // ═══ CONFIGURATION TIMER2 pour PWM sur D3 ═══
  setupTimer2PWM();
  
  // ═══ CONFIGURATION ADC pour mesure batterie ═══
  setupADC();
  
  // ═══ AFFICHAGE INITIAL ═══
  updateFreqIndicators();
  
  // ═══ LECTURE BATTERIE INITIALE ═══
  batteryVoltage = readBatteryVoltage();
  
  // Boot OK : 3 flashs rapides
  for (uint8_t i = 0; i < 3; i++) {
    OCR2B = 255;
    delay(50);
    OCR2B = 0;
    delay(50);
  }
  
  // ═══ ACTIVATION INTERRUPTIONS ═══
  sei();
}

// ═══════════════════════════════════════════════════════════════════════════
//  TIMER2 PWM SETUP - PWM 976Hz sur D3 (OC2B)
// ═══════════════════════════════════════════════════════════════════════════

void setupTimer2PWM() {
  // Timer2 Fast PWM 8-bit
  // F_PWM = F_CPU / (Prescaler × 256)
  // 8MHz : 8000000 / (32 × 256) = 976Hz
  
  TCCR2A = 0;
  TCCR2B = 0;
  
  // Mode Fast PWM
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  
  // Clear OC2B on compare match (non-inverting)
  TCCR2A |= (1 << COM2B1);
  
  // Prescaler adapté à F_CPU
  #if F_CPU == 16000000L
    TCCR2B |= (1 << CS22);                  // Prescaler 64
  #elif F_CPU == 8000000L
    TCCR2B |= (1 << CS21) | (1 << CS20);    // Prescaler 32
  #else
    TCCR2B |= (1 << CS22);                  // Défaut 64
  #endif
  
  OCR2B = 0;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ADC SETUP - Mesure batterie
// ═══════════════════════════════════════════════════════════════════════════

void setupADC() {
  // Référence AVCC (3.3V sur PCBA), canal ADC0 (A0)
  
  ADMUX = 0;
  ADMUX |= (1 << REFS0);  // AVCC reference (3.3V)
  
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
  
  // Warm-up ADC
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
}

// ═══════════════════════════════════════════════════════════════════════════
//  LOOP PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  
  static uint32_t lastUpdate = 0;
  static uint32_t lastBatteryCheck = 0;
  
  uint32_t now = millis();
  
  // ═══ GESTION BOUTONS ═══
  handleButtons();
  
  // ═══ GÉNÉRATION MODULATION ═══
  if (currentMode != MODE_BLANC) {
    generateModulation();
  } else {
    OCR2B = DUTY_BLANC;
  }
  
  // ═══ MESURE BATTERIE (Toutes les 5 secondes) ═══
  if (now - lastBatteryCheck > 5000) {
    lastBatteryCheck = now;
    batteryVoltage = readBatteryVoltage();
    
    // Avertissement batterie faible
    if (batteryVoltage < VBAT_WARNING) {
      blinkBatteryWarning();
    }
    
    // Sécurité : Couper traitement si batterie < 3.0V
    if (batteryVoltage < VBAT_CUTOFF && currentMode == MODE_TRAITEMENT) {
      currentMode = MODE_DETECTION;
      updateFreqIndicators();
    }
  }
  
  // ═══ MISE À JOUR INDICATEURS (Toutes les 100ms) ═══
  if (now - lastUpdate > 100) {
    lastUpdate = now;
    updateFreqIndicators();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  GÉNÉRATION MODULATION
// ═══════════════════════════════════════════════════════════════════════════

void generateModulation() {
  
  uint32_t currentMicros = micros();
  
  float nogierFreq = frequencies[currentFreq];
  float nogierPeriod = 1000000.0 / nogierFreq;
  float nogierPhase = (float)(currentMicros % (uint32_t)nogierPeriod) / nogierPeriod * 2.0 * PI;
  
  float envelope = 0.5 + 0.5 * sin(nogierPhase);
  
  float modulatedIntensity = MIN_INTENSITY + MODULATION_DEPTH * envelope;
  
  if (modulatedIntensity > 1.0) modulatedIntensity = 1.0;
  if (modulatedIntensity < 0.0) modulatedIntensity = 0.0;
  
  uint8_t baseDuty = 0;
  
  switch (currentMode) {
    case MODE_DETECTION:
      baseDuty = DUTY_DETECTION;
      break;
    case MODE_TRAITEMENT:
      baseDuty = DUTY_TRAITEMENT;
      break;
    case MODE_BLANC:
      baseDuty = DUTY_BLANC;
      return;
  }
  
  uint8_t finalDuty = (uint8_t)(baseDuty * modulatedIntensity);
  OCR2B = finalDuty;
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION BOUTONS
// ═══════════════════════════════════════════════════════════════════════════

void handleButtons() {
  
  static bool lastBtnFreq = HIGH;
  static bool lastBtnMode = HIGH;
  static uint32_t lastDebounceFreq = 0;
  static uint32_t lastDebounceMode = 0;
  
  const uint16_t debounceDelay = 50;
  
  // ═══ BOUTON FRÉQUENCE ═══
  bool btnFreq = digitalRead(BTN_FREQ_PIN);
  
  if (btnFreq != lastBtnFreq) {
    lastDebounceFreq = millis();
  }
  
  if ((millis() - lastDebounceFreq) > debounceDelay) {
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      currentFreq = (currentFreq + 1) % numFreqs;
      updateFreqIndicators();
      flashFeedback(2, 80);
      delay(200);
    }
  }
  lastBtnFreq = btnFreq;
  
  // ═══ BOUTON MODE ═══
  bool btnMode = digitalRead(BTN_MODE_PIN);
  
  if (btnMode != lastBtnMode) {
    lastDebounceMode = millis();
  }
  
  if ((millis() - lastDebounceMode) > debounceDelay) {
    if (btnMode == LOW && lastBtnMode == HIGH) {
      currentMode = (Mode)((currentMode + 1) % numModes);
      updateFreqIndicators();
      
      switch (currentMode) {
        case MODE_DETECTION:
          flashFeedback(1, 200);
          break;
        case MODE_TRAITEMENT:
          flashFeedback(2, 100);
          break;
        case MODE_BLANC:
          flashFeedback(3, 60);
          break;
      }
      
      delay(200);
    }
  }
  lastBtnMode = btnMode;
}

// ═══════════════════════════════════════════════════════════════════════════
//  MISE À JOUR INDICATEURS LED RGB (PCF8574)
// ═══════════════════════════════════════════════════════════════════════════

void updateFreqIndicators() {
  
  uint8_t ledPattern = freqIndicators[currentFreq];
  
  Wire.beginTransmission(PCF8574_ADDRESS);
  Wire.write(ledPattern);
  Wire.endTransmission();
}

// ═══════════════════════════════════════════════════════════════════════════
//  TEST PCF8574
// ═══════════════════════════════════════════════════════════════════════════

bool testPCF8574() {
  
  Wire.beginTransmission(PCF8574_ADDRESS);
  uint8_t error = Wire.endTransmission();
  
  return (error == 0);
}

// ═══════════════════════════════════════════════════════════════════════════
//  LECTURE TENSION BATTERIE (VREF = 3.3V PCBA)
// ═══════════════════════════════════════════════════════════════════════════

uint16_t readBatteryVoltage() {
  
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  
  uint16_t adcValue = ADC;
  
  // Conversion en mV
  // VREF = 3.3V (AVCC régulé par MCP1700T)
  // Diviseur 100kΩ/33kΩ → facteur 4.03
  // V_bat = ADC × (3300mV / 1023) × 4.03
  uint16_t voltage = (uint32_t)adcValue * VREF_MV * 403 / 102300;
  
  return voltage;
}

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEUR BATTERIE FAIBLE
// ═══════════════════════════════════════════════════════════════════════════

void blinkBatteryWarning() {
  
  for (uint8_t i = 0; i < 3; i++) {
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0b11111110);  // Rouge seulement
    Wire.endTransmission();
    delay(100);
    
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0b11111111);  // Éteint
    Wire.endTransmission();
    delay(100);
  }
  
  updateFreqIndicators();
}

// ═══════════════════════════════════════════════════════════════════════════
//  FEEDBACK VISUEL
// ═══════════════════════════════════════════════════════════════════════════

void flashFeedback(uint8_t times, uint16_t delayMs) {
  
  uint8_t savedDuty = OCR2B;
  
  for (uint8_t i = 0; i < times; i++) {
    OCR2B = 255;
    delay(delayMs);
    OCR2B = 0;
    delay(delayMs);
  }
  
  OCR2B = savedDuty;
}

// ═══════════════════════════════════════════════════════════════════════════
//  MODE VEILLE
// ═══════════════════════════════════════════════════════════════════════════

void enterSleepMode() {
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  
  ADCSRA &= ~(1 << ADEN);
  power_all_disable();
  
  sei();
  sleep_cpu();
  
  sleep_disable();
  power_all_enable();
  ADCSRA |= (1 << ADEN);
}

// ═══════════════════════════════════════════════════════════════════════════
//  FIN DU CODE v7.0 Flash PCBA
// ═══════════════════════════════════════════════════════════════════════════
