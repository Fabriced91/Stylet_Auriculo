# 🔄 GUIDE DE MIGRATION v7.0 → v8.0

**Stylet Auriculothérapie - Mise à niveau vers OLED**

---

## 📋 Vue d'ensemble

Ce guide détaille toutes les étapes pour migrer votre projet KiCad de la version v7.0 (production minimale) vers la v8.0 (avec écran OLED et modulation réactivée).

### Changements matériels

| Composant | v7.0 | v8.0 | Action |
|-----------|------|------|--------|
| **Boutons** | 2 (FREQ, MODE) | 3 (FREQ, MODE, MENU) | ➕ Ajouter SW3 |
| **Connecteurs** | 2 (ISP, Batterie) | 3 (ISP, Batterie, OLED) | ➕ Ajouter J3 |
| **Affichage** | LEDs RGB uniquement | LEDs RGB + OLED 128×32 | ➕ Ajouter module OLED |
| **Pin PB0** | Non connecté (NC) | Bouton MENU | 🔧 Modifier |
| **Bus I2C** | 1 périphérique (PCF8574) | 2 périphériques (PCF8574 + OLED) | 🔧 Vérifier pull-ups |
| **Coût** | 20.88 € | 25.18 € | +4.30 € (+20.6%) |

---

## 🛠️ PARTIE 1 : MODIFICATIONS SCHÉMA KICAD

### Étape 1.1 : Ouvrir le projet v7.0

```bash
# Créer une copie de sauvegarde
cd "kicad_design/"
cp "Stylet Auriculotherapie.kicad_pro" "Stylet Auriculotherapie_v7_backup.kicad_pro"
cp "Stylet Auriculotherapie.kicad_sch" "Stylet Auriculotherapie_v7_backup.kicad_sch"
```

1. Ouvrir KiCad 9.0
2. Ouvrir le projet : `Stylet Auriculotherapie.kicad_pro`
3. Ouvrir l'éditeur de schéma

### Étape 1.2 : Ajouter le bouton MENU (SW3)

#### a) Placer le symbole

1. Appuyer sur **`A`** (Add Symbol)
2. Rechercher : `SW_Push`
3. Placer à proximité de SW1 et SW2
4. Référence : `SW3`
5. Valeur : `MENU`

#### b) Connecter SW3

1. **Pin 1** de SW3 :
   - Tracer un fil vers **PB0** (U1 pin 12)
   - ⚠️ **IMPORTANT** : Supprimer le flag "No Connect" qui était sur PB0 !
   
2. **Pin 2** de SW3 :
   - Connecter à **GND** (symbole masse)

3. Ajouter une **étiquette locale** sur le fil :
   - Nom : `BTN_MENU`

#### c) Assigner le footprint

1. Sélectionner SW3
2. Appuyer sur **`E`** (Edit Properties)
3. Onglet **Footprint** :
   - Choisir : `Button_Switch_THT:SW_SPST_6x6mm_H9.5mm`
4. OK

### Étape 1.3 : Ajouter le connecteur OLED (J3)

#### a) Placer le symbole connecteur

1. Appuyer sur **`A`** (Add Symbol)
2. Rechercher : `Conn_01x04`
3. Placer en haut à droite du schéma
4. Référence : `J3`
5. Valeur : `OLED_I2C`

#### b) Connecter J3

Ordre des pins (de haut en bas) :

| Pin J3 | Signal | Connexion | Net existant |
|--------|--------|-----------|--------------|
| **1** | GND | Masse | **GND** |
| **2** | VCC | Alimentation | **+3.3V** |
| **3** | SCL | I2C Clock | **SCL** (déjà utilisé par U2) |
| **4** | SDA | I2C Data | **SDA** (déjà utilisé par U2) |

**Instructions de câblage :**

1. **Pin 1** (GND) :
   - Tracer fil vers symbole **GND**

2. **Pin 2** (VCC) :
   - Tracer fil vers symbole **+3.3V**

3. **Pin 3** (SCL) :
   - Tracer fil vers le net **SCL** (rejoindre le bus I2C existant)
   - Ajouter étiquette : `SCL`

