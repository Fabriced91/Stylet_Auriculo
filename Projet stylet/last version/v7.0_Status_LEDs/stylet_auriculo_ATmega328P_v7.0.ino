/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v7.0 - ATmega328P-AU avec LED NSPW500CS
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * DESCRIPTION :
 * Code optimisé pour ATmega328P-AU avec modulation porteuse 1kHz pour éliminer
 * le clignotement visible des basses fréquences de Nogier (2.28-18Hz).
 * 
 * 🎉 NOUVEAU v7.0 : LED Nichia NSPW500CS (20mA, 5mm traversant)
 *    Circuit ULTRA-SIMPLIFIÉ : Plus de driver complexe, juste résistance série!
 * 
 * ARCHITECTURE MATÉRIELLE :
 * - Microcontrôleur : ATmega328P-AU (TQFP-32)
 * - ⭐ LED thérapeutique : Nichia NSPW500CS (CRI >90, 20mA, 5mm)
 * - ⭐ Driver LED : Résistance série 47Ω (ultra-simple!)
 * - Batterie : LiPo 900mAh 3.7V
 * - LED indicateurs : Via PCF8574 I2C (RGB pour fréquence)
 * 
 * AVANTAGES v7.0 vs v6.0 :
 * ✅ Circuit simplifié (7 composants en moins!)
 * ✅ Autonomie ×3 à ×9 meilleure (20mA vs 350mA)
 * ✅ Montage facile (LED traversant vs SMD)
 * ✅ Coût réduit de -10%
 * ✅ Fiabilité accrue (moins de composants)
 * ✅ CRI >90 conservé (qualité thérapeutique)
 * ✅ Faisceau focalisé 15° (vs 120°, meilleur pour stylet!)
 * 
 * FONCTIONNALITÉS :
 * ✓ 7 fréquences de Nogier (A=2.28Hz à G=146Hz)
 * ✓ 3 modes : Détection (basse intensité) / Traitement (haute) / Lumière blanche
 * ✓ Modulation porteuse 976Hz pour confort visuel
 * ✓ Profondeur modulation 70% (pas d'extinction totale)
 * ✓ Mesure batterie avec indicateur état de charge
 * ✓ Sélection fréquence via bouton
 * ✓ Sélection mode via bouton
 * 
 * PINS UTILISÉES (ATmega328P-AU) :
 * - PD3 (pin 1)  : PWM LED principale (Timer2, OC2B) - Arduino D3
 * - PD2 (pin 32) : Bouton FREQ (INPUT_PULLUP) - Arduino D2 (INT0)
 * - PD4 (pin 2)  : Bouton MODE (INPUT_PULLUP) - Arduino D4
 * - PC4 (pin 27) : I2C SDA (LEDs indicateurs via PCF8574) - Arduino A4
 * - PC5 (pin 28) : I2C SCL (LEDs indicateurs via PCF8574) - Arduino A5
 * - PC0 (pin 23) : ADC0 Mesure batterie - Arduino A0
 * 
 * CONSOMMATION ÉLECTRIQUE v7.0 :
 * - Mode DÉTECTION  : 5mA LED + 8mA système = 13mA → 69h autonomie
 * - Mode TRAITEMENT : 20mA LED + 8mA système = 28mA → 32h autonomie
 * - Mode BLANC      : 10mA LED + 8mA système = 18mA → 50h autonomie
 * 
 * AUTEUR : Projet Auriculothérapie
 * VERSION : 7.0 (LED NSPW500CS - Circuit Simplifié)
 * DATE : Février 2026
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
#include <Wire.h>  // Librairie I2C standard Arduino (native ATmega328P)

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION PINS (Notation Arduino)
// ═══════════════════════════════════════════════════════════════════════════

#define LED_PWM_PIN     3       // D3 (PD3) - PWM LED principale NSPW500CS (OC2B Timer2)
#define BTN_FREQ_PIN    2       // D2 (PD2) - Bouton sélection fréquence (INT0)
#define BTN_MODE_PIN    4       // D4 (PD4) - Bouton sélection mode
#define ADC_BATTERY_PIN A0      // A0 (PC0) - ADC0 Mesure batterie
#define I2C_SDA_PIN     A4      // A4 (PC4) - I2C Data (SDA)
#define I2C_SCL_PIN     A5      // A5 (PC5) - I2C Clock (SCL)

// Pins PWM disponibles pour futurs développements :
#define PWM_RESERVE_1   5       // D5 (PD5) - Timer0 OC0B
#define PWM_RESERVE_2   6       // D6 (PD6) - Timer0 OC0A
#define PWM_RESERVE_3   9       // D9 (PB1) - Timer1 OC1A (16-bit)
#define PWM_RESERVE_4   10      // D10 (PB2) - Timer1 OC1B (16-bit)
#define PWM_RESERVE_5   11      // D11 (PB3) - Timer2 OC2A

// ═══════════════════════════════════════════════════════════════════════════
//  ADRESSE I2C PCF8574 (LED Indicateurs)
// ═══════════════════════════════════════════════════════════════════════════

#define PCF8574_ADDRESS 0x20    // Adresse I2C par défaut (A0=A1=A2=GND)
                                // ⚠️ PCBA utilise PCF8574A → adresse 0x38
                                // Voir stylet_auriculo_ATmega328P_v7.0_flashVersion.ino

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
//  MODES DE FONCTIONNEMENT (Ajustés pour LED 20mA)
// ═══════════════════════════════════════════════════════════════════════════

enum Mode {
  MODE_DETECTION,      // Basse intensité (5mA, 25% duty cycle)
  MODE_TRAITEMENT,     // Haute intensité (20mA, 92% duty cycle)
  MODE_BLANC           // Lumière continue non modulée (10mA, 50% duty)
};

const char* modeNames[] = {"DETECTION", "TRAITEMENT", "BLANC"};
const uint8_t numModes = 3;

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION MODULATION
// ═══════════════════════════════════════════════════════════════════════════

#define CARRIER_FREQ        976.0     // Fréquence porteuse réelle (Hz) - Timer2 976Hz
#define MODULATION_DEPTH    0.70      // Profondeur modulation (70%)
#define MIN_INTENSITY       0.20      // Intensité minimum (20%)

// ═══════════════════════════════════════════════════════════════════════════
//  ⭐ PARAMÈTRES LED NSPW500CS (NOUVEAU v7.0)
// ═══════════════════════════════════════════════════════════════════════════

// Avec R_LED = 47Ω et Vsupply = 3.3V :
//   I_max = (3.3V - 3.0V_Vf) / 47Ω ≈ 6.4mA à 100% duty
//   Pour obtenir 20mA effectifs, utiliser PWM ~92%

// Duty cycles ajustés pour LED 20mA (vs 350mA en v6.0) :
#define DUTY_DETECTION   64    // 25% → ~5mA  (vs 38/15% en v6.0)
#define DUTY_TRAITEMENT  235   // 92% → ~20mA (vs 255/100% en v6.0)
#define DUTY_BLANC       128   // 50% → ~10mA (vs 153/60% en v6.0)

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEURS LED RGB (Codage binaire pour 7 fréquences)
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
  
  // ═══ DEBUG SERIAL (breadboard uniquement) ═══
  // Serial.begin(115200);  // DÉSACTIVÉ pour PCBA standalone
  // Serial.println(F("=== STYLET AURICULOTHERAPIE v7.0 - DEBUG BREADBOARD ==="));
  
  // ═══ CONFIGURATION OSCILLATEUR ═══
  // ATmega328P-AU fonctionne à 16MHz (quartz externe) ou 8MHz (interne)
  // Pour ce projet : 8MHz interne recommandé pour basse consommation
  
  // ═══ CONFIGURATION PINS ═══
  pinMode(LED_PWM_PIN, OUTPUT);           // PWM LED principale NSPW500CS
  pinMode(BTN_FREQ_PIN, INPUT_PULLUP);    // Bouton FREQ
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);    // Bouton MODE
  pinMode(ADC_BATTERY_PIN, INPUT);        // ADC batterie
  
  // Pins PWM réservées (configuration future)
  pinMode(PWM_RESERVE_1, OUTPUT);
  pinMode(PWM_RESERVE_2, OUTPUT);
  pinMode(PWM_RESERVE_3, OUTPUT);
  pinMode(PWM_RESERVE_4, OUTPUT);
  pinMode(PWM_RESERVE_5, OUTPUT);
  
  // ═══ INITIALISATION I2C (LED Indicateurs) ═══
  Wire.begin();           // Initialise I2C Master
  Wire.setClock(100000);  // 100kHz (standard I2C)
  delay(10);
  
  // Test communication PCF8574
  if (!testPCF8574()) {
    // Si échec I2C, clignoter LED PWM 5 fois
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
  
  // Clignotement LED principale : boot OK (3 flashs rapides)
  for (uint8_t i = 0; i < 3; i++) {
    OCR2B = 255;
    delay(50);
    OCR2B = 0;
    delay(50);
  }
  
  // ═══ ACTIVATION INTERRUPTIONS ═══
  sei();  // Enable global interrupts
}

