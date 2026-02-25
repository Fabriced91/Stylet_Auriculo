# 📱 GUIDE MENU OLED v8.0

**Stylet Auriculothérapie - Interface utilisateur OLED**

---

## 📋 Vue d'ensemble

Le Stylet v8.0 intègre un écran OLED SSD1306 128×32 pixels qui affiche en temps réel :
- 🔋 Niveau batterie
- 🎨 Mode actif (Détection/Traitement/Blanc)
- 📡 Fréquence Nogier (A-G, Hz)
- ⏱️ Timer de soin (si actif)
- ⚙️ Menu configuration

### Spécifications OLED

| Caractéristique | Valeur |
|-----------------|--------|
| **Résolution** | 128 × 32 pixels |
| **Taille** | 0.91 pouces |
| **Interface** | I2C (adresse 0x3C) |
| **Couleur** | Blanc ou Bleu (selon modèle) |
| **Consommation** | 15mA (allumé), 0mA (éteint) |

---

## 🎮 CONTRÔLES & NAVIGATION

### Les 3 boutons

```
┌─────────────────────────────────────┐
│  [FREQ]    [MODE]       [MENU]      │
│    ▲         ▲            ▲         │
│    │         │            │         │
│  Change   Change   Ouvre/Navigue    │
│  fréq.    mode     dans menu        │
└─────────────────────────────────────┘
```

| Bouton | Fonction principale | Fonction dans menu |
|--------|---------------------|-------------------|
| **FREQ** | Change fréquence Nogier (A→B→C...→G→A) | Modifie valeur sélectionnée (+1) |
| **MODE** | Change mode (Détection→Traitement→Blanc→...) | Valide/démarre l'action |
| **MENU** | Ouvre menu configuration | Navigue entre options / Retour |

---

## 📺 ÉCRANS DISPONIBLES

Le système alterne entre 3 états d'affichage :

### État 1 : Écran principal (par défaut)

C'est l'écran de travail affiché pendant l'utilisation normale.

```
╔════════════════════════════════╗
║ ON | Bat: 87%                  ║  ← Ligne 1: Statut + Batterie
║ Mode: Traitement               ║  ← Ligne 2: Mode actif
║ Freq: D 18.25Hz [MOD]    05:32 ║  ← Ligne 3: Fréq. + Timer
╚════════════════════════════════╝
     ▲                        ▲
     │                        └─ Timer si actif (mm:ss)
     └─ [MOD] si modulation active
```

**Informations affichées :**

| Zone | Contenu | Mise à jour |
|------|---------|-------------|
| **Ligne 1** | État ON + Batterie % | Toutes les 5s |
| **Ligne 2** | Mode actuel | À chaque changement |
| **Ligne 3** | Fréquence (lettre + Hz) | À chaque changement |
| **[MOD]** | Indicateur modulation | Si modulation ON |
| **Timer** | Décompte mm:ss | Toutes les 1s |

**Sortie automatique :**
- Si aucune interaction pendant **10 secondes** → écran s'éteint (économie batterie)
- Appuyer sur n'importe quel bouton pour rallumer

---

### État 2 : Menu Configuration

Accessible via bouton **MENU** (appui court).

```
╔════════════════════════════════╗
║ === MENU ===                   ║  ← Titre
║ > Modulation: ON               ║  ← Option 1 (sélectionnée)
║   Timer: 10 min                ║  ← Option 2
╚════════════════════════════════╝
  ▲
  └─ Curseur ">" indique sélection
```

**Navigation :**
- **MENU** : Change sélection (Option 1 ↔ Option 2)
- **FREQ** : Modifie valeur de l'option sélectionnée
- **MENU** (long appui 2s) : Retour écran principal

**Options disponibles :**

#### Option 1 : Modulation ON/OFF

```
> Modulation: ON   ← FREQ pour basculer OFF
  Modulation: OFF  ← FREQ pour basculer ON
```

**Effet :**
- **ON** : Modulation Nogier active (LED modulée selon fréquence)
- **OFF** : LED intensité fixe (pas de modulation)

