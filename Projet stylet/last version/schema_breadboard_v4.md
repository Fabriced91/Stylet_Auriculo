# ═══════════════════════════════════════════════════════════════
#  SCHÉMA BREADBOARD v4.0 - Avec 3ème bouton MODE
# ═══════════════════════════════════════════════════════════════

## 📐 SCHÉMA DE CÂBLAGE COMPLET

```
ARDUINO NANO/UNO
┌──────────────────┐
│    ARDUINO       │
│                  │
│  VIN  ●──────────┼──── [Pile 9V via Jack +]
│                  │
│  GND  ●──────────┼──┬─ [Rail GND commun]
│                  │  │
│  D11  ●──────────┼──┼──[1kΩ]────┐
│  (PWM)           │  │            │
│                  │  │         [MOSFET]
│  D10  ●──────────┼──┼──[470Ω]─┐ Gate
│                  │  │         │  │
│  D9   ●──────────┼──┼──[470Ω]─┼┐ Drain
│                  │  │         ││ │
│  D8   ●──────────┼──┼──[470Ω]─┼┼┐
│                  │  │         │││
│  D7   ●──────────┼──┼──[470Ω]─┼┼┼┐
│  (LED MODE)      │  │         ││││
│                  │  │    [LED_MODE Blanc]
│  D4   ●──────────┼──┼────[BTN_MODE]────┐││
│  (BTN MODE)      │  │                  │││
│                  │  │    ┌─────────────┘││
│  D3   ●──────────┼──┼────[BTN_INTENS]──┐││
│  (BTN INTENS)    │  │                  │││
│                  │  │    ┌─────────────┘││
│  D2   ●──────────┼──┼────[BTN_FREQ]────┼││
│  (BTN FREQ)      │  │                  ││││
│                  │  │                  ││││
└──────────────────┘  │         │        ││││
                      │    [LED_A Rouge] ││││
                      │         │        ││││
                      │    [LED_B Verte] ││││
                      │         │        ││││
                      │    [LED_C Bleue] ││││
                      │         │         │││
                      │    [Nichia LED+]  │││
                      │         │         │││
    [47Ω résistance]──┤         │         │││
                      │    [LED- toutes]  │││
                      │         │         │││
                      └─────────┴─────────┴┴┴─── [Rail GND]
```

## 🔌 CONNEXIONS DÉTAILLÉES

### LED Principale (Nichia NSPW500CS)

```
Arduino D11 (PWM)
    │
    └──[1kΩ résistance]── Gate MOSFET (2N7000 ou 2N2222)
                              │
                          Drain/Collecteur
                              │
    Arduino 5V ──[47Ω]──[LED Nichia +]──[LED Nichia -]──┤
                                                         │
                                                   Source/Émetteur
                                                         │
                                                        GND
```

**Alternative simplifiée (sans transistor pour test rapide) :**
```
Arduino D11 ──[47Ω]──[LED Nichia +]──[LED Nichia -]── GND
```

### LED Indicateurs Fréquence (RGB)

```
Arduino D10 ──[470Ω]──[LED Rouge +]──[LED Rouge -]── GND

Arduino D9  ──[470Ω]──[LED Verte +]──[LED Verte -]── GND

Arduino D8  ──[470Ω]──[LED Bleue +]──[LED Bleue -]── GND
```

### LED Indicateur Mode

```
Arduino D7  ──[470Ω]──[LED Blanche +]──[LED Blanche -]── GND

Note : Peut être n'importe quelle couleur de LED
Suggestion : LED blanche ou jaune pour différencier
```

### Boutons

```
BTN_FREQ (Sélection fréquence) :
    Arduino D2 ────┬──── [Bouton poussoir] ──── GND
                   │
              (pull-up interne Arduino activé)

BTN_INTENSITY (Réglage intensité) :
    Arduino D3 ────┬──── [Bouton poussoir] ──── GND
                   │
              (pull-up interne Arduino activé)

BTN_MODE (Sélection modulation) :
    Arduino D4 ────┬──── [Bouton poussoir] ──── GND
                   │
              (pull-up interne Arduino activé)

Note : Les résistances de pull-up ne sont PAS nécessaires
       car on utilise INPUT_PULLUP dans le code
```

### Alimentation

```
[Pile 9V +] ──── VIN Arduino
[Pile 9V -] ──── GND Arduino
```

## 🎨 DISPOSITION BREADBOARD RECOMMANDÉE

