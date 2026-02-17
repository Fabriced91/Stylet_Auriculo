# ═══════════════════════════════════════════════════════════════
#  GUIDE KICAD COMPLET - PCB Rectangulaire 15×70mm v4.0
# ═══════════════════════════════════════════════════════════════

## 🎯 OBJECTIF

Créer le PCB rectangulaire 15×70mm pour le stylet d'auriculothérapie avec :
- Schéma électrique complet
- PCB routé 2 couches
- Fichiers Gerber prêts pour fabrication JLCPCB
- BOM pour assemblage

---

## 📋 PRÉREQUIS

### Logiciels installés
```
✅ KiCad 7.x ou 8.x (dernière version stable)
✅ Éditeur de texte (pour édition symboles si besoin)
```

### Connaissances de base KiCad
```
- Navigation interface (zoom, pan)
- Placement composants
- Routage pistes basique
- Génération Gerber

Si débutant : Prévoir 2-3h apprentissage sur tutoriel officiel
```

---

## 🚀 ÉTAPE 1 : CRÉATION DU PROJET (5 min)

### 1.1 Nouveau projet

```
1. Lancer KiCad
2. File → New Project
3. Nom : "Stylet_Auriculo_v4"
4. Location : Choisir dossier projet
5. Cliquer "Create Project"
```

**Fichiers créés :**
- `Stylet_Auriculo_v4.kicad_pro` (projet)
- `Stylet_Auriculo_v4.kicad_sch` (schéma)
- `Stylet_Auriculo_v4.kicad_pcb` (PCB)

### 1.2 Configuration projet

```
1. Dans KiCad Manager, clic droit sur projet
2. Project → Project Setup
3. Onglet "General" :
   - Title : Stylet Auriculothérapie v4.0
   - Date : [Date actuelle]
   - Revision : 1.0
   - Company : [Votre nom]
4. OK
```

---

## 📐 ÉTAPE 2 : SCHÉMA ÉLECTRIQUE (1-2h)

### 2.1 Ouvrir l'éditeur de schéma

```
Dans KiCad Manager :
- Clic sur "Schematic Editor" ou touche E
```

### 2.2 Configurer la grille

```
View → Grid Settings
- Grid : 50 mil (1.27mm) pour placement général
        ou 25 mil pour précision
```

### 2.3 Ajouter les bibliothèques nécessaires

```
Preferences → Manage Symbol Libraries
Vérifier que ces bibliothèques sont actives :
✅ Device (résistances, condensateurs)
✅ MCU_Microchip_ATmega (ATmega328P)
✅ Transistor_FET (MOSFET 2N7002)
✅ Regulator_Linear (HT7333)
✅ Battery_Management (TP4056)
✅ LED (LED standard)
✅ Switch (boutons, switch)
✅ Connector (USB, headers)

Si manquant : Add → Browser → Sélectionner .kicad_sym
```

---

## 🎨 ÉTAPE 2.4 : PLACEMENT DES SYMBOLES (45 min)

### Zone 1 : Alimentation

**1. ATmega328P (U1)**
```
Touche A (Add Symbol) ou clic sur icône "Add Symbol"
Recherche : "ATmega328P"
Choisir : "ATmega328P-AU" (TQFP-32)
Placer au centre de la feuille
Référence : U1
Value : ATmega328P-AU
```

**2. Régulateur 3.3V (U2)**
```
Add Symbol → "HT7333"
Si pas trouvé : Utiliser "AMS1117-3.3"
Placer à gauche de U1
Référence : U2
Value : HT7333-A
```

**3. Chargeur LiPo (U3)**
```
Add Symbol → "TP4056"
Placer en haut à gauche
Référence : U3
Value : TP4056
```

**4. Condensateurs découplage**
```
Add Symbol → "C"
Placer 6 condensateurs autour de U1 et U2 :
- C1 : 10µF (près U2 input)
- C2 : 100nF (près U2 output)
- C3 : 10µF (près U1 VCC)
- C4 : 100nF (près U1 VCC)
- C5 : 10µF (près U1 AVCC)
- C6 : 100nF (près U1 AVCC)

Value : 10uF ou 100nF selon
```

---

### Zone 2 : Driver LED principale

**5. MOSFET (Q1)**
```
Add Symbol → "2N7002" ou "2N7000"
Bibliothèque : Transistor_FET
Placer en haut à droite
Référence : Q1
Value : 2N7002
```

