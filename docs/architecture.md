# Architecture de Promethean Engine

Promethean est un moteur de jeu 2D C++ basé sur SDL2 + OpenGL, orienté modularité.

## Modules principaux

- **Renderer** : OpenGL batching, sprites
- **InputHandler** : Clavier, souris, touch
- **UIManager** : UI native responsive
- **StateManager** : Système de scènes
- **AssetLoader** : Chargement des assets
- **SaveSystem** : JSON + SQLite
- **ECS** : Entités, composants et systèmes

## Structure

- `src/` : logique du moteur
- `include/` : interfaces publiques
- `platform/` : code Android/Windows
- `tests/` : Google Test

## Principes

- 100 % modulaire
- Testable & CI-ready
- Multi-plateforme (Windows, Android)

## ECS

Le moteur s'appuie sur un **Entity Component System** minimal. Le `Registry`
gère la création et destruction d'entités ainsi que l'attachement de
composants. Les `System` parcourent les entités ayant un ensemble de
composants précis et mettent à jour leur logique chaque frame.

## Pathfinding

Un module minimal d'**A\*** calcule des chemins sur une grille 2D. Le
`NavComponent` stocke la position courante, la destination et le chemin
résultant. Le `PathfindingSystem` actualise ces entités à chaque tick en
avançant vers la cellule suivante.

## Debug & ImGui

Un overlay de debug basé sur **Dear ImGui** est intégré pour profiler et
inspecter l'état interne du moteur en temps réel. L'affichage est activable
via <kbd>F1</kbd> et présente les informations de base&nbsp;: FPS, nombre
d'entités, appels de dessin et réglages audio. Des panneaux personnalisés
peuvent être enregistrés dans le code à l'aide de `RegisterPanel()` afin
d'étendre facilement l'interface de débogage.
