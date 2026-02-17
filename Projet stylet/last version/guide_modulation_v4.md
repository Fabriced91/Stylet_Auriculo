# ═══════════════════════════════════════════════════════════════
#  GUIDE D'UTILISATION - MODES DE MODULATION v4.0
# ═══════════════════════════════════════════════════════════════

## 🎛️ LES 3 MODES DE MODULATION

### Vue d'ensemble

Le stylet propose **3 modes de modulation** pour adapter le confort visuel tout en conservant l'efficacité thérapeutique des fréquences de Nogier.

---

## ✨ MODE 1 : SINUSOÏDAL (Recommandé)

### Caractéristiques
```
Forme d'onde :
    ╱╲      ╱╲      ╱╲
  ╱    ╲  ╱    ╲  ╱    ╲
──      ╲╱      ╲╱      ╲──

• Variation DOUCE et progressive
• Pas de transition brutale
• Très confortable visuellement
```

### Avantages
- ✅ **Confort maximal** : Aucun effet stroboscopique
- ✅ **Naturel** : Rappelle la respiration lumineuse
- ✅ **Basses fréquences** : Idéal pour 2.28-9.12 Hz
- ✅ **Efficacité préservée** : Cycle thérapeutique conservé

### Quand l'utiliser
- Pour toutes les **basses fréquences** (A, B, C)
- Utilisation **prolongée** (>5 minutes par séance)
- Patients **sensibles** à la lumière
- Travail en **éclairage faible**

### Feedback visuel
- **LED Mode** : Clignote **LENTEMENT** (1 fois/seconde)
- **Changement de mode** : 1 flash long à l'activation

---

## 📡 MODE 2 : AM (Amplitude Modulation)

### Caractéristiques
```
Porteuse 200Hz modulée :
████████████████████████
    Enveloppe variable
    (fréquence thérapeutique)

• Lumière paraît CONTINUE
• Fréquence thérapeutique dans l'enveloppe
• Double stimulation (porteuse + modulante)
```

### Avantages
- ✅ **Lumière continue** : Porteuse invisible (>200 Hz)
- ✅ **Pas de clignotement** perceptible
- ✅ **Double action** : Stimulation combinée
- ✅ **Moyennes fréquences** : Optimal pour 9-30 Hz

### Quand l'utiliser
- Pour les **moyennes fréquences** (C, D)
- Approche **énergétique avancée**
- Protocoles de **stimulation profonde**
- Alternative au mode SINE

### Feedback visuel
- **LED Mode** : Clignote **MOYENNEMENT** (3 fois/seconde)
- **Changement de mode** : 2 flashs moyens à l'activation

---

## ⚡ MODE 3 : CARRÉ (Classique)

### Caractéristiques
```
Signal ON/OFF brutal :
     ┌──┐  ┌──┐  ┌──┐
  ON │  │  │  │  │  │
─────┘  └──┘  └──┘  └───
 OFF

• Transition INSTANTANÉE
• Signal traditionnel
• Clignotement visible aux basses fréquences
```

### Avantages
- ✅ **Signal pur** : Pas de distorsion
- ✅ **Économie batterie** : Calculs simples
- ✅ **Hautes fréquences** : Invisible >30 Hz
- ✅ **Référence** : Signal traditionnel des appareils

### Quand l'utiliser
- Pour les **hautes fréquences** (E, F, G)
- **Comparaison** avec mode SINE/AM
- **Économie batterie** maximale
- Protocoles **classiques** sans modulation

### Feedback visuel
- **LED Mode** : Clignote **RAPIDEMENT** (5 fois/seconde)
- **Changement de mode** : 3 flashs courts à l'activation

### ⚠️ Attention
Aux basses fréquences (2.28-9.12 Hz), le clignotement est **très visible** et peut être **désagréable** pour le thérapeute.

---

## 🎯 GUIDE DE SÉLECTION RAPIDE

### Par fréquence

