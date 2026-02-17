/*
 * ═══════════════════════════════════════════════════════════════
 *  STYLET AURICULOTHÉRAPIE v4.0 - MODULATION ADAPTATIVE
 * ═══════════════════════════════════════════════════════════════
 * 
 * FONCTIONNALITÉS :
 * - 7 fréquences de Nogier : 2.28Hz à 146Hz
 * - Intensité réglable : 10% à 100% (par pas de 10%)
 * - 3 modes de modulation sélectionnables :
 *   → SINUSOÏDAL : Variation douce (confort maximal)
 *   → AM (Amplitude Modulation) : Porteuse 200Hz modulée
 *   → CARRÉ : Signal classique ON/OFF
 * - 3 LED indicatrices RGB pour affichage fréquence
 * - 1 LED indicatrice pour mode de modulation
 * 
 * HARDWARE :
 * - LED principale (Nichia NSPW500CS) : D11 (PWM)
 * - LED indicateurs fréquence : D10 (R), D9 (G), D8 (B)
 * - LED indicateur mode : D7 (Blanche ou couleur selon mode)
 * - Bouton FREQ : D2 (Sélection fréquence A→G)
 * - Bouton INTENSITY : D3 (Réglage intensité 10%→100%)
 * - Bouton MODE : D4 (Sélection modulation)
 * 
 * AUTEUR : Projet Auriculothérapie
 * VERSION : 4.0
 * DATE : Février 2026
 * ═══════════════════════════════════════════════════════════════
 */

// ═══════════════════════════════════════════════════════════════
//  CONFIGURATION PINS
// ═══════════════════════════════════════════════════════════════

#define LED_MAIN 11              // LED principale thérapeutique (PWM)
#define LED_INDICATOR_A 10       // LED Rouge (indicateur fréquence)
#define LED_INDICATOR_B 9        // LED Verte (indicateur fréquence)
#define LED_INDICATOR_C 8        // LED Bleue (indicateur fréquence)
#define LED_MODE 7               // LED indicateur mode de modulation
#define BTN_FREQ 2               // Bouton sélection fréquence
#define BTN_INTENSITY 3          // Bouton sélection intensité
#define BTN_MODE 4               // Bouton sélection mode de modulation

// ═══════════════════════════════════════════════════════════════
//  FRÉQUENCES DE NOGIER
// ═══════════════════════════════════════════════════════════════

const float frequencies[] = {2.28, 5.56, 9.12, 18.25, 36.50, 73.0, 146.0};
const char* freqNames[] = {"A", "B", "C", "D", "E", "F", "G"};
const int numFreqs = 7;

// ═══════════════════════════════════════════════════════════════
//  INTENSITÉS (10% à 100% par pas de 10%)
// ═══════════════════════════════════════════════════════════════