4. **Pin 4** (SDA) :
   - Tracer fil vers le net **SDA** (rejoindre le bus I2C existant)
   - Ajouter étiquette : `SDA`

#### c) Assigner le footprint

1. Sélectionner J3
2. Propriétés (**`E`**)
3. Footprint : 
   - Option 1 : `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical`
   - Option 2 : `Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical`
4. OK

💡 **Recommandation** : JST-XH est plus robuste et évite les inversions

### Étape 1.4 : Vérifier le bus I2C

Le bus I2C doit maintenant connecter **3 composants** :

```
        +3.3V
          |
    ┌─────┴──────┐
    │            │
   R2a (4.7kΩ) R2b (4.7kΩ)
    │            │
    SDA          SCL
    │            │
    ├────────────┼──────────────────┐
    │            │                  │
  U1 PC4       U1 PC5            U2 pins
  (pin 27)     (pin 28)         SDA/SCL
    │            │              (pins 15/14)
    └────────────┴──────────────┐
                                │
                              J3 pins
                              SDA/SCL
                              (pins 4/3)
```

**Vérifications :**
- ✅ R2a (4.7kΩ) connecté entre SDA et +3.3V
- ✅ R2b (4.7kΩ) connecté entre SCL et +3.3V
- ✅ Tous les SDA connectés au même net
- ✅ Tous les SCL connectés au même net

### Étape 1.5 : Mettre à jour les annotations

1. Menu **Tools** → **Annotate Schematic**
2. Vérifier que SW3 et J3 ont des références uniques
3. **Annotate**

### Étape 1.6 : Vérification ERC

1. Menu **Inspect** → **Electrical Rules Checker**
2. **Run ERC**

**Erreurs attendues :** **0 erreurs**

⚠️ **Si erreurs :**
- "Pin not connected on PB0" : Vous avez oublié de supprimer le flag NC
- "Power pin not driven" : Ajouter PWR_FLAG si nécessaire
- "Conflict on SCL/SDA" : Vérifier que les nets sont bien fusionnés

### Étape 1.7 : Sauvegarder le schéma

1. **Ctrl + S** ou File → Save
2. Fermer l'éditeur de schéma

---

## 🔧 PARTIE 2 : MODIFICATIONS PCB

### Étape 2.1 : Mettre à jour le PCB

1. Ouvrir **PCB Editor** (Stylet Auriculotherapie.kicad_pcb)
2. Menu **Tools** → **Update PCB from Schematic** (F8)
3. Une fenêtre s'ouvre montrant les changements :

```
Changes to be applied:
  ✅ Add SW3 (footprint SW_SPST_6x6mm)
  ✅ Add J3 (footprint PinHeader_1x04 ou JST_XH_B4B)
  ✅ Add nets: BTN_MENU
  ✅ Modify net on U1 pin 12: NC → BTN_MENU
```

4. Cliquer **Update PCB**

### Étape 2.2 : Placer SW3 (Bouton MENU)

Les nouveaux composants apparaissent **en dehors du PCB** (zone grise).

#### Placement de SW3 :

1. Sélectionner **SW3** (cliquer dessus)
2. Appuyer sur **`M`** (Move)
3. Positionner **entre SW1 et SW2** (alignement vertical)
4. Face : **B.Cu** (arrière du PCB)

**Coordonnées recommandées :**
- Si SW1 est à X=5mm et SW2 à X=15mm
- Placer SW3 à X=10mm (centre)
- Même Y que SW1/SW2

5. Appuyer sur **`R`** pour rotation si nécessaire
6. Clic gauche pour valider

### Étape 2.3 : Placer J3 (Connecteur OLED)

#### Placement de J3 :

1. Sélectionner **J3**
2. **`M`** (Move)
3. Position recommandée : 
   - **Bord supérieur** du PCB (côté LED principale D1)
   - Orientation : pins vers l'extérieur du PCB
4. Face : **F.Cu** (avant) ou **B.Cu** selon préférence

