# 🔌 GUIDE DÉTAILLÉ DE CÂBLAGE - Circuit Batterie Wokwi

## ⚡ Connexions du Circuit de Mesure Batterie

### 📋 Vue d'Ensemble

Le circuit de mesure batterie utilise :
- **1× Potentiomètre** (simule la tension de batterie variable)
- **1× Résistance 100kΩ** (diviseur haut)
- **1× Résistance 33kΩ** (diviseur bas)
- **Connexion vers A0** (entrée ADC Arduino)

### 🎯 Schéma ASCII Détaillé

```
                    Arduino Uno
                ┌─────────────────┐
                │                 │
           ┌────┤ 5V              │
           │    │                 │
           │    │                 │
      [Potentiomètre]             │
       "Batterie Sim"             │
           │                      │
       VCC │ (borne +)            │
           │                      │
       SIG ├──────────────┐       │
           │              │       │
           │              │       │
       GND ├──────┐       │       │
           │      │       │       │
           └──────┼───────┼───────┼── GND
                  │       │       │
                  │       │       │
                  │    [100kΩ]    │
                  │   R_bat_high  │
                  │       │       │
                  │       │       │
                  │   ┌───┴───┐   │
                  │   │POINT  │   │
                  │   │MILIEU ├───┤ A0
                  │   │       │   │
                  │   └───┬───┘   │
                  │       │       │
                  │    [33kΩ]     │
                  │   R_bat_low   │
                  │       │       │
                  └───────┴───────┘
```

### 🔍 Détail des Connexions