**6. Résistances driver**
```
Add Symbol → "R"
- R1 : 47Ω (série LED)
- R2 : 1kΩ (gate MOSFET)

Placer près de Q1
```

**7. Connecteur LED principale**
```
Add Symbol → "Conn_01x02"
Bibliothèque : Connector_Generic
Référence : J3
Value : LED_MAIN
```

---

### Zone 3 : LED Indicateurs

**8. LED RGB + Mode (D5-D8)**
```
Add Symbol → "LED"
Placer 4 LED :
- D5 : LED_Rouge
- D6 : LED_Verte
- D7 : LED_Bleue
- D8 : LED_Mode

Ajouter résistances série :
- R3, R4, R5, R6 : 470Ω
```

---

### Zone 4 : Boutons

**9. Boutons (SW1-SW3)**
```
Add Symbol → "SW_Push"
Bibliothèque : Switch
- SW1 : BTN_FREQ
- SW2 : BTN_INTENS
- SW3 : BTN_MODE

Ajouter résistances pull-up :
- R7, R8, R9 : 10kΩ
```

**10. Switch ON/OFF (SW4)**
```
Add Symbol → "SW_SPDT" ou "SW_SPST"
Référence : SW4
Value : SS12D00
```

---

### Zone 5 : Connecteurs

**11. Micro-USB (J1)**
```
Add Symbol → "USB_B_Micro"
Bibliothèque : Connector
Référence : J1
Value : Micro-USB
```

**12. Connecteur Batterie (J2)**
```
Add Symbol → "Conn_01x02"
Référence : J2
Value : BATTERY
Pin 1 : +BAT
Pin 2 : GND
```

**13. Header ISP (J4)**
```
Add Symbol → "Conn_02x03_Odd_Even"
Référence : J4
Value : ISP
Configuration :
Pin 1 : MISO    Pin 2 : VCC
Pin 3 : SCK     Pin 4 : MOSI
Pin 5 : RESET   Pin 6 : GND
```

---

### Zone 6 : Composants additionnels

**14. Résistance TP4056 (R10)**
```
Add Symbol → "R"
Référence : R10
Value : 2kΩ
Note : Rprog pour courant charge 500mA
```

**15. LED Charge/Standby (D2, D3)**
```
Add Symbol → "LED"
- D2 : LED_CHARGE (rouge 0805)
- D3 : LED_STANDBY (vert 0805)
```

---

## 🔌 ÉTAPE 2.5 : CÂBLAGE DES CONNEXIONS (1h)

### Outil de câblage

```
Touche W : Wire (fil)
Touche L : Label (étiquette net)
Touche K : Terminer fil
Touche ESC : Annuler

Conseils :
- Utiliser des labels pour nets longs (VCC, GND, D2-D10)
- Garder le schéma aéré et lisible
- Grouper par fonction
```

---

### Connexions Alimentation

**TP4056 → HT7333 → ATmega**

```
[MICRO-USB J1]
  Pin VBUS (5V) → TP4056 VCC (Pin 2)
  Pin GND → TP4056 GND (Pin 3)

[TP4056 U3]
  Pin BAT+ (4) → [J2 +BAT] + [SW4 common]
  Pin BAT- (4) → GND
  Pin OUT+ (5) → [SW4 NO]
  Pin PROG (6) → [R10 2kΩ] → GND
  Pin CHRG (7) → [D2 LED_CHARGE anode] → [R 1kΩ] → GND
  Pin STDBY (8) → [D3 LED_STANDBY anode] → [R 1kΩ] → GND

[SWITCH SW4]
  NO → HT7333 VIN (Pin 2)

[HT7333 U2]
  VIN (2) → [C1 10µF] → GND
  GND (1) → GND (net global)
  VOUT (3) → [C2 100nF] → GND
  VOUT (3) → VCC (net global = 3.3V)

[ATmega328P U1]
  VCC (7) → VCC + [C3 10µF] → GND + [C4 100nF] → GND
  AVCC (20) → VCC + [C5 10µF] → GND + [C6 100nF] → GND
  GND (8, 22) → GND
  AREF (21) → [C 100nF optionnel] → GND
```

**Label à créer :**
- `VCC` : Rail 3.3V principal
- `GND` : Masse globale
- `VBAT` : Tension batterie (3.7V)

---

### Connexions Driver LED principale