**Conseils placement :**
- Laisser 2-3mm du bord pour soudure
- Éviter de masquer les composants SMD (accès fer à souder)
- Prévoir espace pour câble OLED sortant

5. Rotation (**`R`**) jusqu'à orientation correcte
6. Clic gauche pour valider

### Étape 2.4 : Router les nouvelles pistes

#### a) Router BTN_MENU (PB0 → SW3)

1. Appuyer sur **`X`** (Route Tracks)
2. Cliquer sur pad **PB0** (U1 pin 12)
3. Tracer piste jusqu'à **SW3 pin 1**
4. Largeur piste : **0.3mm** (signal)
5. Couche : **B.Cu** de préférence (SW3 est en face arrière)

#### b) Router SW3 → GND

1. **`X`** (Route)
2. De **SW3 pin 2** vers le plan de masse
3. Ajouter un **via** si nécessaire pour rejoindre zone GND

#### c) Router J3 (4 connexions)

| Pin J3 | Net | Méthode |
|--------|-----|---------|
| Pin 1 (GND) | GND | Via vers zone GND |
| Pin 2 (VCC) | +3.3V | Piste vers net +3.3V (0.5mm) |
| Pin 3 (SCL) | SCL | Piste vers net SCL existant (0.3mm) |
| Pin 4 (SDA) | SDA | Piste vers net SDA existant (0.3mm) |

**Pour chaque pin :**
1. **`X`** (Route)
2. Cliquer sur pad J3
3. Tracer jusqu'au net correspondant
4. Utiliser **vias** si changement de couche nécessaire

💡 **Astuce** : Pour SCL/SDA, chercher les pistes existantes vers U2 (PCF8574) et se connecter dessus.

### Étape 2.5 : Mettre à jour les zones de cuivre (GND)

Les zones de masse doivent entourer les nouveaux composants.

1. Sélectionner une **zone GND** (clic droit → Zones)
2. Menu **Edit** → **Zones** → **Fill All Zones** (B)
3. Les zones se remplissent automatiquement

**Vérifier :**
- ✅ SW3 et J3 sont entourés par le cuivre GND
- ✅ Connexions thermiques (croix) visibles sur pads GND
- ✅ Pas d'îlots isolés

### Étape 2.6 : Vérification DRC

1. Menu **Inspect** → **Design Rules Checker**
2. **Run DRC**

**Erreurs critiques attendues :** **0**

⚠️ **Erreurs possibles :**
- "Clearance violation" : Pistes trop proches → espacer
- "Unconnected items" : Piste incomplète → vérifier connexions
- "Silkscreen over pad" : Warnings uniquement, OK pour ignorer

**Corrections :**
- Ajuster position J3 si collision
- Élargir espacement entre pistes (règle 0.2mm minimum)

### Étape 2.7 : Mettre à jour les sérigraphies

1. Vérifier que les références **SW3** et **J3** sont visibles
2. Si masquées par composants :
   - Sélectionner texte référence
   - **`M`** (Move) pour déplacer
   - Ou **`E`** → changer taille/position

3. Ajouter légendes (optionnel) :
   - Sur couche **F.Silkscreen** ou **B.Silkscreen**
   - Ajouter texte : "MENU" à côté de SW3
   - Ajouter texte : "OLED" à côté de J3

### Étape 2.8 : Sauvegarder le PCB

1. **Ctrl + S** ou File → Save
2. Générer aperçu 3D :
   - Menu **View** → **3D Viewer**
   - Vérifier l'aspect final

---

## 📦 PARTIE 3 : GÉNÉRATION FICHIERS FABRICATION

### Étape 3.1 : Mise à jour BOM

1. Dans **Schematic Editor** :
   - Menu **Tools** → **Generate BOM**
2. Exporter CSV
3. Ajouter manuellement :
   ```
   J3, Conn_01x04, JST_XH_B4B-XH-A, C144395
   SW3, SW_Push, SW_SPST_6x6mm, C318884
   OLED1, Display_OLED, SSD1306_128x32, -
   ```

### Étape 3.2 : Mise à jour CPL (Component Placement List)

