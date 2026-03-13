# Guide de Programmation PCBA avec USBasp

## Câblage USBasp → PCBA (Connecteur ISP)

| USBasp Pin | → | PCBA ATmega328P-AU (TQFP-32) |
|---|---|---|
| MOSI | → | PB3 (pin 15) |
| MISO | → | PB4 (pin 16) |
| SCK | → | PB5 (pin 17) |
| RESET | → | PC6 (pin 29) |
| VCC | → | VCC (pin 4 ou 6) |
| GND | → | GND (pin 3, 5, ou 21) |

## Configuration USBasp

- Positionner le **jumper JP1 sur "slow clock"** (parfois labellé SCK slow)
- Cela force la vitesse ISP à ~8kHz, compatible avec un ATmega à 1MHz (horloge interne par défaut)

## Configuration Arduino IDE

- `Tools > Programmer > USBasp`
- Pas de condensateur 10µF nécessaire
- Pas de sketch intermédiaire à charger
- Brancher directement et programmer

## Étapes de programmation

### Étape 1 : Vérifier la signature du chip

Lancer `Sketch > Upload Using Programmer`

- Si la signature lue est **`1E 95 0F`** → le chip est bien un ATmega328P et il répond correctement
- Si erreur de signature → vérifier le câblage et le jumper slow clock

### Étape 2 : Graver le bootloader (configurer les fuses)

`Tools > Burn Bootloader`

- Cela configure les **fuses** pour l'horloge interne 8MHz
- Installe le bootloader Optiboot (optionnel mais utile pour programmation série future)

**Fuses réels brûlés sur PCBA (8MHz oscillateur interne RC) :**
```
lfuse = 0xE2  (oscillateur interne RC 8MHz, CKDIV8 désactivé)
hfuse = 0xD9  (no bootloader, EESAVE off)
efuse = 0xFF  (BOD désactivé)
```

**Commande avrdude pour vérification :**
```
avrdude -c usbasp -p m328p -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
```

**Commande avrdude pour écriture manuelle des fuses :**
```
avrdude -c usbasp -p m328p -U lfuse:w:0xE2:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
```

### Étape 3 : Uploader le sketch

`Sketch > Upload Using Programmer`

- Charge le fichier `stylet_auriculo_ATmega328P_v7.0_flashVersion.ino` directement dans la flash de l'ATmega328P-AU

**Note** : Ce fichier utilise PCF8574A (adresse I2C = 0x38), pas PCF8574 (0x20).
Les LEDs indicateurs sont 3× rouges, active-HIGH (écrire 1 = ON).

## Avantages du USBasp vs Arduino-as-ISP

| Critère | Arduino as ISP | USBasp |
|---|---|---|
| Condensateur anti-reset | Requis (10µF+) | **Non requis** |
| Sketch intermédiaire | ArduinoISP à charger | **Aucun** |
| Gestion vitesse ISP | Manuelle (code) | **Automatique** |
| Fiabilité | Moyenne (conflits SPI/D10) | **Excellente** |
| Support 3.3V | Via level shifter | **Jumper intégré** |
| Prix | Gratuit (si Arduino dispo) | ~3€ |

## Notes importantes

- Le jumper **JP1 (slow clock)** est essentiel pour un ATmega neuf tournant à 1MHz (fuses usine : CKDIV8 activé)
- Après avoir gravé le bootloader (fuses 8MHz interne), le jumper slow clock peut être retiré pour les uploads suivants
- Pour programmer à **3.3V** (tension nominale du PCBA), vérifier si le USBasp a un jumper de sélection tension VCC (certains modèles proposent 5V/3.3V)
- Le PCBA n'a **PAS de cristal externe** : l'ATmega utilise l'oscillateur interne RC 8MHz (lfuse=0xE2)
- L'Arduino-as-ISP s'est révélé peu fiable pour ce projet (problèmes d'auto-reset non résolus avec avrdude 8.0)
- Le USBasp est **fortement recommandé** pour la programmation du PCBA