| Fréquence | Nom | Mode recommandé | Raison |
|-----------|-----|-----------------|--------|
| 2.28 Hz | A | **SINE** 🌊 | Clignotement très lent → SINE obligatoire |
| 5.56 Hz | B | **SINE** 🌊 | Clignotement lent → SINE recommandé |
| 9.12 Hz | C | **SINE** ou **AM** 📡 | Limite visible → SINE/AM au choix |
| 18.25 Hz | D | **AM** ou **CARRÉ** ⚡ | Quasi invisible → tous modes OK |
| 36.50 Hz | E | **CARRÉ** ⚡ | Invisible → CARRÉ optimal |
| 73 Hz | F | **CARRÉ** ⚡ | Invisible → CARRÉ optimal |
| 146 Hz | G | **CARRÉ** ⚡ | Invisible → CARRÉ optimal |

### Par usage thérapeutique

| Usage | Mode recommandé |
|-------|-----------------|
| **Séance courte** (<5 min) | CARRÉ ⚡ (économie) |
| **Séance longue** (>10 min) | SINE 🌊 (confort) |
| **Patient sensible** | SINE 🌊 (doux) |
| **Stimulation intense** | AM 📡 (double action) |
| **Protocole classique** | CARRÉ ⚡ (référence) |
| **Approche énergétique** | AM 📡 (subtil) |

---

## 🔄 COMMENT CHANGER DE MODE

### Méthode 1 : Bouton MODE (D4)

```
1. Appuyer sur le bouton MODE (3ème bouton)
2. Observer le feedback visuel :
   • 1 flash long    → Mode SINE activé
   • 2 flashs moyens → Mode AM activé
   • 3 flashs courts → Mode CARRÉ activé
3. Observer la LED Mode :
   • Clignote LENT   → Mode SINE
   • Clignote MOYEN  → Mode AM
   • Clignote RAPIDE → Mode CARRÉ
4. Vérifier sur Serial Monitor
```

### Cycle des modes
```
SINE → AM → CARRÉ → SINE → ...
 🌊     📡     ⚡      🌊
```

---

## 📊 COMPARAISON VISUELLE

### Fréquence A (2.28 Hz) selon le mode

```
MODE SINE 🌊 :
    Lumière ─╱╲─╱╲─╱╲─
    Ressenti : Respiration douce, très agréable
    Confort : ★★★★★ (5/5)

MODE AM 📡 :
    Lumière ████▓▓▓▓████
    Ressenti : Continue avec ondulation subtile
    Confort : ★★★★☆ (4/5)

MODE CARRÉ ⚡ :
    Lumière █──█──█──█──
    Ressenti : Flash désagréable, fatiguant
    Confort : ★☆☆☆☆ (1/5)
```

---

## 💡 CONSEILS D'UTILISATION

### Pour le thérapeute

1. **Démarrage** : Toujours commencer en mode SINE 🌊
2. **Test patient** : Laisser le patient choisir le mode qui lui convient
3. **Adaptation** : Changer de mode selon la zone traitée
4. **Comparaison** : Tester les 3 modes sur une même zone pour comparer

### Pour le patient

**Demander au patient :**
- "Vous préférez la lumière douce ou plus marquée ?"
- "Ce mode est-il confortable pour vous ?"
- "Sentez-vous une différence entre les modes ?"

### Protocole suggéré

```
1. Fréquence A (2.28 Hz) → Mode SINE obligatoire
2. Fréquence D (18.25 Hz) → Mode AM recommandé
3. Fréquence G (146 Hz) → Mode CARRÉ optimal

Adaptation selon ressenti du patient et du thérapeute
```

---

## 🔬 EFFICACITÉ THÉRAPEUTIQUE

### Les 3 modes sont-ils aussi efficaces ?

**OUI !** L'efficacité thérapeutique est préservée car :

1. **Fréquence identique** : 2.28 Hz reste 2.28 Hz
2. **Cycle conservé** : Le rythme de stimulation est respecté
3. **Énergie lumineuse** : Quantité totale similaire

### Différences subtiles

| Aspect | SINE | AM | CARRÉ |
|--------|------|-----|-------|
| **Fréquence** | ✅ Identique | ✅ Identique | ✅ Identique |
| **Énergie totale** | ✅ 100% | ✅ 100% | ✅ 100% |
| **Pic d'intensité** | 🟡 Moyen | 🔴 Élevé | 🔴 Maximal |
| **Gradient** | 🟢 Progressif | 🟡 Modulé | 🔴 Brutal |