1. Dans **PCB Editor** :
   - Menu **File** → **Fabrication Outputs** → **Component Placement (.pos)**
2. Format : **CSV**
3. Units : **Millimeters**
4. Include footprints with SMD pads only : **Décoché** (pour inclure J3/SW3 si THT)
5. **Generate Position File**

**Vérifier dans le CSV généré :**
- SW3 présent avec coordonnées X, Y, rotation
- J3 présent avec coordonnées X, Y, rotation

### Étape 3.3 : Regénérer Gerbers

1. Menu **File** → **Plot**
2. Sélectionner toutes les couches :
   - ✅ F.Cu
   - ✅ B.Cu
   - ✅ F.Mask
   - ✅ B.Mask
   - ✅ F.Silkscreen
   - ✅ B.Silkscreen
   - ✅ Edge.Cuts
3. Options :
   - Plot format : **Gerber**
   - Plot footprint values : **Coché**
   - Plot reference designators : **Coché**
4. **Plot**

5. **Generate Drill Files** :
   - PTH (plated through holes)
   - NPTH (non-plated, si nécessaire)
   - Format : **Excellon**

### Étape 3.4 : Vérifier les Gerbers

1. Utiliser **GerbView** (inclus KiCad)
2. Charger tous les .gbr
3. Vérifier visuellement :
   - ✅ SW3 visible sur B.Cu
   - ✅ J3 visible (F.Cu ou B.Cu)
   - ✅ Pads percés correctement
   - ✅ Connexions SCL/SDA complètes

---

## 📋 PARTIE 4 : MISE À JOUR DOCUMENTATION

### Étape 4.1 : Fichiers à mettre à jour

Créer/modifier ces fichiers :

```
last version/
  ├── BOM_complete_v8.0.txt                     [✅ CRÉÉ]
  ├── schema_electronique_ATmega328P_v8.0.txt   [✅ CRÉÉ]
  ├── stylet_auriculo_ATmega328P_v8.0.ino       [✅ CRÉÉ]
  ├── RESUME_EXECUTIF_v8.0.txt                  [✅ CRÉÉ]
  ├── GUIDE_MIGRATION_v7_to_v8.md               [📖 CE FICHIER]
  └── guide_pcb_layout_16x70mm_v8.0.txt         [À CRÉER]
```

### Étape 4.2 : Ajouter notes de version

Créer fichier `CHANGELOG_v8.0.md` :

```markdown
# Changelog v8.0

## Ajouts matériels
- ➕ Écran OLED SSD1306 128×32 I2C (adresse 0x3C)
- ➕ Connecteur J3 (JST-XH-4) pour OLED
- ➕ Bouton MENU (SW3) sur PB0

## Modifications schéma
- 🔧 PB0 (ATmega pin 12) : NC → Bouton MENU
- 🔧 Bus I2C : ajout périphérique OLED (0x3C)

## Modifications code
- ✅ Modulation Nogier réactivée (976Hz + freq)
- ✅ Gestion menu OLED 3 écrans
- ✅ Timer de soin avec chronomètre
- ✅ Extinction auto écran (10s)
- ✅ Sauvegarde config EEPROM

## Coûts
- v7.0 : 20.88 €
- v8.0 : 25.18 € (+4.30 €, +20.6%)
```

---

## 🧪 PARTIE 5 : TESTS & VALIDATION

### Étape 5.1 : Tests sur breadboard (recommandé)

Avant fabrication PCB, tester sur breadboard :

**Matériel nécessaire :**
- Arduino Uno ou Nano
- OLED SSD1306 128×32 I2C
- 3 boutons poussoirs
- Câbles jumper

**Schéma breadboard :**
```
Arduino Uno → OLED
  GND       → GND
  5V        → VCC (⚠️ OLED doit accepter 5V ou utiliser level shifter)
  A4 (SDA)  → SDA
  A5 (SCL)  → SCL

Arduino Uno → Boutons
  D2        → SW1 FREQ (+ GND)
  D4        → SW2 MODE (+ GND)
  D8        → SW3 MENU (+ GND)
```

