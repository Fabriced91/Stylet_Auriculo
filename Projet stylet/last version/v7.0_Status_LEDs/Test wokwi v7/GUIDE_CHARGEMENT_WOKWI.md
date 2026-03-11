# 🚀 Guide de Chargement sur Wokwi

## Méthode 1 : Importation Directe (Recommandée)

### Étape 1 : Ouvrir Wokwi
1. Aller sur [wokwi.com](https://wokwi.com)
2. Cliquer sur **"Start a new project"**
3. Sélectionner **"Arduino Uno"**

### Étape 2 : Remplacer le code
1. Supprimer le code par défaut dans l'éditeur
2. Ouvrir le fichier `sketch.ino` de ce dossier
3. Copier tout le contenu
4. Coller dans l'éditeur Wokwi

### Étape 3 : Configurer le circuit
1. Cliquer sur l'onglet **"diagram.json"** dans Wokwi
2. Supprimer le contenu par défaut
3. Ouvrir le fichier `diagram.json` de ce dossier
4. Copier tout le contenu
5. Coller dans l'éditeur Wokwi

### Étape 4 : Démarrer la simulation
1. Cliquer sur **"Start Simulation"** (bouton vert ▶️)
2. Ouvrir le **Serial Monitor** pour voir les messages de debug
3. Tester les boutons FREQ et MODE

## Méthode 2 : Chargement via URL (Si compte Wokwi)

### Prérequis
- Avoir un compte Wokwi (gratuit)
- Être connecté

### Étapes
1. Créer un nouveau projet Arduino Uno sur Wokwi
2. Sauvegarder le projet (Ctrl+S ou bouton "Save")
3. Copier les fichiers `sketch.ino` et `diagram.json`
4. Dans Wokwi, aller dans **File → Import**
5. Sélectionner les deux fichiers
6. Confirmer l'importation

## Méthode 3 : Partage de Projet (Pour collaboration)

### Créateur du projet
1. Suivre Méthode 1 ou 2
2. Cliquer sur **"Share"** (icône de partage)
3. Copier le lien généré (format : `https://wokwi.com/projects/XXXXXXXXXXXXXX`)
4. Partager le lien

### Utilisateurs du projet partagé
1. Cliquer sur le lien partagé
2. La simulation s'ouvre directement
3. Possibilité de **"Duplicate"** pour modifier sans affecter l'original

## 🎮 Test de la Simulation

### 1. Vérification initiale
Au démarrage, vous devriez voir :
- ✅ LED principale clignote 3× rapidement (boot OK)
- ✅ LED Rouge allumée (fréquence A par défaut)
- ✅ Serial Monitor affiche :
  ```
  ╔═══════════════════════════════════════════════════════════╗
  ║  STYLET AURICULOTHERAPIE v7.0 - SIMULATION WOKWI         ║
  ║  LED Nichia NSPW500CS - Circuit Ultra-Simplifié          ║
  ╚═══════════════════════════════════════════════════════════╝
  
  ✓ Initialisation terminée
  ✓ Fréquence initiale : A (2.28 Hz)
  ✓ Mode initial : DETECTION
  ✓ Batterie : XXXX mV
  ```

### 2. Test bouton FREQ
1. Cliquer sur le bouton **FREQ** (vert)
2. Observer :
   - LED indicateur change de couleur (Rouge → Vert → Jaune → Bleu → Magenta → Cyan → Blanc)
   - LED principale clignote 2× rapidement (feedback)
   - Serial Monitor affiche : `📡 Fréquence : X (X.XX Hz)`

### 3. Test bouton MODE
1. Cliquer sur le bouton **MODE** (bleu)
2. Observer :
   - LED principale change d'intensité
   - Nombre de flashs de feedback change (1, 2 ou 3)
   - Serial Monitor affiche : `⚙️ Mode : XXXXX`

| Mode | Intensité | Flashs |
|------|-----------|--------|
| DÉTECTION | Basse (25%) | 1 flash long |
| TRAITEMENT | Haute (92%) | 2 flashs moyens |
| BLANC | Moyenne (50%) constante | 3 flashs courts |

### 4. Test mesure batterie
1. Tourner le **potentiomètre "Batterie Sim"**
2. Attendre 5 secondes
3. Observer Serial Monitor : `🔋 Batterie : XXXX mV`
4. Si batterie < 3.5V : message `⚠️ BATTERIE FAIBLE` + LED rouge clignote 3×

### 5. Test modulation Nogier
Pour observer la modulation lente :
1. Sélectionner fréquence **G (146Hz)** avec bouton FREQ
2. Mode **DÉTECTION** ou **TRAITEMENT**
3. Observer LED principale : pulsation lente visible (~7× par seconde)

Fréquences plus basses (A-F) ont modulation quasi-imperceptible (trop lente).

## 🐛 Dépannage

### LED principale ne s'allume pas
- ✅ Vérifier que la simulation est démarrée (bouton ▶️ vert)
- ✅ Changer de mode avec bouton MODE
- ✅ Vérifier que le Serial Monitor affiche les messages de boot

### LEDs indicateurs ne changent pas
- ✅ Cliquer plusieurs fois sur bouton FREQ
- ✅ Vérifier dans Serial Monitor que le message "Fréquence : X" s'affiche
- ✅ Recharger la page et recommencer

### Boutons ne répondent pas
- ✅ Attendre 200ms entre chaque clic (anti-rebond)
- ✅ Vérifier que les boutons sont bien câblés (voir `diagram.json`)
- ✅ Ouvrir Serial Monitor pour confirmer que les clics sont détectés

### Serial Monitor vide
- ✅ Cliquer sur l'icône **"Serial Monitor"** en bas de l'écran
- ✅ Vérifier que le baud rate est **115200**
- ✅ Redémarrer la simulation

### Circuit semble incorrect
- ✅ Comparer avec l'image ci-dessous (schéma attendu)
- ✅ Vérifier que `diagram.json` a été correctement copié
- ✅ Supprimer tous les composants et recharger `diagram.json`

## 📸 Schéma Circuit Attendu

```
         Arduino Uno
    ┌──────────────────┐
    │                  │
    │  D3 ──[47Ω]──[LED_Blanc]──GND
    │                  │
    │  D11──[470Ω]─[LED_Rouge]──GND
    │  D10──[470Ω]─[LED_Verte]──GND
    │  D9 ──[470Ω]─[LED_Bleue]──GND
    │                  │
    │  D2 ──[BTN_FREQ]──GND
    │  D4 ──[BTN_MODE]──GND
    │                  │
    │  A0 ──[100kΩ]──[Point milieu diviseur]
    │                  │
    │               [33kΩ]──GND
    │                  │
    │  5V──[Potentiomètre]──GND
    │         (milieu) ─┘
    │                  │
    └──────────────────┘
```

## 💡 Astuces

### Modifier les paramètres
Pour expérimenter avec différents réglages :

1. **Changer duty cycles** (lignes 71-73 de `sketch.ino`) :
   ```cpp
   #define DUTY_DETECTION   64    // Essayer 32, 96, 128...
   #define DUTY_TRAITEMENT  235   // Essayer 180, 200, 255...
   #define DUTY_BLANC       128   // Essayer 64, 192...
   ```

2. **Changer fréquences Nogier** (lignes 31-39) :
   ```cpp
   const float frequencies[] = {
     2.28,   // Doubler pour voir modulation plus rapide
     // ...
   };
   ```

3. **Changer profondeur modulation** (ligne 66) :
   ```cpp
   #define MODULATION_DEPTH    0.70  // Essayer 0.5, 0.9...
   ```

Après modification :
- Sauvegarder (Ctrl+S)
- Redémarrer simulation (Stop puis Start)

### Mesurer consommation (estimation)
- Mode DÉTECTION : ~13mA (LED 5mA + ATmega 8mA)
- Mode TRAITEMENT : ~28mA (LED 20mA + ATmega 8mA)
- Mode BLANC : ~18mA (LED 10mA + ATmega 8mA)

### Autonomie calculée (batterie 900mAh)
- Mode DÉTECTION : 900/13 = **69 heures**
- Mode TRAITEMENT : 900/28 = **32 heures**
- Mode BLANC : 900/18 = **50 heures**

## 🎓 Pour aller plus loin

### Ajouter un oscilloscope virtuel
1. Dans Wokwi, cliquer sur **"+"** (Add Part)
2. Chercher **"Logic Analyzer"**
3. Connecter à **D3** (LED PWM)
4. Observer le signal PWM 976Hz

### Ajouter un affichage LCD (optionnel)
1. Ajouter composant **"LCD 16×2 (I2C)"**
2. Connecter SDA à A4, SCL à A5
3. Modifier code pour afficher fréquence/mode

### Exporter le code pour upload Arduino réel
1. Copier `sketch.ino`
2. Ouvrir Arduino IDE
3. Coller et compiler
4. Upload vers Arduino Uno physique
5. Câbler selon `diagram.json`

## 📧 Support

Pour toute question ou problème :
1. Vérifier le [README.md](README.md) du projet
2. Consulter la [documentation Wokwi](https://docs.wokwi.com)
3. Vérifier les fichiers sources dans `last version/`

---

**Bon test ! 🚀**

_Version du guide : v7.0 - Février 2026_