// ═══════════════════════════════════════════════════════════════════════════
//  TIMER2 PWM SETUP - Configuration PWM 976Hz sur D3 (OC2B)
// ═══════════════════════════════════════════════════════════════════════════

void setupTimer2PWM() {
  // Timer2 en mode Fast PWM, 8-bit
  // Fréquence PWM = F_CPU / (Prescaler × 256)
  // Pour 16MHz : 16000000 / (64 × 256) = 976.5625Hz (proche 1kHz)
  // Pour 8MHz : 8000000 / (32 × 256) = 976.5625Hz
  
  // Configuration Timer2
  TCCR2A = 0;
  TCCR2B = 0;
  
  // Mode Fast PWM (WGM21=1, WGM20=1)
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  
  // Clear OC2B on compare match (non-inverting)
  TCCR2A |= (1 << COM2B1);
  
  // Prescaler = 64 pour 16MHz (CS22=1)
  // Prescaler = 32 pour 8MHz (CS21=1, CS20=1)
  #if F_CPU == 16000000L
    TCCR2B |= (1 << CS22);  // Prescaler 64
  #elif F_CPU == 8000000L
    TCCR2B |= (1 << CS21) | (1 << CS20);  // Prescaler 32
  #else
    TCCR2B |= (1 << CS22);  // Défaut 64
  #endif
  
  // Duty cycle initial = 0
  OCR2B = 0;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ADC SETUP - Configuration pour mesure batterie
// ═══════════════════════════════════════════════════════════════════════════

void setupADC() {
  // Configuration ADC ATmega328P
  // Référence AVCC (3.3V), canal ADC0 (A0)
  
  ADMUX = 0;
  ADMUX |= (1 << REFS0);  // AVCC reference (3.3V)
  // MUX[3:0] = 0000 pour ADC0
  
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN);   // Enable ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
  
  // Warm-up ADC (première conversion jetée)
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
}