```
[ATmega328P U1]
  PD6/D6 (12) → [R2 1kΩ] → [Q1 Gate]

[MOSFET Q1 (2N7002)]
  Gate → [R2 1kΩ] → U1 PD6
  Source → GND
  Drain → [R1 47Ω] → [J3 LED+ Pin 1]

[Connecteur J3 LED]
  Pin 1 (+) → VCC
  Pin 2 (-) → Drain Q1 (via R1)

Note : LED Nichia en série avec R1 entre VCC et Drain MOSFET
```

---

### Connexions LED Indicateurs

```
[ATmega328P U1]
  PB0/D8 (14) → [R3 470Ω] → [D5 LED_Rouge anode]
  PB1/D9 (15) → [R4 470Ω] → [D6 LED_Verte anode]
  PB2/D10 (16) → [R5 470Ω] → [D7 LED_Bleue anode]
  PD7/D7 (13) → [R6 470Ω] → [D8 LED_Mode anode]

[LED D5-D8]
  Cathodes (-) → GND
```

---

### Connexions Boutons

```
[ATmega328P U1]
  PD2/D2 (4) → [SW1 Pin 1] ──┬── [R7 10kΩ] → VCC
                             │
                        [SW1 Pin 2] → GND
  
  PD3/D3 (5) → [SW2 Pin 1] ──┬── [R8 10kΩ] → VCC
                             │
                        [SW2 Pin 2] → GND
  
  PD4/D4 (6) → [SW3 Pin 1] ──┬── [R9 10kΩ] → VCC
                             │
                        [SW3 Pin 2] → GND

Note : Avec INPUT_PULLUP dans le code, R7-R9 peuvent être omises
       Mais recommandé de les mettre pour robustesse
```

---

### Connexions ISP

```
[Header ISP J4]
  Pin 1 (MISO) → U1 PB4/MISO (18)
  Pin 2 (VCC)  → VCC
  Pin 3 (SCK)  → U1 PB5/SCK (19)
  Pin 4 (MOSI) → U1 PB3/MOSI (17)
  Pin 5 (RESET)→ U1 RESET (1) + [R 10kΩ] → VCC
  Pin 6 (GND)  → GND
```

---

## ✅ ÉTAPE 2.6 : VÉRIFICATION SCHÉMA (15 min)

### ERC (Electrical Rule Check)

```
1. Clic sur icône "Perform Electrical Rules Check"
   ou Inspect → Electrical Rules Checker

2. Cliquer "Run ERC"

3. Vérifier 0 erreur (warnings OK)

Erreurs courantes à corriger :
❌ "Pin not connected" : Ajouter flag "No Connect" (X)
❌ "Pin connected to other pin" : Vérifier câblage
❌ "Power pin not driven" : Ajouter symbole PWR_FLAG

4. Si erreurs → Corriger
5. Re-run ERC jusqu'à 0 erreur
```

### Ajouter PWR_FLAG si nécessaire

```
Si ERC indique "Power pin not driven by power output" :

Add Symbol → "PWR_FLAG"
Placer sur :
- Net VCC (1 flag)
- Net GND (1 flag)
- Net VBAT (1 flag si utilisé)
```

---

## 📐 ÉTAPE 3 : ATTRIBUTION DES EMPREINTES (30 min)

### 3.1 Ouvrir l'outil d'association

```
Dans Schematic Editor :
Tools → Assign Footprints
ou touche Ctrl+F
```

### 3.2 Assigner les empreintes (footprints)

**Format : Bibliothèque:Footprint**

```
MICROCONTRÔLEUR
────────────────────────────────────────────────
U1 (ATmega328P-AU)
→ Package_QFP:TQFP-32_7x7mm_P0.8mm

RÉGULATEURS
────────────────────────────────────────────────
U2 (HT7333-A)
→ Package_TO_SOT_SMD:SOT-89-3

U3 (TP4056)
→ Package_TO_SOT_SMD:SOT-23-6

TRANSISTOR
────────────────────────────────────────────────
Q1 (2N7002)
→ Package_TO_SOT_SMD:SOT-23

RÉSISTANCES (CMS 0805)
────────────────────────────────────────────────
R1-R10
→ Resistor_SMD:R_0805_2012Metric

CONDENSATEURS (CMS 0805)
────────────────────────────────────────────────
C1-C6
→ Capacitor_SMD:C_0805_2012Metric

LED INDICATEURS (Traversant 3mm)
────────────────────────────────────────────────
D5-D8
→ LED_THT:LED_D3.0mm

LED CHARGE/STANDBY (CMS 0805)
────────────────────────────────────────────────
D2, D3
→ LED_SMD:LED_0805_2012Metric

BOUTONS
────────────────────────────────────────────────
SW1-SW3
→ Button_Switch_SMD:SW_SPST_TL3342
ou rechercher "tactile" → choisir 4×4mm side-mount

SW4 (ON/OFF)
→ Button_Switch_SMD:SW_SPDT_PCM12

CONNECTEURS
────────────────────────────────────────────────
J1 (Micro-USB)
→ Connector_USB:USB_Micro-B_Molex-105017-0001
ou chercher "USB_Micro" → choisir edge-mount

J2, J3 (Connecteur 2 pins)
→ Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical
ou
→ Connector_JST:JST_PH_B2B-PH-K_1x02_P2.00mm_Vertical

J4 (ISP)
→ Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical
ou pads de test :
→ TestPoint:TestPoint_Pad_D1.5mm (×6)
```