**Test code v8.0 :**
1. Installer bibliothèques :
   - Adafruit GFX
   - Adafruit SSD1306
2. Uploader `stylet_auriculo_ATmega328P_v8.0.ino`
3. Ouvrir Serial Monitor (115200 baud)

**Vérifications :**
- ✅ OLED s'allume et affiche splash screen
- ✅ Scanner I2C détecte 0x3C
- ✅ Bouton FREQ change fréquence
- ✅ Bouton MODE change mode
- ✅ Bouton MENU ouvre menu config
- ✅ Timer démarre et compte

### Étape 5.2 : Commande PCB prototype

**Recommandation :** Commander **1 PCB seul** avant lot de 5.

**Fournisseurs :**
1. **JLCPCB** (Chine, rapide)
   - 1 PCB nu : ~2 € + port
   - Délai : 1 semaine

2. **PCBWay** (Chine)
   - 1 PCB nu : ~5 € + port
   - Qualité supérieure

3. **Eurocircuits** (Europe)
   - 1 PCB nu : ~15 € + port
   - Livraison 48h

### Étape 5.3 : Assemblage prototype

**Ordre soudure :**
1. Composants SMD (si pas PCBA JLCPCB)
2. Cristal Y1
3. Connecteurs J1, J2, **J3**
4. Boutons SW1, SW2, **SW3**
5. LED principale D1

**Outils nécessaires :**
- Fer à souder 350°C
- Étain 0.5mm
- Flux
- Pince brucelles
- Loupe/microscope (SMD 0805)

### Étape 5.4 : Programmation & test final

1. **Installer bootloader ATmega328P :**
   ```bash
   # Via USBasp ou Arduino as ISP
   avrdude -c usbasp -p m328p -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0xFD:m
   ```

2. **Uploader code v8.0 :**
   - Via ISP ou UART (si bootloader installé)
   - Fichier : `stylet_auriculo_ATmega328P_v8.0.ino`

3. **Connecter OLED :**
   - Câble JST-4 : vérifier ordre pins !
   - GND-VCC-SCL-SDA (peut varier selon module)

4. **Tests fonctionnels :**
   ```
   ✅ Affichage OLED au démarrage
   ✅ Batterie affichée (%)
   ✅ LEDs RGB changent (FREQ)
   ✅ Modes changent (MODE)
   ✅ Menu s'ouvre (MENU)
   ✅ Timer configurable
   ✅ Modulation visible oscilloscope
   ```

---

## ⚠️ PROBLÈMES COURANTS & SOLUTIONS

### Problème 1 : OLED ne s'affiche pas

**Symptômes :** Écran noir, pas de splash screen

**Causes possibles :**
1. Mauvais câblage J3
   - **Solution :** Vérifier ordre pins GND/VCC/SCL/SDA
   
2. Adresse I2C incorrecte
   - **Solution :** Scanner I2C avec sketch test :
   ```cpp
   #include <Wire.h>
   void setup() {
     Wire.begin();
     Serial.begin(115200);
     Serial.println("Scanning I2C...");
     for(byte i = 0; i < 128; i++) {
       Wire.beginTransmission(i);
       if(Wire.endTransmission() == 0) {
         Serial.print("Device at 0x");
         Serial.println(i, HEX);
       }
     }
   }
   ```
   - Doit afficher : `0x20` (PCF8574) et `0x3C` (OLED)

3. OLED défectueux
   - **Solution :** Tester sur Arduino Uno séparément

### Problème 2 : Bouton MENU ne répond pas

**Causes :**
1. PB0 toujours marqué NC dans schéma
   - **Solution :** Retour Étape 1.2b, supprimer flag NC

2. Mauvais numéro pin dans code
   - **Vérifier code ligne ~15 :**
   ```cpp
   const uint8_t PIN_BTN_MENU = 8;  // PB0 = D8 sur ATmega328P
   ```

3. Pull-up interne désactivé
   - **Vérifier code setup() :**
   ```cpp
   pinMode(PIN_BTN_MENU, INPUT_PULLUP);
   ```