### Choix selon l'objectif

- **Action douce/subtile** → SINE 🌊
- **Action profonde** → AM 📡
- **Action intense** → CARRÉ ⚡

---

## ⚡ CONSOMMATION BATTERIE

### Impact sur l'autonomie

| Mode | Calculs requis | Impact batterie | Autonomie (500mAh) |
|------|----------------|-----------------|-------------------|
| **SINE** | Modérés | +5% | ~2h20 |
| **AM** | Élevés | +10% | ~2h15 |
| **CARRÉ** | Minimaux | Référence | ~2h30 |

**Recommandation** : La différence est minime (<10 minutes), privilégier le **confort** plutôt que l'autonomie.

---

## 📱 AFFICHAGE SERIAL MONITOR

### Exemple de sortie

```
╔═══════════════════════════════════════════════════════╗
║     STYLET AURICULOTHÉRAPIE v4.0                      ║
║     Fréquences de Nogier + Modulation Adaptative      ║
╚═══════════════════════════════════════════════════════╝

📋 COMMANDES :
  • BTN_FREQ (D2)      : Changer fréquence (A→G)
  • BTN_INTENSITY (D3) : Changer intensité (10%→100%)
  • BTN_MODE (D4)      : Changer modulation (SINE→AM→CARRÉ)

► Fréquence A (2.28 Hz) | Intensité : 50% | Mode : SINUSOÏDAL | Couleur : 🔴 ROUGE

[Appui sur BTN_MODE]

► Fréquence A (2.28 Hz) | Intensité : 50% | Mode : AM (200Hz) | Couleur : 🔴 ROUGE

[Appui sur BTN_MODE]

► Fréquence A (2.28 Hz) | Intensité : 50% | Mode : CARRÉ | Couleur : 🔴 ROUGE
```

---

## 🎓 FORMATION RAPIDE

### Pour un nouveau thérapeute (3 minutes)

```
ÉTAPE 1 : Allumer le stylet
→ LED Rouge clignote doucement (mode SINE)

ÉTAPE 2 : Changer de fréquence
→ Appuyer BTN_FREQ pour parcourir A→G
→ Observer changement de couleur LED

ÉTAPE 3 : Régler l'intensité
→ Appuyer BTN_INTENSITY pour 10%→100%
→ Observer luminosité LED

ÉTAPE 4 : Tester les modes (optionnel)
→ Appuyer BTN_MODE pour SINE→AM→CARRÉ
→ Comparer confort visuel

ÉTAPE 5 : Commencer le traitement
→ Pointer sur zone auriculaire
→ Maintenir 10-30 secondes par point
```

---

## 🆘 FAQ

**Q : Quel mode utiliser en priorité ?**
R : Mode SINE pour les basses fréquences (A, B, C), Mode CARRÉ pour les hautes (E, F, G).

**Q : Le mode change-t-il l'efficacité ?**
R : Non, l'efficacité est identique. Seul le confort visuel change.

**Q : Puis-je rester en mode SINE pour tout ?**
R : Oui ! C'est le plus confortable et fonctionne pour toutes les fréquences.

**Q : Pourquoi 3 modes si SINE suffit ?**
R : Pour s'adapter aux préférences de chaque thérapeute et patient, et permettre la comparaison avec les appareils classiques (mode CARRÉ).

**Q : La LED Mode est-elle obligatoire ?**
R : Non, elle est optionnelle. On peut identifier le mode par le feedback des flashs.

---

## ✅ CHECKLIST UTILISATION

```
AVANT CHAQUE SÉANCE :
□ Vérifier batterie chargée
□ Allumer stylet (LED Rouge s'allume)
□ Vérifier mode SINE activé (LED Mode clignote lent)
□ Tester sur soi-même 5 secondes

PENDANT LA SÉANCE :
□ Adapter fréquence selon point traité
□ Ajuster intensité selon ressenti patient
□ Changer de mode si inconfort visuel

APRÈS LA SÉANCE :
□ Éteindre stylet
□ Nettoyer embout
□ Recharger si batterie <30%
```

═══════════════════════════════════════════════════════════════
