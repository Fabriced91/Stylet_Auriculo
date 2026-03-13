# 🎨 GUIDE COMPLET KICAD - Stylet Auriculothérapie v7.0

## 📋 Spécifications du PCB

### Dimensions
- **Taille** : 16mm × 70mm (rectangulaire)
- **Épaisseur** : 1.6mm (standard)
- **Couches** : 2 layers (Top + Bottom)
- **Finition** : HASL (Hot Air Solder Leveling) ou ENIG

### Organisation en 4 Zones
```
┌─ ZONE 1 (0-10mm) ─┬─ ZONE 2 (10-25mm) ─┬─ ZONE 3 (25-50mm) ─┬─ ZONE 4 (50-70mm) ─┐
│ Alimentation      │ Interface          │ Contrôle           │ Sortie             │
│ - Pads OUT+/OUT-  │ - Boutons (2×)     │ - ATmega328P       │ - LED NSPW500CS    │
│ - Switch ON/OFF   │ - LEDs Indic (3×)  │ - Passifs          │ - Résistance 47Ω   │
│ - LDO 3.3V        │ - PCF8574A         │ - Diviseur ADC     │                    │
│ - Condensateurs   │ - Résistances LED  │ - Pull-ups I2C     │                    │
└───────────────────┴────────────────────┴────────────────────┴────────────────────┘
```

---

## 📦 ÉTAPE 1 : Préparation - Liste des Composants

### Télécharger les Symboles et Empreintes

Avant de commencer, assurez-vous d'avoir :

#### Composants Standard KiCad (déjà inclus)
- ✅ Résistances (Device:R)
- ✅ Condensateurs (Device:C)
- ✅ LEDs (Device:LED)
- ✅ Boutons (Switch:SW_Push)

#### Composants à Ajouter/Vérifier

**ATmega328P-AU** :
- Symbole : `MCU_Microchip_ATmega:ATmega328P-AU`
- Empreinte : `Package_QFP:TQFP-32_7x7mm_P0.8mm`

**PCF8574A** :
- Symbole : `Interface_Expansion:PCF8574`
- Empreinte : `Package_SO:SOIC-16_3.9x9.9mm_P1.27mm`
- **ATTENTION** : Variante "A" utilisée sur PCBA (adresse I2C = 0x38 avec A0=A1=A2=GND)
  Le PCF8574 standard (sans "A") a l'adresse 0x20. Vérifier le composant commandé !

**LDO MCP1700** :
- Symbole : `Regulator_Linear:MCP1700-3302E_SOT23`
- Empreinte : `Package_TO_SOT_SMD:SOT-23`

**Switch Slide** :
- Symbole : `Switch:SW_SPDT`
- Empreinte : Custom ou `Button_Switch_SMD:SW_SPDT_PCM12`

### Composants Traversants Choisis

**Boutons** : 6×6mm traversants
- Empreinte : `Button_Switch_THT:SW_PUSH_6mm`

**LEDs Indicateurs** : 0805 SMD (3× Rouge sur PCBA, active-HIGH)
- Empreinte : `LED_SMD:LED_0805_2012Metric`
- NOTE PCBA : Toutes les LEDs sont ROUGES. Le codage fréquence utilise
  les combinaisons de 1 à 3 LEDs allumées (pas un codage couleur RGB).

**LED NSPW500CS** : 5mm traversante
- Empreinte : `LED_THT:LED_D5.0mm`

---

## 🖥️ ÉTAPE 2 : Création du Projet KiCad

### 2.1 - Nouveau Projet

