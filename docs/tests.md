# Tests unitaires

Ce dossier regroupe l'ensemble des tests Google Test permettant de vérifier le fonctionnement des modules du moteur.

## Particularités SDL_mixer

L'AudioEngine repose sur SDL_mixer. Afin d'éviter toute sortie audio ou écriture de fichier pendant les tests, les fonctions `Mix_*` sont redirigées via l'option de link `--wrap`. **Cette redirection n'est disponible que sous Linux** (linker GNU ou `lld`).

Sur Windows, des stubs équivalents sont fournis directement dans les tests.

Sur macOS et dans l'environnement Android NDK, le linker ne supporte pas `--wrap`. Les tests nécessitant cette fonctionnalité sont donc ignorés via `GTEST_SKIP()`.
