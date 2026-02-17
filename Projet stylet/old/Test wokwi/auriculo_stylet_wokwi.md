# Stylet Auriculothérapie - Circuit Wokwi

## 🔗 Lien direct vers le simulateur

**Voici le lien complet à copier-coller dans votre navigateur :**

```
https://wokwi.com/projects/new/arduino-uno
```

Ensuite, suivez les instructions ci-dessous pour configurer le circuit.

---

## 📋 Configuration du circuit Wokwi

### Étape 1 : Créer le projet

1. Allez sur https://wokwi.com
2. Cliquez sur **"New Project"** → **"Arduino Uno"**
3. Vous arrivez sur l'éditeur avec un Arduino Uno vide

### Étape 2 : Ajouter les composants

Cliquez sur le bouton **"+"** (Add a new part) et ajoutez :

| Composant | Nom dans Wokwi | Quantité |
|-----------|----------------|----------|
| Arduino Uno | Arduino Uno | 1 (déjà présent) |
| LED rouge | Red LED | 1 |
| Résistance | Resistor (220Ω) | 1 |
| Bouton poussoir | Pushbutton | 2 |
| Alimentation | 9V Battery | 1 (optionnel) |

### Étape 3 : Câblage

**Voici les connexions à faire :**

#### LED (rouge)
- **Anode (+)** de la LED → Résistance 220Ω → Pin **D11** de l'Arduino
- **Cathode (-)** de la LED → **GND** de l'Arduino

#### Bouton 1 (Sélection fréquence)
- **Pin 1** du bouton → Pin **D2** de l'Arduino
- **Pin 2** du bouton → **GND** de l'Arduino

#### Bouton 2 (ON/OFF)
- **Pin 1** du bouton → Pin **D3** de l'Arduino
- **Pin 2** du bouton → **GND** de l'Arduino

---

## 💻 Code à copier dans l'éditeur Wokwi

Copiez ce code dans l'onglet **"sketch.ino"** de Wokwi :

```cpp
/*
 * STYLET AURICULOTHÉRAPIE - SIMULATION WOKWI
 * 
 * Fréquences de pulsation : 2.28Hz à 146Hz
 * Bouton D2 : Sélection fréquence (A→G)
 * Bouton D3 : ON/OFF
 * LED D11 : Signal pulsé
 */

// ===== CONFIGURATION PINS =====
#define LED_PIN 11        // Pin PWM pour LED
#define BTN_FREQ 2        // Bouton sélection fréquence
#define BTN_PWR 3         // Bouton marche/arrêt

// ===== FRÉQUENCES THÉRAPEUTIQUES =====
const float frequencies[] = {2.28, 5.56, 9.12, 18.25, 36.50, 73.0, 146.0};
const char* freqNames[] = {"A", "B", "C", "D", "E", "F", "G"};
const int numFreqs = 7;

// ===== VARIABLES GLOBALES =====
int currentFreq = 0;
bool deviceOn = true;
bool ledState = false;
unsigned long previousMicros = 0;
unsigned long halfPeriodMicros = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_FREQ, INPUT_PULLUP);
  pinMode(BTN_PWR, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.println(F("\n╔═══════════════════════════════════╗"));
  Serial.println(F("║  STYLET AURICULOTHÉRAPIE v1.0     ║"));
  Serial.println(F("╚═══════════════════════════════════╝"));
  Serial.println(F("\nCommandes:"));
  Serial.println(F("- BTN_FREQ (D2) : Changer fréquence"));
  Serial.println(F("- BTN_PWR  (D3) : ON/OFF\n"));
  
  updateFrequency();
  printCurrentFrequency();
  
  digitalWrite(LED_PIN, LOW);
}

void updateFrequency() {
  halfPeriodMicros = (unsigned long)(500000.0 / frequencies[currentFreq]);
}

void printCurrentFrequency() {
  Serial.print(F("► Fréquence "));
  Serial.print(freqNames[currentFreq]);
  Serial.print(F(" : "));
  Serial.print(frequencies[currentFreq], 2);
  Serial.print(F(" Hz (T/2 = "));
  Serial.print(halfPeriodMicros);
  Serial.println(F(" µs)"));
}

void flashFeedback(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }
}

void loop() {
  // ═══ GESTION BOUTON FRÉQUENCE ═══
  static bool lastBtnFreq = HIGH;
  static unsigned long lastDebounceFreq = 0;
  bool btnFreq = digitalRead(BTN_FREQ);
  
  if (btnFreq != lastBtnFreq) {
    lastDebounceFreq = millis();
  }
  
  if ((millis() - lastDebounceFreq) > 50) {
    if (btnFreq == LOW && lastBtnFreq == HIGH) {
      currentFreq = (currentFreq + 1) % numFreqs;
      updateFrequency();
      
      flashFeedback(currentFreq + 1);
      
      printCurrentFrequency();
      
      previousMicros = micros();
      ledState = false;
      
      delay(300);
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
    
    if (currentMicros - previousMicros >= halfPeriodMicros) {
      previousMicros = currentMicros;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}
```