### 3.3 Validation

```
Vérifier que TOUS les symboles ont une empreinte assignée
(colonne "Footprint" remplie)

Apply → Save Schematic & Continue → Close
```

---

## 🎨 ÉTAPE 4 : PCB LAYOUT (2-3h)

### 4.1 Ouvrir l'éditeur PCB

```
Dans KiCad Manager :
- Clic sur "PCB Editor" ou touche B
```

### 4.2 Importer le netlist

```
Tools → Update PCB from Schematic
ou touche F8

Cocher :
☑ Delete extra footprints
☑ Re-associate footprints by reference

Cliquer "Update PCB"

→ Tous les composants apparaissent en tas au centre
```

### 4.3 Définir le contour du PCB

```
1. Sélectionner couche "Edge.Cuts" (menu déroulant haut)

2. Outil "Draw Rectangle" (touche R)
   ou Draw → Draw Rectangle

3. Dessiner rectangle 15mm × 70mm :
   - Clic point origine (ex: 100, 100)
   - Clic point opposé (115, 170)
   
   Vérifier dimensions :
   - Largeur = 15mm
   - Hauteur = 70mm

4. Si dimensions incorrectes :
   - Edit → Edit Text and Graphics Properties
   - Ajuster coordonnées exactes

Alternative : Utiliser grille
   - View → Grid Settings → Grid : 0.5mm
   - Compter 30 carrés × 140 carrés
```

### 4.4 Définir les paramètres PCB

```
File → Board Setup

Onglet "Board Stackup" :
─────────────────────────────────────────
Layer 1 (F.Cu) : 35µm copper (1 oz)
Prepreg       : FR4 0.7mm
Layer 2 (B.Cu) : 35µm copper (1 oz)
Total thickness : 1.6mm

Onglet "Design Rules" → "Constraints" :
─────────────────────────────────────────
Minimum clearance : 0.2mm (ou 0.15mm si serré)
Minimum track width : 0.2mm
Minimum via diameter : 0.6mm
Minimum via drill : 0.3mm

Onglet "Design Rules" → "Pre-defined Sizes" :
─────────────────────────────────────────
Track Widths :
- 0.2mm (signal faible)
- 0.3mm (signal standard)
- 0.5mm (VCC)
- 0.8mm (LED principale)
- 1.0mm (VBAT)

Via Sizes :
- 0.6/0.3mm (standard)
- 0.8/0.4mm (GND/power)

Onglet "Text & Graphics" :
─────────────────────────────────────────
Silkscreen (F.SilkS) :
- Line thickness : 0.15mm
- Text height : 1.0mm
- Text width : 0.15mm

OK
```

---

## 📍 ÉTAPE 4.5 : PLACEMENT DES COMPOSANTS (1h)

### Organisation générale

```
Touche M : Move (déplacer)
Touche R : Rotate (pivoter)
Touche F : Flip (retourner face)
Touche E : Properties (éditer)

Grille recommandée :
- Placement général : 0.5mm ou 1mm
- Placement fin : 0.25mm ou 0.1mm
```

### Positionnement selon zones (origine = coin haut gauche PCB)

**ZONE 1 : SORTIE LED (0-10mm)**

```
Position Y = 5mm du haut

J3 (Pads LED) : X=2mm, Y=5mm
R1 (47Ω)     : X=5mm, Y=5mm
R2 (1kΩ)     : X=7mm, Y=5mm
Q1 (2N7000)  : X=9mm, Y=5mm

Orientation : Composants alignés horizontalement
```

**ZONE 2 : LED INDICATEURS (10-25mm)**