const int intensityLevels[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const int numIntensities = 10;

// ═══════════════════════════════════════════════════════════════
//  INDICATEURS LED (codage RGB pour 7 fréquences)
// ═══════════════════════════════════════════════════════════════

// Format : {LED_A (Rouge), LED_B (Vert), LED_C (Bleu)}
const bool freqIndicators[7][3] = {
  {1, 0, 0},  // A (2.28Hz)  : Rouge
  {0, 1, 0},  // B (5.56Hz)  : Vert
  {1, 1, 0},  // C (9.12Hz)  : Jaune (R+G)
  {0, 0, 1},  // D (18.25Hz) : Bleu
  {1, 0, 1},  // E (36.50Hz) : Magenta (R+B)
  {0, 1, 1},  // F (73Hz)    : Cyan (G+B)
  {1, 1, 1}   // G (146Hz)   : Blanc (R+G+B)
};

// ═══════════════════════════════════════════════════════════════
//  MODES DE MODULATION
// ═══════════════════════════════════════════════════════════════

enum ModulationMode {
  MODE_SINE,      // Modulation sinusoïdale (confortable, doux)
  MODE_AM,        // Amplitude Modulation avec porteuse 200Hz
  MODE_SQUARE     // Signal carré classique (ON/OFF brutal)
};

const char* modeNames[] = {"SINUSOÏDAL", "AM (200Hz)", "CARRÉ"};
const int numModes = 3;

#define CARRIER_FREQ 200.0  // Fréquence porteuse pour mode AM (Hz)

// ═══════════════════════════════════════════════════════════════
//  VARIABLES GLOBALES
// ═══════════════════════════════════════════════════════════════

int currentFreq = 0;                    // Index fréquence actuelle (0-6)
int currentIntensity = 4;               // Index intensité actuelle (0-9, démarrage 50%)
ModulationMode currentMode = MODE_SINE; // Mode de modulation (démarrage SINE)

// ═══════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════

void setup() {
  // Configuration des pins en sortie (LED)
  pinMode(LED_MAIN, OUTPUT);
  pinMode(LED_INDICATOR_A, OUTPUT);
  pinMode(LED_INDICATOR_B, OUTPUT);
  pinMode(LED_INDICATOR_C, OUTPUT);
  pinMode(LED_MODE, OUTPUT);
  
  // Configuration des pins en entrée avec pull-up (Boutons)
  pinMode(BTN_FREQ, INPUT_PULLUP);
  pinMode(BTN_INTENSITY, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  
  // Initialisation Serial pour debug
  Serial.begin(115200);
  
  // Bannière de démarrage
  Serial.println(F("\n╔═══════════════════════════════════════════════════════╗"));
  Serial.println(F("║     STYLET AURICULOTHÉRAPIE v4.0                      ║"));
  Serial.println(F("║     Fréquences de Nogier + Modulation Adaptative      ║"));
  Serial.println(F("╚═══════════════════════════════════════════════════════╝"));
  Serial.println(F("\n📋 COMMANDES :"));
  Serial.println(F("  • BTN_FREQ (D2)      : Changer fréquence (A→G)"));
  Serial.println(F("  • BTN_INTENSITY (D3) : Changer intensité (10%→100%)"));
  Serial.println(F("  • BTN_MODE (D4)      : Changer modulation (SINE→AM→CARRÉ)"));
  Serial.println(F("\n🎨 INDICATEURS LED :"));
  Serial.println(F("  • LED RGB (D10/D9/D8) : Code couleur fréquence"));
  Serial.println(F("  • LED Mode (D7)       : Clignote selon mode actif\n"));
  
  // Initialisation des indicateurs
  updateFreqIndicators();
  updateModeIndicator();
  
  // Affichage statut initial
  printStatus();
  
  // LED principale éteinte au démarrage
  digitalWrite(LED_MAIN, LOW);
}

// ═══════════════════════════════════════════════════════════════
//  MISE À JOUR INDICATEURS FRÉQUENCE (RGB)
// ═══════════════════════════════════════════════════════════════

void updateFreqIndicators() {
  digitalWrite(LED_INDICATOR_A, freqIndicators[currentFreq][0]);
  digitalWrite(LED_INDICATOR_B, freqIndicators[currentFreq][1]);
  digitalWrite(LED_INDICATOR_C, freqIndicators[currentFreq][2]);
}

// ═══════════════════════════════════════════════════════════════
//  MISE À JOUR INDICATEUR MODE (LED clignotante)
// ═══════════════════════════════════════════════════════════════

void updateModeIndicator() {
  // LED mode clignote avec pattern différent selon mode
  // SINE : clignotement lent (1 Hz)
  // AM : clignotement moyen (3 Hz)
  // SQUARE : clignotement rapide (5 Hz)
  
  // Note : Géré dans loop() de manière non-bloquante
}

// ═══════════════════════════════════════════════════════════════
//  AFFICHAGE STATUT (Serial Monitor)
// ═══════════════════════════════════════════════════════════════

void printStatus() {
  Serial.print(F("\n► Fréquence "));
  Serial.print(freqNames[currentFreq]);
  Serial.print(F(" ("));
  Serial.print(frequencies[currentFreq], 2);
  Serial.print(F(" Hz)"));
  
  Serial.print(F(" | Intensité : "));
  Serial.print(intensityLevels[currentIntensity]);
  Serial.print(F("%"));
  
  Serial.print(F(" | Mode : "));
  Serial.print(modeNames[currentMode]);
  
  // Affichage couleur indicateur fréquence
  Serial.print(F(" | Couleur : "));
  if (freqIndicators[currentFreq][0] && !freqIndicators[currentFreq][1] && !freqIndicators[currentFreq][2]) {
    Serial.println(F("🔴 ROUGE"));
  } else if (!freqIndicators[currentFreq][0] && freqIndicators[currentFreq][1] && !freqIndicators[currentFreq][2]) {
    Serial.println(F("🟢 VERT"));
  } else if (freqIndicators[currentFreq][0] && freqIndicators[currentFreq][1] && !freqIndicators[currentFreq][2]) {
    Serial.println(F("🟡 JAUNE"));
  } else if (!freqIndicators[currentFreq][0] && !freqIndicators[currentFreq][1] && freqIndicators[currentFreq][2]) {
    Serial.println(F("🔵 BLEU"));
  } else if (freqIndicators[currentFreq][0] && !freqIndicators[currentFreq][1] && freqIndicators[currentFreq][2]) {
    Serial.println(F("🟣 MAGENTA"));
  } else if (!freqIndicators[currentFreq][0] && freqIndicators[currentFreq][1] && freqIndicators[currentFreq][2]) {
    Serial.println(F("🩵 CYAN"));
  } else if (freqIndicators[currentFreq][0] && freqIndicators[currentFreq][1] && freqIndicators[currentFreq][2]) {
    Serial.println(F("⚪ BLANC"));
  }
}

// ═══════════════════════════════════════════════════════════════
//  FEEDBACK VISUEL (flash LED principale)
// ═══════════════════════════════════════════════════════════════

void flashFeedback(int times, int delayMs = 100) {
  for(int i = 0; i < times; i++) {
    analogWrite(LED_MAIN, 255);
    delay(delayMs);
    analogWrite(LED_MAIN, 0);
    delay(delayMs);
  }
}

// ═══════════════════════════════════════════════════════════════
//  BOUCLE PRINCIPALE
// ═══════════════════════════════════════════════════════════════

void loop() {
  
  // ═══ GESTION BOUTON FRÉQUENCE ═══
  static bool lastBtnFreq = HIGH;
  static unsigned long lastDebounceFreq = 0;
  bool btnFreq = digitalRead(BTN_FREQ);
  
  if (btnFreq != lastBtnFreq) {
    lastDebounceFreq = millis();
  }
  
  if ((millis() - lastDebounceFreq) > 50) {  // Anti-rebond 50ms
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      // Passage à la fréquence suivante
      currentFreq = (currentFreq + 1) % numFreqs;
      updateFreqIndicators();
      
      // Feedback : 2 flashs courts
      flashFeedback(2, 80);
      
      printStatus();
      delay(300);  // Évite détection multiple
    }
  }
  lastBtnFreq = btnFreq;
  
  
  // ═══ GESTION BOUTON INTENSITÉ ═══
  static bool lastBtnIntensity = HIGH;
  static unsigned long lastDebounceIntensity = 0;
  bool btnIntensity = digitalRead(BTN_INTENSITY);
  
  if (btnIntensity != lastBtnIntensity) {
    lastDebounceIntensity = millis();
  }
  
  if ((millis() - lastDebounceIntensity) > 50) {
    if (btnIntensity == LOW && lastBtnIntensity == HIGH) {
      // Passage à l'intensité suivante
      currentIntensity = (currentIntensity + 1) % numIntensities;
      
      // Feedback : 1 flash court
      flashFeedback(1, 50);
      
      printStatus();
      delay(300);
    }
  }
  lastBtnIntensity = btnIntensity;
  
  
  // ═══ GESTION BOUTON MODE (NOUVEAU) ═══
  static bool lastBtnMode = HIGH;
  static unsigned long lastDebounceMode = 0;
  bool btnMode = digitalRead(BTN_MODE);
  
  if (btnMode != lastBtnMode) {
    lastDebounceMode = millis();
  }
  
  if ((millis() - lastDebounceMode) > 50) {
    if (btnMode == LOW && lastBtnMode == HIGH) {
      // Passage au mode suivant
      currentMode = (ModulationMode)((currentMode + 1) % numModes);
      
      // Feedback visuel selon mode :
      // - SINE : 1 flash long
      // - AM : 2 flashs moyens
      // - SQUARE : 3 flashs courts
      switch(currentMode) {
        case MODE_SINE:
          flashFeedback(1, 200);
          break;
        case MODE_AM:
          flashFeedback(2, 100);
          break;
        case MODE_SQUARE:
          flashFeedback(3, 60);
          break;
      }
      
      printStatus();
      delay(300);
    }
  }
  lastBtnMode = btnMode;
  
  
  // ═══ CLIGNOTEMENT LED INDICATEUR MODE (non-bloquant) ═══
  static unsigned long lastModeBlink = 0;
  unsigned long modeBlinkInterval;
  
  // Définir vitesse clignotement selon mode
  switch(currentMode) {
    case MODE_SINE:
      modeBlinkInterval = 500;  // 1 Hz (lent)
      break;
    case MODE_AM:
      modeBlinkInterval = 166;  // 3 Hz (moyen)
      break;
    case MODE_SQUARE:
      modeBlinkInterval = 100;  // 5 Hz (rapide)
      break;
  }
  
  if (millis() - lastModeBlink >= modeBlinkInterval) {
    lastModeBlink = millis();
    digitalWrite(LED_MODE, !digitalRead(LED_MODE));  // Toggle
  }
  
  
  // ═══ GÉNÉRATION SIGNAL SELON MODE ═══
  generateSignal();
}

// ═══════════════════════════════════════════════════════════════
//  GÉNÉRATION DU SIGNAL LUMINEUX
// ═══════════════════════════════════════════════════════════════

void generateSignal() {
  unsigned long currentMicros = micros();
  float period = 1000000.0 / frequencies[currentFreq];  // Période en microsecondes
  float phase = (float)(currentMicros % (unsigned long)period) / period * 2.0 * PI;
  
  int pwmValue = 0;
  
  switch(currentMode) {
    
    // ═══ MODE SINUSOÏDAL ═══
    case MODE_SINE:
      {
        // Génération onde sinusoïdale pure
        // sin(phase) varie de -1 à +1
        // On ramène entre 0 et 1 : (sin + 1) / 2
        float sineValue = (sin(phase) + 1.0) / 2.0;
        
        // Application de l'intensité réglable
        pwmValue = (int)(sineValue * intensityLevels[currentIntensity] * 2.55);
      }
      break;
    
    
    // ═══ MODE AM (AMPLITUDE MODULATION) ═══
    case MODE_AM:
      {
        // Calcul phase de la porteuse (200 Hz)
        float carrierPeriod = 1000000.0 / CARRIER_FREQ;
        float carrierPhase = (float)(currentMicros % (unsigned long)carrierPeriod) 
                             / carrierPeriod * 2.0 * PI;
        
        // Génération signal porteuse (onde carrée rapide)
        float carrier = (sin(carrierPhase) > 0) ? 1.0 : 0.0;
        
        // Génération signal modulant (fréquence thérapeutique)
        float modulator = (sin(phase) + 1.0) / 2.0;  // 0 à 1
        
        // Signal final = carrier × modulator × intensité
        pwmValue = (int)(carrier * modulator * intensityLevels[currentIntensity] * 2.55);
      }
      break;
    
    
    // ═══ MODE CARRÉ (CLASSIQUE) ═══
    case MODE_SQUARE:
      {
        // Signal carré simple : ON si phase < π, OFF sinon
        bool state = (phase < PI);
        pwmValue = state ? (intensityLevels[currentIntensity] * 255 / 100) : 0;
      }
      break;
  }
  
  // Écriture de la valeur PWM sur la LED principale
  analogWrite(LED_MAIN, pwmValue);
}

// ═══════════════════════════════════════════════════════════════
//  FIN DU CODE
// ═══════════════════════════════════════════════════════════════
