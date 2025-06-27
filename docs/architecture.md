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
