/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v7.0 - SIMULATION WOKWI
 *  Version simplifiée pour test breadboard Arduino Uno
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * DIFFÉRENCES avec version finale ATmega328P-AU :
 * - LEDs indicateurs RGB connectées directement (D9, D10, D11)
 * - Pas de PCF8574 I2C (non supporté par Wokwi)
 * - Fréquence CPU 16MHz (vs 8MHz version finale basse conso)
 * - Simulation batterie via potentiomètre A0
 * 
 * FONCTIONNALITÉS TESTÉES :
 * ✓ Modulation 1kHz des fréquences de Nogier
 * ✓ Circuit LED simplifié avec résistance 47Ω
 * ✓ 3 modes (Détection / Traitement / Blanc)
 * ✓ Sélection fréquence via bouton
 * ✓ Indicateurs LED RGB
 * ✓ Mesure batterie simulée
 */

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION PINS
// ═══════════════════════════════════════════════════════════════════════════

#define LED_PWM_PIN     3       // D3 - PWM LED principale NSPW500CS (OC2B Timer2)
#define BTN_FREQ_PIN    2       // D2 - Bouton sélection fréquence
#define BTN_MODE_PIN    4       // D4 - Bouton sélection mode
#define ADC_BATTERY_PIN A0      // A0 - ADC0 Mesure batterie (potentiomètre)

// LEDs indicateurs RGB (direct, pas I2C pour Wokwi)
#define LED_RED_PIN     11      // D11 - LED Rouge
#define LED_GREEN_PIN   10      // D10 - LED Verte
#define LED_BLUE_PIN    9       // D9 - LED Bleue

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
  MODE_DETECTION,      // Basse intensité (5mA, 25% duty cycle)
  MODE_TRAITEMENT,     // Haute intensité (20mA, 92% duty cycle)
  MODE_BLANC           // Lumière continue non modulée (10mA, 50% duty)
};

const char* modeNames[] = {"DETECTION", "TRAITEMENT", "BLANC"};
const uint8_t numModes = 3;

// ═══════════════════════════════════════════════════════════════════════════
//  CONFIGURATION MODULATION
// ═══════════════════════════════════════════════════════════════════════════

#define CARRIER_FREQ        976.0     // Fréquence porteuse réelle (Hz)
#define MODULATION_DEPTH    0.70      // Profondeur modulation (70%)
#define MIN_INTENSITY       0.20      // Intensité minimum (20%)

// Duty cycles pour LED 20mA NSPW500CS avec R=47Ω
#define DUTY_DETECTION   64    // 25% → ~5mA
#define DUTY_TRAITEMENT  235   // 92% → ~20mA
#define DUTY_BLANC       128   // 50% → ~10mA

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEURS LED RGB (Codage binaire pour 7 fréquences)
// ═══════════════════════════════════════════════════════════════════════════

