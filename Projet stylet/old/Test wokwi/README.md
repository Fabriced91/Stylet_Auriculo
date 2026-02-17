# 🎯 STYLET AURICULOTHÉRAPIE - Projet Wokwi

## 📦 Contenu du package

Ce dossier contient tout le nécessaire pour simuler votre stylet d'auriculothérapie sur Wokwi :

- `sketch.ino` : Code Arduino complet
- `diagram.json` : Configuration du circuit (composants + connexions)
- `wokwi.toml` : Configuration du projet Wokwi
- `auriculo_stylet_wokwi.md` : Documentation complète

---

## 🚀 DÉMARRAGE RAPIDE (2 minutes)

### Option A : Import automatique (RECOMMANDÉ)

1. **Allez sur https://wokwi.com**
2. **Créez un compte** (gratuit) si ce n'est pas déjà fait
3. **Cliquez sur "New Project"**
4. **Sélectionnez "Import Project"**
5. **Glissez-déposez** les 3 fichiers : `sketch.ino`, `diagram.json`, `wokwi.toml`
6. **Cliquez sur le bouton vert ▶️** pour lancer la simulation !

### Option B : Création manuelle (si l'import ne fonctionne pas)

#### Étape 1 : Créer le projet
1. Allez sur https://wokwi.com/projects/new/arduino-uno
2. Vous arrivez sur un projet vide avec un Arduino Uno

#### Étape 2 : Copier le code
1. Ouvrez le fichier `sketch.ino` (avec un éditeur de texte)
2. **Sélectionnez tout le code** (Ctrl+A ou Cmd+A)
3. **Copiez-le** (Ctrl+C ou Cmd+C)
4. Dans Wokwi, **supprimez le code existant** et **collez** le vôtre

#### Étape 3 : Ajouter les composants
Cliquez sur le bouton **"+ Add Part"** et ajoutez :

| Composant | Rechercher | Quantité |
|-----------|-----------|----------|
| LED rouge | "LED" puis choisir couleur rouge | 1 |
| Résistance 220Ω | "Resistor" puis mettre 220 | 1 |
| Bouton poussoir | "Pushbutton" | 2 |

#### Étape 4 : Faire les connexions
Faites glisser les fils entre les composants :

**LED (rouge) :**
- Pin longue (anode +) → Résistance 220Ω → Pin D11 Arduino
- Pin courte (cathode -) → GND Arduino

**Bouton 1 (vert, étiqueté "FREQ") :**
- Pin gauche → Pin D2 Arduino
- Pin droite → GND Arduino

**Bouton 2 (rouge, étiqueté "PWR") :**
- Pin gauche → Pin D3 Arduino
- Pin droite → GND Arduino

#### Étape 5 : Lancer !
Cliquez sur le bouton vert **▶️ Start Simulation**

---

## 🎮 UTILISATION

### Démarrage
- Cliquez sur **▶️ Start Simulation** (bouton vert en haut)
- La LED devrait commencer à clignoter **lentement** (2.28 Hz)
- Ouvrez le **Serial Monitor** (icône en bas) pour voir les messages

### Contrôles

| Bouton | Action | Résultat |
|--------|--------|----------|
| **FREQ** (vert) | Clic | Change de fréquence A→B→C→D→E→F→G→A... |
| **PWR** (rouge) | Clic | Active/Désactive le système |

### Feedback visuel
Quand vous changez de fréquence :
- La LED **flashe** N fois (N = numéro de la fréquence)
  - 1 flash = Fréquence A (2.28 Hz)
  - 2 flashs = Fréquence B (5.56 Hz)
  - 7 flashs = Fréquence G (146 Hz)
- Puis elle se met à pulser à la nouvelle fréquence

---

## 📊 FRÉQUENCES DISPONIBLES

| Nom | Fréquence | Période | Visibilité à l'œil |
|-----|-----------|---------|-------------------|
| A | 2.28 Hz | 439 ms | ✓ Clignotement lent |
| B | 5.56 Hz | 180 ms | ✓ Clignotement moyen |
| C | 9.12 Hz | 110 ms | ✓ Clignotement rapide |
| D | 18.25 Hz | 55 ms | ~ Limite de perception |
| E | 36.50 Hz | 27 ms | ✗ Paraît continu |
| F | 73 Hz | 14 ms | ✗ Paraît continu |
| G | 146 Hz | 7 ms | ✗ Paraît continu |

**Note :** À partir de ~20-25 Hz, l'œil humain perçoit la lumière comme continue (phénomène de fusion perceptive).

---

## 🔬 TESTS À RÉALISER

### ✅ Checklist de validation