```
Position Y = 12-22mm, espacés de 5mm

D5 (Rouge)   : X=3mm, Y=12mm, Rotation=90° (vertical)
R3 (470Ω)    : X=6mm, Y=12mm
D6 (Verte)   : X=3mm, Y=17mm, Rotation=90°
R4 (470Ω)    : X=6mm, Y=17mm
D7 (Bleue)   : X=3mm, Y=22mm, Rotation=90°
R5 (470Ω)    : X=6mm, Y=22mm
D8 (Mode)    : X=9mm, Y=17mm, Rotation=90°
R6 (470Ω)    : X=12mm, Y=17mm

LED orientation :
- Anode (patte +) vers le haut (proche bord PCB)
- Cathode vers le bas (vers centre PCB)
```

**ZONE 3 : MICROCONTRÔLEUR (25-45mm)**

```
Position Y = 35mm (centre vertical PCB)

U1 (ATmega328P) : X=7.5mm, Y=35mm, Rotation=45°
                  (rotation 45° pour optimiser routage)

Condensateurs autour de U1 :
C3 (10µF)  : X=2mm, Y=30mm
C4 (100nF) : X=4mm, Y=30mm
C5 (10µF)  : X=2mm, Y=40mm
C6 (100nF) : X=4mm, Y=40mm

Note : Condensateurs découplage AU PLUS PRÈS des pins VCC
```

**ZONE 4 : BOUTONS (45-60mm)**

```
Position Y = 48-58mm, espacés de 6mm

SW1 (FREQ)   : X=2mm, Y=48mm (side-mount, corps vers bord)
R7 (10kΩ)    : X=9mm, Y=48mm
SW2 (INTENS) : X=2mm, Y=54mm
R8 (10kΩ)    : X=9mm, Y=54mm
SW3 (MODE)   : X=2mm, Y=60mm
R9 (10kΩ)    : X=9mm, Y=60mm

Boutons orientation :
- Corps dépassant du bord gauche PCB
- Pins soudés sur PCB
```

**ZONE 5 : ALIMENTATION (60-68mm)**

```
Position Y = 63-66mm

U3 (TP4056)  : X=3mm, Y=63mm
R10 (2kΩ)    : X=6mm, Y=63mm
D2 (LED CHG) : X=3mm, Y=65mm
D3 (LED STB) : X=5mm, Y=65mm

U2 (HT7333)  : X=11mm, Y=63mm
C1 (10µF)    : X=11mm, Y=65mm
C2 (100nF)   : X=13mm, Y=65mm
```

**ZONE 6 : CONNECTEURS (68-70mm)**

```
Position Y = 69mm (bord bas PCB)

J1 (Micro-USB) : X=2mm, Y=69mm (edge-mount)
SW4 (ON/OFF)   : X=13mm, Y=69mm (slide, opposé à USB)
J2 (Batterie)  : X=7.5mm, Y=68.5mm (pads ou JST)
J4 (ISP pads)  : X=7.5mm, Y=69mm (6 pads 2×3)
```

---

### Conseils placement

```
✅ Respecter espacement composants :
   - CMS : mini 1mm entre bords
   - Traversant : mini 2mm

✅ Orienter composants pour routage optimal :
   - Pins connectées proches les unes des autres

✅ Laisser espace autour ATmega pour via GND

✅ Placer condensateurs découplage AU PLUS PRÈS des IC

✅ LED traversantes : Plier pattes à 90° pour sortie latérale
```

---

## 🛤️ ÉTAPE 4.6 : ROUTAGE DES PISTES (2h)

### Configuration du routage

```
Route → Interactive Router Settings

Mode : Walkaround (contournement obstacles)
Effort : High
Shove : Always

Track Width : Selon signal (voir "Pre-defined Sizes")
Via Size : 0.6/0.3mm standard
```

### Ordre de routage

**1. GND (plan de masse) - PRIORITAIRE**

```
1. Sélectionner couche "B.Cu" (Bottom)

2. Menu : Place → Filled Zones
   ou touche Ctrl+Shift+Z

3. Clic contour PCB (4 coins)
   ESC pour fermer zone

4. Properties zone :
   - Layer : B.Cu (Bottom Copper)
   - Net : GND
   - Clearance : 0.2mm
   - Min width : 0.2mm
   - Fill : Solid
   - Thermal relief : Yes
   - Thermal clearance : 0.5mm
   
5. OK

6. Clic droit zone → Fill Zone
   → Plan GND apparaît en bleu

7. Ajouter via GND partout :
   - Sous chaque pin GND de IC
   - Sous chaque cathode LED
   - Sous source MOSFET
   - Espacés ~5mm sur bord PCB
   
   Via GND :
   - Touche V ou Place → Via
   - Taille : 0.8/0.4mm
   - Net : GND
```

