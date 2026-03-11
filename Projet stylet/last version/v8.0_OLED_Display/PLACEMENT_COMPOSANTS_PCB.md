# 📐 PLACEMENT COMPOSANTS PCB 16×70mm - Référence Rapide

## 🎯 Coordonnées Exactes des Composants

### Système de Coordonnées
- **Origine (0,0)** : Coin inférieur gauche du PCB
- **X** : Axe horizontal (longueur 70mm)
- **Y** : Axe vertical (largeur 16mm)
- **Centre axe** : Y = 8mm

---

## 📍 ZONE 1 : Alimentation (0-10mm)

| Référence | Composant | X (mm) | Y (mm) | Rotation | Layer | Notes |
|-----------|-----------|--------|--------|----------|-------|-------|
| **J1** | Pads OUT+/OUT- | 2.0 | 8.0 | 90° | Top | Pads verticaux, OUT+ en haut |
| **SW1** | Switch ON/OFF | 6.0 | 8.0 | 0° | Top | Levier vers haut (Y+) |
| **U1** | LDO MCP1700 SOT23 | 8.0 | 11.0 | 0° | Top | Pin 1 = GND (en bas) |
| **C1** | 1µF (entrée LDO) | 5.0 | 4.0 | 0° | Top | Proche VCC_BAT |
| **C2** | 1µF (sortie LDO) | 9.0 | 4.0 | 0° | Top | Proche VCC_3V3 |

**Distances clés** :
- J1 → SW1 : 4mm (horizontal)
- SW1 → U1 : 2mm (horizontal)
- C1 ↔ C2 : 4mm (espacement)

---

## 📍 ZONE 2 : Interface (10-25mm)

| Référence | Composant | X (mm) | Y (mm) | Rotation | Layer | Notes |
|-----------|-----------|--------|--------|----------|-------|-------|
| **SW2** | Bouton FREQ 6×6mm | 15.0 | 8.0 | 0° | Top | **Centré axe** |
| **SW3** | Bouton MODE 6×6mm | 22.0 | 8.0 | 0° | Top | **Centré axe** |
| **LED1** | LED Rouge 5mm | 14.0 | 4.0 | 0° | Top | **Excentrée** Y-4mm, anode vers haut |
| **LED2** | LED Verte 5mm | 18.0 | 4.0 | 0° | Top | **Excentrée** Y-4mm |
| **LED3** | LED Bleue 5mm | 22.0 | 4.0 | 0° | Top | **Excentrée** Y-4mm |
| **R5** | 470Ω (LED Rouge) | 14.0 | 12.0 | 90° | Bottom | Sous PCF8574 si place manque |
| **R6** | 470Ω (LED Verte) | 18.0 | 12.0 | 90° | Bottom | Idem |
| **R7** | 470Ω (LED Bleue) | 22.0 | 12.0 | 90° | Bottom | Idem |
| **U3** | PCF8574 SOIC-16 | 18.0 | 12.0 | 0° | Top | Pin 1 en haut-gauche |

**Alignements** :
- **Boutons** : Y = 8.0mm (axe central) ← **Important !**
- **LEDs** : Y = 4.0mm (excentrées vers bas)
- Espacement boutons : 7mm (centre à centre)
- Espacement LEDs : 4mm (alignées avec boutons)

---

## 📍 ZONE 3 : Contrôle (25-50mm)

| Référence | Composant | X (mm) | Y (mm) | Rotation | Layer | Notes |
|-----------|-----------|--------|--------|----------|-------|-------|
| **U2** | ATmega328P TQFP-32 | 37.0 | 8.0 | 0° | Top | **Centré zone**, Pin 1 top-left |
| **C3** | 100nF (découplage 1) | 33.0 | 13.0 | 0° | Top | Près VCC pin 4 |
| **C4** | 100nF (découplage 2) | 41.0 | 13.0 | 0° | Top | Près AVCC pin 18 |
| **C5** | 100nF (découplage 3) | 37.0 | 3.0 | 0° | Top | Près VCC pin 6 |
| **R1** | 4.7kΩ (pull-up SDA) | 30.0 | 10.0 | 90° | Top | PC4 → VCC |
| **R2** | 4.7kΩ (pull-up SCL) | 30.0 | 6.0 | 90° | Top | PC5 → VCC |
| **R3** | 100kΩ (diviseur haut) | 44.0 | 10.0 | 90° | Top | Vertical, vers VCC_BAT |
| **R4** | 33kΩ (diviseur bas) | 44.0 | 6.0 | 90° | Top | Vertical, vers GND |
| **J2** | Connecteur ISP 2×3 | 48.0 | 8.0 | 0° | Top | Optionnel, pin 1 top-left |

**Symétrie ATmega** :
- Centre géométrique : (37, 8)
- Condensateurs en triangle autour du chip
- Diviseur batterie à droite (X=44mm)
- Pull-ups I2C à gauche (X=30mm)

**Point milieu diviseur** :
- Entre R3 et R4 → vers PC0 (pin 23)

---

## 📍 ZONE 4 : Sortie (50-70mm)