```
□ La simulation démarre sans erreur
□ Le Serial Monitor affiche le message de bienvenue
□ La LED clignote visiblement à la fréquence A (2.28 Hz)
□ Clic sur FREQ : la LED flashe 2 fois puis passe à fréquence B
□ Clic sur FREQ plusieurs fois : parcours toutes les fréquences
□ À fréquence G (146 Hz) : la LED paraît allumée en continu
□ Clic sur PWR : la LED s'éteint
□ Re-clic sur PWR : la LED se rallume
□ Serial Monitor affiche les changements de fréquence
□ Aucun comportement erratique ou message d'erreur
```

### 📈 Observer les différentes fréquences

1. **Démarrage** : Fréquence A (2.28 Hz) → clignotement lent, bien visible
2. **Appuyez 2 fois sur FREQ** : Fréquence C (9.12 Hz) → clignotement rapide mais encore visible
3. **Appuyez 4 fois de plus** : Fréquence G (146 Hz) → la LED semble allumée en continu

---

## 🐛 DÉPANNAGE

### La simulation ne démarre pas
- **Vérifiez** que vous avez bien collé tout le code (scroll jusqu'en bas)
- **Essayez** de cliquer sur "Stop" puis "Start"
- **Rechargez** la page du navigateur

### La LED ne clignote pas
- **Vérifiez** les connexions :
  - LED anode (+) → Résistance → Pin D11
  - LED cathode (-) → GND
- **Vérifiez** que la résistance est bien 220Ω

### Les boutons ne fonctionnent pas
- **Vérifiez** les connexions :
  - Bouton FREQ : un côté à D2, l'autre à GND
  - Bouton PWR : un côté à D3, l'autre à GND
- **Essayez** de cliquer plusieurs fois

### Le Serial Monitor n'affiche rien
- **Cliquez** sur l'icône Serial Monitor (en bas de l'écran)
- **Vérifiez** que le baudrate est 115200 (menu déroulant)

### La LED clignote trop vite/lent
- C'est **normal** ! Les fréquences varient de 2.28 Hz (lent) à 146 Hz (très rapide)
- Utilisez le bouton FREQ pour changer de fréquence

---

## 📱 MESURE DES FRÉQUENCES (avancé)

Wokwi ne permet pas de mesurer directement les fréquences, mais vous pouvez :

1. **Observer le Serial Monitor** : il affiche la fréquence théorique et la demi-période en microsecondes
2. **Compter manuellement** (pour les basses fréquences) :
   - Chronométrez 10 secondes
   - Comptez le nombre de clignotements
   - Divisez par 10 → vous obtenez la fréquence en Hz

---

## 🎯 PROCHAINES ÉTAPES

Une fois la simulation validée :

1. ✅ **Reproduire sur breadboard** avec votre Arduino physique
2. ✅ **Commander LED 660nm** (longueur d'onde thérapeutique) → ~2€
3. ✅ **Ajouter fibre optique** 2mm pour directivité → ~5€
4. ✅ **Tester sur points auriculaires** (oreille)
5. ✅ **Concevoir le PCB** pour miniaturisation
6. ✅ **Intégrer batterie** LiPo + charge
7. ✅ **Créer boîtier** format stylo (~Ø20mm × 100mm)

---

## 💡 MODIFICATIONS POSSIBLES

### Changer une fréquence
Dans `sketch.ino`, ligne ~18 :
```cpp
const float frequencies[] = {2.28, 5.56, 9.12, 18.25, 36.50, 73.0, 146.0};
```
Modifiez les valeurs selon vos besoins.

### Ajouter une fréquence
1. Ajoutez la valeur dans le tableau `frequencies[]`
2. Ajoutez le nom dans `freqNames[]`
3. Augmentez `numFreqs`

Exemple pour ajouter 10 Hz :
```cpp
const float frequencies[] = {2.28, 5.56, 9.12, 10.0, 18.25, 36.50, 73.0, 146.0};
const char* freqNames[] = {"A", "B", "C", "X", "D", "E", "F", "G"};
const int numFreqs = 8;
```

### Changer la couleur de la LED
1. Dans le simulateur, **cliquez** sur la LED
2. Dans le panneau **"Properties"** à droite
3. Changez **"color"** : red, green, blue, yellow, white

---

## 📚 RESSOURCES

- **Documentation Wokwi** : https://docs.wokwi.com
- **Forum Wokwi** : https://wokwi.com/discord (pour questions)
- **Arduino Reference** : https://www.arduino.cc/reference/en/

---

## 🆘 SUPPORT

Si vous rencontrez un problème :
1. Vérifiez la section **DÉPANNAGE** ci-dessus
2. Consultez le **Serial Monitor** pour les messages d'erreur
3. Comparez votre circuit avec le `diagram.json` fourni

---

**Bonne simulation ! 🚀**

*Version 1.0 - Février 2026*
*Projet Stylet Auriculothérapie*