**2. VCC (rail 3.3V)**

```
Couche F.Cu (Top)

Piste principale VCC horizontale :
- De HT7333 VOUT (zone 5) vers ATmega VCC (zone 3)
- Largeur : 0.5mm
- Continue tout le long

Connexions VCC aux composants :
- ATmega VCC + AVCC : 0.5mm
- Pull-up résistances : 0.3mm
- LED anode (via résistances) : 0.3mm
```

**3. VBAT (batterie 3.7V)**

```
Couche F.Cu

De J2 (+BAT) vers :
- TP4056 BAT+ : 1.0mm
- SW4 : 1.0mm
- HT7333 VIN (via SW4) : 1.0mm

Important : Isoler de VCC et GND
```

**4. LED principale (courant élevé)**

```
De U1 PD6 vers Q1 Gate : 0.3mm (signal)
De Q1 Drain vers J3 : 0.8mm (courant LED)
De VCC vers R1 : 0.8mm
```

**5. Signaux digitaux**

```
Largeur : 0.3mm

Priorité de routage :
a) ISP (MISO, MOSI, SCK, RESET) : Garder courts
b) LED indicateurs (D8-D10, D7) : Courts vers résistances
c) Boutons (D2-D4) : Moyens vers SW

Technique :
- Touche X : Route track
- Clic départ → Clic arrivée
- ESC pour terminer
- U : Changer de couche (via automatique)
```

**6. USB D+ / D-**

```
⚠️ CRITIQUE : Lignes différentielles

De J1 (Micro-USB) vers TP4056 :
- Largeur : 0.3mm
- Longueur égale D+ et D-
- Espacement constant 0.3mm
- Pas de via si possible (direct)
- Pas de stubs (branches)
```

---

### Vérification routage

```
Après routage complet :

1. View → Ratsnest (fils blancs)
   → Doit être vide (tous routés)

2. Inspect → Design Rules Checker
   → Run DRC
   → Vérifier 0 erreur

Erreurs courantes :
❌ Clearance violation : Pistes trop proches
❌ Unconnected items : Oublié de router
❌ Track width : Piste trop fine
```

---

## ✨ ÉTAPE 4.7 : FINITIONS (30 min)

### Sérigraphie (Silkscreen)

**Face TOP (F.SilkS)**

```
1. Sélectionner couche F.SilkS

2. Texte à ajouter :
   - Titre : "Auriculo v4.0"
     Position : Centre, Y=2mm
     Taille : 1.5mm
   
   - Références composants : R1, R2, U1, etc.
     (Déjà présentes, vérifier lisibilité)
   
   - Polarité batterie :
     Près J2 : "+BAT" et "-BAT"
     Couleur rouge pour + (dans fichier)
   
   - Labels boutons :
     Près SW1 : "FREQ"
     Près SW2 : "INT"
     Près SW3 : "MODE"
   
   - Marqueur pin 1 ATmega :
     Petit cercle ou triangle près pin 1

3. Outil texte : Place → Text
   Propriétés : 1.0mm height, 0.15mm thickness
```

**Face BOTTOM (B.SilkS)**

```
Sélectionner couche B.SilkS

Texte minimal :
- Version : "Rev 1.0"
- Date : "2026-02"
- Copyright : "© Votre Nom"

Position : Coin bas, petit (0.8mm)
```

### Trous de montage (optionnel)

```
Si fixation par vis :

Place → Pad
Type : NPTH (Non-Plated Through Hole)
Diamètre : 2.2mm (pour vis M2)

Positions :
- Trou 1 : X=7.5mm, Y=5mm
- Trou 2 : X=7.5mm, Y=65mm
```

### Zone interdite composants (Keepout)

```
Si zones sensibles (près USB, près bord LED) :

Place → Keepout Area
Dessiner zone où composants interdits
Properties :
☑ No tracks
☑ No vias
☑ No copper pour
```

---

## 📤 ÉTAPE 5 : GÉNÉRATION GERBERS (15 min)

### 5.1 Plot Gerber files