💾 **Sauvegarde** : Réglage enregistré en EEPROM (persistant après redémarrage)

#### Option 2 : Timer (durée en minutes)

```
  Timer: 10 min  ← FREQ pour incrémenter
  Timer: 11 min  ← Appuis successifs : 12, 13... 60, 1, 2...
```

**Plage :** 1 à 60 minutes (cycle bouclé)

💾 **Sauvegarde** : Durée timer enregistrée en EEPROM

**Pour démarrer le timer :**
- Sélectionner l'option Timer (curseur ">")
- Appuyer sur **MODE** → Lance le timer avec durée affichée
- Retour automatique écran principal avec décompte actif

---

### État 3 : Configuration Timer

Accessible depuis Menu Config → sélectionner "Timer" → appuyer **MODE**.

```
╔════════════════════════════════╗
║ Timer Configuration            ║  ← Titre
║ Duree: 15 min                  ║  ← Durée configurée
║ FREQ:+1  MODE:Start            ║  ← Aide boutons
╚════════════════════════════════╝
```

**Contrôles :**
- **FREQ** : Incrémente durée (+1 minute par appui)
- **MODE** : Démarre le timer et retourne à l'écran principal
- **MENU** : Annule et retourne au menu config

**Déroulement timer :**

1. Appui sur **MODE** → Timer démarre
2. Retour écran principal avec décompte :
   ```
   ╔════════════════════════════════╗
   ║ ON | Bat: 87%                  ║
   ║ Mode: Traitement               ║
   ║ Freq: D 18.25Hz [MOD]    14:58 ║  ← Décompte en temps réel
   ╚════════════════════════════════╝
   ```

3. Quand timer atteint **00:00** :
   - ⏰ **ALARME** : LED clignote 6× (200ms ON/OFF)
   - Timer s'arrête automatiquement

**Annuler un timer en cours :**
- Appuyer sur **MENU** → Ouvrir menu config → Timer s'arrête

---

## 🔋 INDICATEURS BATTERIE

### Affichage pourcentage

```
Bat: 100%  ← Batterie pleine (4.2V)
Bat: 75%   ← Batterie bonne
Bat: 50%   ← Batterie moyenne
Bat: 25%   ← Batterie faible
Bat: 10%   ← ⚠️ Recharger rapidement
Bat: 5%    ← ⚠️ Critique
```

**Calcul automatique :**
- Mesure tension batterie toutes les 5 secondes
- Conversion linéaire : 3.0V (0%) → 4.2V (100%)
- Diviseur résistif R8/R9 (100kΩ/33kΩ) sur ADC0

### États batterie

| Niveau | Action recommandée |
|--------|-------------------|
| **100-80%** | ✅ Autonomie complète |
| **80-50%** | 🟢 Utilisation normale |
| **50-20%** | 🟡 Prévoir recharge prochaine |
| **20-10%** | 🟠 Recharger bientôt |
| **<10%** | 🔴 Recharger immédiatement |

⚠️ **Message série** si batterie <20% :
```
⚠️ Batterie faible: 18%
```

---

## 🎨 MODES D'UTILISATION

### Mode 1 : Détection

```
╔════════════════════════════════╗
║ Mode: Detection                ║
║ Freq: A 2.28Hz [MOD]           ║
╚════════════════════════════════╝
```

**Caractéristiques :**
- LED principale : **25% intensité** (5mA)
- Usage : Recherche points auriculaires sensibles
- Modulation : Active (si ON dans menu)
- Autonomie : ~75h (OLED allumé), ~174h (éteint)

**Couleur LED RGB :** Selon fréquence (voir tableau)

---

### Mode 2 : Traitement

```
╔════════════════════════════════╗
║ Mode: Traitement               ║
║ Freq: E 36.50Hz [MOD]          ║
╚════════════════════════════════╝
```

**Caractéristiques :**
- LED principale : **92% intensité** (20mA)
- Usage : Stimulation thérapeutique point identifié
- Modulation : Active (si ON dans menu)
- Autonomie : ~48h (OLED allumé)