```
═══════════════════════════════════════════════════════════════
                        BREADBOARD 400 POINTS
═══════════════════════════════════════════════════════════════

Rail + (rouge) ────────────────────────────────────────────────
                    │
              [Pile 9V +]
                    │
Rail - (bleu)  ─────┴──────────────────────────────────────────

    Colonne A-E             Colonne F-J
    
    [Arduino Nano]
    ┌────────────┐
    │ D11 D10 D9 │ D8 D7 ... D4 D3 D2
    └─┬───┬──┬───┘  │  │      │  │  │
      │   │  │      │  │      │  │  │
    [1kΩ]│  │      │  │      │  │  │
      │   │  │      │  │      │  │  │
      │ [470Ω]     │  │      │  │  │
      │   │[470Ω]  │  │      │  │  │
      │   │  │[470Ω]│[470Ω]  │  │  │
      │   │  │  │   │  │     │  │  │
   [MOSFET]│  │  │  │  │     │  │  │
      D│   │  │  │  │  │     │  │  │
      ├─┐  │  │  │  │  │     │  │  │
   [47Ω] │ │  │  │  │  │     │  │  │
      │  S │  │  │  │  │     │  │  │
      │  │ │  │  │  │  │     │  │  │
  [LED+] │ │  │  │  │  │     │  │  │
  Nichia │ │  │  │  │  │     │  │  │
      │  │ │  │  │  │  │     │  │  │
  [LED-] │[A][B][C][M]│       │  │  │
      │  │ │  │  │  │  │   [BTN][BTN][BTN]
      │  │ │  │  │  │  │   MODE INT FREQ
      └──┴─┴──┴──┴──┴──┴─────┴──┴──┴───Rail - (GND)

Légende :
[A] = LED Rouge (Indicateur A)
[B] = LED Verte (Indicateur B)
[C] = LED Bleue (Indicateur C)
[M] = LED Mode (Blanche/Jaune)
```

## 📋 CHECKLIST DE MONTAGE

```
ÉTAPE 1 : PRÉPARATION
□ Sortir breadboard propre
□ Vérifier Arduino fonctionne (upload blink test)
□ Trier composants sur table

ÉTAPE 2 : ALIMENTATION
□ Connecter pile 9V au VIN Arduino
□ Connecter GND pile au GND Arduino
□ Vérifier LED power Arduino s'allume

ÉTAPE 3 : LED PRINCIPALE
□ Insérer transistor 2N7000 sur breadboard
□ Connecter D11 → [1kΩ] → Gate transistor
□ Connecter Source transistor → GND
□ Connecter Drain → [47Ω] → LED Nichia anode (+)
□ Connecter LED Nichia cathode (-) → GND
□ ⚠️ VÉRIFIER POLARITÉ LED (patte longue = +)

ÉTAPE 4 : LED INDICATEURS FRÉQUENCE
□ D10 → [470Ω] → LED Rouge (+) → (-) GND
□ D9  → [470Ω] → LED Verte (+) → (-) GND
□ D8  → [470Ω] → LED Bleue (+) → (-) GND

ÉTAPE 5 : LED INDICATEUR MODE
□ D7  → [470Ω] → LED Blanche (+) → (-) GND

ÉTAPE 6 : BOUTONS
□ D2 → Bouton FREQ → GND
□ D3 → Bouton INTENSITY → GND
□ D4 → Bouton MODE → GND

ÉTAPE 7 : VÉRIFICATION FINALE
□ Vérifier aucun court-circuit (multimètre)
□ Vérifier toutes les LED ont résistances
□ Vérifier polarité toutes les LED
□ Vérifier transistor dans bon sens
```

## 🧪 TESTS DE VALIDATION

### TEST 1 : Upload code (2 min)
```
1. Brancher Arduino en USB
2. Uploader le code v4.0
3. Ouvrir Serial Monitor (115200 bauds)
4. Vérifier message de bienvenue s'affiche
```

### TEST 2 : LED indicateurs (2 min)
```
1. Au démarrage :
   ✓ LED Rouge allumée (Fréquence A)
   ✓ LED Mode clignote lentement (Mode SINE)
2. Appuyer BTN_FREQ :
   ✓ LED passe au Vert (Fréquence B)
3. Continuer appuis BTN_FREQ :
   ✓ Parcourir toutes couleurs (R→G→Jaune→Bleu→Magenta→Cyan→Blanc)
```

### TEST 3 : LED principale (3 min)
```
1. Observer LED Nichia :
   ✓ Pulsation douce (mode SINE par défaut)
   ✓ Fréquence 2.28 Hz visible (lent)
2. Appuyer BTN_FREQ plusieurs fois :
   ✓ Fréquence augmente progressivement
   ✓ À 146 Hz, LED paraît continue
```

