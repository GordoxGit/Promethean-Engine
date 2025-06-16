# Promethean Engine 🚀

Un **moteur de jeu 2D natif, modulaire et multiplateforme**, écrit en **C++17/C++20** avec **SDL2 + OpenGL 3.3+**, conçu pour être utilisé dans des projets de jeux de stratégie, simulation ou systèmes orientés UI.  
Fonctionne sur **Windows** (.exe) et **Android** (.apk), avec une structure modulaire, testable, évolutive, et intégrée à un pipeline CI/CD complet.

---

## 🌟 Fonctionnalités principales

- 🎮 Rendu GPU via OpenGL (sprites, batching, atlas, blending alpha)
- 🖱 UI native C++ avec layout responsive (DPI-aware)
- 🎮 Gestion complète des entrées (clavier, souris, multitouch)
- 🔄 Système de scènes/states avec transitions et événements
- 💾 Sauvegarde JSON versionnée (+ SQLite support optionnel)
- 🔊 Gestion audio avec SDL_mixer
- 🧪 Tests unitaires (Google Test)
- ⚙️ Build multiplateforme avec CMake
- ✅ Intégration continue via GitHub Actions
- 🔍 Logs, overlay debug & profiling en temps réel

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
  libnlohmann-json-dev \
  lcov

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --parallel

ctest --output-on-failure
```

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
- API `SaveSystem::Save(path)`, `::Load(path)`

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

- 1 ticket = 1 PR (suivi via GitHub Projects)
- Tests obligatoires pour les modules
- Review obligatoire avant merge
- Linting, formatage et naming cohérent (`snake_case`, `PascalCase` pour classes)

---

## 🗺 Feuille de route

| Phase | Objectif |
|-------|----------|
| ✅ Phase 1 | Modules essentiels : Renderer, UI, Input, State |
| ⏳ Phase 2 | SaveSystem, EventBus, AudioManager |
| 🔜 Phase 3 | Debug overlay, plugin system |
| 🔜 Phase 4 | CI Android, packaging stable |

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