**Conseil :** Utiliser avec timer (typiquement 5-10 min par point)

---

### Mode 3 : Full Spectre (Blanc)

```
╔════════════════════════════════╗
║ Mode: Full Spectre             ║
║ Freq: G 146.00Hz               ║  ← Pas de [MOD]
╚════════════════════════════════╝
```

**Caractéristiques :**
- LED principale : **50% intensité** (10mA)
- Usage : Éclairage continu sans modulation
- Modulation : **Désactivée** (même si ON dans menu)
- Autonomie : ~63h

**Particularité :** Mode sans fréquence thérapeutique, simple éclairage

---

## 📡 FRÉQUENCES NOGIER

### Tableau des 7 fréquences

```
╔═══╦═══════╦═══════════╦══════════════╗
║ # ║ Lettre║ Fréquence ║ Couleur LED  ║
╠═══╬═══════╬═══════════╬══════════════╣
║ 1 ║   A   ║  2.28 Hz  ║ 🔴 Rouge     ║
║ 2 ║   B   ║  4.56 Hz  ║ 🟢 Vert      ║
║ 3 ║   C   ║  9.12 Hz  ║ 🟡 Jaune     ║
║ 4 ║   D   ║ 18.25 Hz  ║ 🔵 Bleu      ║
║ 5 ║   E   ║ 36.50 Hz  ║ 🟣 Magenta   ║
║ 6 ║   F   ║ 73.00 Hz  ║ 🔷 Cyan      ║
║ 7 ║   G   ║ 146.00 Hz ║ ⚪ Blanc     ║
╚═══╩═══════╩═══════════╩══════════════╝
```

### Affichage à l'écran

**Format :** `Freq: [Lettre] [Valeur]Hz [MOD]`

Exemples :
```
Freq: A 2.28Hz [MOD]   ← Fréquence A avec modulation
Freq: D 18.25Hz        ← Fréquence D sans modulation
Freq: G 146.00Hz       ← Fréquence G (mode blanc, jamais [MOD])
```

**Cycle :** Appuis successifs sur **FREQ** :
```
A → B → C → D → E → F → G → A → B → ...
```

---

## ⏱️ TIMER DE SOIN

### Utilisation typique

**Scénario : Traiter un point pendant 8 minutes**

1. **Configurer durée :**
   - Appuyer **MENU** → Menu config s'affiche
   - Curseur sur "Timer: 10 min"
   - Appuyer **FREQ** plusieurs fois jusqu'à "Timer: 8 min"

2. **Démarrer timer :**
   - Appuyer **MODE** → Timer démarre
   - Écran principal affiche décompte

3. **Traitement en cours :**
   ```
   ╔════════════════════════════════╗
   ║ ON | Bat: 92%                  ║
   ║ Mode: Traitement               ║
   ║ Freq: C 9.12Hz [MOD]     07:45 ║  ← Décompte actif
   ╚════════════════════════════════╝
   ```

4. **Fin du timer :**
   - À **00:00** : LED clignote 6× rapidement
   - Message série : `⏰ TIMER TERMINÉ !`
   - Timer s'arrête, écran reste sur 00:00

5. **Recommencer :**
   - Appuyer **MENU** → Reconfigurer timer
   - Ou changer de point et relancer

### Pause/Annulation

**Mettre en pause :**
- ❌ Pas de fonction pause directe
- Astuce : Passer en mode Blanc (arrête modulation) mais timer continue

**Annuler :**
- Appuyer **MENU** → Ouvre menu config
- Timer s'arrête automatiquement
- Reconfigurer durée si besoin

**Redémarrer :**
- Depuis menu config :
  - Sélectionner "Timer: X min"
  - Appuyer **MODE**

---

## 🔧 MODULATION EXPLIQUÉE

### Qu'est-ce que la modulation ?

La modulation consiste à faire varier l'intensité de la LED selon une **sinusoïde** à la fréquence Nogier sélectionnée.

