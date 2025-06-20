| Phase                                           | Objectif                                                       | Statut |
| ----------------------------------------------- | -------------------------------------------------------------- | :----: |
| **Phase 1** 🧱 Fondations & Infrastructure      |                                                                |        |
|                                                 | Initialisation du projet (`CMake`, `vcpkg`)                    |    ✅   |
|                                                 | Logging (`LogSystem` via `spdlog`)                             |    ✅   |
|                                                 | Bus d’événements (`EventBus` thread-safe)                      |    ✅   |
|                                                 | CI/CD multiplateforme (Windows, Linux, macOS, Android)         |    ✅   |
|                                                 | Headless-GL (tests sans affichage `OpenGL`)                    |    ✅   |
|                                                 | Système de tickets AI-ready (template Codex Markdown + YAML)   |    ✅   |
|                                                 | Refactor `CMake` cross-platform clean                          |    ✅   |
| **Phase 2** 🖼 Systèmes Graphiques & UI         |                                                                |        |
|                                                 | `BatchRenderer` (OpenGL, VAO/VBO, batching, quad, ortho)       |    ✅   |
|                                                 | Implémentation UI de base (`Widget`, `Button`, `TextLabel`)    |   🔄   |
|                                                 | Gestionnaire de Layout (`Grid`, `Horizontal`, `Vertical`)      |   🔜   |
|                                                 | Overlay Debug (draw line, sphere, logs visuels)                |   🔜   |
| **Phase 3** 🧰 Systèmes Fonctionnels            |                                                                |        |
|                                                 | Gestionnaire d’Entrées (`Keyboard`, `Mouse`, `Touch`)          |    ✅   |
|                                                 | Gestionnaire d’Assets (textures, sons, fonts, JSON, LRU cache) |   🔄   |
|                                                 | Sauvegarde versionnée (`JSON` + fallback `SQLite`)             |   🔜   |
|                                                 | `AudioManager` (musique + FX, pause/loop, volume)              |   🔜   |
|                                                 | Support DPI-aware, multi-résolution (Android & Desktop)        |    ✅   |
| **Phase 4** 🧬 Systèmes Avancés & Extensibilité |                                                                |        |
|                                                 | ECS modulaire (`Entity`, `Component`, `System`)                |   🔜   |
|                                                 | `PluginManager` (modules dynamiques, chargement à chaud)       |   🔜   |
|                                                 | `AssetStreamingManager` (threaded, async, cache LRU)           |   🔜   |
|                                                 | Simulation orientée data (update déterministe)                 |   🔜   |
| **Phase 5** 🧪 Tests, Debug & Release           |                                                                |        |
|                                                 | Tests unitaires complets (`GoogleTest`)                        |    ✅   |
|                                                 | DebugOverlay (logs live, draw debug, FPS/mémoire)              |   🔜   |
|                                                 | Packaging `.apk` / `.exe` via GitHub Actions CI                |    ✅   |
|                                                 | Profiler intégré (auto-instrumentation, macros)                |   🔜   |
| **Phase 6** 🎮 Jeu de Démo Natif                |                                                                |        |
|                                                 | Prototype stratégie/gestion (style Modern Age++)               |   🔜   |
|                                                 | UI ingame complète (menus, sliders, cartes, options)           |   🔜   |
|                                                 | Déploiement Windows + Android (Play Store-ready)               |   🔜   |
