# 🛠️ README – Montage du robot

Ce document décrit les étapes à suivre pour assembler la partie **électronique** et **mécanique** du robot. Il comprend les instructions de câblage, d’alimentation, et d’intégration physique des différents composants.

---

## 🔌 1. Électronique

### 1.1 Bat Board Stop (Carte d'arrêt)

- La carte d’arrêt dispose d’un interrupteur ON/OFF. C’est ici que l’on câble l’interrupteur à bascule.
- Les câbles d’alimentation se soudent sur le pinout double en bas à droite :
  - **+12V** : en bas  
  - **GND** : au milieu  
  - **+5V** : en haut

> La LED rouge au centre de la carte s’allume pour signaler que l’arrêt est **activé**.

#### Séquences d’armement du PAMI :
- **Premier allumage** : appui sur l’arrêt détecté en front **montant** = armement.
- **Désarmement** : appui à nouveau en front **montant**.
- **Réarmement** : maintenir l’interrupteur enfoncé pendant plus de 5 secondes, relâchement détecté en front **descendant**.

⚠️ Lorsque non armé, le PAMI désactive les alimentations **+12V** et **+5V** → aucune puissance n’est transmise à la carte de contrôle ou aux moteurs.

---

### 1.2 Main Board (Carte principale)

- Installer les drivers puis le module LoRa (le plus proche de l’intérieur).
- Reconnecter le pin **Enable** des drivers stepper avec un vieux fil (peu importe l’esthétique).
- Câbler les fils d’alimentation :

  - **+12V** : à l’extérieur  
  - **GND** : au milieu  
  - **+5V** : à l’intérieur

  (Repère : connecteur 2x3 à côté du module LoRa)

> Ne pas oublier de connecter le **moteur DC** sur le **+5V** et le **GND**.

---

### 1.3 Faisceau

- Les câbles d’alimentation passent dans le trou arrière sur la face supérieure de la **pièce du milieu**.
- Les autres câbles passent par les ouvertures latérales prévues pour le **câble management**.

#### Suiveur de ligne :

- Un connecteur est prévu sur le côté **extérieur de l’ESP**.
- L’ordre des pins (depuis l’USB jusqu’au chipset) est :
  - **+3.3V**  
  - **Signal**  
  - **GND**

---

## 🧱 2. Mécanique

### Intégration électronique :
Monter les éléments **dans l’ordre inverse** :
1. D’abord installer la **coiffe**  
2. Puis faire passer les câbles dans la **pièce du milieu**  
3. Enfin, connecter le tout dans le **châssis**

---

### 2.1 Châssis

Le châssis est la **pièce la plus basse**. Voici les étapes :

- Insérer les **heat inserts** :
  - 4 à l’avant  
  - 2 à l’arrière  
  - 2 sur les murs latéraux (en haut) pour fixer la **carte batterie**

#### Ordre de montage :
1. Capteur de ligne  
2. Capteur ultrason  
3. Moteurs **steppers**  
4. Carte batterie avec des entretoises de ~6 mm (utiliser des rondelles ou heat inserts)

> ⚠️ **Placer les batteries à cette étape.**

---

### 2.2 Pièce du Milieu

1. Installer 3 entretoises :
   - 2 à l’arrière  
   - 1 à l’avant (vis par le dessous)
2. Ajouter 4 **heat inserts** sur la face supérieure (pour le PCB)

💡 *Le PCB peut être monté “à la zob” sans entretoises, mais ce n’est pas recommandé.*

- Faire passer les câbles via les trous de **câble management**
- Connecter à l’électronique dans le châssis
- Positionner la pièce du milieu et visser avec des **vis M3 coniques courtes** (sachet 4) :
  - 2 à l’avant (en diagonale, méthode **Belpois™**)  
  - 2 à l’arrière

---

### 2.3 Coiffe

- Connecter le **moteur DC** (lecteur DVD) au **+5V** et **GND** de la Main Board.
- Poser la coiffe et la fixer avec **3 vis coniques M3**.
- Ajouter un petit **drapeau** fait en **scotch électrique** au bout du moteur DC.

---

### 2.4 Roues

- Réaliser les pneus avec du **caoutchouc**, puis les emmancher.
- Les nouvelles roues disposent d’un **détrompeur** pour faciliter le montage.

---

## ✅ Checklist Finale

- [ ] Batteries placées  
- [ ] Connecteurs bien soudés  
- [ ] LED rouge OK au démarrage  
- [ ] Moteur DC branché  
- [ ] Capteurs en place  
- [ ] Roues correctement montées  
- [ ] Câble management propre
