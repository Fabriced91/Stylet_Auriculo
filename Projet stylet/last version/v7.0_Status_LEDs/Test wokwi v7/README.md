# 🧪 Simulation Wokwi - Stylet Auriculothérapie v7.0

## 📋 Description

Cette simulation Wokwi permet de tester le circuit breadboard du stylet auriculothérapie version 7.0 avec la LED Nichia NSPW500CS et le circuit ultra-simplifié (résistance série 47Ω).

## 🎯 Fonctionnalités testées

✓ **Modulation 1kHz** des fréquences de Nogier (2.28Hz à 146Hz)  
✓ **Circuit LED simplifié** avec résistance 47Ω (vs driver complexe v6)  
✓ **3 modes de fonctionnement** :
  - DÉTECTION : Basse intensité (5mA, 25% duty cycle)
  - TRAITEMENT : Haute intensité (20mA, 92% duty cycle)
  - BLANC : Lumière continue non modulée (10mA, 50% duty)  
✓ **Sélection fréquence** via bouton FREQ (7 fréquences A-G)  
✓ **Indicateurs LED RGB** (Rouge, Vert, Bleu)  
✓ **Mesure batterie** simulée via potentiomètre

## 🔌 Connexions

### LED Principale (NSPW500CS simulée)
- **D3 (PWM)** → Résistance 47Ω → LED blanche → GND

### LEDs Indicateurs RGB
- **D11** → Résistance 470Ω → LED Rouge → GND
- **D10** → Résistance 470Ω → LED Verte → GND
- **D9** → Résistance 470Ω → LED Bleue → GND

### Boutons
- **D2** → Bouton FREQ → GND (pull-up interne activé)
- **D4** → Bouton MODE → GND (pull-up interne activé)

### Mesure Batterie (simulée)
- **5V** → Potentiomètre (VCC)
- **Potentiomètre (SIG)** → Résistance 100kΩ → A0
- **Point milieu** → Résistance 33kΩ → GND
- **Potentiomètre (GND)** → GND

## 🚀 Utilisation

### Démarrage
1. Ouvrir le projet sur [Wokwi.com](https://wokwi.com)
2. Charger les fichiers `sketch.ino` et `diagram.json`
3. Cliquer sur "Start Simulation"

### Contrôles
- **Bouton FREQ (vert)** : Cycle à travers les 7 fréquences de Nogier (A→B→C→D→E→F→G→A)
- **Bouton MODE (bleu)** : Cycle à travers les 3 modes (DÉTECTION→TRAITEMENT→BLANC)
- **Potentiomètre** : Simule la tension batterie (tourner pour tester l'indicateur batterie faible)

### Indicateurs LED RGB

| Fréquence | Hz    | Couleur LED      |
|-----------|-------|------------------|
| A         | 2.28  | 🔴 Rouge         |
| B         | 4.56  | 🟢 Vert          |
| C         | 9.12  | 🟡 Jaune (R+G)   |
| D         | 18.25 | 🔵 Bleu          |
| E         | 36.50 | 🟣 Magenta (R+B) |
| F         | 73.00 | 🩵 Cyan (G+B)    |
| G         | 146.00| ⚪ Blanc (R+G+B) |

### Moniteur Série
Ouvrir le moniteur série (115200 bauds) pour voir :
- État d'initialisation
- Changements de fréquence
- Changements de mode
- Mesures de batterie toutes les 5 secondes
- Alertes batterie faible (< 3.5V)

## 🔍 Observations

### LED Principale
- La LED clignote rapidement (976Hz) - imperceptible à l'œil nu
- L'intensité varie doucement selon la fréquence de Nogier sélectionnée
- En mode BLANC, la LED reste constante (pas de modulation)

### Modulation visible
Pour observer la modulation Nogier :
- Sélectionner fréquence G (146Hz) : modulation visible comme pulsation lente
- Fréquences plus basses (A-F) : modulation très lente, quasi-statique

## 📊 Différences avec le circuit final ATmega328P-AU

| Aspect | Simulation Wokwi | Version finale PCB |
|--------|------------------|---------------------|
| CPU | Arduino Uno 16MHz | ATmega328P-AU 8MHz |
| LED indicateurs | Directes (D9-D11) | Via PCF8574 I2C |
| Alimentation | 5V USB | LiPo 3.7V + LDO 3.3V |
| Batterie | Potentiomètre | LiPo 900mAh réelle |
| Consommation | ~40mA | 13-28mA selon mode |

## 🎉 Nouveautés v7.0 vs v6.0

### Circuit Ultra-Simplifié
- ✅ **v6.0** : 11 composants (PWM → Filtre RC → Op-amp → MOSFET → Rsense → LED)
- ✅ **v7.0** : 2 composants (PWM → Résistance 47Ω → LED)
- 📉 **Réduction** : -82% composants !

### Consommation Réduite
- ✅ **v6.0** : 254mA en mode traitement (LED 519A 350mA)
- ✅ **v7.0** : 28mA en mode traitement (LED NSPW500CS 20mA)
- 📉 **Réduction** : -89% consommation !

### Autonomie Augmentée
- ✅ **v6.0** : 3.5h en traitement
- ✅ **v7.0** : 32h en traitement
- 📈 **Amélioration** : ×9 autonomie !

## 🛠️ Paramètres PWM

```cpp
#define CARRIER_FREQ        976.0   // Fréquence porteuse (Hz)
#define MODULATION_DEPTH    0.70    // Profondeur modulation (70%)
#define MIN_INTENSITY       0.20    // Intensité minimum (20%)

// Duty cycles pour LED 20mA NSPW500CS
#define DUTY_DETECTION   64         // 25% → ~5mA
#define DUTY_TRAITEMENT  235        // 92% → ~20mA
#define DUTY_BLANC       128        // 50% → ~10mA
```

## 📝 Notes importantes

### Limitations Wokwi
- ⚠️ Pas de support PCF8574 → LEDs indicateurs en direct
- ⚠️ Fréquence CPU 16MHz fixe → Timer ajusté automatiquement
- ⚠️ Pas de mesure réelle de consommation

### Avantages simulation
- ✅ Test rapide sans matériel
- ✅ Validation du code avant upload
- ✅ Debug via Serial Monitor
- ✅ Modification facile des paramètres

## 🔗 Liens utiles

- [Wokwi Simulator](https://wokwi.com)
- [Documentation ATmega328P](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
- [LED Nichia NSPW500CS Datasheet](https://www.nichia.co.jp/specification/en/product/led/NSPW500CS-E3.pdf)

## 📜 Licence

MIT License - Projet Auriculothérapie 2026