| Référence | Composant | X (mm) | Y (mm) | Rotation | Layer | Notes |
|-----------|-----------|--------|--------|----------|-------|-------|
| **R8** | 47Ω 1/4W | 55.0 | 8.0 | 0° | Top | Horizontal, axial |
| **LED4** | NSPW500CS 5mm | 64.0 | 8.0 | 180° | Top | **Pointe stylet**, anode vers R8 (gauche) |

**Alignement** :
- LED4 à **6mm du bord** (70 - 6 = 64mm)
- Anode vers gauche (vers R8)
- Cathode vers droite (vers extrémité)
- **Centré axe** : Y = 8.0mm

**Zone étroite** :
- Largeur disponible : 14-16mm
- Pas d'autres composants après X=60mm
- Réserver pour boîtier conique

---

## 🎨 Vue d'Ensemble Annotée

```
        Y
        ↑
   16mm │                        ┌────────── LEDs RGB excentrées (Y=4mm)
        │  ●●●                   │
        │                        │   ┌────── Boutons centrés (Y=8mm)
    12mm│      ┌───────┐        ││   ●●
        │      │PCF8574│         ││
        │                    ┌───────────┐
     8mm├──●──●────────────────ATmega328P│────────●─────●──→ X
        │  SW SW              └───────────┘        R8   LED4
        │                                                
     4mm│  ●●                                      ▲
        │  C1C2                                    │
        │                                  ZONE LIBRE
     0mm└───────────────────────────────────────────────────→
        0  5  10  15  20  25  30  35  40  45  50  55  60  65  70mm
        │←ZONE1→│←──ZONE2──→│←─────ZONE3─────→│←──ZONE4──→│
```

---

## 📏 Contraintes Mécaniques

### Boutons Traversants 6×6mm
- **Hauteur totale** : ~8mm (avec capuchon)
- **Débattement** : 2mm (course appui)
- **Découpe boîtier** : 7×7mm minimum
- **Centrage axe** : Facilite symétrie du boîtier

### LEDs 5mm Traversantes
- **Diamètre LED** : 5mm
- **Hauteur lentille** : 8.6mm
- **Découpe boîtier** : Ø6mm (serrage léger)
- **Excentrage** : Y=4mm permet visibilité depuis côté

### LED Principale NSPW500CS
- **Position pointe** : X=64mm (6mm du bord)
- **Angle faisceau** : 15° (focalisé)
- **Sortie lumière** : Doit affleurer boîtier ou dépasser 1-2mm

### Switch Slide
- **Hauteur** : 2.5mm
- **Levier** : Doit dépasser boîtier (découpe rectangulaire)
- **Position** : Zone large (Y=8mm ± 4mm)

---

## 🔧 Tolérances et Ajustements

### Précision Placement
- **SMD** : ±0.5mm acceptable (fabricant ajuste)
- **Traversants** : ±0.2mm (trous guidés)
- **Alignement visuel** : ±1mm (LEDs, boutons)

### Clearances Minimales
- **Composant ↔ Bord** : 1mm minimum
- **Composant ↔ Composant** : 0.5mm (SMD), 2mm (traversant)
- **Trace ↔ Bord** : 0.3mm

### Zones Interdites
- **0-1mm** : Bords (fragilité découpe)
- **68-70mm** : Extrémité pointe (réservé LED4)

---

## ✅ Checklist Placement

Avant routage, vérifier :

- [ ] Tous les composants dans les bonnes zones
- [ ] Boutons SW2/SW3 à Y=8.0mm (axe central)
- [ ] LEDs RGB à Y=4.0mm (excentrées)
- [ ] LED4 à X=64mm (proche extrémité)
- [ ] ATmega centré en Zone 3
- [ ] Condensateurs découplage près des VCC
- [ ] Diviseur batterie vertical (R3 au-dessus R4)
- [ ] Pas de composant au-delà de X=66mm
- [ ] Orientation Pin 1 des ICs correcte
- [ ] Polarité LEDs respectée (anode vers résistance)

---

## 📊 Densité par Zone

| Zone | Surface (mm²) | Composants | Densité |
|------|---------------|------------|---------|
| Zone 1 | 160 | 5 | Faible ✅ |
| Zone 2 | 240 | 9 | **Élevée** ⚠️ |
| Zone 3 | 400 | 10 | Moyenne |
| Zone 4 | 320 | 2 | Très faible ✅ |

**Zone 2 critique** : Espace limité entre boutons/LEDs/PCF8574
- Solution : Résistances LEDs en bottom layer
- Alternative : Décaler PCF8574 vers Zone 3

---

## 🎯 Points de Référence Clés

Pour vérification rapide dans KiCad :

```
Coin PCB : (0, 0)
Centre axe : Y = 8mm
Centre Zone 1 : (5, 8)
Centre Zone 2 : (17.5, 8)
Centre Zone 3 : (37.5, 8)
Centre Zone 4 : (60, 8)
Extrémité : (70, 16)
```

**Grille recommandée** : 1mm (View → Grid → 1mm)

---

Utilisez ce fichier comme référence pendant le placement dans KiCad ! 📐