**Formule :**
```
PWM(t) = Duty_moyen ± (Amplitude × sin(2π × Freq_Nogier × t))
```

### Visualisation

**Exemple fréquence D (18.25 Hz) :**

```
Intensité LED
    ▲
100%┤     ╱╲       ╱╲       ╱╲
    │    ╱  ╲     ╱  ╲     ╱  ╲
 50%┼───────────────────────────  ← Duty moyen (50%)
    │   ╱    ╲   ╱    ╲   ╱    ╲
  0%┤  ╱      ╲ ╱      ╲ ╱      ╲
    └──────────────────────────────► Temps
       ←─ 54.8ms ─→  (1 cycle @ 18.25Hz)
```

**Paramètres :**
- **Porteuse** : 976 Hz (fréquence PWM Timer2)
- **Modulation** : Fréquence Nogier (2.28 à 146 Hz)
- **Profondeur** : 70% (amplitude ±70% autour duty moyen)

### ON vs OFF

| État | Affichage | Comportement LED |
|------|-----------|------------------|
| **Modulation ON** | `[MOD]` visible | Intensité oscille selon fréquence Nogier |
| **Modulation OFF** | Pas de `[MOD]` | Intensité fixe (duty cycle constant) |

**Désactiver modulation :**
- Menu config → "Modulation: ON"
- Appuyer **FREQ** → bascule "OFF"
- Sauvegardé en EEPROM (rémanent)

**Pourquoi désactiver ?**
- Économie batterie (~10% économie CPU)
- Simplicité (certains préfèrent lumière stable)
- Test/diagnostic

---

## 💾 SAUVEGARDE EEPROM

### Données persistantes

Le stylet sauvegarde automatiquement certains réglages en **mémoire EEPROM** (non volatile).

**Données sauvegardées :**
- ✅ État modulation (ON/OFF)
- ✅ Durée timer (1-60 minutes)

**Non sauvegardé :**
- ❌ Fréquence actuelle (A-G)
- ❌ Mode actuel (Détection/Traitement/Blanc)
- ❌ État timer en cours

**Au redémarrage :**
```
Fréquence : A (par défaut)
Mode      : Détection (par défaut)
Modulation: Dernière valeur sauvegardée
Timer durée: Dernière valeur sauvegardée
```

**Pourquoi ?**
- Préserver préférences utilisateur
- Éviter de reconfigurer à chaque allumage

**Corruption EEPROM :**
- Si valeurs anormales détectées → réinitialisation valeurs par défaut :
  - Modulation : ON
  - Timer : 10 minutes

---

## 🌙 ÉCONOMIE D'ÉNERGIE

### Extinction automatique écran

**Timeout : 10 secondes**

```
Utilisateur appuie sur bouton
         ↓
    OLED s'allume
         ↓
   Compteur 10s démarre
         ↓
   [10 secondes passent]
         ↓
    OLED s'éteint (économie 15mA)
         ↓
   Prochain appui bouton → Rallume OLED
```

**Message série :**
```
💤 OLED éteint (économie énergie)
💡 OLED rallumé
```

### Impact sur autonomie

**Scénario réaliste (usage mixte) :**
- 30% du temps : OLED allumé (interactions)
- 70% du temps : OLED éteint (pause)

**Calcul autonomie mode Détection :**
```
Consommation moyenne = 0.3 × 26.5mA + 0.7 × 11.5mA
                     = 7.95 + 8.05 = 16mA

Autonomie batterie 2000mAh = 2000 / 16 = 125 heures (5.2 jours)
```

**Comparaison v7.0 vs v8.0 :**

| Version | Mode | OLED | Consommation | Autonomie |
|---------|------|------|--------------|-----------|
| **v7.0** | Détection | - | 11.5mA | 174h (7.2j) |
| **v8.0** | Détection | ON | 26.5mA | 75h (3.1j) |
| **v8.0** | Détection | OFF | 11.5mA | 174h (7.2j) |
| **v8.0** | Mixte 70/30 | Auto | 16mA | **125h (5.2j)** |