### Problème 3 : Conflit adresses I2C

**Symptôme :** Un des deux périphériques ne répond pas

**Diagnostic :**
```cpp
// Scanner I2C doit afficher :
// Device at 0x20  ← PCF8574
// Device at 0x3C  ← OLED
```

**Si 0x20 absent :**
- Vérifier soudure PCF8574 (SOIC-16)
- Vérifier pins A0/A1/A2 à GND

**Si 0x3C absent :**
- Vérifier câble OLED
- Vérifier tension VCC (doit être 3.3V)

### Problème 4 : Consommation excessive

**Symptôme :** Batterie se vide trop vite

**Causes :**
1. OLED reste allumé en permanence
   - **Solution :** Vérifier extinction auto (timeout 10s)
   - Code ligne ~75 : `const uint16_t DISPLAY_TIMEOUT = 10000;`

2. Modulation CPU intensive
   - **Solution :** Normale, modulation recalcule PWM toutes les 1ms
   - Compromis : désactiver modulation via menu

**Consommation normale v8.0 :**
- Détection OLED ON : 26.5mA
- Détection OLED OFF : 11.5mA
- Traitement OLED ON : 41.5mA

---

## 📊 CHECKLIST FINALE

### Avant fabrication PCB

- [ ] Schéma v8.0 : ERC = 0 erreurs
- [ ] PCB v8.0 : DRC = 0 erreurs critiques
- [ ] SW3 ajouté et connecté à PB0
- [ ] J3 ajouté avec 4 pins (GND/VCC/SCL/SDA)
- [ ] PB0 n'a plus de flag NC
- [ ] Bus I2C connecte 3 composants (U1/U2/J3)
- [ ] Pull-ups I2C présents (R2a, R2b = 4.7kΩ)
- [ ] Zones GND refilled (englobent SW3/J3)
- [ ] Gerbers générés (9 fichiers .gbr + .drl)
- [ ] BOM mise à jour avec J3/SW3/OLED
- [ ] CPL mise à jour avec nouvelles positions

### Avant assemblage

- [ ] Composants SMD soudés (ou PCBA JLCPCB)
- [ ] Cristal Y1 soudé
- [ ] Boutons SW1/SW2/SW3 soudés
- [ ] Connecteurs J1/J2/J3 soudés
- [ ] LED D1 soudée (polarité respectée)
- [ ] Continuité testée (+3.3V, GND, SDA, SCL)

### Avant programmation

- [ ] Bibliothèques Arduino installées (Adafruit GFX + SSD1306)
- [ ] Code v8.0 compilé sans erreur
- [ ] Fuses ATmega328P configurés (8MHz cristal)
- [ ] Programmateur ISP prêt (USBasp ou Arduino as ISP)

### Avant test final

- [ ] OLED connecté via câble J3 (ordre pins vérifié)
- [ ] Batterie Li-Ion connectée (polarité vérifiée)
- [ ] Multimètre : tension +3.3V stable
- [ ] Scanner I2C détecte 0x20 et 0x3C

---

## 🎉 FÉLICITATIONS !

Si toutes les étapes sont validées, votre **Stylet v8.0** est prêt !

### Prochaines étapes

1. **Tests terrain** : Utiliser en condition réelle (séance auriculothérapie)
2. **Boîtier 3D** : Concevoir/imprimer boîtier avec fenêtre OLED
3. **Manuel utilisateur** : Documenter navigation menu OLED
4. **Partage projet** : Publier sur GitHub avec photos/vidéos

### Ressources

- 📖 [Guide Menu OLED](GUIDE_MENU_OLED_v8.0.md) - Navigation interface
- 📋 [BOM Complète v8.0](BOM_complete_v8.0.txt)
- 🔧 [Schéma Électronique v8.0](schema_electronique_ATmega328P_v8.0.txt)
- 💾 [Code Source v8.0](stylet_auriculo_ATmega328P_v8.0.ino)

---

**Bon courage pour votre migration v7.0 → v8.0 ! 🚀**

*Guide créé par Fabrice Deconynck - Février 2026*