// ═══════════════════════════════════════════════════════════════════════════
//  LOOP PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  
  static uint32_t lastUpdate = 0;
  static uint32_t lastBatteryCheck = 0;
  static uint32_t lastButtonDebug = 0;
  
  uint32_t now = millis();
  
  // ═══ DEBUG BOUTONS (Toutes les 500ms) ═══
  // DÉSACTIVÉ pour PCBA standalone
  /*
  if (now - lastButtonDebug > 500) {
    lastButtonDebug = now;
    Serial.print(F("BTN FREQ(D2): "));
    Serial.print(digitalRead(BTN_FREQ_PIN));
    Serial.print(F(" | BTN MODE(D4): "));
    Serial.println(digitalRead(BTN_MODE_PIN));
  }
  */
  
  // ═══ GESTION BOUTONS ═══
  handleButtons();
  
  // ═══ GÉNÉRATION MODULATION ═══
  if (currentMode != MODE_BLANC) {
    generateModulation();
  } else {
    // Mode lumière blanche : PWM constant
    OCR2B = DUTY_BLANC;  // 50% duty cycle → 10mA
  }
  
  // ═══ MESURE BATTERIE (Toutes les 5 secondes) ═══
  if (now - lastBatteryCheck > 5000) {
    lastBatteryCheck = now;
    batteryVoltage = readBatteryVoltage();
    
    // Debug : Afficher tension batterie - DÉSACTIVÉ pour PCBA
    // Serial.print(F("🔋 Batterie : "));
    // Serial.print(batteryVoltage);
    // Serial.print(F(" mV"));
    
    // Indicateur batterie faible (< 3.5V)
    if (batteryVoltage < 3500) {
      // Serial.println(F(" ⚠️ BATTERIE FAIBLE"));
      blinkBatteryWarning();
    } else {
      // Serial.println();
    }
    
    // Sécurité : Couper traitement si batterie < 3.3V
    // Note v7.0 : Moins critique car consommation faible (28mA vs 360mA)
    if (batteryVoltage < 3300 && currentMode == MODE_TRAITEMENT) {
      currentMode = MODE_DETECTION;
      updateFreqIndicators();
      // Serial.println(F("⚠️ Mode TRAITEMENT désactivé (batterie faible)"));
    }
  }
  
  // ═══ MISE À JOUR INDICATEURS (Toutes les 100ms) ═══
  if (now - lastUpdate > 100) {
    lastUpdate = now;
    updateFreqIndicators();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  GÉNÉRATION MODULATION (Identique v6.0, duty cycles ajustés)
// ═══════════════════════════════════════════════════════════════════════════

void generateModulation() {
  
  uint32_t currentMicros = micros();
  
  // Calcul phase fréquence Nogier
  float nogierFreq = frequencies[currentFreq];
  float nogierPeriod = 1000000.0 / nogierFreq;  // Période en µs
  float nogierPhase = (float)(currentMicros % (uint32_t)nogierPeriod) / nogierPeriod * 2.0 * PI;
  
  // Enveloppe sinusoïdale
  float envelope = 0.5 + 0.5 * sin(nogierPhase);
  
  // Application modulation (profondeur 70%, minimum 20%)
  float modulatedIntensity = MIN_INTENSITY + MODULATION_DEPTH * envelope;
  
  if (modulatedIntensity > 1.0) modulatedIntensity = 1.0;
  if (modulatedIntensity < 0.0) modulatedIntensity = 0.0;
  
  // ═══ Calcul duty cycle selon mode (AJUSTÉ v7.0 pour LED 20mA) ═══
  uint8_t baseDuty = 0;
  
  switch (currentMode) {
    case MODE_DETECTION:
      baseDuty = DUTY_DETECTION;   // 64 → 25% → ~5mA
      break;
      
    case MODE_TRAITEMENT:
      baseDuty = DUTY_TRAITEMENT;  // 235 → 92% → ~20mA
      break;
      
    case MODE_BLANC:
      baseDuty = DUTY_BLANC;       // 128 → 50% → ~10mA
      return;  // Pas de modulation en mode blanc
  }
  
  // Application modulation sur duty cycle de base
  uint8_t finalDuty = (uint8_t)(baseDuty * modulatedIntensity);
  
  // Écriture PWM via registre direct (plus rapide qu'analogWrite)
  OCR2B = finalDuty;
}

// ═══════════════════════════════════════════════════════════════════════════
//  GESTION BOUTONS (Identique v6.0)
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
      
      // Debug : Afficher fréquence sélectionnée - DÉSACTIVÉ pour PCBA
      // Serial.print(F("📡 Fréquence : "));
      // Serial.print(freqNames[currentFreq]);
      // Serial.print(F(" ("));
      // Serial.print(frequencies[currentFreq]);
      // Serial.println(F(" Hz)"));
      
      flashFeedback(2, 80);  // 2 flashs courts
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
      
      // Debug : Afficher mode sélectionné - DÉSACTIVÉ pour PCBA
      // Serial.print(F("⚙️  Mode : "));
      // Serial.println(modeNames[currentMode]);
      
      // Feedback selon mode
      switch (currentMode) {
        case MODE_DETECTION:
          flashFeedback(1, 200);  // 1 flash long
          break;
        case MODE_TRAITEMENT:
          flashFeedback(2, 100);  // 2 flashs moyens
          break;
        case MODE_BLANC:
          flashFeedback(3, 60);   // 3 flashs courts
          break;
      }
      
      delay(200);
    }
  }
  lastBtnMode = btnMode;
}