💡 **Recommandation :** Laisser extinction auto activée (timeout 10s)

---

## 🎯 SCÉNARIOS D'UTILISATION

### Scénario 1 : Séance auriculothérapie complète

**Objectif :** Traiter 5 points, 8 minutes chacun

1. **Démarrage :**
   - Allumer stylet
   - Vérifier batterie : `Bat: 95%` ✅
   - Mode par défaut : Détection

2. **Recherche point 1 :**
   - Mode : Détection (intensité faible)
   - Fréquence : C (9.12 Hz)
   - Palper zone jusqu'à sensation

3. **Traitement point 1 :**
   - Appuyer **MODE** → Passe en Traitement (intensité forte)
   - Appuyer **MENU** → Menu config
   - Régler timer : 8 min
   - Appuyer **MODE** → Timer démarre
   - Maintenir stylet sur point pendant décompte
   - ⏰ Alarme à 00:00 → Passer au point suivant

4. **Points suivants :**
   - Répéter étapes 2-3 pour chaque point
   - Changer fréquence si protocole différent (FREQ)

5. **Fin séance :**
   - 5 points × 8 min = 40 minutes
   - Batterie finale : `Bat: 82%` ✅
   - Éteindre stylet

---

### Scénario 2 : Test nouvelle fréquence

**Objectif :** Essayer fréquence F (73 Hz) sans modulation

1. **Configuration :**
   - Allumer stylet
   - Appuyer **FREQ** plusieurs fois jusqu'à "Freq: F 73.00Hz"
   - LED RGB affiche **Cyan** ✅

2. **Désactiver modulation :**
   - Appuyer **MENU** → Menu config
   - "Modulation: ON" sélectionné
   - Appuyer **FREQ** → Bascule "OFF"
   - `[MOD]` disparaît de l'écran principal

3. **Test traitement :**
   - Appuyer **MODE** jusqu'à mode Traitement
   - LED intensité fixe (pas de variation)
   - Observer effet clinique

4. **Réactiver modulation :**
   - **MENU** → "Modulation: OFF"
   - **FREQ** → "Modulation: ON"
   - `[MOD]` réapparaît ✅

---

### Scénario 3 : Utilisation prolongée (journée entière)

**Objectif :** Maximiser autonomie pour 10h de travail

1. **Optimisations :**
   - Utiliser majoritairement mode **Détection** (consommation faible)
   - Laisser extinction auto OLED (10s) active
   - Désactiver modulation si non essentielle

2. **Monitoring batterie :**
   - Début journée : `Bat: 100%`
   - Après 2h : `Bat: 92%`
   - Après 5h : `Bat: 75%`
   - Après 8h : `Bat: 55%`
   - Après 10h : `Bat: 40%` ✅ OK

3. **Recharge :**
   - Si `Bat: <20%` → Recharger avant prochaine utilisation
   - Module TP4056 : ~2h pour charge complète

---

## 🐛 DÉPANNAGE INTERFACE

### Problème : OLED reste noir

**Diagnostic :**
```
1. Vérifier alimentation OLED (J3 pin 2) : doit être 3.3V
2. Vérifier câble J3 (ordre pins GND/VCC/SCL/SDA)
3. Scanner I2C → doit détecter 0x3C
```

**Solution :**
- Si 0x3C absent → OLED défectueux ou mal câblé
- Tester OLED sur Arduino Uno séparément

---

### Problème : Affichage corrompu

**Symptômes :** Caractères illisibles, pixels aléatoires

**Causes :**
1. Interférences I2C (câble trop long)
2. Condensateur découplage insuffisant

**Solutions :**
- Ajouter condensateur 100nF proche OLED
- Raccourcir câble J3 (max 15cm recommandé)
- Vérifier résistances pull-up I2C (4.7kΩ)

---

### Problème : Menu ne répond pas

**Diagnostic :**
```cpp
// Ajouter debug dans code :
Serial.print("Menu state: ");
Serial.println(menuState);
```

