/*
 * STYLET AURICULOTHÉRAPIE - SIMULATION WOKWI
 * 
 * Fréquences de pulsation : 2.28Hz à 146Hz
 * Bouton D2 : Sélection fréquence (A→G)
 * Bouton D3 : ON/OFF
 * LED D11 : Signal pulsé
 * 
 * Version : 1.0
 * Date : Février 2026
 */

// ===== CONFIGURATION PINS =====
#define LED_PIN 11        // Pin PWM pour LED (OBLIGATOIRE: pin PWM)
#define BTN_FREQ 2        // Bouton sélection fréquence
#define BTN_PWR 3         // Bouton marche/arrêt

// ===== FRÉQUENCES THÉRAPEUTIQUES =====
const float frequencies[] = {2.28, 5.56, 9.12, 18.25, 36.50, 73.0, 146.0};
const char* freqNames[] = {"A", "B", "C", "D", "E", "F", "G"};
const int numFreqs = 7;

// ===== VARIABLES GLOBALES =====
int currentFreq = 0;              // Index fréquence actuelle (0-6)
bool deviceOn = true;             // État ON/OFF
bool ledState = false;            // État actuel LED
unsigned long previousMicros = 0; // Timestamp dernière bascule
unsigned long halfPeriodMicros = 0; // Demi-période en microsecondes

// ===== SETUP =====
void setup() {
  // Configuration des pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_FREQ, INPUT_PULLUP);  // Pull-up interne activé
  pinMode(BTN_PWR, INPUT_PULLUP);   // Pull-up interne activé
  
  // Initialisation Serial pour debug
  Serial.begin(115200);
  Serial.println(F("\n╔═══════════════════════════════════╗"));
  Serial.println(F("║  STYLET AURICULOTHÉRAPIE v1.0     ║"));
  Serial.println(F("╚═══════════════════════════════════╝"));
  Serial.println(F("\nCommandes:"));
  Serial.println(F("- BTN_FREQ (D2) : Changer fréquence"));
  Serial.println(F("- BTN_PWR  (D3) : ON/OFF\n"));
  
  // Calcul de la première fréquence
  updateFrequency();
  printCurrentFrequency();
  
  digitalWrite(LED_PIN, LOW);
}

// ===== MISE À JOUR FRÉQUENCE =====
void updateFrequency() {
  // Calcul demi-période : T/2 = 1/(2*f)
  // En microsecondes : 500000/f
  halfPeriodMicros = (unsigned long)(500000.0 / frequencies[currentFreq]);
}

// ===== AFFICHAGE FRÉQUENCE =====
void printCurrentFrequency() {
  Serial.print(F("► Fréquence "));
  Serial.print(freqNames[currentFreq]);
  Serial.print(F(" : "));
  Serial.print(frequencies[currentFreq], 2);
  Serial.print(F(" Hz (T/2 = "));
  Serial.print(halfPeriodMicros);
  Serial.println(F(" µs)"));
}

// ===== FEEDBACK VISUEL =====
void flashFeedback(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }
}

// ===== BOUCLE PRINCIPALE =====
void loop() {
  
  // ═══ GESTION BOUTON FRÉQUENCE ═══
  static bool lastBtnFreq = HIGH;
  static unsigned long lastDebounceFreq = 0;
  bool btnFreq = digitalRead(BTN_FREQ);
  
  // Détection front descendant avec anti-rebond
  if (btnFreq != lastBtnFreq) {
    lastDebounceFreq = millis();
  }
  
  if ((millis() - lastDebounceFreq) > 50) {  // 50ms debounce
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      // Passage à la fréquence suivante
      currentFreq = (currentFreq + 1) % numFreqs;
      updateFrequency();
      
      // Feedback : flash N fois (N = numéro fréquence)
      flashFeedback(currentFreq + 1);
      
      printCurrentFrequency();
      
      // Reset du timing pour éviter les glitchs
      previousMicros = micros();
      ledState = false;
      
      delay(300); // Anti-rebond supplémentaire
    }
  }
  lastBtnFreq = btnFreq;
  
  // ═══ GESTION BOUTON POWER ═══
  static bool lastBtnPwr = HIGH;
  static unsigned long lastDebouncePwr = 0;
  bool btnPwr = digitalRead(BTN_PWR);
  
  if (btnPwr != lastBtnPwr) {
    lastDebouncePwr = millis();
  }
  
  if ((millis() - lastDebouncePwr) > 50) {
    if (btnPwr == LOW && lastBtnPwr == HIGH) {
      deviceOn = !deviceOn;
      
      if (deviceOn) {
        Serial.println(F("✓ Device ON"));
        flashFeedback(1);
      } else {
        Serial.println(F("✗ Device OFF"));
        digitalWrite(LED_PIN, LOW);
      }
      
      delay(300);
    }
  }
  lastBtnPwr = btnPwr;
  
  // ═══ GÉNÉRATION SIGNAL PULSÉ ═══
  if (deviceOn) {
    unsigned long currentMicros = micros();
    
    // Basculement LED si demi-période écoulée
    if (currentMicros - previousMicros >= halfPeriodMicros) {
      previousMicros = currentMicros;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
  
  // Note : Pas de delay() ici pour garder la précision temporelle !
}