// Format : {Rouge, Vert, Bleu}
const bool freqIndicators[7][3] = {
  {1, 0, 0},  // A (2.28Hz)  : Rouge
  {0, 1, 0},  // B (4.56Hz)  : Vert
  {1, 1, 0},  // C (9.12Hz)  : Jaune
  {0, 0, 1},  // D (18.25Hz) : Bleu
  {1, 0, 1},  // E (36.50Hz) : Magenta
  {0, 1, 1},  // F (73Hz)    : Cyan
  {1, 1, 1}   // G (146Hz)   : Blanc
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
  
  // Configuration Serial pour debug Wokwi
  Serial.begin(115200);
  Serial.println(F("╔═══════════════════════════════════════════════════════════╗"));
  Serial.println(F("║  STYLET AURICULOTHERAPIE v7.0 - SIMULATION WOKWI         ║"));
  Serial.println(F("║  LED Nichia NSPW500CS - Circuit Ultra-Simplifié          ║"));
  Serial.println(F("╚═══════════════════════════════════════════════════════════╝"));
  Serial.println();
  
  // Configuration pins
  pinMode(LED_PWM_PIN, OUTPUT);           // PWM LED principale
  pinMode(BTN_FREQ_PIN, INPUT_PULLUP);    // Bouton FREQ
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);    // Bouton MODE
  pinMode(ADC_BATTERY_PIN, INPUT);        // ADC batterie
  
  // LEDs indicateurs RGB
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  
  // Configuration Timer2 pour PWM 976Hz sur D3
  setupTimer2PWM();
  
  // Configuration ADC
  setupADC();
  
  // Affichage initial
  updateFreqIndicators();
  
  // Lecture batterie initiale
  batteryVoltage = readBatteryVoltage();
  
  Serial.println(F("✓ Initialisation terminée"));
  Serial.print(F("✓ Fréquence initiale : "));
  Serial.print(freqNames[currentFreq]);
  Serial.print(F(" ("));
  Serial.print(frequencies[currentFreq]);
  Serial.println(F(" Hz)"));
  Serial.print(F("✓ Mode initial : "));
  Serial.println(modeNames[currentMode]);
  Serial.print(F("✓ Batterie : "));
  Serial.print(batteryVoltage);
  Serial.println(F(" mV"));
  Serial.println();
  
  // Clignotement LED principale : boot OK (3 flashs rapides)
  for (uint8_t i = 0; i < 3; i++) {
    OCR2B = 255;
    delay(50);
    OCR2B = 0;
    delay(50);
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  TIMER2 PWM SETUP - Configuration PWM 976Hz sur D3
// ═══════════════════════════════════════════════════════════════════════════

void setupTimer2PWM() {
  // Timer2 en mode Fast PWM, 8-bit
  // Fréquence PWM = 16MHz / (64 × 256) = 976.5625Hz
  
  TCCR2A = 0;
  TCCR2B = 0;
  
  // Mode Fast PWM (WGM21=1, WGM20=1)
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  
  // Clear OC2B on compare match (non-inverting)
  TCCR2A |= (1 << COM2B1);
  
  // Prescaler = 64 pour 16MHz
  TCCR2B |= (1 << CS22);
  
  // Duty cycle initial = 0
  OCR2B = 0;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ADC SETUP - Configuration pour mesure batterie
// ═══════════════════════════════════════════════════════════════════════════

void setupADC() {
  ADMUX = 0;
  ADMUX |= (1 << REFS0);  // AVCC reference (5V pour Wokwi)
  
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN);   // Enable ADC
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
  
  // Gestion boutons
  handleButtons();
  
  // Génération modulation
  if (currentMode != MODE_BLANC) {
    generateModulation();
  } else {
    OCR2B = DUTY_BLANC;  // Mode lumière blanche : PWM constant
  }
  
  // Mesure batterie (toutes les 5 secondes)
  if (now - lastBatteryCheck > 5000) {
    lastBatteryCheck = now;
    batteryVoltage = readBatteryVoltage();
    
    Serial.print(F("🔋 Batterie : "));
    Serial.print(batteryVoltage);
    Serial.print(F(" mV"));
    
    // Indicateur batterie faible (< 3.5V)
    if (batteryVoltage < 3500) {
      Serial.println(F(" ⚠️ BATTERIE FAIBLE"));
      blinkBatteryWarning();
    } else {
      Serial.println();
    }
  }
  
  // Mise à jour indicateurs (toutes les 100ms)
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
  
  // Calcul phase fréquence Nogier
  float nogierFreq = frequencies[currentFreq];
  float nogierPeriod = 1000000.0 / nogierFreq;
  float nogierPhase = (float)(currentMicros % (uint32_t)nogierPeriod) / nogierPeriod * 2.0 * PI;
  
  // Enveloppe sinusoïdale
  float envelope = 0.5 + 0.5 * sin(nogierPhase);
  
  // Application modulation (profondeur 70%, minimum 20%)
  float modulatedIntensity = MIN_INTENSITY + MODULATION_DEPTH * envelope;
  
  if (modulatedIntensity > 1.0) modulatedIntensity = 1.0;
  if (modulatedIntensity < 0.0) modulatedIntensity = 0.0;
  
  // Calcul duty cycle selon mode
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
      return;
  }
  
  // Application modulation sur duty cycle de base
  uint8_t finalDuty = (uint8_t)(baseDuty * modulatedIntensity);
  
  // Écriture PWM via registre direct
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
  
  // Bouton fréquence
  bool btnFreq = digitalRead(BTN_FREQ_PIN);
  
  if (btnFreq != lastBtnFreq) {
    lastDebounceFreq = millis();
  }
  
  if ((millis() - lastDebounceFreq) > debounceDelay) {
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      currentFreq = (currentFreq + 1) % numFreqs;
      updateFreqIndicators();
      
      Serial.print(F("📡 Fréquence : "));
      Serial.print(freqNames[currentFreq]);
      Serial.print(F(" ("));
      Serial.print(frequencies[currentFreq]);
      Serial.println(F(" Hz)"));
      
      flashFeedback(2, 80);
      delay(200);
    }
  }
  lastBtnFreq = btnFreq;
  
  // Bouton mode
  bool btnMode = digitalRead(BTN_MODE_PIN);
  
  if (btnMode != lastBtnMode) {
    lastDebounceMode = millis();
  }
  
  if ((millis() - lastDebounceMode) > debounceDelay) {
    if (btnMode == LOW && lastBtnMode == HIGH) {
      currentMode = (Mode)((currentMode + 1) % numModes);
      updateFreqIndicators();
      
      Serial.print(F("⚙️  Mode : "));
      Serial.println(modeNames[currentMode]);
      
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
//  MISE À JOUR INDICATEURS LED RGB
// ═══════════════════════════════════════════════════════════════════════════

void updateFreqIndicators() {
  // Contrôle direct des LEDs (pas I2C pour Wokwi)
  digitalWrite(LED_RED_PIN, freqIndicators[currentFreq][0]);
  digitalWrite(LED_GREEN_PIN, freqIndicators[currentFreq][1]);
  digitalWrite(LED_BLUE_PIN, freqIndicators[currentFreq][2]);
}

// ═══════════════════════════════════════════════════════════════════════════
//  LECTURE TENSION BATTERIE
// ═══════════════════════════════════════════════════════════════════════════

uint16_t readBatteryVoltage() {
  
  // Lancer conversion ADC
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  
  uint16_t adcValue = ADC;
  
  // Conversion en mV (diviseur 100k/33k, Vref 5V pour Wokwi)
  // V_bat = ADC × (5000mV / 1023) × 4.03
  uint16_t voltage = (uint32_t)adcValue * 5000 * 403 / 102300;
  
  return voltage;
}

// ═══════════════════════════════════════════════════════════════════════════
//  INDICATEUR BATTERIE FAIBLE
// ═══════════════════════════════════════════════════════════════════════════

void blinkBatteryWarning() {
  
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(LED_RED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_RED_PIN, LOW);
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