**Vérifier :**
- Bouton MENU bien connecté à PB0 (pin 12)
- Pull-up interne activé : `INPUT_PULLUP`
- Anti-rebond fonctionnel (50ms)

---

### Problème : Timer ne démarre pas

**Causes possibles :**
1. Variable `timerEnabled` reste `false`
2. Durée `timerSetMinutes` = 0

**Debug :**
```cpp
Serial.print("Timer enabled: ");
Serial.println(timerEnabled);
Serial.print("Timer minutes: ");
Serial.println(timerSetMinutes);
```

**Solution :**
- Recharger configuration EEPROM
- Réinitialiser EEPROM :
  ```cpp
  EEPROM.write(EEPROM_ADDR_TIMER_MINUTES, 10);  // Défaut 10 min
  ```

---

## 📚 RÉFÉRENCE RAPIDE

### Raccourcis clavier

| Action | Bouton(s) | Contexte |
|--------|-----------|----------|
| Changer fréquence | **FREQ** | Écran principal |
| Changer mode | **MODE** | Écran principal |
| Ouvrir menu | **MENU** | Écran principal |
| Toggle modulation | **FREQ** | Menu config (option 1 sélectionnée) |
| Incrémenter timer | **FREQ** | Menu config (option 2 sélectionnée) |
| Démarrer timer | **MODE** | Menu config (option 2 sélectionnée) |
| Naviguer menu | **MENU** | Menu config |
| Retour écran principal | **MENU** (2s) | N'importe où |
| Rallumer OLED | N'importe quel bouton | OLED éteint |

---

### Codes d'état série (debug)

**Messages informatifs :**
```
✅ OLED initialisé
✅ Système initialisé
📋 Menu config ouvert
⏱️ Configuration timer
🔙 Retour écran principal
🔙 Retour menu config
💤 OLED éteint (économie énergie)
💡 OLED rallumé
💾 Configuration sauvegardée
📂 Configuration chargée
```

**Messages d'alerte :**
```
⚠️ Batterie faible: 18%
⏰ TIMER TERMINÉ !
❌ OLED non détecté !
❌ PCF8574 non détecté !
```

---

## 🎓 CONSEILS D'UTILISATION

### Bonnes pratiques

1. **Batterie :**
   - Recharger quand <20%
   - Ne pas laisser descendre à 0% (endommage Li-Ion)

2. **OLED :**
   - Laisser extinction auto active (prolonge durée de vie OLED)
   - Éviter affichage statique prolongé (risque burn-in)

3. **Modulation :**
   - Tester avec/sans pour comparer effet thérapeutique
   - Désactiver si batterie faible (économie 10% CPU)

4. **Timer :**
   - Configurer une fois, sauvegarder (EEPROM)
   - Utiliser systématiquement pour traçabilité séances

### Personnalisation

**Modifier timeout écran :**
```cpp
// Dans code ligne ~75 :
const uint16_t DISPLAY_TIMEOUT = 10000;  // 10s
// Changer en :
const uint16_t DISPLAY_TIMEOUT = 30000;  // 30s
```

**Modifier profondeur modulation :**
```cpp
// Dans code ligne ~65 :
const float MODULATION_DEPTH = 0.70;  // 70%
// Changer en :
const float MODULATION_DEPTH = 0.50;  // 50% (plus subtil)
```

---

## 🔗 RESSOURCES COMPLÉMENTAIRES

- 📖 [Guide Migration v7→v8](GUIDE_MIGRATION_v7_to_v8.md)
- 🔧 [Schéma Électronique v8.0](schema_electronique_ATmega328P_v8.0.txt)
- 💾 [Code Source v8.0](stylet_auriculo_ATmega328P_v8.0.ino)
- 📋 [BOM Complète v8.0](BOM_complete_v8.0.txt)

---

**Bon usage de votre Stylet v8.0 ! 🎉**

*Manuel utilisateur créé par Fabrice Deconynck - Février 2026*