1. **Ouvrir KiCad**
2. **File → New Project**
3. **Nom** : `Stylet_Auriculo_v7`
4. **Emplacement** : `...\Projet stylet\last version\KiCad_PCB\`
5. **Créer le dossier** et **Save**

### 2.2 - Ouvrir l'Éditeur de Schéma

1. Cliquer sur **Schematic Layout Editor** (icône schéma)
2. Une feuille vierge s'ouvre

---

## ⚡ ÉTAPE 3 : Schéma Électronique

### 3.1 - Configuration de la Feuille

1. **Page Settings** (icône en haut à droite)
   - Title : `Stylet Auriculothérapie v7.0`
   - Company : Votre nom
   - Revision : `7.0`
   - Date : Automatique
   - Sheet size : **A4**

### 3.2 - Placement des Symboles

#### Bloc Alimentation (en haut à gauche)

**Pads Entrée OUT+/OUT-** :
1. Appuyer **A** (Add Symbol)
2. Chercher : `Connector:Conn_01x02`
3. Placer et nommer **J1** (label: "TP4056_OUT")
4. Annotate pins :
   - Pin 1 : OUT+ (VCC_BAT)
   - Pin 2 : OUT- (GND)

**Switch ON/OFF** :
1. Appuyer **A**
2. Chercher : `Switch:SW_SPDT`
3. Placer **SW1**
4. Connecter :
   - Pin 1 (COM) → OUT+ de J1
   - Pin 2 (NO) → Entrée LDO
   - Pin 3 (NC) → Non connecté

**LDO 3.3V** :
1. Appuyer **A**
2. Chercher : `Regulator_Linear:MCP1700-3302E_SOT23`
3. Placer **U1**
4. Connecter :
   - Pin 1 (GND) → GND
   - Pin 2 (VCC_OUT / 3.3V) → Rail VCC_3V3
   - Pin 3 (VCC_IN) → Sortie Switch (VCC_BAT)

**Condensateurs LDO** :
1. Appuyer **A** → `Device:C`
2. Placer **C1** (1µF) entre VCC_BAT et GND (proche pin 3 LDO)
3. Placer **C2** (1µF) entre VCC_3V3 et GND (proche pin 2 LDO)
4. **Properties** → Value : `1uF`

**Symboles Alimentation** :
1. Appuyer **P** (Power)
2. Ajouter **GND** (plusieurs fois)
3. Ajouter **+3.3V** (créer si nécessaire : label `VCC_3V3`)

#### Bloc Microcontrôleur (centre)

**ATmega328P-AU** :
1. Appuyer **A**
2. Chercher : `MCU_Microchip_ATmega:ATmega328P-AU`
3. Placer **U2**
4. Connecter alimentation :
   - VCC (pin 4, 6) → VCC_3V3
   - AVCC (pin 18) → VCC_3V3
   - GND (pin 3, 5, 21) → GND

**Condensateurs Découplage** :
1. Placer **C3, C4, C5** (100nF) près de chaque VCC
2. Value : `100nF`

**Pull-ups I2C** :
1. Placer **R1** (4.7kΩ) sur SDA (PC4/pin 27) vers VCC_3V3
2. Placer **R2** (4.7kΩ) sur SCL (PC5/pin 28) vers VCC_3V3

**Diviseur Batterie** :
1. Placer **R3** (100kΩ) entre VCC_BAT et net `VBAT_DIV`
2. Placer **R4** (33kΩ) entre `VBAT_DIV` et GND
3. Connecter `VBAT_DIV` à PC0 (ADC0, pin 23)

#### Bloc Interface (gauche)

**Boutons** :
1. Placer **SW2** (`Switch:SW_Push`)
   - Pin 1 → PD2 (pin 32, INT0)
   - Pin 2 → GND
   - Label : `BTN_FREQ`

2. Placer **SW3** (`Switch:SW_Push`)
   - Pin 1 → PD4 (pin 2)
   - Pin 2 → GND
   - Label : `BTN_MODE`

**PCF8574A** :
1. Appuyer **A**
2. Chercher : `Interface_Expansion:PCF8574`
3. Placer **U3** (PCF8574A sur PCBA, adresse 0x38)
4. Connecter :
   - VCC (pin 16) → VCC_3V3
   - GND (pin 8) → GND
   - SDA (pin 15) → PC4 (ATmega pin 27)
   - SCL (pin 14) → PC5 (ATmega pin 28)
   - A0, A1, A2 (pins 1-3) → GND (donne adresse 0x38 pour PCF8574A)
   - INT (pin 13) → Non connecté (ou PC1 si besoin)

**LEDs Indicateurs** (3× Rouge sur PCBA, active-HIGH) :
1. Placer **LED1** (Rouge D1)
   - Anode → Résistance **R5** (470Ω) → PCF8574A P0 (pin 4)
   - Cathode → GND

2. Placer **LED2** (Rouge D2)
   - Anode → Résistance **R6** (470Ω) → PCF8574A P1 (pin 5)
   - Cathode → GND

3. Placer **LED3** (Rouge D3)
   - Anode → Résistance **R7** (470Ω) → PCF8574A P2 (pin 6)
   - Cathode → GND

**Note** : LEDs en mode ACTIVE-HIGH (écrire 1 sur PCF8574A P0-P2 = LED ON)

#### Bloc Sortie LED (droite)

**Résistance Série** :
1. Placer **R8** (47Ω)
   - Pin 1 → PD3 (pin 1, OC2B)
   - Pin 2 → Anode LED principale

**LED NSPW500CS** :
1. Placer **LED4**
   - Anode → R8
   - Cathode → GND
   - Label : `LED_MAIN`

#### Connecteur ISP (Optionnel)

1. Placer **J2** : `Connector:AVR-ISP-6`
2. Connecter :
   - MISO → PB4 (pin 18)
   - SCK → PB5 (pin 19)
   - RESET → PC6 (pin 1)
   - MOSI → PB3 (pin 17)
   - VCC → VCC_3V3
   - GND → GND

### 3.3 - Annotations et Labels

1. **Annotate Schematic** (icône ou Tools → Annotate)
2. **Run Electrical Rules Check** (icône insecte)
   - Résoudre les erreurs/warnings

### 3.4 - Assignation des Empreintes

1. **Tools → Assign Footprints**
2. Pour chaque composant, sélectionner l'empreinte :

| Référence | Composant | Empreinte |
|-----------|-----------|-----------|
| U1 | MCP1700 | `Package_TO_SOT_SMD:SOT-23` |
| U2 | ATmega328P-AU | `Package_QFP:TQFP-32_7x7mm_P0.8mm` |
| U3 | PCF8574A | `Package_SO:SOIC-16_3.9x9.9mm_P1.27mm` |
| SW1 | Switch slide | `Button_Switch_SMD:SW_SPDT_PCM12` (ou custom) |
| SW2, SW3 | Boutons | `Button_Switch_THT:SW_PUSH_6mm` |
| LED1-3 | LEDs rouge 0805 | `LED_SMD:LED_0805_2012Metric` |
| R1-R7 | Résistances SMD | `Resistor_SMD:R_0805_2012Metric` |
| R8 | Résistance 1/4W | `Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal` |
| C1-C5 | Condensateurs | `Capacitor_SMD:C_0805_2012Metric` |
| J1 | Pads OUT+/OUT- | `Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical` (ou pads custom) |
| J2 | ISP | `Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical` |

3. **Apply, Save Schematic & Continue**

---

## 🎨 ÉTAPE 4 : Layout PCB (PCB Editor)

### 4.1 - Ouvrir PCB Editor

1. Depuis KiCad, ouvrir **PCB Layout Editor**
2. **Tools → Update PCB from Schematic** (F8)
3. Tous les composants apparaissent en vrac

### 4.2 - Configuration PCB

**Board Setup** :
1. **File → Board Setup**
2. **Board Stackup** :
   - Layers : **2 layers** (F.Cu + B.Cu)
   - Thickness : 1.6mm
   - Copper : 35µm (1oz)

3. **Design Rules → Constraints** :
   - Minimum track width : **0.2mm**
   - Minimum via diameter : **0.6mm**
   - Minimum clearance : **0.2mm**

4. **Design Rules → Net Classes** :
   - Créer **Power** :
     - Track width : **0.8mm**
     - Via size : **0.8mm / 0.4mm**
     - Nets : VCC_3V3, VCC_BAT, GND
   
   - Créer **Signal** (défaut) :
     - Track width : **0.3mm**
     - Via size : **0.6mm / 0.3mm**

### 4.3 - Définir le Contour du PCB

1. **Edge.Cuts Layer** (sélectionner en haut)
2. **Draw Line** (ou appuyer **Ctrl+Shift+L**)
3. Tracer rectangle **16mm × 70mm** :
   - Coin inférieur gauche : (0, 0)
   - Coin supérieur droit : (70mm, 16mm)

**Astuce** : Utiliser la grille 1mm (View → Grid Settings)

### 4.4 - Placement des Composants - Zone par Zone

#### ZONE 1 : Alimentation (0-10mm)

**Ordre de placement** :

1. **J1** (Pads OUT+/OUT-) :
   - Position : X=2mm, Y=8mm (centre vertical)
   - Orientation : Vertical (pads vers bord gauche)

2. **SW1** (Switch) :
   - Position : X=6mm, Y=8mm
   - Orientation : Horizontal (levier vers haut)

3. **U1** (LDO MCP1700) :
   - Position : X=8mm, Y=11mm
   - Orientation : 0° (pins vers bas)

4. **C1** (1µF) :
   - Position : X=5mm, Y=4mm (proche VCC_BAT)

5. **C2** (1µF) :
   - Position : X=9mm, Y=4mm (proche VCC_3V3)

#### ZONE 2 : Interface (10-25mm)

1. **SW2** (Bouton FREQ) :
   - Position : X=15mm, Y=8mm (centré axe)
   - Orientation : 0°

2. **SW3** (Bouton MODE) :
   - Position : X=22mm, Y=8mm (centré axe)
   - Orientation : 0°

3. **LED1** (Rouge) :
   - Position : X=14mm, Y=4mm (excentré vers bas)
   - Orientation : Anode vers haut

4. **LED2** (Verte) :
   - Position : X=18mm, Y=4mm

5. **LED3** (Bleue) :
   - Position : X=22mm, Y=4mm

6. **R5, R6, R7** (470Ω) :
   - Près des LEDs respectives, côté bottom si place manque

7. **U3** (PCF8574A) :
   - Position : X=18mm, Y=12mm
   - Orientation : IC vers haut (pin 1 en haut à gauche)

#### ZONE 3 : Contrôle (25-50mm)

1. **U2** (ATmega328P) :
   - Position : X=37mm, Y=8mm (centré)
   - Orientation : Pin 1 vers haut-gauche

2. **C3, C4, C5** (100nF) :
   - Répartir près des pins VCC de l'ATmega
   - Ex : C3 à (33mm, 13mm), C4 à (41mm, 13mm), C5 à (37mm, 3mm)

3. **R1, R2** (Pull-ups I2C 4.7kΩ) :
   - Près des pins PC4/PC5
   - Ex : (30mm, 10mm) et (30mm, 6mm)

4. **R3, R4** (Diviseur batterie) :
   - Vertical près de PC0
   - Ex : (44mm, 8mm) et (44mm, 4mm)

5. **J2** (ISP - optionnel) :
   - Position : X=48mm, Y=8mm (bord droit zone)
   - Orientation : Pins vers bord supérieur

#### ZONE 4 : Sortie (50-70mm)

1. **R8** (47Ω) :
   - Position : X=55mm, Y=8mm
   - Orientation : Horizontal

2. **LED4** (NSPW500CS) :
   - Position : X=64mm, Y=8mm (très proche extrémité)
   - Orientation : Anode vers gauche (vers R8)

**Astuce** : Utiliser **M** (Move) et **R** (Rotate) pour ajuster

### 4.5 - Routage des Pistes

#### Ordre de Routage Recommandé

1. **GND (plan de masse)** :
   - **Add Filled Zone** (ou **Ctrl+Shift+Z**)
   - Layer : **B.Cu** (bottom)
   - Net : **GND**
   - Clearance : 0.2mm
   - Remplir toute la surface (sauf bords 0.5mm)
   - Appuyer **B** pour recalculer

2. **VCC_3V3** (traces larges) :
   - Utiliser **Route Track** (X)
   - Width : **0.8mm**
   - Connecter :
     - LDO OUT → ATmega VCC pins
     - ATmega VCC → PCF8574 VCC
   - Ajouter **vias** vers GND pour découplage

3. **VCC_BAT** :
   - Width : **0.8mm**
   - Depuis J1 OUT+ → SW1 → LDO IN
   - Vers diviseur batterie R3

4. **Signaux I2C** (SDA/SCL) :
   - Width : **0.3mm**
   - Traces parallèles (espacement 0.5mm)
   - ATmega PC4 → PCF8574 SDA
   - ATmega PC5 → PCF8574 SCL
   - Pull-ups vers VCC_3V3

5. **PWM LED (D3 → R8 → LED4)** :
   - Width : **0.5mm** (courant 20mA)
   - Trace directe, éviter angles 90°
   - ATmega PD3 (pin 1) → R8 → LED4 anode

6. **Boutons** :
   - Width : **0.3mm**
   - SW2 → ATmega PD2
   - SW3 → ATmega PD4
   - Autres pins boutons → GND (via proche)

7. **LEDs indicateurs (rouge 0805)** :
   - Width : **0.3mm**
   - PCF8574A P0/P1/P2 → R5/R6/R7 → LED anodes

8. **ADC Batterie** :
   - Width : **0.3mm**
   - R3-R4 point milieu → ATmega PC0

9. **ISP (si présent)** :
   - Width : **0.3mm**
   - Traces courtes vers ATmega

#### Astuces Routage

- **Vias** : Utiliser généreusement pour passer entre layers
- **GND** : Toujours via proche pour retour courant
- **Angles** : 45° préférés (pas 90°)
- **Clearance** : Respecter 0.2mm minimum
- **Testpoints** : Ajouter pads sur VCC_3V3, GND, D3 pour debug

### 4.6 - Sérigraphie (Silkscreen)

**F.Silkscreen Layer** :

1. Ajouter **texte** :
   - "Stylet Auriculo v7.0" (X=35mm, Y=1mm)
   - "ON/OFF" près du switch
   - "FREQ" / "MODE" près des boutons
   - Noms composants (U1, U2, LED1-4, etc.)

2. Ajouter **marqueurs** :
   - Flèche ou point pour LED principale (direction pointe)
   - Polarité LEDs (+/-)
   - Pin 1 des ICs

3. **Taille texte** : 1mm hauteur minimum (lisibilité)

### 4.7 - Vérifications Finales

1. **Design Rules Check (DRC)** :
   - **Tools → DRC**
   - Résoudre toutes les erreurs
   - Warnings acceptables : courtoisie silkscreen

2. **3D View** :
   - **View → 3D Viewer**
   - Vérifier placement composants
   - Vérifier hauteurs (boutons, LEDs doivent dépasser)

3. **Netlist Check** :
   - **Tools → Update PCB from Schematic**
   - Confirmer "No changes"

---

## 📤 ÉTAPE 5 : Génération des Fichiers de Fabrication

### 5.1 - Fichiers Gerber

1. **File → Plot**
2. **Layers à inclure** :
   - ✅ F.Cu (Top copper)
   - ✅ B.Cu (Bottom copper)
   - ✅ F.Paste (Top solder paste, si SMD)
   - ✅ B.Paste
   - ✅ F.Silkscreen
   - ✅ B.Silkscreen
   - ✅ F.Mask (Top solder mask)
   - ✅ B.Mask
   - ✅ Edge.Cuts

3. **Options** :
   - Format : Gerber (RS-274X)
   - Drill file format : Excellon
   - ✅ Include netlist attributes
   - ✅ Subtract soldermask from silkscreen

4. **Plot** → Génère fichiers dans `/gerber/`

5. **Generate Drill Files** :
   - Format : Excellon
   - Units : Millimeters
   - **Generate**

### 5.2 - Fichier BOM (Bill of Materials)

1. **Tools → Generate BOM**
2. Sélectionner script (ou export CSV)
3. Vérifier quantités et références

### 5.3 - Fichier Position (Pick & Place)

1. **File → Fabrication Outputs → Footprint Position (.pos)**
2. Format : CSV
3. Units : Millimeters
4. Side : Front + Back

### 5.4 - Vérification Gerber

1. Télécharger **Gerbv** ou utiliser viewer en ligne
2. Charger tous les fichiers Gerber
3. Vérifier :
   - Contours nets
   - Pads correctement masqués
   - Vias présents
   - Texte lisible

---

## 🏭 ÉTAPE 6 : Commande PCB

### Fabricants Recommandés

| Fabricant | Délai | Prix (5 pcs) | Qualité |
|-----------|-------|--------------|---------|
| **JLCPCB** | 5-7 jours | ~2€ + shipping | ⭐⭐⭐⭐ |
| **PCBWay** | 7-10 jours | ~5€ + shipping | ⭐⭐⭐⭐⭐ |
| **AISLER** (EU) | 10-15 jours | ~15€ (inclus) | ⭐⭐⭐⭐ |

### Options de Commande (JLCPCB exemple)

- **Dimensions** : 16mm × 70mm
- **Layers** : 2
- **Thickness** : 1.6mm
- **Surface Finish** : HASL (ou ENIG si budget)
- **Copper Weight** : 1oz (35µm)
- **Solder Mask** : Vert (ou noir/blanc)
- **Silkscreen** : Blanc
- **Edge Connector** : Non
- **Remove Order Number** : Oui (specify location si non)

### Upload

1. Zipper tous les fichiers Gerber
2. Upload sur site fabricant
3. Vérifier aperçu généré
4. Commander !

---

## 🔧 ÉTAPE 7 : Assemblage

### Ordre de Soudure Recommandé

1. **SMD** (du plus petit au plus grand) :
   - Résistances 0805
   - Condensateurs 0805
   - LDO SOT23
   - ATmega328P TQFP-32 (pâte à souder + air chaud)
   - PCF8574 SOIC-16
   - Switch slide

2. **Traversants** :
   - Pads J1 (fils soudés)
   - Résistance 47Ω
   - Boutons SW2, SW3
   - LEDs RGB (attention polarité !)
   - LED NSPW500CS

3. **Connecteurs** :
   - ISP header (si présent)

### Tests Avant Mise Sous Tension

1. **Multimètre continuité** :
   - VCC_BAT ne doit PAS toucher GND (switch OFF)
   - GND plan continu

2. **Résistance** :
   - Entre VCC_3V3 et GND : >1kΩ (à vide)

3. **Inspection visuelle** :
   - Pas de ponts de soudure
   - Composants bien orientés (ATmega pin 1 !)

### Premier Démarrage

1. **Connecter TP4056** (OUT+/OUT- aux pads J1)
2. **Switch ON**
3. **Mesurer tensions** :
   - VCC_BAT : 3.0-4.2V (selon batterie)
   - VCC_3V3 : 3.3V ±0.1V
   - Si pas 3.3V → Vérifier LDO, C1, C2

4. **Programmer ATmega** via ISP
5. **Tester fonctions** :
   - LED principale
   - Boutons
   - LEDs RGB

---

## 📋 Checklist Finale

Avant fabrication, vérifier :

- [ ] Schéma électrique validé (ERC OK)
- [ ] Empreintes assignées à tous les composants
- [ ] PCB 16×70mm exact
- [ ] DRC passé sans erreurs critiques
- [ ] 3D viewer : pas de collision composants
- [ ] Gerbers générés et vérifiés
- [ ] BOM complète
- [ ] Polarités LEDs correctes (anode vers résistance)
- [ ] Pin 1 ATmega bien marqué
- [ ] Vias sur GND nombreux
- [ ] Traces VCC/GND larges (0.8mm)
- [ ] Clearance respectée partout
- [ ] Silkscreen lisible
- [ ] Switch accessible
- [ ] Boutons centré axe PCB
- [ ] LEDs RGB excentrées
- [ ] LED principale à l'extrémité

---

## 🆘 Dépannage Courant

### Problème : Composants trop serrés

**Solution** : Passer certains composants en bottom layer, utiliser vias

### Problème : Traces ne passent pas

**Solution** : Réduire width à 0.25mm pour signaux, utiliser 2 layers intelligemment

### Problème : DRC erreurs clearance

**Solution** : Augmenter espacement composants, réduire pad size si custom

### Problème : GND plan fragmenté

**Solution** : Ajouter traces GND manuelles pour relier zones, plus de vias

---

**Bon courage pour votre PCB ! N'hésitez pas si vous avez des questions sur une étape spécifique.** 🚀
