# Foire aux questions

## Comment compiler rapidement ?

Utilisez CMake :

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Où trouver les headers publics ?

Après installation, tous les en-têtes sont disponibles sous `include/promethean`. Vous pouvez donc inclure le moteur ainsi :

```cpp
#include <promethean/core/Engine.h>
```

## Comment générer la documentation API ?

Installez Doxygen puis exécutez :

```bash
doxygen Doxyfile
```

Les fichiers HTML seront créés dans `docs/api`.