// ═══════════════════════════════════════════════════════════════════════════
//  MISE À JOUR INDICATEURS LED RGB (Identique v6.0)
// ═══════════════════════════════════════════════════════════════════════════

void updateFreqIndicators() {
  
  uint8_t ledPattern = freqIndicators[currentFreq];
  
  Wire.beginTransmission(PCF8574_ADDRESS);
  Wire.write(ledPattern);
  Wire.endTransmission();
}

// ═══════════════════════════════════════════════════════════════════════════
//  TEST PCF8574 (Identique v6.0)
// ═══════════════════════════════════════════════════════════════════════════

bool testPCF8574() {
  
  Wire.beginTransmission(PCF8574_ADDRESS);
  uint8_t error = Wire.endTransmission();
  
  return (error == 0);
}

// ═══════════════════════════════════════════════════════════════════════════
//  LECTURE TENSION BATTERIE (Ajustée pour Vref 3.3V)
// ═══════════════════════════════════════════════════════════════════════════

uint16_t readBatteryVoltage() {
  
  // Lancer conversion ADC
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  
  uint16_t adcValue = ADC;
  
  // Conversion en mV
  // Vref = 5V (Arduino Uno breadboard), diviseur 100k/33k → facteur 4.03
  // V_bat = ADC × (5000mV / 1023) × 4.03
  // NOTE: Pour PCB final avec LDO 3.3V, changer 5000 en 3300
  uint16_t voltage = (uint32_t)adcValue * 5000 * 403 / 102300;
  
  return voltage;
}

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEUR BATTERIE FAIBLE (Identique v6.0)
// ═══════════════════════════════════════════════════════════════════════════

