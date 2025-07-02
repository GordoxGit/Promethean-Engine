# Architecture de Promethean Engine

Promethean est un moteur de jeu 2D C++ basé sur SDL2 + OpenGL, orienté modularité.

## Modules principaux

- **Renderer** : OpenGL batching, sprites
- **InputHandler** : Clavier, souris, touch
- **UIManager** : UI native responsive
- **UIOverlay** : empilement de fenêtres (menus, popups)
- **StateManager** : Système de scènes
- **AssetLoader** : Chargement des assets
- **SaveManager** : JSON + SQLite (persist world/ecs)
- **ECS** : Entités, composants et systèmes

## Structure

- `src/` : logique du moteur
- `include/` : interfaces publiques
- `platform/` : code Android/Windows
- `tests/` : Google Test

### CMake

La génération suit le pattern Modern CMake. Le dossier `src/` est inclus en
`PRIVATE` pour la compilation de toutes les plateformes afin que les modules du
moteur accèdent à leurs en-têtes internes. L'interface publique n'expose que le
répertoire `include/` via `INSTALL_INTERFACE`, évitant tout chemin absolu dans
les cibles exportées.

Les exécutables locaux et la suite de tests ajoutent explicitement ce même
chemin `src/` via `target_include_directories()` pour inclure directement les
en-têtes internes.

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

## Behavior

Un **BehaviorSystem** minimal gère l'IA des entités via une petite machine à
états. Le `BehaviorComponent` stocke l'état courant (Idle, Seek…) ainsi que les
paramètres de transition. Chaque tick, le système met à jour la minuterie et fait
évoluer l'état, par exemple en déplaçant l'entité vers une cible lorsque l'état

`Seek` est actif.

## UI / Overlays

Le moteur inclut un système d'interfaces gérées par `UIManager`. Les fenêtres
(`UIWindow`) peuvent être empilées pour créer des menus ou des popups.
Chaque `UIButton` déclenche un callback lors du clic et les overlays sont
ajoutés à la `StateStack` afin d'apparaître au-dessus du jeu.

## Debug & ImGui

Un overlay de debug basé sur **Dear ImGui** est intégré pour profiler et
inspecter l'état interne du moteur en temps réel. L'affichage est activable
via <kbd>F1</kbd> et présente les informations de base&nbsp;: FPS, nombre
d'entités, appels de dessin et réglages audio. Des panneaux personnalisés
    peuvent être enregistrés dans le code à l'aide de `RegisterPanel()` afin
    d'étendre facilement l'interface de débogage.

## SaveManager

Le `SaveManager` sérialise l'état complet du monde (ECS, tilemap et variables
globales) au format **JSON** via `nlohmann::json`. Chaque composant expose les
méthodes `ToJSON()` et `FromJSON()` pour permettre une persistance fiable. Les
méthodes principales sont `SaveToFile(path)` et `LoadFromFile(path)` qui
écrivent ou restaurent un fichier de sauvegarde multiplateforme.

## World/Map Editor

Un éditeur minimal de monde permet de créer rapidement une carte 2D. Il gère
une tilemap unique modifiable à la souris via une interface rudimentaire.
L'éditeur peut exporter ou importer la carte au format JSON afin d'être chargé
par le moteur via `AssetManager`.

## Scripting Lua

Un module `ScriptingManager` charge des scripts Lua externes. Il expose une API
très simple pour créer des entités et modifier leurs composants depuis un
script. Les fonctions disponibles sont `create_entity`, `add_position` et
`log_info`. Un script d'exemple est fourni dans `assets/scripts/test_agent.lua`.