#### Connexion 1 : Potentiomètre VCC
```
Arduino 5V ──────→ Potentiomètre VCC (borne +, généralement à droite)
```
✅ **Rôle** : Simule la borne positive de la batterie (jusqu'à 5V)

#### Connexion 2 : Potentiomètre GND
```
Arduino GND ─────→ Potentiomètre GND (borne -, généralement à gauche)
```
✅ **Rôle** : Référence de masse

#### Connexion 3 : Potentiomètre SIG → Diviseur
```
Potentiomètre SIG (curseur central) ──→ Résistance 100kΩ (patte 1)
```
✅ **Rôle** : La tension variable du curseur entre dans le diviseur

#### Connexion 4 : Point Milieu (3 fils !)
```
Résistance 100kΩ (patte 2) ──┬──→ Résistance 33kΩ (patte 1)
                             │
                             └──→ Arduino A0
```
✅ **Rôle** : Ce point est connecté à **3 endroits** :
   - Sortie de R_100kΩ (patte 2)
   - Entrée de R_33kΩ (patte 1)
   - **Entrée A0 de l'Arduino** ← MESURE ICI

#### Connexion 5 : Diviseur vers GND
```
Résistance 33kΩ (patte 2) ──→ Arduino GND
```
✅ **Rôle** : Complète le diviseur de tension vers la masse

## 📊 Tableau Récapitulatif

| Composant | Borne | Se Connecte À |
|-----------|-------|---------------|
| **Potentiomètre** | VCC (droite) | 5V Arduino |
| **Potentiomètre** | GND (gauche) | GND Arduino |
| **Potentiomètre** | SIG (centre) | R_100kΩ patte 1 |
| **R_100kΩ** | Patte 1 | Pot SIG |
| **R_100kΩ** | Patte 2 | Point Milieu (jonction 3 fils) |
| **Point Milieu** | - | A0 Arduino |
| **Point Milieu** | - | R_33kΩ patte 1 |
| **R_33kΩ** | Patte 1 | Point Milieu |
| **R_33kΩ** | Patte 2 | GND Arduino |

## 🎨 Code Couleur des Fils (Suggestion)

Pour faciliter le câblage :

```
5V → Potentiomètre VCC        : Fil ROUGE
GND → Potentiomètre GND       : Fil NOIR
Pot SIG → R_100kΩ             : Fil ORANGE
R_100kΩ → Point Milieu        : Fil ORANGE (continuation)
Point Milieu → R_33kΩ         : Fil ORANGE (continuation)
Point Milieu → A0             : Fil JAUNE (branche)
R_33kΩ → GND                  : Fil NOIR
```

## 🧪 Test de Fonctionnement

### Étape 1 : Vérifier les Tensions

Avec un multimètre virtuel Wokwi ou le Serial Monitor :

1. **Potentiomètre au minimum** (tourné vers GND)
   - Tension Pot SIG : **~0V**
   - Tension A0 : **~0V**
   - Serial affiche : `Batterie : ~0 mV` ⚠️ BATTERIE FAIBLE

2. **Potentiomètre au maximum** (tourné vers VCC)
   - Tension Pot SIG : **~5V**
   - Tension A0 : **~1.24V** (divisé par 4.03)
   - Serial affiche : `Batterie : ~5000 mV`

3. **Potentiomètre au milieu** (position médiane)
   - Tension Pot SIG : **~2.5V**
   - Tension A0 : **~0.62V**
   - Serial affiche : `Batterie : ~2500 mV`

### Étape 2 : Simuler États Batterie LiPo

Pour simuler les différents états de charge :

| État Batterie | Tension Réelle | Tension A0 Attendue | Position Pot | Affichage Serial |
|---------------|----------------|---------------------|--------------|------------------|
| Vide (3.0V)   | 3.0V | 0.74V | 60% | `Batterie : 3000 mV ⚠️` |
| Faible (3.5V) | 3.5V | 0.87V | 70% | `Batterie : 3500 mV ⚠️` |
| Normale (3.7V)| 3.7V | 0.92V | 74% | `Batterie : 3700 mV` |
| Pleine (4.2V) | 4.2V | 1.04V | 84% | `Batterie : 4200 mV` |
| Max simu (5V) | 5.0V | 1.24V | 100% | `Batterie : 5000 mV` |

## 🐛 Dépannage

### Problème : Serial affiche toujours 0 mV

✅ **Solutions** :
1. Vérifier que le **Point Milieu est connecté à A0**
2. Vérifier que le potentiomètre n'est pas à zéro
3. Vérifier que VCC du pot est bien sur 5V

### Problème : Serial affiche toujours 5000 mV (max)

✅ **Solutions** :
1. Vérifier que les résistances sont bien en série (100k puis 33k)
2. Vérifier que R_33kΩ est bien connectée à GND
3. Vérifier les valeurs des résistances (pas inversées)

### Problème : Tension ne change pas quand je tourne le pot

✅ **Solutions** :
1. Vérifier que VCC et GND du pot sont connectés
2. Vérifier que SIG du pot est connecté au diviseur
3. Essayer un autre potentiomètre dans Wokwi

### Problème : Valeurs de tension incohérentes

✅ **Solutions** :
1. Vérifier le calcul du diviseur dans le code (ligne ~450 du sketch.ino)
2. S'assurer que Vref = 5V pour Wokwi (ligne ~447)
3. Vérifier que le facteur 4.03 est correct

## 💡 Pourquoi ce Circuit ?

### Contexte Projet Réel (PCB final)
Dans le vrai stylet :
- Batterie LiPo : **3.0V à 4.2V**
- Vref ADC : **3.3V** (régulé par LDO)
- Sans diviseur, 4.2V > 3.3V → **risque de griller l'ADC !**

### Diviseur de Tension
Le diviseur **100kΩ / 33kΩ** ramène la tension :
- Facteur : 33k / (100k + 33k) = **0.248**
- 4.2V × 0.248 = **1.04V** < 3.3V ✅ Sécurisé !

### Simulation Wokwi
- Arduino Uno : Vref = **5V** (vs 3.3V du projet réel)
- On garde le même diviseur pour cohérence
- Le potentiomètre remplace la batterie variable

## 📐 Formule de Conversion

### Dans le Code (sketch.ino)
```cpp
uint16_t voltage = (uint32_t)adcValue * 5000 * 403 / 102300;
```

### Explication
1. `adcValue` : Valeur brute ADC (0-1023)
2. `× 5000` : Conversion vers mV (Vref = 5V)
3. `× 403` : Multiplication par facteur diviseur (4.03)
4. `÷ 102300` : Division combinée (1023 × 100)

### Exemple Calcul
- ADC lit **213** (valeur brute)
- Tension A0 = 213 × 5V / 1023 = **1.04V**
- Tension batterie = 1.04V × 4.03 = **4.19V** ✅ Batterie pleine !

## ✨ Astuces Wokwi

### Astuce 1 : Ajuster le Potentiomètre Précisément
- Clic droit sur le pot → "Edit Properties"
- Entrer une valeur exacte (0-100%)
- Pour 4.2V : entrer **84%**

### Astuce 2 : Ajouter un Voltmètre Virtuel
1. Ajouter un composant "Voltmeter"
2. Connecter aux bornes du Point Milieu et GND
3. Observer la tension en temps réel

### Astuce 3 : Debug via Serial
Le code affiche la tension toutes les 5 secondes :
```
🔋 Batterie : 4200 mV
🔋 Batterie : 4150 mV
🔋 Batterie : 3480 mV ⚠️ BATTERIE FAIBLE
```

---

**Circuit validé ✅**  
**Guide v7.0 - Février 2026**
