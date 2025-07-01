# Promethean Engine 🚀

Un **moteur de jeu 2D natif, modulaire et multiplateforme**, écrit en **C++17/C++20** avec **SDL2 + OpenGL 3.3+**, conçu pour être utilisé dans des projets de jeux de stratégie, simulation ou systèmes orientés UI.
Fonctionne sur **Windows**, **macOS**, **Linux** et **Android** (.apk), avec une structure modulaire, testable, évolutive, et intégrée à un pipeline CI/CD complet.

---

## 🌟 Fonctionnalités principales

- 🎮 Rendu GPU via OpenGL (sprites, batching, atlas, blending alpha)
- 🖱 UI native C++ avec layout responsive (DPI-aware)
- 🎮 Gestion complète des entrées (clavier, souris, multitouch)
- 🔄 Système de scènes/states avec transitions et événements
- 💾 Sauvegarde JSON versionnée (+ SQLite support optionnel)
- 🛠 Éditeur de monde minimal (export/import JSON)
- 🔊 Gestion audio avec SDL_mixer
- 🧪 Tests unitaires (Google Test)
- ⚙️ Build multiplateforme avec CMake
- ✅ Intégration continue via GitHub Actions
- 🔍 Logs, overlay debug & profiling en temps réel
- 🎛 Overlay debug ImGui activable avec <kbd>F1</kbd>

---

## 📁 Arborescence du projet

```
/src               Code source principal du moteur
/include           Interfaces publiques
/platform/android  Code spécifique Android (NDK, assets)
/platform/windows  Spécificités Windows (init système)
/assets            Fichiers de ressources (images, fonts, sons)
/tests             Tests unitaires (Google Test)
/build             Répertoire de build (non versionné)
/docs              Documentation technique (architecture, diagrammes)
CMakeLists.txt     Build principal
```

---

## ⚙️ Installation & compilation (Linux / Codex)

```bash
sudo apt update && sudo apt install -y \
  cmake \
  build-essential \
  libsdl2-dev \
  libsdl2-image-dev \
  libsdl2-ttf-dev \
  libsdl2-mixer-dev \
  libgl1-mesa-dev \
  libsqlite3-dev \
  libnlohmann-json-dev (>=3.12.0) \
  lcov

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --parallel

ctest --output-on-failure
```

## 📦 Building with vcpkg

This project uses **vcpkg** to manage its third-party libraries. The manifest
specifies a fixed baseline commit so that dependency versions remain
deterministic. Ensure your clone of vcpkg is able to access this baseline:

```json
"builtin-baseline": "f5ca1a1e2b8f6bc59f2a5b4c4b8ab0d3e7f2d5a0"
```

After bootstrapping vcpkg, install the dependencies from the manifest:

```bash
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install --x-manifest-root=.
```

### Modern CMake include pattern

Les cibles suivent le schéma Modern CMake afin de ne jamais exposer de
chemins absolus provenant du répertoire source lorsqu'elles sont installées.
Les en-têtes du moteur sont donc visibles seulement pendant la compilation
locale via `BUILD_INTERFACE`, puis référencés simplement depuis `include/` une fois
installés. Sur macOS et Android, ce chemin de source n'est jamais ajouté à
l'interface publique.


---

## 🧪 Tests & CI

- Tests unitaires : **Google Test** dans `/tests`
- CI/CD : **GitHub Actions**
  - Compilation multiplateforme
  - Exécution des tests
  - Analyse de code et couverture
- PR gating avec rapports automatiques

---

## 🖼 Rendu & UI

- OpenGL 3.3+ via SDL2
- Textures optimisées (atlas), sprites batchés
- DPI-aware et responsive (Android & PC)
- `UIElement` abstrait, `UIButton`, `UISlider`, `UITextLabel`
- Système de menus in-game minimal (UIManager, fenêtres empilées)
- Layout manager, layering UI, gestion tactile/souris

---

## ⌨️ Entrée utilisateur

- SDL Keyboard/Mouse/Touch
- Multitouch Android (SDL_TouchFingerEvent)
- Détection de gestes (swipe, tap, long press)
- Abstraction d’actions configurable

---

## 💾 Sauvegarde

- Par défaut : JSON versionné (`nlohmann::json`)
- Option : SQLite embarqué
- Support multi-version / migration / backup auto
- API `SaveManager::SaveToFile(path)` / `LoadFromFile(path)`

---

## 🎧 Audio

- `AudioManager` abstrait basé sur SDL_mixer
- Sons/musiques, volume global ou par type
- Pause, resume, loop, stop
- Gestion mémoire/audio optimisée

---

## 🛠 Logs & Profiling

- `LogSystem` :
  - Niveaux (`DEBUG`, `INFO`, `WARN`, `ERROR`)
  - Fichier + console
- Profiling : compteur FPS, usage CPU/mémoire
- Overlay runtime (affichage debug UI en live)

---

## 🔧 Modularité

- Chaque module peut être activé/désactivé via CMake
- Architecture indépendante des jeux
- Compatible avec l’utilisation de **GPT-4o ou Codex** pour ajouter des tickets automatiquement
- Base solide pour plusieurs types de jeux (stratégie, sandbox, gestion, etc.)

---

## 📜 Scripting (Lua)

- Intégration minimale de Lua via `ScriptingManager`
- API disponible côté script : `create_entity`, `add_position`, `log_info`
- Exemple de script dans `assets/scripts/test_agent.lua`

---

## 📦 Compilation Android

- Build via **CMake + Android NDK**
- Dossier `platform/android/`
- Assets dans `/assets/`
- Signature + export `.apk` supporté

---

## 🧩 Exemple d’utilisation

```cpp
#include <Engine.h>

int main() {
    Promethean::Engine engine;
    engine.Init();
    engine.RegisterState(new MainMenuState());
    engine.Run();
    engine.Shutdown();
    return 0;
}
```

---

## 📝 Contribution

- Tests obligatoires pour les modules
- Review obligatoire avant merge
- Linting, formatage et naming cohérent (`snake_case`, `PascalCase` pour classes)

---

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


---

## 📚 Documentation

- `/docs/architecture.md` : diagrammes, architecture du moteur
- `README.md` : ce fichier
- Documentation générée avec **Doxygen**
- Diagrammes en PlantUML ou ASCII

---

## 📄 Licence

Ce projet est sous licence **MIT**.
Voir le fichier `LICENSE`.

---

## 🤝 Support

- Tickets GitHub pour signaler des bugs ou proposer des features
- Discussions via l’espace communautaire (si ouvert)
- Contributeurs bienvenus — standard open source moderne

---

**Promethean Engine** – Le socle natif, modulaire et moderne pour créer tes propres jeux 2D système-oriented, sur Windows et Android.
## Build (quick)
```bash
cmake -B build && cmake --build build
```

CI build pipeline uses the optional `PE_CI` flag:

```bash
cmake -B build -DPE_CI=ON && cmake --build build
```

To load a Tiled map at runtime:

```bash
./promethean --map=path/to/level.tmx
```

## Packaging release

Run the helper script to produce a distributable archive for your platform:

```bash
scripts/release.sh
```

Packages will be generated in `build-release` and include the sample assets and documentation.