```
File → Fabrication Outputs → Gerbers (.gbr)

Onglet "General" :
─────────────────────────────────────────
Output directory : "Gerbers/"

Layers to plot :
☑ F.Cu (Top copper)
☑ B.Cu (Bottom copper)
☑ F.SilkS (Top silkscreen)
☑ B.SilkS (Bottom silkscreen)
☑ F.Mask (Top solder mask)
☑ B.Mask (Bottom solder mask)
☑ Edge.Cuts (Board outline)

Options :
☑ Plot border and title block : NO
☑ Plot footprint values : YES
☑ Plot footprint references : YES
☑ Exclude PCB edge layer : NO
☑ Use Protel filename extensions : YES (recommandé JLCPCB)

Format :
- Units : Millimeters
- Coordinate format : 4.6 (default)

Cliquer "Plot"
```

### 5.2 Drill files

```
Cliquer "Generate Drill Files..."

Drill File Format :
- Excellon
- Units : Millimeters
- Zeros Format : Decimal format

Options :
☑ Minimal header
☐ Mirror Y axis
☑ PTH and NPTH in single file : NO (2 fichiers séparés)

Cliquer "Generate Drill File"
```

### 5.3 Vérifier les fichiers

```
Dans dossier "Gerbers/" :

Fichiers attendus (Protel extensions) :
✅ Stylet_Auriculo_v4-F_Cu.gtl (Top copper)
✅ Stylet_Auriculo_v4-B_Cu.gbl (Bottom copper)
✅ Stylet_Auriculo_v4-F_SilkS.gto (Top silkscreen)
✅ Stylet_Auriculo_v4-B_SilkS.gbo (Bottom silkscreen)
✅ Stylet_Auriculo_v4-F_Mask.gts (Top soldermask)
✅ Stylet_Auriculo_v4-B_Mask.gbs (Bottom soldermask)
✅ Stylet_Auriculo_v4-Edge_Cuts.gm1 (Board outline)
✅ Stylet_Auriculo_v4.drl (Drill file PTH)
✅ Stylet_Auriculo_v4-NPTH.drl (Drill file NPTH, si trous)

Total : 8-9 fichiers
```

### 5.4 Visualiser avec Gerber Viewer

```
Dans KiCad Manager :
- Gerber Viewer (icône)

File → Open Gerber Files
Sélectionner tous les .gtl/.gbl/.gto/etc.

File → Open Excellon Drill Files
Sélectionner .drl

View → 3D Viewer (si dispo)

Vérifier :
✅ Contour PCB correct (15×70mm)
✅ Toutes pistes connectées
✅ Pas de zones isolées
✅ Sérigraphie lisible
✅ Perçages aux bons endroits
```

---

## 📦 ÉTAPE 6 : BOM & CPL (15 min)

### 6.1 Export BOM

```
Dans Schematic Editor :

File → Export → BOM...

Plugin : "bom_csv_grouped_by_value"
ou utiliser plugin externe

Format CSV souhaité :
──────────────────────────────────────────
Reference, Value, Footprint, Quantity
U1, ATmega328P-AU, Package_QFP:TQFP-32..., 1
R1, 47, Resistor_SMD:R_0805..., 1
...
```

### 6.2 Créer BOM pour JLCPCB

```
Si assemblage JLCPCB, format requis :

Colonnes :
- Comment (Value)
- Designator (Reference)
- Footprint
- LCSC Part # (chercher sur lcsc.com)

Exemple :
Comment,   Designator, Footprint,      LCSC Part #
10uF,      C1 C3 C5,   C_0805_2012,    C15850
100nF,     C2 C4 C6,   C_0805_2012,    C49678
470,       R3-R6,      R_0805_2012,    C17710
ATmega328P-AU, U1,     TQFP-32,        C14877
```

### 6.3 Export CPL (Component Placement List)

```
Dans PCB Editor :

File → Fabrication Outputs → Component Placement (.pos)

Format : CSV
Units : Millimeters
Files : Separate files for front and back

Générer :
- Stylet_Auriculo_v4-top-pos.csv
- Stylet_Auriculo_v4-bottom-pos.csv

Format attendu :
Ref,Val,Package,PosX,PosY,Rot,Side
U1,ATmega328P-AU,TQFP-32,7.5,35.0,45,top
R1,47,R_0805,5.0,5.0,0,top
```

---

## 📤 ÉTAPE 7 : COMMANDE JLCPCB (10 min)

### 7.1 Zipper les Gerbers