---

## 🎮 Utilisation de la simulation

### 1. Démarrer la simulation
- Cliquez sur le bouton **vert "Start Simulation"** (▶️)
- La LED devrait commencer à clignoter lentement (2.28 Hz)

### 2. Observer le Serial Monitor
- Cliquez sur l'icône **Serial Monitor** (en bas de l'écran)
- Vous verrez les messages de fréquence

### 3. Tester les boutons
- **Cliquez sur le bouton connecté à D2** : Change de fréquence (vous verrez des flashs puis un nouveau clignotement)
- **Cliquez sur le bouton connecté à D3** : Active/désactive le système

### 4. Observer les fréquences
- **Fréquence A (2.28 Hz)** : Clignotement lent visible
- **Fréquence B (5.56 Hz)** : Clignotement moyen
- **Fréquence C (9.12 Hz)** : Clignotement rapide (limite de visibilité)
- **Fréquences D-G (18-146 Hz)** : La LED paraît continue (fusion perceptive)

---

## 📝 Notes importantes sur Wokwi

### Limitations de la simulation
1. **Hautes fréquences** : Au-delà de ~50 Hz, Wokwi peut ne pas rendre parfaitement le clignotement (limitation graphique)
2. **Timing** : La précision temporelle est bonne mais pas parfaite comme sur du matériel réel
3. **Visuel** : À 146 Hz, la LED paraîtra continue dans la simulation

### Avantages
✅ Pas besoin de matériel physique
✅ Modification instantanée du code
✅ Serial Monitor intégré
✅ Pas de risque de griller un composant
✅ Sauvegarde automatique du projet

---

## 🔧 Fichier diagram.json (optionnel)

Si vous voulez créer le circuit automatiquement, voici le fichier de configuration à copier dans l'onglet **"diagram.json"** :

```json
{
  "version": 1,
  "author": "Stylet Auriculothérapie",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-arduino-uno", "id": "uno", "top": 0, "left": 0, "attrs": {} },
    { "type": "wokwi-led", "id": "led1", "top": -57.6, "left": 153.6, "attrs": { "color": "red" } },
    { "type": "wokwi-resistor", "id": "r1", "top": -28.8, "left": 124.8, "attrs": { "value": "220" } },
    { "type": "wokwi-pushbutton", "id": "btn1", "top": 38.4, "left": 230.4, "attrs": { "color": "green", "label": "FREQ" } },
    { "type": "wokwi-pushbutton", "id": "btn2", "top": 105.6, "left": 230.4, "attrs": { "color": "red", "label": "PWR" } }
  ],
  "connections": [
    [ "led1:A", "r1:1", "green", [ "v0" ] ],
    [ "r1:2", "uno:11", "green", [ "v0" ] ],
    [ "led1:C", "uno:GND.2", "black", [ "v0" ] ],
    [ "btn1:1.l", "uno:2", "green", [ "h0" ] ],
    [ "btn1:2.l", "uno:GND.1", "black", [ "h0" ] ],
    [ "btn2:1.l", "uno:3", "red", [ "h0" ] ],
    [ "btn2:2.l", "uno:GND.1", "black", [ "h0" ] ]
  ],
  "dependencies": {}
}
```

---

## 🎯 Test de validation

### Checklist à cocher pendant la simulation :

```
□ La simulation démarre sans erreur
□ Le Serial Monitor affiche le message de bienvenue
□ La LED clignote à 2.28 Hz (lent)
□ Bouton FREQ : change de fréquence (flashs = numéro)
□ Bouton PWR : éteint/rallume la LED
□ Serial Monitor affiche les changements de fréquence
□ Les 7 fréquences sont accessibles (A→G)
```

---

## 🚀 Prochaines étapes

Une fois la simulation validée sur Wokwi :

1. ✅ **Reproduire sur breadboard** avec votre matériel physique
2. ✅ **Tester avec LED 660nm** (commande ~7€)
3. ✅ **Ajouter fibre optique** pour directivité
4. ✅ **Miniaturiser** vers PCB custom

---

## 💡 Astuces Wokwi

### Modifier une fréquence
Dans le code, ligne ~11, vous pouvez modifier :
```cpp
const float frequencies[] = {2.28, 5.56, 9.12, 18.25, 36.50, 73.0, 146.0};
```

### Changer la couleur de la LED
Dans diagram.json, ligne LED, remplacez `"color": "red"` par :
- `"blue"` pour bleu
- `"green"` pour vert
- `"white"` pour blanc

### Sauvegarder votre projet
- Créez un compte Wokwi (gratuit)
- Cliquez sur "Save" pour sauvegarder le projet
- Vous obtiendrez une URL unique à partager

---

## 📞 Support

Si vous rencontrez un problème avec Wokwi, dites-moi à quelle étape vous bloquez !

**Bon test ! 🔬**