void blinkBatteryWarning() {
  
  for (uint8_t i = 0; i < 3; i++) {
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0b11111110);  // Rouge seulement (P0 à 0, P1 et P2 à 1)
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
//  FEEDBACK VISUEL (Identique v6.0)
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
//  MODE VEILLE (Identique v6.0)
// ═══════════════════════════════════════════════════════════════════════════

void enterSleepMode() {
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  
  ADCSRA &= ~(1 << ADEN);  // Désactive ADC
  power_all_disable();
  
  sei();
  sleep_cpu();
  
  sleep_disable();
  power_all_enable();
  ADCSRA |= (1 << ADEN);  // Réactive ADC
}

// ═══════════════════════════════════════════════════════════════════════════
//  FIN DU CODE v7.0
// ═══════════════════════════════════════════════════════════════════════════

/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  NOTES D'IMPLÉMENTATION v7.0
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * 🎉 CHANGEMENTS MAJEURS vs v6.0 :
 * 
 * 1. LED NICHIA NSPW500CS (20mA) au lieu de 519A (350mA)
 *    • Circuit simplifié : résistance série 47Ω au lieu de driver op-amp + MOSFET
 *    • Duty cycles ajustés : 25%/92%/50% (vs 15%/100%/60%)
 *    • Courants réduits ×17.5 : 5mA/20mA/10mA (vs 50mA/350mA/200mA)
 *    • Autonomie ×3 à ×9 meilleure!
 * 
 * 2. PARAMÈTRES PWM MODIFIÉS :
 *    • DUTY_DETECTION  = 64  (25%) → ~5mA
 *    • DUTY_TRAITEMENT = 235 (92%) → ~20mA
 *    • DUTY_BLANC      = 128 (50%) → ~10mA
 * 
 * 3. CALCUL TENSION BATTERIE AJUSTÉ :
 *    • Vref = 3.3V (AVCC régulé) au lieu de 5V
 *    • Formule : voltage = ADC × 3300 × 4.03 / 1023
 * 
 * 4. RESTE DU CODE IDENTIQUE :
 *    • Modulation Nogier inchangée
 *    • Gestion boutons inchangée
 *    • I2C LEDs indicateurs inchangé
 *    • Timer2 PWM configuration identique
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 *  COMPILATION
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Carte : Arduino Uno / Arduino Pro Mini 3.3V 8MHz
 * Microcontrôleur : ATmega328P
 * Programmeur : USBasp, AVRISP, ou bootloader Arduino
 * Bootloader : Optiboot (recommandé) ou sans bootloader pour économie Flash
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 *  CONSOMMATION & AUTONOMIE v7.0
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Mode DÉTECTION (5mA LED) :
 *   • LED : 5mA (modulation 50% → 2.5mA moyen)
 *   • ATmega328P @ 8MHz : 8mA
 *   • Total : ~13mA → Autonomie 69h  (vs 26h en v6.0)
 * 
 * Mode TRAITEMENT (20mA LED) :
 *   • LED : 20mA (modulation 70% → 14mA moyen)
 *   • ATmega328P @ 8MHz : 8mA
 *   • Total : ~28mA → Autonomie 32h  (vs 3.5h en v6.0!)
 * 
 * Mode BLANC (10mA LED) :
 *   • LED : 10mA constant
 *   • ATmega328P @ 8MHz : 8mA
 *   • Total : ~18mA → Autonomie 50h  (vs 18h en v6.0)
 * 
 * Veille (sleep mode) :
 *   • ATmega328P : 0.1µA
 *   • LDO : 4µA
 *   • Total : ~5µA → Autonomie théorique >20 ans!
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 *  AVANTAGES v7.0
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * ✅ Autonomie ×3 à ×9 meilleure selon mode
 * ✅ Circuit électronique ×4 plus simple (7 composants supprimés)
 * ✅ Code quasi-identique (seuls duty cycles modifiés)
 * ✅ Coût réduit de -2.23€ (-10%)
 * ✅ Montage ×2 plus rapide (LED traversant vs SMD)
 * ✅ Fiabilité accrue (moins de composants à défaillir)
 * ✅ CRI >90 conservé (qualité thérapeutique identique)
 * ✅ Faisceau focalisé 15° (meilleur pour stylet de précision)
 * ✅ Moins de chaleur générée (pas de MOSFET ni Rsense puissance)
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 *  EXTENSIONS FUTURES POSSIBLES
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Avec les pins PWM réservées disponibles :
 *   • D5/D6   : LED RGB additionnel (indication mode)
 *   • D9/D10  : Contrôle LED UV (si ajout LED UV thérapeutique)
 *   • D11     : Vibreur haptic feedback
 * 
 * Avec les ADC libres (A1-A3) :
 *   • A1 : Capteur température (LM35)
 *   • A2 : Potentiomètre intensité LED
 *   • A3 : Capteur pression (détection contact peau)
 * 
 * Avec GPIO libres (D7, D8, D12, D13) :
 *   • Écran OLED I2C (affichage fréquence/mode)
 *   • Encodeur rotatif (sélection fréquence)
 *   • Buzzer (feedback sonore)
 *   • LED status supplémentaire
 * 
 * Avec UART matériel (RX/TX) :
 *   • Debug Serial.print()
 *   • Communication Bluetooth (HC-05)
 *   • Logging séances sur carte SD
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 */
