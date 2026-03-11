# 📌 AIDE-MÉMOIRE RAPIDE - Connexions Batterie

## 🎯 Réponse Directe à Vos Questions

### Question 1 : Où se connecte le VCC du potentiomètre ?
```
Potentiomètre VCC (borne +) ──→ 5V de l'Arduino
```

### Question 2 : Où se connecte le milieu du pont diviseur ?
```
Point milieu (entre 100kΩ et 33kΩ) ──→ A0 de l'Arduino
```

## ⚡ Schéma Ultra-Simplifié

```
        5V
         │
         ╞═══════════╗
         │           ║
    ┌────┴────┐      ║ Potentiomètre VCC
    │   POT   │      ║
    │         │      ║
    │ Curseur ├──────╫─────┐
    │         │            │
    └────┬────┘            │
         │                 │
         ╞═══════════╗     │
         │           ║     │
        GND          ║     └─── [100kΩ]
                     ║              │
         Pot GND     ║              ├──── A0
                     ║              │
                     ║         [33kΩ]
                     ║              │
                     ║             GND
```

## 📝 Liste de Vérification (Checklist)

### Potentiomètre (3 connexions)
- [ ] **VCC** (borne droite) → 5V Arduino (fil rouge)
- [ ] **GND** (borne gauche) → GND Arduino (fil noir)
- [ ] **SIG** (borne centrale) → Résistance 100kΩ patte 1 (fil orange)

### Résistance 100kΩ (2 connexions)
- [ ] **Patte 1** → Potentiomètre SIG (fil orange)
- [ ] **Patte 2** → Point Milieu (fil orange)

### Point Milieu (3 connexions - IMPORTANT !)
- [ ] **Fil 1** → Résistance 100kΩ patte 2 (continuité)
- [ ] **Fil 2** → Résistance 33kΩ patte 1 (continuité)
- [ ] **Fil 3** → **A0 Arduino** (fil jaune) ← NE PAS OUBLIER !

### Résistance 33kΩ (2 connexions)
- [ ] **Patte 1** → Point Milieu (fil orange)
- [ ] **Patte 2** → GND Arduino (fil noir)

## 🔢 Les Chiffres Importants

| Paramètre | Valeur |
|-----------|--------|
| Résistance haute | **100 kΩ** (marron-noir-jaune) |
| Résistance basse | **33 kΩ** (orange-orange-orange) |
| Facteur division | **4.03** (133kΩ / 33kΩ) |
| Vref Arduino Uno | **5V** |
| Batterie simulée | **0-5V** (via potentiomètre) |
| Batterie réelle | **3.0-4.2V** (LiPo) |

## 🎮 Test Rapide

1. **Démarrer la simulation Wokwi**
2. **Ouvrir Serial Monitor** (115200 bauds)
3. **Tourner le potentiomètre** à fond vers la droite
4. **Attendre 5 secondes**
5. **Vérifier** : Serial affiche `Batterie : ~5000 mV`

✅ Si OK → Circuit correct !  
❌ Si 0 mV → Vérifier connexion A0 au point milieu  
❌ Si valeur fixe → Vérifier VCC/GND du potentiomètre

## 🖼️ Photo du Montage Correct

```
         Arduino Uno
    ┌──────────────────┐
    │                  │
    │  5V   ●──────────┼──┐
    │                  │  │  Potentiomètre
    │  GND  ●──────────┼──┼──┐    ╔════╗
    │                  │  │  │    ║ 🎚️ ║ ← Tourner
    │  A0   ●──────────┼──┼──┼────╫────╫──┐
    │                  │  │  │    ║    ║  │
    └──────────────────┘  │  │    ╚════╝  │
                          │  │            │
                          │  │      VCC   │
                          │  │       │    │
                          │  │      SIG───┘
                          │  │       │
                          │  │      GND
                          │  │       │
                          │  └───────┤
                          │          │
                          │     [100kΩ]
                          │          │
                          └──────────┼──── Point Milieu
                                     │         (3 fils !)
                                [33kΩ]
                                     │
                                    GND
```

## 💬 En Mots Simples

Imaginez que vous remplissez un réservoir (batterie) :
1. Le **potentiomètre** = robinet qui contrôle le niveau d'eau (0-5V)
2. Les **résistances 100k/33k** = tuyau qui réduit la pression (diviseur)
3. Le **point milieu** = endroit où on mesure (A0)

Le point milieu doit avoir **3 fils** :
- Un qui vient de la résistance du haut (100kΩ)
- Un qui va vers la résistance du bas (33kΩ)
- **Un qui va vers A0** ← C'est celui qu'on oublie souvent !

---

**Bon câblage ! ⚡**
