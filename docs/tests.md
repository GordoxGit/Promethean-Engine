# Tests unitaires

Ce dossier regroupe l'ensemble des tests Google Test permettant de vérifier le fonctionnement des modules du moteur.

## Particularités SDL_mixer

L'AudioEngine repose sur SDL_mixer. Pour garder les tests totalement headless,
les appels `Mix_*` sont remplacés par des implémentations factices communes à
toutes les plateformes (`tests/mocks/SDL_mixer_stubs.cpp`). Aucun fichier audio
n'est donc chargé et aucune sortie n'est produite.

L'initialisation et la fermeture de SDL sont gérées une seule fois dans
`tests/TestMain.cpp` afin de garantir un état cohérent entre chaque test.