### TEST 4 : Intensité (2 min)
```
1. Appuyer BTN_INTENSITY :
   ✓ LED devient plus lumineuse
2. Continuer appuis :
   ✓ Cycle 10% → 20% → ... → 100% → 10%
```

### TEST 5 : Modes de modulation (3 min)
```
1. Régler sur Fréquence A (2.28 Hz, bien visible)
2. Appuyer BTN_MODE :
   ✓ 1 flash long → Mode SINE
   ✓ Pulsation douce, confortable
3. Appuyer BTN_MODE :
   ✓ 2 flashs moyens → Mode AM
   ✓ Lumière paraît plus continue
4. Appuyer BTN_MODE :
   ✓ 3 flashs courts → Mode CARRÉ
   ✓ Clignotement brutal (désagréable)
5. Re-appuyer BTN_MODE :
   ✓ Retour mode SINE (confortable)
```

### TEST 6 : Indicateur mode LED (2 min)
```
1. Observer LED Mode (D7) :
   ✓ Mode SINE : Clignote LENT (1 Hz)
   ✓ Mode AM : Clignote MOYEN (3 Hz)
   ✓ Mode CARRÉ : Clignote RAPIDE (5 Hz)
```

## 🎯 TABLEAU DE VALIDATION

```
┌────────────────────────────────────────────────────────────┐
│  VALIDATION PROTOTYPE BREADBOARD v4.0                      │
├────────────────────────────────────────────────────────────┤
│                                                            │
│ UPLOAD & DÉMARRAGE                                         │
│ □ Code uploadé sans erreur                                │
│ □ Serial Monitor affiche bannière v4.0                    │
│ □ Arduino alimenté (LED power ON)                         │
│                                                            │
│ TEST LED INDICATEURS FRÉQUENCE                             │
│ □ Freq A : LED Rouge                                      │
│ □ Freq B : LED Verte                                      │
│ □ Freq C : LED Jaune (R+G)                                │
│ □ Freq D : LED Bleue                                      │
│ □ Freq E : LED Magenta (R+B)                              │
│ □ Freq F : LED Cyan (G+B)                                 │
│ □ Freq G : LED Blanche (R+G+B)                            │
│                                                            │
│ TEST LED PRINCIPALE                                        │
│ □ LED Nichia s'allume                                     │
│ □ Pulsation visible à 2.28 Hz                             │
│ □ Pulsation invisible à 146 Hz                            │
│                                                            │
│ TEST INTENSITÉ                                             │
│ □ 10% : LED très faible                                   │
│ □ 50% : LED moyenne                                       │
│ □ 100% : LED maximale                                     │
│                                                            │
│ TEST MODES MODULATION                                      │
│ □ Mode SINE : Pulsation douce, confortable                │
│ □ Mode AM : Lumière plus continue                         │
│ □ Mode CARRÉ : Clignotement brutal                        │
│                                                            │
│ TEST LED INDICATEUR MODE                                   │
│ □ Mode SINE : Clignote lent (1 Hz)                        │
│ □ Mode AM : Clignote moyen (3 Hz)                         │
│ □ Mode CARRÉ : Clignote rapide (5 Hz)                     │
│                                                            │
│ TEST STABILITÉ                                             │
│ □ Aucun comportement erratique                            │
│ □ Aucun échauffement excessif                             │
│ □ Fonctionne stable sur 5 minutes                         │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

## ⚠️ DÉPANNAGE

### LED principale ne s'allume pas
```
1. Vérifier polarité LED Nichia (longue patte = +)
2. Vérifier connexion transistor (bon sens)
3. Tester LED en direct : D11 → [47Ω] → LED → GND
4. Mesurer tension sur Gate transistor (~3V quand actif)
```

### LED indicateurs ne s'allument pas
```
1. Vérifier polarité (patte longue = +)
2. Vérifier résistances 470Ω présentes
3. Tester en direct : 5V → [470Ω] → LED → GND
```

### Boutons ne répondent pas
```
1. Vérifier câblage : Pin → Bouton → GND
2. Tester bouton avec multimètre (continuité)
3. Vérifier pas de résistances en série (INPUT_PULLUP suffit)
```

### Serial Monitor n'affiche rien
```
1. Vérifier baudrate : 115200
2. Débrancher/rebrancher USB
3. Vérifier bon port COM sélectionné
```

═══════════════════════════════════════════════════════════════