```
Créer archive ZIP contenant :
✅ Tous fichiers .gtl, .gbl, .gto, etc.
✅ Fichiers .drl
✅ (Optionnel) BOM.csv
✅ (Optionnel) CPL.csv

Nom : Stylet_Auriculo_v4_Gerbers.zip
```

### 7.2 Upload sur JLCPCB

```
1. Aller sur https://jlcpcb.com/quote

2. "Add Gerber file" → Upload ZIP

3. Paramètres détectés automatiquement :
   - Dimensions : 15×70mm ✅
   - Layers : 2
   - Thickness : 1.6mm

4. Ajuster options :
   ────────────────────────────────────
   PCB Qty : 5 ou 10
   PCB Color : Black (ou Green/Blue)
   Surface Finish : ENIG (recommandé) ou LeadFree HASL
   Remove Order Number : Yes (+1.50$)
   
5. Si assemblage SMT :
   ────────────────────────────────────
   ☑ SMT Assembly
   PCBA Qty : 5
   PCBA Type : Economic
   Tooling holes : Added by JLCPCB
   
   Upload :
   - BOM file (CSV)
   - CPL file (CSV)
   
   Confirm parts → Review → Save to cart

6. Prix estimé :
   - PCB seul (5 pcs) : ~5-10€
   - PCB assemblé (5 pcs) : ~80-120€
   
7. Proceed to checkout → Paiement
```

---

## ✅ CHECKLIST FINALE

```
AVANT FABRICATION :
□ Schéma : ERC 0 erreur
□ PCB : DRC 0 erreur
□ Dimensions : 15×70mm exact
□ Clearance : ≥0.2mm partout
□ Track width : Conforme design rules
□ Via : Tous connectés à GND quand nécessaire
□ Sérigraphie : Lisible, pas sur pads
□ Gerber : 8-9 fichiers générés
□ Gerber Viewer : Visualisation OK
□ BOM : Tous composants listés
□ CPL : Positions correctes

APRÈS RÉCEPTION PCB :
□ Inspection visuelle qualité
□ Mesure dimensions (15.0×70.0mm)
□ Test continuité GND (multimètre)
□ Test isolation VCC/GND (>10MΩ)
```

---

## 🎓 RESSOURCES COMPLÉMENTAIRES

### Tutoriels KiCad

```
Officiel : https://docs.kicad.org/
Youtube : "KiCad 7 tutorial" par DigiKey
Forum : https://forum.kicad.info/
```

### Bibliothèques composants

```
Si composant manquant :
- SnapEDA.com : Télécharger footprint
- Component Search Engine
- Créer manuellement (Footprint Editor)
```

### Vérification design

```
Outil en ligne : 
- https://gerber-viewer.ucamco.com/
  (Upload Gerbers → Visualisation 3D)
```

---

## 💡 CONSEILS FINAUX

### Pour gagner du temps

```
✅ Utiliser templates de projet KiCad
✅ Sauvegarder configurations DRC favorites
✅ Créer bibliothèque perso composants fréquents
✅ Utiliser raccourcis clavier (liste : touche ?)
```

### Pour éviter erreurs

```
⚠️ Toujours vérifier polarité (LED, condensateurs polarisés)
⚠️ Double-check pin 1 des IC (repère)
⚠️ Tester montage breadboard AVANT PCB
⚠️ Commander 5-10 PCB (au cas où erreur)
⚠️ Garder schéma PDF pour référence soudure
```

### Après première fabrication

```
Si erreur détectée :
1. Noter erreur précisément
2. Corriger dans KiCad
3. Incrémenter version (Rev 1.1)
4. Re-générer Gerbers
5. Commander Rev corrigée

C'est normal d'avoir 2-3 révisions !
```

---

## 🎯 PROCHAINES ÉTAPES

### Maintenant :

1. ✅ Suivre ce guide étape par étape
2. ✅ Prendre son temps (ne pas rush)
3. ✅ Sauvegarder fréquemment (Ctrl+S)

### Une fois PCB conçu :

4. ✅ Commander fabrication JLCPCB
5. ✅ Préparer assemblage (fer à souder, composants)
6. ✅ Suivre guide montage complet

### Temps estimé total :

```
Schéma : 2h
PCB Layout : 3h
Vérifications : 1h
─────────────────
Total : 6h (première fois)

Révisions futures : 2-3h
```

---

**Bon courage pour la conception ! 🚀**

N'hésitez pas si vous bloquez à une étape. Le plus difficile est le premier PCB - après ça devient fluide !

═══════════════════════════════════════════════════════════════
