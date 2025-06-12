PIXEL DEPTHS
Document de Conception Complet v1.0
Jeu de Mining/Action RPG 2D - 100% Procédural

Version : 1.0
 Date : Juin 2025
 Plateforme : PC Windows (.exe)
 Technologie : C++ + SDL2 (100% Code-Generated)
 Développement : Full Programmatique - Zéro Assets Externes

TABLE DES MATIÈRES
Vue d'Ensemble du Projet
Architecture Technique
Systèmes de Gameplay
Génération Pixel Art Procédural
Système Audio Procédural
Monde et Génération Procédurale
Systèmes de Progression
Interface Utilisateur
Plan de Développement
Compilation et Distribution
Spécifications Techniques

1. VUE D'ENSEMBLE DU PROJET
1.1 Concept Core
PIXEL DEPTHS est un jeu de mining/farming/action RPG où le joueur explore un monde souterrain procédural infini, mine des ressources, combat des créatures, construit sa base et progresse dans un système de compétences complexe.
1.2 Philosophie de Développement
100% Code-Generated : Aucun fichier graphique/audio externe
Procédural à tous les niveaux : Monde, sprites, sons, musiques
Haute qualité : Pixel art 2.5D avec effets visuels avancés
Performance optimale : Moteur C++ custom optimisé
Rejouabilité infinie : Contenu procédural adaptatif
1.3 Objectifs v1.0
Monde : 200x150 chunks procéduraux (6400x4800 tiles)
Biomes : 5 biomes complets avec écosystèmes uniques
Ressources : 25+ types de minerais/matériaux
Ennemis : 30+ types avec IA comportementale
Craft : 150+ recettes dans 8 stations différentes
Magie : 16 sorts dans 4 écoles de magie
Base Building : 60+ structures avec automatisation
Durée de jeu : 30-60h de contenu pour la v1
Quêtes : Storyline principale + 40+ quêtes secondaires

2. ARCHITECTURE TECHNIQUE
2.1 Structure du Code
PixelDepths/
├── src/
│   ├── core/                   # Moteur de base
│   │   ├── Engine.cpp          # Boucle principale
│   │   ├── Renderer.cpp        # Système de rendu
│   │   ├── InputManager.cpp    # Gestion input
│   │   ├── AudioEngine.cpp     # Moteur audio
│   │   ├── StateManager.cpp    # États de jeu
│   │   └── ResourceManager.cpp # Gestion mémoire
│   │
│   ├── graphics/               # Génération visuelle
│   │   ├── PixelArtGenerator.cpp  # Sprites procéduraux
│   │   ├── ParticleSystem.cpp     # Système particules
│   │   ├── LightingEngine.cpp     # Éclairage dynamique
│   │   ├── ShaderManager.cpp      # Effets visuels
│   │   └── AnimationSystem.cpp    # Animations fluides
│   │
│   ├── world/                  # Génération de monde
│   │   ├── WorldGenerator.cpp     # Terrain procédural
│   │   ├── BiomeManager.cpp       # Gestion biomes
│   │   ├── ChunkManager.cpp       # Streaming chunks
│   │   ├── StructureGenerator.cpp # Donjons/structures
│   │   └── WeatherSystem.cpp      # Météo dynamique
│   │
│   ├── entities/               # Entités de jeu
│   │   ├── Player.cpp             # Joueur
│   │   ├── Enemy.cpp              # Système d'ennemis
│   │   ├── NPC.cpp                # PNJ et marchands
│   │   ├── Projectile.cpp         # Projectiles
│   │   └── EntityManager.cpp      # Gestion entités
│   │
│   ├── systems/                # Systèmes de gameplay
│   │   ├── MiningSystem.cpp       # Mining et ressources
│   │   ├── CraftingSystem.cpp     # Système de craft
│   │   ├── CombatSystem.cpp       # Combat et dégâts
│   │   ├── MagicSystem.cpp        # Sorts et magie
│   │   ├── BuildingSystem.cpp     # Construction base
│   │   ├── QuestSystem.cpp        # Quêtes et progression
│   │   └── InventorySystem.cpp    # Inventaire et stockage
│   │
│   ├── audio/                  # Audio procédural
│   │   ├── SoundGenerator.cpp     # Génération sons
│   │   ├── MusicComposer.cpp      # Musique adaptative
│   │   ├── AudioMixer.cpp         # Mixage 3D
│   │   └── SoundEffects.cpp       # Effets sonores
│   │
│   └── ui/                     # Interface utilisateur
│       ├── UIManager.cpp          # Gestion UI
│       ├── MenuSystem.cpp         # Menus
│       ├── HUD.cpp                # Interface jeu
│       ├── InventoryUI.cpp        # Interface inventaire
│       └── DialogSystem.cpp       # Dialogues PNJ
│
├── data/                       # Configurations JSON
│   ├── gameConfig.json           # Config générale
│   ├── biomes.json               # Définitions biomes
│   ├── recipes.json              # Recettes craft
│   ├── enemies.json              # Stats ennemis
│   ├── spells.json               # Configuration sorts
│   └── quests.json               # Données quêtes
│
└── build/                      # Système de build
    ├── CMakeLists.txt            # Configuration CMake
    ├── build_release.bat         # Script build Windows
    └── package.bat               # Script packaging

2.2 Dépendances Techniques
Core Libraries :
SDL2 (2.28+) : Rendu, input, fenêtrage
SDL2_image : Support formats d'image
SDL2_mixer : Audio avancé et streaming
nlohmann/json : Parsing et sérialisation JSON
FastNoise2 : Génération de bruit procédural
Compilateur :
MSVC 2022 ou GCC 11+ avec support C++20
CMake 3.20+ pour la gestion de build
2.3 Design Patterns Utilisés
// Singleton pour managers globaux
class Renderer {
    static Renderer& getInstance();
    void render(const RenderQueue& queue);
};

// Component-Entity System
class Entity {
    std::vector<std::unique_ptr<Component>> components;
    template<typename T> T* getComponent();
};

// Factory pour génération procédurale
class SpriteFactory {
    std::unique_ptr<Sprite> createSprite(EntityType type, uint32_t seed);
};

// Observer pour events
class EventSystem {
    void subscribe(EventType type, std::function<void(Event)> callback);
    void emit(EventType type, const Event& event);
};


3. SYSTÈMES DE GAMEPLAY
3.1 Système de Mining Avancé
3.1.1 Types de Ressources (25+)
Tier 1 - Surface (0-100m)
Pierre commune : Construction basique, 90% spawn
Charbon : Combustible, énergie, 15% spawn
Cuivre : Premiers outils, électronique, 12% spawn
Étain : Alliages bronze, 8% spawn
Argile : Céramique, construction, 10% spawn
Tier 2 - Shallow Underground (100-300m)
Fer : Outils intermédiaires, construction, 18% spawn
Argent : Enchantements, monnaie, 6% spawn
Zinc : Alliages, protection, 7% spawn
Soufre : Explosifs, alchimie, 5% spawn
Saltpeter : Poudre à canon, 4% spawn
Tier 3 - Deep Underground (300-600m)
Or : Trading, électronique avancée, 8% spawn
Platine : Catalyseurs, high-tech, 3% spawn
Gemmes communes : Rubis, Saphir, Émeraude, 4% spawn
Cristaux énergétiques : Stockage mana, 2% spawn
Minerai magique : Base sorts, 3% spawn
Tier 4 - Abyssal Depths (600-1000m)
Mithril : Équipement magique léger, 2% spawn
Adamantium : Équipement ultime, 1% spawn
Cristaux élémentaires : Feu/Glace/Foudre/Terre, 1.5% spawn
Dark Matter : Technologie alien, 0.5% spawn
Quantum Crystals : Manipulation réalité, 0.3% spawn
Tier 5 - Core Realm (1000m+)
Voidstone : Matériau dimensionnel, 0.2% spawn
Temporite : Contrôle du temps, 0.1% spawn
Essence divine : Craft légendaire, 0.05% spawn
3.1.2 Mécaniques de Mining
class MiningSystem {
    struct Tool {
        MaterialType material;
        int durability;
        float miningSpeed;
        float efficiency;
        std::vector<MaterialType> canMine;
        int miningLevel;
    };
    
    struct MiningNode {
        MaterialType type;
        int hardness;
        float respawnTime;
        std::vector<Drop> possibleDrops;
        bool requiresSpecialTool;
    };
    
    // Techniques spéciales
    void performVeinMining(Position start, MaterialType type);
    void performExplosiveMining(Position center, float radius);
    void performLaserMining(Position target, float duration);
};

Outils par tier :
Bois : Pierre, argile (vitesse 1x)
Cuivre : + charbon, cuivre (vitesse 1.5x)
Bronze : + fer, argent (vitesse 2x)
Fer : + or, gemmes (vitesse 2.5x)
Acier : + matériaux magiques (vitesse 3x)
Mithril : + cristaux élémentaires (vitesse 4x)
Adamantium : Tous matériaux (vitesse 5x)
3.2 Système de Craft Multi-Niveaux
3.2.1 Stations de Craft (8 stations)
enum class CraftingStation {
    WORKBENCH,      // Craft basique bois/pierre
    FORGE,          // Métallurgie et armes
    ANVIL,          // Amélioration équipement
    ARCANE_TABLE,   // Enchantements et magie
    ALCHEMY_LAB,    // Potions et transformations
    TECH_STATION,   // Électronique et machines
    ANCIENT_ALTAR,  // Craft légendaire
    QUANTUM_FORGE   // Matériaux dimensionnels
};

class CraftingSystem {
    struct Recipe {
        std::vector<ItemStack> ingredients;
        std::vector<ItemStack> results;
        CraftingStation requiredStation;
        int skillLevel;
        float craftTime;
        float successChance;
    };
    
    void unlockRecipe(RecipeID id);
    bool canCraft(RecipeID id, const Inventory& inv);
    void startCrafting(RecipeID id, float qualityBonus);
};

3.2.2 Recettes par Catégorie (150+ total)
Outils (30 recettes)
Pioches : 7 tiers x 3 variations (speed/durability/efficiency)
Haches : 6 tiers pour abattage rapide
Pelles : 5 tiers pour excavation
Faux : 4 tiers pour farming
Outils spéciaux : Drill laser, Excavatrice quantique
Armes (40 recettes)
Mêlée : Épées (8), Masses (6), Lances (5), Haches de guerre (4)
Distance : Arcs (6), Arbalètes (4), Fusils magiques (3)
Magique : Bâtons (4), Orbes (3), Grimoires (1)
Armures (25 recettes)
Sets complets : 5 tiers x 4 pièces (casque, torse, jambes, bottes)
Armures spécialisées : Mining (+vitesse), Combat (+défense), Magie (+mana)
Consommables (35 recettes)
Potions : Soin (5 tiers), Mana (5), Vitesse (3), Force (3), autres (10)
Nourriture : 15 types avec bonus variés
Explosifs : 4 types pour mining et combat
Structures (20 recettes)
Base : Murs, portes, fenêtres, décorations
Machines : Générateurs, convoyeurs, stockage automatique
Défenses : Tours, pièges, boucliers énergétiques
3.3 Système de Combat Tactical
3.3.1 Mécaniques de Combat
class CombatSystem {
    struct DamageType {
        PHYSICAL, FIRE, ICE, LIGHTNING, POISON, 
        ARCANE, HOLY, SHADOW, VOID
    };
    
    struct CombatStats {
        int attack, defense, hp, mana;
        float critChance, critMultiplier;
        float attackSpeed, moveSpeed;
        std::map<DamageType, float> resistances;
        std::vector<StatusEffect> activeEffects;
    };
    
    float calculateDamage(const Attack& attack, const CombatStats& target);
    void applyStatusEffect(Entity& target, StatusEffect effect);
    bool checkCriticalHit(float critChance);
};

Types de dégâts et résistances :
Physique : Dégâts bruts, réduits par armure
Élémentaire : Feu (burn), Glace (slow), Foudre (stun)
Magique : Arcane (pur), Saint (undead bonus), Ombre (living malus)
Spécial : Poison (DoT), Void (ignore résistances)
3.3.2 Ennemis par Biome (30+ types)
Surface Plains
Slime Vert : HP 25, Attaque 8, Vitesse rapide, Drop : Gel
Loup Gris : HP 45, Attaque 15, Pack behavior, Drop : Fourrure
Ours Brun : HP 120, Attaque 25, Tank, Drop : Viande, Cuir
Bandit : HP 80, Attaque 20, Utilise outils, Drop : Équipement
Forest Biome
Araignée Géante : HP 60, Attaque 18, Poison, Drop : Soie
Ent Mineur : HP 200, Attaque 30, Régénération, Drop : Bois magique
Pixie Malicieuse : HP 35, Attaque 12, Vol + confusion, Drop : Poudre fée
Underground Caves
Chauve-souris de Caverne : HP 40, Attaque 14, Vol en essaim
Golem de Pierre : HP 300, Attaque 35, Très lent mais tank
Squelette Mineur : HP 70, Attaque 22, Attaques à distance
Deep Caverns
Démon Mineur : HP 150, Attaque 40, Magie noire
Wyvern Souterraine : HP 400, Attaque 50, Souffle acide
Spectre Ancien : HP 100, Attaque 35, Intangible périodiquement
Abyssal Depths
Seigneur Démon : HP 800, Attaque 80, Boss intermédiaire
Dragon des Profondeurs : HP 1500, Attaque 120, Boss majeur
Entité du Void : HP 2000, Attaque 150, Boss final
3.3.3 Système de Magie
École Élémentaire
// Sorts de Feu
Fireball: 50 dégâts, AoE 3x3, 3s cooldown
Flame_Shield: +25% résistance feu, 30s durée
Meteor: 200 dégâts, AoE 5x5, 15s cooldown, channeling 3s

// Sorts de Glace
Ice_Shard: 40 dégâts + slow 50%, 2.5s cooldown
Freeze: Immobilise 3s, 8s cooldown
Blizzard: 30 dégâts/s, AoE persistante, 20s cooldown

// Sorts de Foudre
Lightning_Bolt: 60 dégâts, chaîne 3 ennemis, 4s cooldown
Thunder_Storm: Zone électrifiée, 25s durée, 30s cooldown

École Arcane
Teleport: Déplacement instantané 10 tiles, 5s cooldown
Mana_Shield: Absorbe dégâts avec mana, 45s durée
Time_Slow: Ralentit tout sauf joueur, 10s durée, 60s cooldown
Dispel: Supprime effets magiques, 3s cooldown

École Nature
Heal: +100 HP instantané, 4s cooldown
Regeneration: +10 HP/s pendant 10s, 15s cooldown
Summon_Wolf: Familier temporaire 60s, 90s cooldown
Growth: Accélère repousse ressources, 2min durée

École Ombre
Shadow_Step: Téléportation + invisibilité 3s, 8s cooldown
Life_Drain: Vole HP ennemi, 6s cooldown
Curse: -50% stats ennemi 20s, 12s cooldown
Raise_Skeleton: Réanime squelette allié, 45s durée

3.4 Base Building et Automatisation
3.4.1 Système de Construction
class BuildingSystem {
    struct Structure {
        StructureType type;
        Position position;
        int durability;
        bool isPowered;
        std::vector<Connection> connections;
        float efficiency;
    };
    
    bool canPlace(StructureType type, Position pos);
    void placeStructure(StructureType type, Position pos);
    void connectStructures(Structure& a, Structure& b);
    void updatePowerGrid();
};

Types de Structures (60+ total)
Habitation
Murs : Bois (100 HP), Pierre (250 HP), Métal (500 HP), Cristal (1000 HP)
Portes : Simple, Renforcée, Magique, Automatique
Fenêtres : Verre, Cristal, Blindé
Mobilier : Lit (+repos), Coffre (+stockage), Table de craft
Production
Générateurs : Charbon (10 energy/s), Éolien (5 stable), Solaire (8 jour), Magique (20)
Convoyeurs : Transport automatique items
Extracteurs : Mining automatique
Raffineries : Transformation automatique ressources
Défenses
Tours de Garde : Attaque automatique ennemis portée 8
Ballistes : Gros dégâts, portée 12, lent
Tours Magiques : Sorts automatiques selon type
Pièges : Fosse, Poison, Explosion, Ralentissement
Utilitaires
Stockage : Coffres (100 slots), Silos (500), Entrepôts (2000)
Téléporteurs : Transport instantané entre bases
Laboratoires : Recherche nouvelles technologies
Jardins : Production nourriture automatique
3.4.2 Système d'Énergie
class PowerSystem {
    struct PowerGrid {
        std::vector<PowerSource> sources;
        std::vector<PowerConsumer> consumers;
        float totalGeneration;
        float totalConsumption;
        bool isStable;
    };
    
    void updateGrid();
    bool hasSufficientPower(float required);
    void distributeEnergy();
};

Sources d'énergie :
Charbon : 10 energy/s, consomme charbon
Éolien : 5 energy/s stable, nécessite hauteur
Solaire : 8 energy/s jour, 0 nuit
Géothermique : 15 energy/s, profondeur 200m+
Magique : 20 energy/s, consomme cristaux mana
Quantique : 50 energy/s, très cher à construire

4. GÉNÉRATION PIXEL ART PROCÉDURAL
4.1 Système de Rendu Graphique
class PixelArtGenerator {
private:
    struct ColorPalette {
        std::vector<Color> primary;
        std::vector<Color> secondary;
        std::vector<Color> accent;
        std::vector<Color> shadow;
        std::vector<Color> highlight;
    };
    
    struct SpriteTemplate {
        int width, height;
        std::vector<std::vector<int>> pattern;
        std::vector<AnimationFrame> frames;
    };

public:
    // Génération sprites entités
    Sprite generatePlayerSprite(uint32_t seed, EquipmentSet equipment);
    Sprite generateEnemySprite(EnemyType type, uint32_t seed);
    Sprite generateNPCSprite(NPCType type, uint32_t personalitySeed);
    
    // Génération tiles monde
    Sprite generateTileSprite(MaterialType material, BiomeType biome);
    Sprite generateStructureSprite(StructureType structure, int tier);
    
    // Génération effets
    std::vector<Sprite> generateParticleFrames(EffectType effect);
    Sprite generateProjectileSprite(ProjectileType type, float angle);
    
    // Animation procédurale
    std::vector<Sprite> generateWalkCycle(const Sprite& base, float speed);
    std::vector<Sprite> generateAttackAnimation(WeaponType weapon);
    std::vector<Sprite> generateSpellCastAnimation(SpellSchool school);
};

4.2 Génération de Palettes Procédurales
class ColorPaletteGenerator {
public:
    ColorPalette generateBiomePalette(BiomeType biome, uint32_t seed) {
        ColorPalette palette;
        
        switch(biome) {
            case SURFACE_PLAINS:
                palette.primary = {
                    Color(34, 139, 34),   // Forest Green
                    Color(107, 142, 35),  // Olive Drab  
                    Color(154, 205, 50)   // Yellow Green
                };
                palette.secondary = {
                    Color(139, 69, 19),   // Saddle Brown
                    Color(160, 82, 45)    // Rosy Brown
                };
                break;
                
            case UNDERGROUND:
                palette.primary = {
                    Color(64, 64, 64),    // Dark Gray
                    Color(105, 105, 105), // Dim Gray
                    Color(128, 128, 128)  // Gray
                };
                // Ajout variation procédurale basée sur seed
                addSeedVariation(palette, seed, 0.1f);
                break;
                
            case CRYSTAL_CAVERNS:
                palette.primary = generateCrystalColors(seed);
                addGlowEffect(palette, 0.3f);
                break;
        }
        
        return palette;
    }
    
private:
    void addSeedVariation(ColorPalette& palette, uint32_t seed, float intensity);
    std::vector<Color> generateCrystalColors(uint32_t seed);
    void addGlowEffect(ColorPalette& palette, float glowIntensity);
};

4.3 Templates de Sprites Procéduraux
// Exemple : Génération d'un slime procédural
Sprite PixelArtGenerator::generateSlimeSprite(uint32_t seed) {
    Random rng(seed);
    
    // Taille variable basée sur seed
    int size = 12 + (rng.next() % 8); // 12-20 pixels
    
    // Couleur principale
    Color baseColor = generateSlimeColor(seed);
    Color shadowColor = darken(baseColor, 0.3f);
    Color highlightColor = lighten(baseColor, 0.4f);
    
    // Pattern de base (forme arrondie)
    std::vector<std::vector<Color>> pixels(size, std::vector<Color>(size));
    
    // Génération forme organique
    for(int y = 0; y < size; y++) {
        for(int x = 0; x < size; x++) {
            float distance = distanceFromCenter(x, y, size/2, size/2);
            float noise = perlinNoise(x * 0.3f, y * 0.3f, seed);
            
            if(distance < size/2 + noise * 2) {
                // Pixel fait partie du slime
                if(y < size * 0.3f) {
                    pixels[y][x] = highlightColor; // Partie haute brillante
                } else if(y > size * 0.8f) {
                    pixels[y][x] = shadowColor;    // Partie basse sombre
                } else {
                    pixels[y][x] = baseColor;      // Corps principal
                }
            }
        }
    }
    
    // Ajout des yeux
    addSlimeEyes(pixels, size, rng);
    
    return Sprite(pixels);
}

4.4 Système d'Animation Procédurale
class AnimationGenerator {
public:
    std::vector<Sprite> generateWalkAnimation(const Sprite& baseSprite, float speed) {
        std::vector<Sprite> frames;
        const int frameCount = 8;
        
        for(int i = 0; i < frameCount; i++) {
            float phase = (float)i / frameCount * 2.0f * PI;
            
            Sprite frame = baseSprite;
            
            // Bobbing vertical
            float bobOffset = sin(phase) * 2.0f;
            frame = translateVertical(frame, bobOffset);
            
            // Compression/étirement
            float squash = 1.0f + sin(phase * 2) * 0.1f;
            frame = scaleVertical(frame, squash);
            
            // Variation couleur (breathing effect)
            float breathe = sin(phase) * 0.05f + 1.0f;
            frame = modulateColors(frame, breathe);
            
            frames.push_back(frame);
        }
        
        return frames;
    }
    
    std::vector<Sprite> generateAttackAnimation(WeaponType weapon, const Sprite& wielder) {
        switch(weapon) {
            case SWORD:
                return generateSlashAnimation(wielder);
            case HAMMER:
                return generateSmashAnimation(wielder);
            case BOW:
                return generateDrawBowAnimation(wielder);
            case STAFF:
                return generateCastAnimation(wielder);
        }
    }
    
private:
    std::vector<Sprite> generateSlashAnimation(const Sprite& wielder);
    std::vector<Sprite> generateMagicCastAnimation(SpellSchool school);
};

4.5 Effets Visuels Procéduraux
class VisualEffectsGenerator {
public:
    // Génération particules
    ParticleSystem generateExplosion(Position center, float intensity, Color baseColor) {
        ParticleSystem system;
        
        int particleCount = (int)(intensity * 20);
        for(int i = 0; i < particleCount; i++) {
            Particle p;
            p.position = center + randomOffset(intensity * 10);
            p.velocity = randomDirection() * (intensity * 5);
            p.color = varyColor(baseColor, 0.3f);
            p.lifetime = 1.0f + random() * 2.0f;
            p.size = 1 + random() * 3;
            
            system.particles.push_back(p);
        }
        
        return system;
    }
    
    // Effets magiques
    ParticleSystem generateSpellEffect(SpellType spell, Position origin, Position target) {
        switch(spell) {
            case FIREBALL:
                return generateFireTrail(origin, target);
            case LIGHTNING:
                return generateElectricArc(origin, target);
            case ICE_SHARD:
                return generateFrostEffect(origin, target);
        }
    }
    
    // Éclairage dynamique
    LightSource generateTorchLight(Position pos) {
        LightSource light;
        light.position = pos;
        light.color = Color(255, 200, 100); // Orange chaud
        light.intensity = 0.8f + sin(gameTime * 3) * 0.2f; // Scintillement
        light.radius = 50 + sin(gameTime * 2) * 5; // Variation rayon
        light.falloff = LightFalloff::QUADRATIC;
        
        return light;
    }
};


5. SYSTÈME AUDIO PROCÉDURAL
5.1 Génération Sonore Programmatique
class SoundGenerator {
private:
    struct WaveForm {
        enum Type { SINE, SQUARE, TRIANGLE, SAWTOOTH, NOISE };
        Type type;
        float frequency;
        float amplitude;
        float phase;
    };
    
    struct AudioSample {
        std::vector<float> samples;
        int sampleRate;
        float duration;
    };

public:
    // Génération sons d'impact
    AudioSample generateImpactSound(MaterialType material1, MaterialType material2) {
        AudioSample sound;
        sound.sampleRate = 44100;
        sound.duration = 0.3f;
        
        // Fréquence basée sur dureté matériaux
        float baseFreq = getMaterialHardness(material1) * 100 + 200;
        float modFreq = getMaterialHardness(material2) * 80 + 150;
        
        // Génération forme d'onde
        for(int i = 0; i < sound.duration * sound.sampleRate; i++) {
            float t = (float)i / sound.sampleRate;
            
            // Onde principale
            float sample = sin(2 * PI * baseFreq * t) * 0.5f;
            
            // Harmoniques selon matériaux
            sample += sin(2 * PI * modFreq * t) * 0.3f;
            
            // Enveloppe d'amplitude (attack rapide, decay exponentiel)
            float envelope = exp(-t * 8.0f);
            sample *= envelope;
            
            // Ajout bruit selon texture matériau
            if(material1 == STONE || material2 == STONE) {
                sample += (random() * 2 - 1) * 0.1f * envelope;
            }
            
            sound.samples.push_back(sample);
        }
        
        return sound;
    }
    
    // Sons de minage spécifiques
    AudioSample generateMiningSound(MaterialType material, ToolType tool) {
        AudioSample sound;
        
        float hardness = getMaterialHardness(material);
        float toolEfficiency = getToolEfficiency(tool, material);
        
        // Son continu de minage
        sound.duration = 2.0f; // Son de 2 secondes
        
        for(int i = 0; i < sound.duration * 44100; i++) {
            float t = (float)i / 44100;
            
            // Rythme de frappe basé sur efficacité outil
            float hitRate = toolEfficiency * 3.0f;
            float hit = sin(2 * PI * hitRate * t);
            hit = hit > 0.7f ? 1.0f : 0.0f; // Impulsions nettes
            
            // Son de base selon matériau
            float sample = 0;
            if(material == STONE) {
                sample = sin(2 * PI * 300 * t) * 0.4f;
                sample += (random() * 2 - 1) * 0.2f; // Bruit pierreux
            } else if(material == METAL) {
                sample = sin(2 * PI * 800 * t) * 0.3f;
                sample += sin(2 * PI * 1600 * t) * 0.2f; // Harmoniques métalliques
            }
            
            sample *= hit; // Application du rythme
            sound.samples.push_back(sample);
        }
        
        return sound;
    }
    
    // Sons de combat
    AudioSample generateWeaponSound(WeaponType weapon, bool hit) {
        AudioSample sound;
        
        switch(weapon) {
            case SWORD:
                if(hit) {
                    sound = generateSlashHitSound();
                } else {
                    sound = generateSwordWhooshSound();
                }
                break;
                
            case HAMMER:
                sound = generateHammerSound(hit);
                break;
                
            case BOW:
                sound = generateBowReleaseSound();
                break;
        }
        
        return sound;
    }

private:
    float getMaterialHardness(MaterialType material);
    float getToolEfficiency(ToolType tool, MaterialType material);
    AudioSample generateSlashHitSound();
    AudioSample generateSwordWhooshSound();
};

5.2 Musique Adaptative Procédurale
class AdaptiveMusicComposer {
private:
    struct MusicalPhrase {
        std::vector<Note> notes;
        float tempo;
        TimeSignature timeSignature;
        Key key;
    };
    
    struct Note {
        float frequency;
        float duration;
        float velocity;
        Instrument instrument;
    };

public:
    AudioTrack composeBackgroundMusic(BiomeType biome, ThreatLevel threat, float exploration) {
        AudioTrack track;
        
        // Tonalité selon biome
        Key key = getBiomeKey(biome);
        
        // Tempo selon niveau de menace
        float baseTempo = 60 + threat * 20; // 60-120 BPM
        
        // Génération mélodie principale
        MusicalPhrase melody = generateMelody(key, baseTempo, exploration);
        
        // Harmonies selon biome
        MusicalPhrase harmony = generateHarmony(melody, biome);
        
        // Percussions selon tension
        MusicalPhrase drums = generatePercussion(baseTempo, threat);
        
        // Ambiance selon exploration
        MusicalPhrase ambient = generateAmbient(biome, exploration);
        
        // Mixage final
        track = mixTracks({melody, harmony, drums, ambient});
        
        return track;
    }
    
    void updateMusicIntensity(float combatIntensity, float explorationWonder) {
        currentIntensity = combatIntensity;
        currentWonder = explorationWonder;
        
        // Transition douce entre thèmes
        targetTempo = baseTempo + combatIntensity * 40;
        targetVolume = 0.7f + combatIntensity * 0.3f;
        
        // Ajout d'instruments selon contexte
        if(combatIntensity > 0.5f) {
            addCombatInstruments();
        }
        
        if(explorationWonder > 0.7f) {
            addMagicalInstruments();
        }
    }

private:
    Key getBiomeKey(BiomeType biome) {
        switch(biome) {
            case SURFACE: return Key::C_MAJOR;     // Paisible
            case FOREST: return Key::G_MAJOR;      // Naturel
            case UNDERGROUND: return Key::D_MINOR; // Sombre
            case CRYSTAL_CAVES: return Key::F_SHARP_MAJOR; // Mystique
            case ABYSSAL: return Key::B_MINOR;     // Menaçant
        }
    }
    
    MusicalPhrase generateMelody(Key key, float tempo, float exploration);
    MusicalPhrase generateHarmony(const MusicalPhrase& melody, BiomeType biome);
    MusicalPhrase generatePercussion(float tempo, ThreatLevel threat);
    AudioTrack mixTracks(const std::vector<MusicalPhrase>& phrases);
};

5.3 Audio 3D et Ambiance
class Audio3DManager {
private:
    struct AudioSource3D {
        Position position;
        AudioSample sample;
        float volume;
        float pitch;
        bool isLooping;
        float maxDistance;
        float rolloffFactor;
    };

public:
    void playSound3D(const AudioSample& sound, Position worldPos, float baseVolume = 1.0f) {
        AudioSource3D source;
        source.sample = sound;
        source.position = worldPos;
        source.volume = baseVolume;
        source.maxDistance = 200.0f; // Distance max audible
        source.rolloffFactor = 1.0f; // Atténuation selon distance
        
        // Calcul volume et pan selon position joueur
        float distance = distanceToPlayer(worldPos);
        float angle = angleToPlayer(worldPos);
        
        // Atténuation distance
        float distanceAttenuation = 1.0f / (1.0f + source.rolloffFactor * distance / source.maxDistance);
        
        // Pan stéréo selon angle
        float leftVolume = source.volume * distanceAttenuation * (1.0f - (sin(angle) + 1.0f) * 0.5f);
        float rightVolume = source.volume * distanceAttenuation * (1.0f + (sin(angle) + 1.0f) * 0.5f);
        
        // Effet Doppler pour objets rapides
        float dopplerShift = calculateDopplerShift(worldPos);
        source.pitch *= dopplerShift;
        
        activeSources.push_back(source);
    }
    
    void generateAmbientSounds(BiomeType biome, WeatherType weather, float timeOfDay) {
        // Sons d'ambiance selon biome
        switch(biome) {
            case SURFACE:
                if(timeOfDay < 0.3f || timeOfDay > 0.7f) { // Nuit
                    playAmbientLoop(generateNightSounds(), 0.3f);
                } else { // Jour
                    playAmbientLoop(generateBirdSongs(), 0.2f);
                }
                break;
                
            case UNDERGROUND:
                playAmbientLoop(generateCaveAmbient(), 0.4f);
                // Sons d'eau qui goutte
                if(random() < 0.01f) {
                    Position dropPos = playerPos + randomOffset(100);
                    playSound3D(generateWaterDrop(), dropPos, 0.1f);
                }
                break;
                
            case CRYSTAL_CAVES:
                playAmbientLoop(generateCrystalResonance(), 0.3f);
                // Cristaux qui résonnent
                if(random() < 0.005f) {
                    Position crystalPos = findNearestCrystal();
                    playSound3D(generateCrystalChime(), crystalPos, 0.2f);
                }
                break;
        }
        
        // Effets météo
        if(weather == RAIN) {
            playAmbientLoop(generateRainSound(), 0.5f);
        } else if(weather == STORM) {
            playAmbientLoop(generateRainSound(), 0.8f);
            if(random() < 0.002f) { // Tonnerre occasionnel
                playSound3D(generateThunder(), playerPos + randomOffset(500), 1.0f);
            }
        }
    }
    
private:
    std::vector<AudioSource3D> activeSources;
    Position playerPos;
    
    float distanceToPlayer(Position worldPos);
    float angleToPlayer(Position worldPos);
    float calculateDopplerShift(Position objectPos);
    AudioSample generateNightSounds();
    AudioSample generateBirdSongs();
    AudioSample generateCaveAmbient();
};


6. MONDE ET GÉNÉRATION PROCÉDURALE
6.1 Système de Génération de Terrain
class WorldGenerator {
private:
    struct BiomeData {
        float temperature;      // -1.0 à 1.0
        float humidity;         // 0.0 à 1.0
        float elevation;        // 0.0 à 1.0
        float magicDensity;     // 0.0 à 1.0
        std::vector<MaterialType> commonMaterials;
        std::vector<EnemyType> nativeEnemies;
    };

public:
    void generateWorld(uint32_t seed, int width, int height) {
        worldSeed = seed;
        worldWidth = width;
        worldHeight = height;
        
        // 1. Génération heightmap de base
        generateHeightMap();
        
        // 2. Génération cartes de biomes
        generateTemperatureMap();
        generateHumidityMap();
        generateMagicMap();
        
        // 3. Classification biomes
        classifyBiomes();
        
        // 4. Génération détaillée par chunk
        for(int chunkY = 0; chunkY < height / CHUNK_SIZE; chunkY++) {
            for(int chunkX = 0; chunkX < width / CHUNK_SIZE; chunkX++) {
                generateChunk(chunkX, chunkY);
            }
        }
        
        // 5. Placement structures
        generateStructures();
        
        // 6. Population entités
        populateWorld();
    }
    
private:
    void generateHeightMap() {
        heightMap.resize(worldWidth * worldHeight);
        
        // Combinaison de plusieurs octaves de bruit
        for(int y = 0; y < worldHeight; y++) {
            for(int x = 0; x < worldWidth; x++) {
                float height = 0;
                
                // Bruit principal (continents)
                height += perlinNoise(x * 0.001f, y * 0.001f, worldSeed) * 0.5f;
                
                // Détails moyens (collines)
                height += perlinNoise(x * 0.005f, y * 0.005f, worldSeed + 1) * 0.3f;
                
                // Détails fins (variation locale)
                height += perlinNoise(x * 0.02f, y * 0.02f, worldSeed + 2) * 0.2f;
                
                // Normalisation [0, 1]
                height = (height + 1.0f) * 0.5f;
                heightMap[y * worldWidth + x] = height;
            }
        }
    }
    
    void generateChunk(int chunkX, int chunkY) {
        Chunk chunk;
        chunk.x = chunkX;
        chunk.y = chunkY;
        
        for(int localY = 0; localY < CHUNK_SIZE; localY++) {
            for(int localX = 0; localX < CHUNK_SIZE; localX++) {
                int worldX = chunkX * CHUNK_SIZE + localX;
                int worldY = chunkY * CHUNK_SIZE + localY;
                
                TileType tile = determineTileType(worldX, worldY);
                chunk.tiles[localY][localX] = tile;
                
                // Génération végétation
                if(shouldSpawnVegetation(worldX, worldY)) {
                    VegetationType vegType = selectVegetation(worldX, worldY);
                    chunk.vegetation[localY][localX] = vegType;
                }
                
                // Génération ressources
                if(shouldSpawnResource(worldX, worldY)) {
                    ResourceNode resource = generateResourceNode(worldX, worldY);
                    chunk.resources.push_back(resource);
                }
            }
        }
        
        chunks[chunkY * (worldWidth/CHUNK_SIZE) + chunkX] = chunk;
    }
    
    TileType determineTileType(int x, int y) {
        float height = getHeight(x, y);
        BiomeType biome = getBiome(x, y);
        float depth = calculateDepth(y);
        
        // Surface
        if(depth < 0.1f) {
            switch(biome) {
                case PLAINS: return height > 0.4f ? GRASS : WATER;
                case FOREST: return GRASS;
                case DESERT: return SAND;
                case MOUNTAINS: return height > 0.7f ? ROCK : GRASS;
            }
        }
        // Souterrain shallow
        else if(depth < 0.3f) {
            float stoneChance = 0.7f + depth * 0.2f;
            return random() < stoneChance ? STONE : DIRT;
        }
        // Souterrain profond
        else if(depth < 0.7f) {
            float stoneChance = 0.9f;
            if(random() < stoneChance) {
                // Chance de minerai selon profondeur
                float oreChance = depth * 0.15f;
                if(random() < oreChance) {
                    return selectOreType(depth, biome);
                }
                return STONE;
            }
            return BEDROCK;
        }
        // Abysses
        else {
            return VOID_STONE;
        }
    }
};

6.2 Génération de Structures Complexes
class StructureGenerator {
public:
    void generateDungeon(Position center, DungeonType type, int difficulty) {
        DungeonLayout layout;
        
        // 1. Génération layout de base
        layout = generateDungeonLayout(type, difficulty);
        
        // 2. Création des salles
        for(auto& room : layout.rooms) {
            generateRoom(room, type);
        }
        
        // 3. Connexion par corridors
        generateCorridors(layout);
        
        // 4. Placement contenu
        placeDungeonContent(layout, difficulty);
        
        // 5. Placement boss final
        placeBossRoom(layout, type, difficulty);
    }
    
private:
    DungeonLayout generateDungeonLayout(DungeonType type, int difficulty) {
        DungeonLayout layout;
        
        int roomCount = 5 + difficulty * 2; // 5-15 salles
        
        // Algorithme de placement salles
        for(int i = 0; i < roomCount; i++) {
            Room room;
            
            // Taille variable selon type
            switch(type) {
                case ANCIENT_TEMPLE:
                    room.width = 8 + random() % 6;  // 8-14
                    room.height = 6 + random() % 4; // 6-10
                    break;
                case CRYSTAL_CAVERN:
                    room.width = 12 + random() % 8; // 12-20 (plus grandes)
                    room.height = 8 + random() % 6; // 8-14
                    break;
                case ABANDONED_MINE:
                    room.width = 6 + random() % 4;  // 6-10 (plus petites)
                    room.height = 4 + random() % 3; // 4-7
                    break;
            }
            
            // Placement évitant les chevauchements
            bool placed = false;
            int attempts = 0;
            while(!placed && attempts < 100) {
                room.x = random() % 50;
                room.y = random() % 50;
                
                if(!overlapsExistingRoom(room, layout.rooms)) {
                    layout.rooms.push_back(room);
                    placed = true;
                }
                attempts++;
            }
        }
        
        return layout;
    }
    
    void generateRoom(Room& room, DungeonType type) {
        // Génération forme de base
        for(int y = 0; y < room.height; y++) {
            for(int x = 0; x < room.width; x++) {
                if(x == 0 || x == room.width-1 || y == 0 || y == room.height-1) {
                    room.tiles[y][x] = WALL;
                } else {
                    room.tiles[y][x] = FLOOR;
                }
            }
        }
        
        // Personnalisation selon type donjon
        switch(type) {
            case ANCIENT_TEMPLE:
                addTempleDecorations(room);
                break;
            case CRYSTAL_CAVERN:
                addCrystalFormations(room);
                addNaturalWalls(room); // Forme plus organique
                break;
            case ABANDONED_MINE:
                addMineSupports(room);
                addMinecartTracks(room);
                break;
        }
        
        // Ajout détails aléatoires
        addRoomDetails(room);
    }
    
    void addTempleDecorations(Room& room) {
        // Piliers aux coins
        if(room.width > 6 && room.height > 6) {
            room.tiles[2][2] = PILLAR;
            room.tiles[2][room.width-3] = PILLAR;
            room.tiles[room.height-3][2] = PILLAR;
            room.tiles[room.height-3][room.width-3] = PILLAR;
        }
        
        // Autel au centre
        int centerX = room.width / 2;
        int centerY = room.height / 2;
        room.tiles[centerY][centerX] = ALTAR;
        
        // Motifs décoratifs sur les murs
        for(int x = 2; x < room.width-2; x += 2) {
            if(room.tiles[1][x] == WALL) {
                room.decorations.push_back({x, 1, TEMPLE_RELIEF});
            }
        }
    }
    
    void addCrystalFormations(Room& room) {
        // Cristaux aléatoires dans la salle
        int crystalCount = 3 + random() % 5;
        
        for(int i = 0; i < crystalCount; i++) {
            int x = 2 + random() % (room.width - 4);
            int y = 2 + random() % (room.height - 4);
            
            if(room.tiles[y][x] == FLOOR) {
                CrystalType type = (CrystalType)(random() % 4);
                room.crystals.push_back({x, y, type});
                
                // Éclairage magique
                room.lightSources.push_back({x, y, getCrystalColor(type), 0.8f, 30});
            }
        }
        
        // Formations cristallines sur les murs
        addWallCrystals(room);
    }
};

6.3 Système de Biomes Dynamiques
class BiomeManager {
private:
    struct BiomeTransition {
        BiomeType from, to;
        float transitionWidth;
        std::vector<TileType> blendTiles;
    };

public:
    BiomeType getBiomeAt(float x, float y) {
        float temperature = getTemperature(x, y);
        float humidity = getHumidity(x, y);
        float elevation = getElevation(x, y);
        float magic = getMagicDensity(x, y);
        
        // Classification selon paramètres
        if(elevation > 0.8f) {
            return MOUNTAINS;
        } else if(temperature < -0.5f) {
            return TUNDRA;
        } else if(humidity < 0.2f && temperature > 0.3f) {
            return DESERT;
        } else if(magic > 0.7f) {
            return MAGICAL_FOREST;
        } else if(humidity > 0.6f && temperature > 0.0f) {
            return FOREST;
        } else {
            return PLAINS;
        }
    }
    
    void generateBiomeTransitions() {
        // Détection frontières entre biomes
        for(int y = 1; y < worldHeight - 1; y++) {
            for(int x = 1; x < worldWidth - 1; x++) {
                BiomeType center = getBiomeAt(x, y);
                
                // Vérification voisins
                std::set<BiomeType> neighborBiomes;
                for(int dy = -1; dy <= 1; dy++) {
                    for(int dx = -1; dx <= 1; dx++) {
                        BiomeType neighbor = getBiomeAt(x + dx, y + dy);
                        if(neighbor != center) {
                            neighborBiomes.insert(neighbor);
                        }
                    }
                }
                
                // Création zone de transition
                if(!neighborBiomes.empty()) {
                    createTransitionZone(x, y, center, neighborBiomes);
                }
            }
        }
    }
    
    void updateBiomeDynamics(float deltaTime) {
        // Évolution temporelle des biomes
        for(auto& biome : activeBiomes) {
            updateBiomeWeather(biome, deltaTime);
            updateBiomeSeasons(biome, deltaTime);
            updateBiomeMagic(biome, deltaTime);
        }
    }
    
private:
    void createTransitionZone(int x, int y, BiomeType center, const std::set<BiomeType>& neighbors) {
        TransitionZone zone;
        zone.center = {x, y};
        zone.primaryBiome = center;
        zone.secondaryBiomes = std::vector<BiomeType>(neighbors.begin(), neighbors.end());
        
        // Génération tiles de transition
        zone.blendTiles = generateBlendTiles(center, zone.secondaryBiomes);
        
        transitionZones.push_back(zone);
    }
    
    std::vector<TileType> generateBlendTiles(BiomeType primary, const std::vector<BiomeType>& secondary) {
        std::vector<TileType> blendTiles;
        
        // Mélange caractéristiques des biomes
        for(auto secondaryBiome : secondary) {
            auto transition = getBiomeTransition(primary, secondaryBiome);
            blendTiles.insert(blendTiles.end(), transition.blendTiles.begin(), transition.blendTiles.end());
        }
        
        return blendTiles;
    }
    
    void updateBiomeWeather(BiomeData& biome, float deltaTime) {
        // Cycles météorologiques
        biome.weatherTimer += deltaTime;
        
        if(biome.weatherTimer > biome.weatherCycleDuration) {
            // Changement météo
            WeatherType newWeather = selectRandomWeather(biome.type);
            transitionWeather(biome, newWeather);
            biome.weatherTimer = 0;
        }
        
        // Effets météo sur environnement
        applyWeatherEffects(biome);
    }
};


7. SYSTÈMES DE PROGRESSION
7.1 Système de Compétences Multi-Arbre
class SkillSystem {
private:
    struct Skill {
        SkillID id;
        std::string name;
        std::string description;
        int currentLevel;
        int maxLevel;
        float experience;
        float experienceToNext;
        std::vector<SkillID> prerequisites;
        std::vector<SkillBonus> bonuses;
        bool isUnlocked;
    };
    
    struct SkillTree {
        SkillCategory category;
        std::vector<Skill> skills;
        std::map<int, std::vector<SkillID>> tierSkills; // Skills par tier
    };

public:
    void initializeSkillTrees() {
        // Arbre Mining
        SkillTree miningTree;
        miningTree.category = MINING;
        
        // Tier 1 - Bases
        miningTree.skills.push_back({
            BASIC_MINING, "Mining Basique", "Augmente vitesse mining +1%/niveau",
            0, 50, 0, 100, {}, {{MINING_SPEED, 0.01f}}, true
        });
        
        miningTree.skills.push_back({
            TOOL_DURABILITY, "Durabilité Outils", "Réduit usure outils -2%/niveau",
            0, 25, 0, 150, {BASIC_MINING}, {{TOOL_WEAR_REDUCTION, 0.02f}}, false
        });
        
        // Tier 2 - Spécialisations
        miningTree.skills.push_back({
            VEIN_MINING, "Minage en Veine", "Permet miner veines entières",
            0, 1, 0, 500, {BASIC_MINING}, {{UNLOCK_ABILITY, VEIN_MINE_ABILITY}}, false
        });
        
        miningTree.skills.push_back({
            ORE_DETECTION, "Détection Minerai", "Révèle minerais portée +1/niveau",
            0, 20, 0, 200, {BASIC_MINING}, {{ORE_DETECT_RANGE, 1.0f}}, false
        });
        
        // Tier 3 - Maîtrise
        miningTree.skills.push_back({
            MINING_LUCK, "Chance Mining", "Chance drops bonus +1%/niveau",
            0, 30, 0, 300, {VEIN_MINING, ORE_DETECTION}, {{MINING_LUCK_BONUS, 0.01f}}, false
        });
        
        skillTrees[MINING] = miningTree;
        
        // Arbre Combat
        initializeCombatTree();
        
        // Arbre Magie
        initializeMagicTree();
        
        // Arbre Craft
        initializeCraftingTree();
    }
    
    void addExperience(SkillCategory category, float amount) {
        if(skillTrees.find(category) == skillTrees.end()) return;
        
        auto& tree = skillTrees[category];
        
        // Distribution XP selon skills actives
        for(auto& skill : tree.skills) {
            if(skill.isUnlocked && skill.currentLevel < skill.maxLevel) {
                skill.experience += amount;
                
                // Check level up
                while(skill.experience >= skill.experienceToNext) {
                    levelUpSkill(skill);
                }
            }
        }
    }
    
private:
    void levelUpSkill(Skill& skill) {
        skill.experience -= skill.experienceToNext;
        skill.currentLevel++;
        
        // Calcul XP pour niveau suivant (courbe exponentielle)
        skill.experienceToNext = skill.experienceToNext * 1.15f;
        
        // Application bonus
        applySkillBonuses(skill);
        
        // Déblocage nouvelles compétences
        unlockDependentSkills(skill.id);
        
        // Événement level up
        triggerSkillLevelUpEvent(skill);
        
        // Effets visuels
        showSkillLevelUpEffects(skill);
    }
    
    void applySkillBonuses(const Skill& skill) {
        for(const auto& bonus : skill.bonuses) {
            float totalBonus = bonus.valuePerLevel * skill.currentLevel;
            
            switch(bonus.type) {
                case MINING_SPEED:
                    player.miningSpeedBonus += totalBonus;
                    break;
                case ATTACK_DAMAGE:
                    player.attackDamageBonus += totalBonus;
                    break;
                case SPELL_POWER:
                    player.spellPowerBonus += totalBonus;
                    break;
                case CRAFT_SUCCESS_CHANCE:
                    player.craftSuccessBonus += totalBonus;
                    break;
            }
        }
    }
    
    void initializeCombatTree() {
        SkillTree combatTree;
        combatTree.category = COMBAT;
        
        // Branches spécialisées
        // Mêlée
        combatTree.skills.push_back({
            MELEE_MASTERY, "Maîtrise Mêlée", "Dégâts mêlée +2%/niveau",
            0, 40, 0, 120, {}, {{MELEE_DAMAGE, 0.02f}}, true
        });
        
        combatTree.skills.push_back({
            SWORD_SPECIALIST, "Spécialiste Épée", "Épées +5% dégâts et vitesse",
            0, 20, 0, 200, {MELEE_MASTERY}, {{SWORD_DAMAGE, 0.05f}, {SWORD_SPEED, 0.05f}}, false
        });
        
        combatTree.skills.push_back({
            BERSERKER_RAGE, "Rage Berserker", "Faible HP = +dégâts, -défense",
            0, 1, 0, 800, {SWORD_SPECIALIST}, {{UNLOCK_ABILITY, BERSERKER_MODE}}, false
        });
        
        // Distance
        combatTree.skills.push_back({
            RANGED_MASTERY, "Maîtrise Distance", "Dégâts distance +2%/niveau",
            0, 40, 0, 120, {}, {{RANGED_DAMAGE, 0.02f}}, true
        });
        
        combatTree.skills.push_back({
            ARCHERY_EXPERT, "Expert Archerie", "Portée +10%, critique +2%",
            0, 15, 0, 250, {RANGED_MASTERY}, {{BOW_RANGE, 0.1f}, {BOW_CRIT, 0.02f}}, false
        });
        
        skillTrees[COMBAT] = combatTree;
    }
    
    void initializeMagicTree() {
        SkillTree magicTree;
        magicTree.category = MAGIC;
        
        // École Élémentaire
        magicTree.skills.push_back({
            ELEMENTAL_BASICS, "Bases Élémentaires", "Mana max +5/niveau",
            0, 30, 0, 100, {}, {{MAX_MANA, 5.0f}}, true
        });
        
        magicTree.skills.push_back({
            FIRE_MASTERY, "Maîtrise Feu", "Sorts feu +3% dégâts/niveau",
            0, 25, 0, 180, {ELEMENTAL_BASICS}, {{FIRE_SPELL_DAMAGE, 0.03f}}, false
        });
        
        magicTree.skills.push_back({
            METEOR_SPELL, "Sort Météore", "Débloque sort Météore",
            0, 1, 0, 1000, {FIRE_MASTERY}, {{UNLOCK_SPELL, METEOR}}, false
        });
        
        // École Arcane
        magicTree.skills.push_back({
            ARCANE_KNOWLEDGE, "Savoir Arcane", "Régén mana +1%/niveau",
            0, 20, 0, 150, {ELEMENTAL_BASICS}, {{MANA_REGEN, 0.01f}}, false
        });
        
        magicTree.skills.push_back({
            TELEPORTATION, "Téléportation", "Débloque téléportation",
            0, 1, 0, 600, {ARCANE_KNOWLEDGE}, {{UNLOCK_SPELL, TELEPORT}}, false
        });
        
        skillTrees[MAGIC] = magicTree;
    }
    
    std::map<SkillCategory, SkillTree> skillTrees;
    Player& player;
};

7.2 Système d'Achievements et Découvertes
class AchievementSystem {
private:
    struct Achievement {
        AchievementID id;
        std::string name;
        std::string description;
        AchievementType type;
        std::vector<Condition> conditions;
        std::vector<Reward> rewards;
        bool isUnlocked;
        bool isHidden; // Achievements secrets
        float progress;
    };
    
    struct Condition {
        ConditionType type;
        std::variant<int, float, std::string> targetValue;
        std::variant<int, float, std::string> currentValue;
    };

public:
    void initializeAchievements() {
        // Achievements Mining
        achievements.push_back({
            FIRST_STONE, "Premier Pas", "Minez votre première pierre",
            MINING, {{MINE_MATERIAL, "stone", 1}}, {{EXPERIENCE, 50}}, false, false, 0
        });
        
        achievements.push_back({
            STONE_COLLECTOR, "Collectionneur Pierre", "Minez 1000 pierres",
            MINING, {{MINE_MATERIAL, "stone", 1000}}, {{SKILL_POINTS, 5}}, false, false, 0
        });
        
        achievements.push_back({
            RARE_FINDER, "Chercheur de Rareté", "Trouvez votre premier cristal",
            EXPLORATION, {{FIND_MATERIAL, "crystal", 1}}, {{UNLOCK_RECIPE, "crystal_pickaxe"}}, false, false, 0
        });
        
        // Achievements Combat
        achievements.push_back({
            FIRST_KILL, "Premier Sang", "Tuez votre premier ennemi",
            COMBAT, {{KILL_ENEMY, "any", 1}}, {{EXPERIENCE, 100}}, false, false, 0
        });
        
        achievements.push_back({
            SLIME_SLAYER, "Tueur de Slimes", "Éliminez 100 slimes",
            COMBAT, {{KILL_ENEMY, "slime", 100}}, {{TITLE, "Fléau des Slimes"}}, false, false, 0
        });
        
        achievements.push_back({
            BOSS_HUNTER, "Chasseur de Boss", "Vainquez tous les boss",
            COMBAT, {{KILL_BOSS, "all", 5}}, {{LEGENDARY_ITEM, "crown_of_victory"}}, false, true, 0
        });
        
        // Achievements Exploration
        achievements.push_back({
            DEEP_EXPLORER, "Explorateur des Profondeurs", "Atteignez 500m de profondeur",
            EXPLORATION, {{REACH_DEPTH, 500}}, {{SKILL_POINTS, 3}}, false, false, 0
        });
        
        achievements.push_back({
            BIOME_MASTER, "Maître des Biomes", "Visitez tous les biomes",
            EXPLORATION, {{VISIT_BIOMES, 5}}, {{TELEPORT_STONES, 10}}, false, false, 0
        });
        
        // Achievements Craft
        achievements.push_back({
            FIRST_CRAFT, "Premier Artisan", "Craftez votre premier objet",
            CRAFTING, {{CRAFT_ITEM, "any", 1}}, {{EXPERIENCE, 75}}, false, false, 0
        });
        
        achievements.push_back({
            MASTER_CRAFTER, "Maître Artisan", "Craftez 500 objets",
            CRAFTING, {{CRAFT_ITEM, "any", 500}}, {{CRAFT_SPEED_BONUS, 0.25f}}, false, false, 0
        });
        
        // Achievements Secrets
        achievements.push_back({
            VOID_TOUCHED, "Touché par le Vide", "Survivez à une exposition au Void",
            SECRET, {{SURVIVE_VOID_EXPOSURE, 1}}, {{VOID_RESISTANCE, 0.5f}}, false, true, 0
        });
        
        achievements.push_back({
            TIME_MASTER, "Maître du Temps", "Utilisez la temporite 100 fois",
            SECRET, {{USE_ITEM, "temporite", 100}}, {{TIME_MANIPULATION_POWERS, true}}, false, true, 0
        });
    }
    
    void checkAchievements(const GameEvent& event) {
        for(auto& achievement : achievements) {
            if(achievement.isUnlocked) continue;
            
            bool allConditionsMet = true;
            
            for(auto& condition : achievement.conditions) {
                if(!evaluateCondition(condition, event)) {
                    allConditionsMet = false;
                    break;
                }
            }
            
            if(allConditionsMet) {
                unlockAchievement(achievement);
            } else {
                updateAchievementProgress(achievement, event);
            }
        }
    }
    
private:
    void unlockAchievement(Achievement& achievement) {
        achievement.isUnlocked = true;
        achievement.progress = 1.0f;
        
        // Récompenses
        for(const auto& reward : achievement.rewards) {
            grantReward(reward);
        }
        
        // Notification joueur
        showAchievementNotification(achievement);
        
        // Effets visuels
        playAchievementEffects(achievement);
        
        // Sauvegarde
        saveAchievementProgress();
    }
    
    void showAchievementNotification(const Achievement& achievement) {
        AchievementNotification notification;
        notification.title = achievement.name;
        notification.description = achievement.description;
        notification.icon = getAchievementIcon(achievement.type);
        notification.displayTime = 5.0f;
        notification.animationType = SLIDE_IN_FROM_TOP;
        
        uiManager.showNotification(notification);
        
        // Son de déblocage
        audioManager.playAchievementSound(achievement.type);
    }
    
    std::vector<Achievement> achievements;
    std::map<AchievementID, float> progressTracker;
};

7.3 Système de Quêtes Dynamiques
class QuestSystem {
private:
    struct Quest {
        QuestID id;
        std::string title;
        std::string description;
        QuestType type;
        QuestGiver giver;
        std::vector<QuestObjective> objectives;
        std::vector<Reward> rewards;
        QuestStatus status;
        int playerLevel; // Niveau requis
        std::vector<QuestID> prerequisites;
    };
    
    struct QuestObjective {
        ObjectiveType type;
        std::string target;
        int requiredAmount;
        int currentAmount;
        bool isCompleted;
        std::string hint;
    };

public:
    void initializeQuests() {
        // Quête principale - Chapitre 1
        quests.push_back({
            MAIN_001, "Les Premiers Pas", 
            "Apprenez les bases du minage et de la survie",
            MAIN_STORY, NPC_TUTORIAL_GUIDE,
            {
                {MINE_MATERIAL, "stone", 10, 0, false, "Utilisez votre pioche sur de la pierre"},
                {CRAFT_ITEM, "wooden_pickaxe", 1, 0, false, "Créez une pioche en bois à l'établi"},
                {KILL_ENEMY, "slime", 3, 0, false, "Éliminez quelques slimes hostiles"}
            },
            {{EXPERIENCE, 200}, {ITEM, "iron_pickaxe", 1}, {UNLOCK_RECIPE, "basic_armor"}},
            AVAILABLE, 1, {}
        });
        
        // Quête principale - Chapitre 2
        quests.push_back({
            MAIN_002, "Vers les Profondeurs",
            "Explorez les cavernes souterraines",
            MAIN_STORY, NPC_VILLAGE_ELDER,
            {
                {REACH_DEPTH, "", 100, 0, false, "Descendez à 100m de profondeur"},
                {FIND_STRUCTURE, "underground_ruins", 1, 0, false, "Trouvez des ruines anciennes"},
                {COLLECT_ITEM, "ancient_artifact", 1, 0, false, "Récupérez un artefact"}
            },
            {{EXPERIENCE, 500}, {SKILL_POINTS, 3}, {UNLOCK_SPELL, "light"}},
            LOCKED, 5, {MAIN_001}
        });
        
        // Quêtes secondaires - Collection
        quests.push_back({
            SIDE_001, "Marchand Affamé",
            "Le marchand a besoin de nourriture fraîche",
            COLLECTION, NPC_MERCHANT,
            {
                {COLLECT_ITEM, "bread", 5, 0, false, "Cuisinez ou achetez du pain"},
                {COLLECT_ITEM, "meat", 3, 0, false, "Chassez des animaux pour la viande"}
            },
            {{GOLD, 150}, {REPUTATION, "merchant_guild", 25}},
            AVAILABLE, 1, {}
        });
        
        // Quêtes dynamiques générées
        generateDynamicQuests();
    }
    
    void generateDynamicQuests() {
        // Génération basée sur actions joueur récentes
        PlayerStats stats = getPlayerStats();
        
        if(stats.totalMined > 500) {
            // Quête mining avancée
            Quest miningQuest = {
                DYNAMIC_MINING_001, "Maître Mineur Local",
                "Votre réputation de mineur grandit. Prouvez vos compétences.",
                SKILL_CHALLENGE, NPC_MINING_FOREMAN,
                {{MINE_RARE_MATERIAL, "rare_gems", 20, 0, false, "Trouvez des gemmes rares"}},
                {{SKILL_BOOK, "advanced_mining"}, {TITLE, "Maître Mineur"}},
                AVAILABLE, stats.level, {}
            };
            dynamicQuests.push_back(miningQuest);
        }
        
        if(stats.enemiesKilled > 100) {
            // Quête combat épique
            Quest combatQuest = generateCombatChallenge(stats.level);
            dynamicQuests.push_back(combatQuest);
        }
        
        // Quêtes exploration basées sur biomes non visités
        for(auto biome : getUnvisitedBiomes()) {
            Quest explorationQuest = generateExplorationQuest(biome);
            dynamicQuests.push_back(explorationQuest);
        }
    }
    
    void updateQuestProgress(const GameEvent& event) {
        for(auto& quest : activeQuests) {
            if(quest.status != IN_PROGRESS) continue;
            
            for(auto& objective : quest.objectives) {
                if(objective.isCompleted) continue;
                
                if(matchesObjective(objective, event)) {
                    objective.currentAmount++;
                    
                    if(objective.currentAmount >= objective.requiredAmount) {
                        objective.isCompleted = true;
                        showObjectiveCompleteNotification(objective);
                    } else {
                        showObjectiveProgressNotification(objective);
                    }
                }
            }
            
            // Check si quête complète
            if(areAllObjectivesComplete(quest)) {
                completeQuest(quest);
            }
        }
    }
    
private:
    Quest generateCombatChallenge(int playerLevel) {
        Quest challenge;
        challenge.id = generateDynamicQuestID();
        challenge.title = "Défi de Combat";
        challenge.type = SKILL_CHALLENGE;
        challenge.giver = NPC_ARENA_MASTER;
        challenge.playerLevel = playerLevel;
        
        // Objectifs adaptés au niveau
        if(playerLevel < 10) {
            challenge.objectives.push_back({
                KILL_ENEMY, "slime", 20, 0, false, "Éliminez 20 slimes"
            });
        } else if(playerLevel < 20) {
            challenge.objectives.push_back({
                KILL_ENEMY, "golem", 10, 0, false, "Détruisez 10 golems"
            });
        } else {
            challenge.objectives.push_back({
                KILL_BOSS, "any", 1, 0, false, "Vainquez un boss"
            });
        }
        
        // Récompenses échelonnées
        int expReward = playerLevel * 50;
        challenge.rewards.push_back({EXPERIENCE, expReward});
        
        if(playerLevel >= 15) {
            challenge.rewards.push_back({LEGENDARY_ITEM, selectRandomLegendaryWeapon()});
        }
        
        return challenge;
    }
    
    void completeQuest(Quest& quest) {
        quest.status = COMPLETED;
        
        // Attribution récompenses
        for(const auto& reward : quest.rewards) {
            grantReward(reward);
        }
        
        // Déblocage quêtes suivantes
        unlockDependentQuests(quest.id);
        
        // Notification joueur
        showQuestCompleteNotification(quest);
        
        // Mise à jour réputation
        if(quest.giver != NPC_NONE) {
            increaseNPCReputation(quest.giver, 10 + quest.playerLevel);
        }
        
        // Événement global
        triggerQuestCompleteEvent(quest);
    }
    
    std::vector<Quest> quests;
    std::vector<Quest> activeQuests;
    std::vector<Quest> dynamicQuests;
    std::map<QuestID, QuestProgress> questProgress;
};


8. INTERFACE UTILISATEUR
8.1 Système d'Interface Modulaire
class UIManager {
private:
    struct UIElement {
        UIElementID id;
        UIElementType type;
        Position position;
        Size size;
        bool isVisible;
        bool isInteractable;
        std::function<void()> onClick;
        std::unique_ptr<UIStyle> style;
        std::vector<std::unique_ptr<UIElement>> children;
    };

public:
    void initializeUI() {
        // HUD principal
        createMainHUD();
        
        // Menus de jeu
        createInventoryMenu();
        createCraftingMenu();
        createSkillTreeMenu();
        createQuestLog();
        createSettingsMenu();
        
        // Interface de combat
        createCombatUI();
        
        // Dialogues PNJ
        createDialogSystem();
    }
    
private:
    void createMainHUD() {
        auto hud = std::make_unique<UIElement>();
        hud->id = MAIN_HUD;
        hud->type = PANEL;
        hud->position = {0, 0};
        hud->size = {screenWidth, screenHeight};
        hud->isVisible = true;
        
        // Barre de vie
        auto healthBar = createHealthBar();
        hud->children.push_back(std::move(healthBar));
        
        // Barre de mana
        auto manaBar = createManaBar();
        hud->children.push_back(std::move(manaBar));
        
        // Barre d'expérience
        auto xpBar = createExperienceBar();
        hud->children.push_back(std::move(xpBar));
        
        // Hotbar compétences
        auto hotbar = createSkillHotbar();
        hud->children.push_back(std::move(hotbar));
        
        // Indicateur ressources
        auto resourceDisplay = createResourceDisplay();
        hud->children.push_back(std::move(resourceDisplay));
        
        // Minimap
        auto minimap = createMinimap();
        hud->children.push_back(std::move(minimap));
        
        uiElements[MAIN_HUD] = std::move(hud);
    }
    
    std::unique_ptr<UIElement> createHealthBar() {
        auto healthBar = std::make_unique<UIElement>();
        healthBar->id = HEALTH_BAR;
        healthBar->type = PROGRESS_BAR;
        healthBar->position = {20, 20};
        healthBar->size = {200, 20};
        
        // Style barre de vie
        auto style = std::make_unique<ProgressBarStyle>();
        style->backgroundColor = Color(64, 0, 0);    // Rouge sombre
        style->fillColor = Color(255, 0, 0);         // Rouge vif
        style->borderColor = Color(255, 255, 255);   // Blanc
        style->borderWidth = 2;
        style->cornerRadius = 5;
        
        // Animation pulse quand vie faible
        style->animations.push_back({
            PULSE, 1.0f, INFINITE, EASE_IN_OUT,
            [](float t) -> Color {
                float intensity = sin(t * PI * 2) * 0.3f + 0.7f;
                return Color(255 * intensity, 0, 0);
            }
        });
        
        healthBar->style = std::move(style);
        return healthBar;
    }
    
    std::unique_ptr<UIElement> createInventoryMenu() {
        auto inventory = std::make_unique<UIElement>();
        inventory->id = INVENTORY_MENU;
        inventory->type = WINDOW;
        inventory->position = {screenWidth/2 - 300, screenHeight/2 - 250};
        inventory->size = {600, 500};
        inventory->isVisible = false;
        
        // Style fenêtre
        auto windowStyle = std::make_unique<WindowStyle>();
        windowStyle->backgroundColor = Color(40, 40, 40, 240); // Semi-transparent
        windowStyle->borderColor = Color(100, 100, 100);
        windowStyle->titleBarColor = Color(60, 60, 60);
        windowStyle->titleText = "Inventaire";
        windowStyle->isDraggable = true;
        windowStyle->hasCloseButton = true;
        
        inventory->style = std::move(windowStyle);
        
        // Grille d'objets
        auto itemGrid = createInventoryGrid();
        inventory->children.push_back(std::move(itemGrid));
        
        // Info objet sélectionné
        auto itemInfo = createItemInfoPanel();
        inventory->children.push_back(std::move(itemInfo));
        
        // Boutons d'action
        auto actionButtons = createInventoryActionButtons();
        inventory->children.push_back(std::move(actionButtons));
        
        return inventory;
    }
    
    std::unique_ptr<UIElement> createSkillTreeMenu() {
        auto skillTree = std::make_unique<UIElement>();
        skillTree->id = SKILL_TREE_MENU;
        skillTree->type = WINDOW;
        skillTree->position = {50, 50};
        skillTree->size = {screenWidth - 100, screenHeight - 100};
        skillTree->isVisible = false;
        
        // Onglets pour différents arbres
        auto tabContainer = std::make_unique<UIElement>();
        tabContainer->type = TAB_CONTAINER;
        tabContainer->position = {10, 40};
        tabContainer->size = {skillTree->size.width - 20, 50};
        
        // Onglet Mining
        auto miningTab = createSkillTreeTab("Mining", MINING_TREE);
        tabContainer->children.push_back(std::move(miningTab));
        
        // Onglet Combat
        auto combatTab = createSkillTreeTab("Combat", COMBAT_TREE);
        tabContainer->children.push_back(std::move(combatTab));
        
        // Onglet Magic
        auto magicTab = createSkillTreeTab("Magie", MAGIC_TREE);
        tabContainer->children.push_back(std::move(magicTab));
        
        skillTree->children.push_back(std::move(tabContainer));
        
        // Zone d'affichage arbre
        auto treeDisplay = createSkillTreeDisplay();
        skillTree->children.push_back(std::move(treeDisplay));
        
        return skillTree;
    }
    
    void createDialogSystem() {
        auto dialogBox = std::make_unique<UIElement>();
        dialogBox->id = DIALOG_BOX;
        dialogBox->type = DIALOG;
        dialogBox->position = {50, screenHeight - 200};
        dialogBox->size = {screenWidth - 100, 150};
        dialogBox->isVisible = false;
        
        // Style dialogue
        auto dialogStyle = std::make_unique<DialogStyle>();
        dialogStyle->backgroundColor = Color(0, 0, 0, 200);
        dialogStyle->borderColor = Color(255, 255, 255);
        dialogStyle->textColor = Color(255, 255, 255);
        dialogStyle->font = "PixelFont";
        dialogStyle->fontSize = 16;
        dialogStyle->typewriterEffect = true;
        dialogStyle->typewriterSpeed = 50; // caractères par seconde
        
        dialogBox->style = std::move(dialogStyle);
        
        // Portrait PNJ
        auto npcPortrait = std::make_unique<UIElement>();
        npcPortrait->type = IMAGE;
        npcPortrait->position = {10, 10};
        npcPortrait->size = {80, 80};
        dialogBox->children.push_back(std::move(npcPortrait));
        
        // Texte dialogue
        auto dialogText = std::make_unique<UIElement>();
        dialogText->type = TEXT_AREA;
        dialogText->position = {100, 10};
        dialogText->size = {dialogBox->size.width - 120, 80};
        dialogBox->children.push_back(std::move(dialogText));
        
        // Options de réponse
        auto responseContainer = std::make_unique<UIElement>();
        responseContainer->type = VERTICAL_LAYOUT;
        responseContainer->position = {100, 95};
        responseContainer->size = {dialogBox->size.width - 120, 50};
        dialogBox->children.push_back(std::move(responseContainer));
        
        uiElements[DIALOG_BOX] = std::move(dialogBox);
    }

public:
    void updateUI(float deltaTime) {
        // Mise à jour animations
        updateAnimations(deltaTime);
        
        // Mise à jour valeurs dynamiques
        updateHealthBar();
        updateManaBar();
        updateExperienceBar();
        updateResourceDisplay();
        updateMinimap();
        
        // Gestion input UI
        handleUIInput();
        
        // Mise à jour tooltips
        updateTooltips();
    }
    
    void renderUI() {
        // Rendu éléments par ordre de profondeur
        std::vector<UIElement*> renderQueue = buildRenderQueue();
        
        for(auto element : renderQueue) {
            if(element->isVisible) {
                renderElement(element);
            }
        }
        
        // Rendu curseur
        renderCursor();
    }
    
private:
    void renderElement(UIElement* element) {
        switch(element->type) {
            case PROGRESS_BAR:
                renderProgressBar(element);
                break;
            case BUTTON:
                renderButton(element);
                break;
            case WINDOW:
                renderWindow(element);
                break;
            case TEXT_AREA:
                renderTextArea(element);
                break;
            case IMAGE:
                renderImage(element);
                break;
        }
        
        // Rendu enfants
        for(auto& child : element->children) {
            renderElement(child.get());
        }
    }
    
    void renderProgressBar(UIElement* element) {
        auto style = static_cast<ProgressBarStyle*>(element->style.get());
        
        // Background
        drawRectangle(element->position, element->size, style->backgroundColor);
        
        // Fill (selon valeur actuelle)
        float fillWidth = element->size.width * getCurrentValue(element->id);
        Size fillSize = {fillWidth, element->size.height - 4};
        Position fillPos = {element->position.x + 2, element->position.y + 2};
        
        drawRectangle(fillPos, fillSize, style->fillColor);
        
        // Border
        drawRectangleBorder(element->position, element->size, style->borderColor, style->borderWidth);
        
        // Texte (valeur numérique)
        if(style->showText) {
            std::string text = formatProgressText(element->id);
            Position textPos = centerText(text, element->position, element->size);
            drawText(text, textPos, style->textColor, style->font);
        }
    }
    
    std::map<UIElementID, std::unique_ptr<UIElement>> uiElements;
    std::vector<Animation> activeAnimations;
    UIElement* hoveredElement = nullptr;
    UIElement* focusedElement = nullptr;
};


9. PLAN DE DÉVELOPPEMENT
9.1 Phases de Développement
Phase 1 : Fondations Core (Semaines 1-6)
Semaine 1-2 : Moteur de Base
// Priorités
- Architecture SDL2 + systèmes de base
- Boucle de jeu principale
- Gestionnaire d'états (menu, jeu, pause)
- Système d'input complet
- Gestionnaire de ressources mémoire

Objectifs mesurables :
[x] Fenêtre 800x600 stable 60 FPS
[x] Input clavier/souris responsive
[x] États de jeu fonctionnels
[x] Allocation mémoire optimisée
Semaine 3-4 : Rendu et Pixel Art
// Systèmes graphiques
- Renderer 2D avec batching
- Générateur de sprites procéduraux
- Système d'animations
- Gestion palettes de couleurs
- Camera 2D avec smooth follow

Livrables :
[x] Sprites 16x16 générés en temps réel
[x] Animations walk/idle fluides
[x] Palette procédurale par biome
[x] Caméra centrée sur joueur
Semaine 5-6 : Audio Procédural
// Système sonore
- Générateur de sons basique
- Mixage 3D positionnel
- Musique adaptative simple
- Sons d'impact matériaux

Phase 2 : Monde et Génération (Semaines 7-12)
Semaine 7-8 : Génération Terrain
class WorldGenMilestones {
    // Objectifs Phase 2A
    void generateBasicTerrain() {
        - Perlin noise multicouches
        - 3 biomes fonctionnels (Plains, Forest, Underground)
        - Chunks 32x32 avec streaming
        - Placement ressources basique
    }
};

Semaine 9-10 : Systèmes de Tiles
// Mining et interaction monde
- Système de tiles destructibles
- Mining basique avec outils
- Drops et collecte ressources
- Inventaire 40 slots
- Craft station établi

Semaine 11-12 : Structures et Donjons
// Génération contenu
- Algorithme génération donjons
- 2 types structures (temple, mine)
- Placement automatique dans monde
- Système de salles connectées

Phase 3 : Systèmes de Gameplay (Semaines 13-18)
Semaine 13-14 : Combat et Entités
class CombatMilestones {
    void implementCombatCore() {
        - Joueur avec stats (HP, ATK, DEF)
        - 5 types d'ennemis + IA basique
        - Système de dégâts et résistances
        - Combat mêlée fonctionnel
        - Drops équipement
    }
};

Semaine 15-16 : Magie et Compétences
// Systèmes avancés
- 8 sorts dans 3 écoles magie
- Système mana et cooldowns
- Arbre compétences Mining/Combat
- Système d'expérience
- Level up avec bonus stats

Semaine 17-18 : Base Building
// Construction et automatisation
- 15 structures de base
- Système de placement
- Générateur d'énergie simple
- Convoyeurs et stockage
- Interface construction

Phase 4 : Contenu et Polish (Semaines 19-24)
Semaine 19-20 : Contenu Gameplay
// Remplissage contenu
- 5 biomes complets
- 3 boss avec mécaniques uniques
- 50+ recettes craft
- 20+ types de ressources
- Système de trading PNJ

Semaine 21-22 : Interface et UX
// UI/UX complet
- Interface craft intuitive
- Arbre compétences visuel
- Système de quêtes
- Journal du joueur
- Menus settings complets

Semaine 23-24 : Optimisation et Bug Fixes
// Finalisation
- Profiling performance
- Correction bugs critiques
- Balance gameplay
- Sauvegarde/chargement stable
- Tests utilisateur

9.2 Métriques de Qualité
Performance Targets
struct PerformanceMetrics {
    int targetFPS = 60;
    float maxFrameTime = 16.67f; // ms
    size_t maxRAMUsage = 512 * 1024 * 1024; // 512MB
    size_t maxVRAMUsage = 256 * 1024 * 1024; // 256MB
    
    // Temps de chargement
    float worldGenTime = 3.0f;  // secondes
    float chunkLoadTime = 0.1f; // secondes
    float saveTime = 2.0f;      // secondes
};

Contenu Quantifié v1.0
struct ContentMetrics {
    int biomeCount = 5;
    int enemyTypes = 30;
    int craftRecipes = 150;
    int resourceTypes = 25;
    int spellCount = 16;
    int questCount = 45;
    int achievementCount = 60;
    int structureTypes = 60;
    
    // Durée de vie
    float mainStoryHours = 15;
    float sideContentHours = 25;
    float endgameHours = 20;
    float totalContentHours = 60;
};


10. COMPILATION ET DISTRIBUTION
10.1 Environnement de Développement
Setup Windows Development
@echo off
echo Configuration Pixel Depths Development Environment

:: Installation vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

:: Installation dépendances
vcpkg install sdl2:x64-windows
vcpkg install sdl2-image:x64-windows  
vcpkg install sdl2-mixer:x64-windows
vcpkg install nlohmann-json:x64-windows
vcpkg install fastnoise2:x64-windows

echo Environnement configuré avec succès!

Structure CMake Complète
cmake_minimum_required(VERSION 3.20)
project(PixelDepths VERSION 1.0.0 LANGUAGES CXX)

# Configuration C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration Release optimisée
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Flags de compilation optimisée
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /GL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")
endif()

# Recherche des dépendances
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(nlohmann_json REQUIRED)

# Définition des sources
set(CORE_SOURCES
    src/core/Engine.cpp
    src/core/Renderer.cpp
    src/core/InputManager.cpp
    src/core/AudioEngine.cpp
    src/core/StateManager.cpp
    src/core/ResourceManager.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/PixelArtGenerator.cpp
    src/graphics/ParticleSystem.cpp
    src/graphics/LightingEngine.cpp
    src/graphics/ShaderManager.cpp
    src/graphics/AnimationSystem.cpp
)

set(WORLD_SOURCES
    src/world/WorldGenerator.cpp
    src/world/BiomeManager.cpp
    src/world/ChunkManager.cpp
    src/world/StructureGenerator.cpp
    src/world/WeatherSystem.cpp
)

set(ENTITIES_SOURCES
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/NPC.cpp
    src/entities/Projectile.cpp
    src/entities/EntityManager.cpp
)

set(SYSTEMS_SOURCES
    src/systems/MiningSystem.cpp
    src/systems/CraftingSystem.cpp
    src/systems/CombatSystem.cpp
    src/systems/MagicSystem.cpp
    src/systems/BuildingSystem.cpp
    src/systems/QuestSystem.cpp
    src/systems/InventorySystem.cpp
)

set(AUDIO_SOURCES
    src/audio/SoundGenerator.cpp
    src/audio/MusicComposer.cpp
    src/audio/AudioMixer.cpp
    src/audio/SoundEffects.cpp
)

set(UI_SOURCES
    src/ui/UIManager.cpp
    src/ui/MenuSystem.cpp
    src/ui/HUD.cpp
    src/ui/InventoryUI.cpp
    src/ui/DialogSystem.cpp
)

# Création de l'exécutable
add_executable(PixelDepths
    src/main.cpp
    ${CORE_SOURCES}
    ${GRAPHICS_SOURCES}
    ${WORLD_SOURCES}
    ${ENTITIES_SOURCES}
    ${SYSTEMS_SOURCES}
    ${AUDIO_SOURCES}
    ${UI_SOURCES}
)

# Link des bibliothèques
target_link_libraries(PixelDepths
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(PixelDepths PRIVATE
    src/
    src/core/
    src/graphics/
    src/world/
    src/entities/
    src/systems/
    src/audio/
    src/ui/
)

# Compilation flags spécifiques
if(WIN32)
    target_compile_definitions(PixelDepths PRIVATE PLATFORM_WINDOWS)
    set_target_properties(PixelDepths PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Configuration Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(PixelDepths PRIVATE DEBUG_BUILD)
endif()

10.2 Scripts de Build Automatisés
Build Script Windows
@echo off
setlocal EnableDelayedExpansion

echo ========================================
echo    PIXEL DEPTHS BUILD SYSTEM v1.0
echo ========================================

:: Configuration
set BUILD_TYPE=Release
set PLATFORM=x64
set OUTPUT_DIR=dist

:: Nettoyage
if exist build rmdir /s /q build
if exist %OUTPUT_DIR% rmdir /s /q %OUTPUT_DIR%

mkdir build
mkdir %OUTPUT_DIR%

echo [1/4] Configuration CMake...
cd build
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

if %ERRORLEVEL% neq 0 (
    echo ERREUR: Configuration CMake échouée
    exit /b 1
)

echo [2/4] Compilation...
cmake --build . --config %BUILD_TYPE% --parallel 8

if %ERRORLEVEL% neq 0 (
    echo ERREUR: Compilation échouée
    exit /b 1
)

echo [3/4] Copie des fichiers...
cd ..

:: Copie exécutable
copy build\%BUILD_TYPE%\PixelDepths.exe %OUTPUT_DIR%\

:: Copie DLLs SDL2
copy vcpkg\installed\x64-windows\bin\SDL2.dll %OUTPUT_DIR%\
copy vcpkg\installed\x64-windows\bin\SDL2_image.dll %OUTPUT_DIR%\
copy vcpkg\installed\x64-windows\bin\SDL2_mixer.dll %OUTPUT_DIR%\

:: Copie dossier data
xcopy /E /I data %OUTPUT_DIR%\data

:: Création dossier saves
mkdir %OUTPUT_DIR%\saves

echo [4/4] Création du package...
:: Compression avec 7-Zip si disponible
where 7z >nul 2>nul
if %ERRORLEVEL% equ 0 (
    7z a -tzip PixelDepths_v1.0_Windows.zip %OUTPUT_DIR%\*
    echo Package créé: PixelDepths_v1.0_Windows.zip
) else (
    echo Package prêt dans le dossier: %OUTPUT_DIR%\
)

echo.
echo ========================================
echo         BUILD TERMINÉ AVEC SUCCÈS
echo ========================================
echo Taille exécutable: 
for %%I in (%OUTPUT_DIR%\PixelDepths.exe) do echo   %%~zI bytes
echo.

Configuration Fichiers de Données
// data/gameConfig.json
{
    "version": "1.0.0",
    "window": {
        "title": "Pixel Depths",
        "width": 1024,
        "height": 768,
        "fullscreen": false,
        "vsync": true
    },
    "graphics": {
        "pixelPerfect": true,
        "targetFPS": 60,
        "particleLimit": 1000,
        "lightingQuality": "high"
    },
    "audio": {
        "masterVolume": 0.8,
        "musicVolume": 0.6,
        "sfxVolume": 0.8,
        "sampleRate": 44100,
        "bufferSize": 1024
    },
    "gameplay": {
        "autosaveInterval": 120,
        "maxSaveSlots": 10,
        "difficulty": "normal"
    }
}

10.3 Système de Distribution
Structure Package Final
PixelDepths_v1.0/
├── PixelDepths.exe          # Exécutable principal (8-12 MB)
├── SDL2.dll                 # SDL2 runtime (1.2 MB)
├── SDL2_image.dll          # Support images (0.8 MB) 
├── SDL2_mixer.dll          # Support audio (0.6 MB)
├── README.txt              # Instructions installation
├── LICENSE.txt             # Licence du jeu
├── data/                   # Fichiers de configuration
│   ├── gameConfig.json     # Configuration générale
│   ├── biomes.json         # Définitions biomes
│   ├── recipes.json        # Recettes craft
│   ├── enemies.json        # Stats ennemis
│   ├── spells.json         # Configuration sorts
│   └── quests.json         # Données quêtes
└── saves/                  # Dossier sauvegardes (vide)

Optimisations Release
// Flags de compilation optimisée
#ifdef RELEASE_BUILD
    #define ENABLE_PROFILING 0
    #define ENABLE_DEBUG_DRAW 0
    #define ENABLE_CONSOLE 0
    #define OPTIMIZE_GRAPHICS 1
    #define COMPRESS_SAVES 1
#endif

// Optimisations spécifiques
class ReleaseOptimizations {
public:
    static void configureForRelease() {
        // Désactivation assertions debug
        #ifndef DEBUG_BUILD
            #define assert(x) ((void)0)
        #endif
        
        // Pool de mémoire pré-allouée
        EntityPool::preallocate(1000);
        ParticlePool::preallocate(5000);
        
        // Optimisation rendering
        Renderer::enableBatching(true);
        Renderer::enableFrustumCulling(true);
        
        // Cache des assets générés
        AssetCache::enable(true);
        AssetCache::setMaxSize(100 * 1024 * 1024); // 100MB
    }
};























Installer Optionnel (NSIS)
; Script NSIS pour installer Pixel Depths
!define APPNAME "Pixel Depths"
!define VERSION "1.0.0"
!define PUBLISHER "Votre Studio"

Name "${APPNAME}"
OutFile "PixelDepths_Installer_v1.0.exe"
InstallDir "$PROGRAMFILES\${APPNAME}"

Page directory
Page instfiles

Section "Main Application"
    SetOutPath $INSTDIR
    
    ; Fichiers principaux
    File "PixelDepths.exe"
    File "SDL2.dll"
    File "SDL2_image.dll"
    File "SDL2_mixer.dll"
    File "README.txt"
    File "LICENSE.txt"
    
    ; Dossiers
    SetOutPath "$INSTDIR\data"
    File /r "data\*"
    
    CreateDirectory "$INSTDIR\saves"
    
    ; Raccourcis
    CreateDirectory "$SMPROGRAMS\${APPNAME}"
    CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\PixelDepths.exe"
    CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\PixelDepths.exe"
    
    ; Uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd




11. SPÉCIFICATIONS TECHNIQUES
11.1 Exigences Système
Configuration Minimale
struct MinimumRequirements {
    // OS
    std::string os = "Windows 10 64-bit";
    
    // Processeur
    std::string cpu = "Intel Core i3-4000 / AMD FX-6000 series";
    int cpuCores = 2;
    float cpuFrequency = 2.5f; // GHz
    
    // Mémoire
    size_t ram = 4 * 1024 * 1024 * 1024; // 4 GB
    size_t vram = 512 * 1024 * 1024;     // 512 MB
    
    // Stockage
    size_t diskSpace = 200 * 1024 * 1024; // 200 MB
    
    // Graphiques
    std::string gpu = "DirectX 11 compatible";
    std::string shaderModel = "3.0";
};





















Configuration Recommandée
struct RecommendedRequirements {
    // OS
    std::string os = "Windows 11 64-bit";
    
    // Processeur
    std::string cpu = "Intel Core i5-8000 / AMD Ryzen 5 2000 series";
    int cpuCores = 4;
    float cpuFrequency = 3.0f; // GHz
    
    // Mémoire
    size_t ram = 8 * 1024 * 1024 * 1024;  // 8 GB
    size_t vram = 1024 * 1024 * 1024;     // 1 GB
    
    // Stockage
    size_t diskSpace = 500 * 1024 * 1024; // 500 MB
    std::string diskType = "SSD";
    
    // Graphiques
    std::string gpu = "GTX 1050 / RX 560 ou équivalent";
};

























11.2 Métriques de Performance
Benchmarks Cibles
class PerformanceBenchmarks {
public:
    struct FrameTimeStats {
        float averageFPS = 60.0f;
        float minFPS = 45.0f;        // 1% low
        float maxFrameTime = 22.0f;  // ms, 99th percentile
        float targetFrameTime = 16.67f; // ms @ 60 FPS
    };
    
    struct MemoryUsage {
        size_t baseRAM = 256 * 1024 * 1024;     // 256 MB base
        size_t peakRAM = 512 * 1024 * 1024;     // 512 MB peak
        size_t vramUsage = 128 * 1024 * 1024;   // 128 MB VRAM
        
        // Allocations par frame
        size_t maxAllocPerFrame = 1024 * 1024;  // 1 MB max
        int allocationsPerSecond = 100;         // Max 100/sec
    };
    
    struct LoadingTimes {
        float gameStartup = 3.0f;      // secondes
        float worldGeneration = 2.0f;  // secondes
        float chunkLoading = 0.05f;    // secondes
        float saveGame = 1.0f;         // secondes
        float loadGame = 1.5f;         // secondes
    };
    
    void runBenchmarks() {
        // Test génération monde
        auto start = std::chrono::high_resolution_clock::now();
        generateTestWorld(100, 100);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        assert(duration.count() < 2000); // Moins de 2 secondes
        
        // Test rendering 1000 sprites
        start = std::chrono::high_resolution_clock::now();
        renderTestSprites(1000);
        end = std::chrono::high_resolution_clock::now();
        
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        assert(duration.count() < 16670); // Moins de 16.67ms
        
        // Test génération audio
        start = std::chrono::high_resolution_clock::now();
        generateTestSounds(50);
        end = std::chrono::high_resolution_clock::now();
        
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        assert(duration.count() < 100); // Moins de 100ms
    }
};














11.3 Architecture de Sauvegarde
Format de Sauvegarde Optimisé
class SaveSystem {
private:
    struct SaveHeader {
        uint32_t magic = 0x50495845; // "PIXE"
        uint32_t version = 1;
        uint64_t timestamp;
        uint32_t compressedSize;
        uint32_t uncompressedSize;
        uint32_t checksum;
    };
    
    struct WorldSave {
        uint32_t worldSeed;
        uint32_t chunkCount;
        std::vector<ChunkData> modifiedChunks; // Seulement chunks modifiés
        std::vector<StructureData> structures;
        WeatherState weather;
    };
    
    struct PlayerSave {
        Vector2 position;
        PlayerStats stats;
        InventoryData inventory;
        SkillTreeData skills;
        QuestProgress quests;
        std::vector<Achievement> unlockedAchievements;
    };

public:
    bool saveGame(const std::string& filename, int slot) {
        SaveData saveData;
        saveData.header.timestamp = getCurrentTimestamp();
        
        // Collecte données
        saveData.world = serializeWorld();
        saveData.player = serializePlayer();
        saveData.gameState = serializeGameState();
        
        // Sérialisation JSON
        nlohmann::json jsonData;
        jsonData["world"] = saveData.world;
        jsonData["player"] = saveData.player;
        jsonData["gameState"] = saveData.gameState;
        
        std::string jsonString = jsonData.dump();
        
        // Compression GZIP
        std::vector<uint8_t> compressed = compressGZIP(jsonString);
        
        // Calcul checksum
        saveData.header.compressedSize = compressed.size();
        saveData.header.uncompressedSize = jsonString.size();
        saveData.header.checksum = calculateCRC32(compressed);
        
        // Écriture fichier
        std::ofstream file(filename, std::ios::binary);
        if(!file.is_open()) return false;
        
        file.write(reinterpret_cast<const char*>(&saveData.header), sizeof(SaveHeader));
        file.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
        
        return true;
    }
    
    bool loadGame(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if(!file.is_open()) return false;
        
        // Lecture header
        SaveHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(SaveHeader));
        
        // Validation
        if(header.magic != 0x50495845) return false;
        if(header.version > 1) return false;
        
        // Lecture données compressées
        std::vector<uint8_t> compressed(header.compressedSize);
        file.read(reinterpret_cast<char*>(compressed.data()), header.compressedSize);
        
        // Validation checksum
        if(calculateCRC32(compressed) != header.checksum) return false;
        
        // Décompression
        std::string jsonString = decompressGZIP(compressed);
        if(jsonString.size() != header.uncompressedSize) return false;
        
        // Parsing JSON
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);
        
        // Restauration état
        deserializeWorld(jsonData["world"]);
        deserializePlayer(jsonData["player"]);
        deserializeGameState(jsonData["gameState"]);
        
        return true;
    }
    
private:
    std::vector<uint8_t> compressGZIP(const std::string& data);
    std::string decompressGZIP(const std::vector<uint8_t>& compressed);
    uint32_t calculateCRC32(const std::vector<uint8_t>& data);
};


11.4 Estimation Finale des Ressources
Taille du Jeu Compilé
struct GameSize {
    // Exécutable
    size_t executable = 12 * 1024 * 1024;        // 12 MB
    
    // DLLs
    size_t sdl2_dll = 1200 * 1024;               // 1.2 MB
    size_t sdl2_image_dll = 800 * 1024;          // 0.8 MB
    size_t sdl2_mixer_dll = 600 * 1024;          // 0.6 MB
    
    // Données
    size_t configFiles = 500 * 1024;             // 0.5 MB
    
    // Total installation
    size_t totalSize = executable + sdl2_dll + sdl2_image_dll + 
                      sdl2_mixer_dll + configFiles;
    // ≈ 15 MB total
    
    // Estimation sauvegardes
    size_t saveFileSize = 2 * 1024 * 1024;       // 2 MB par save
    size_t maxSaves = 10;
    size_t maxSaveSpace = saveFileSize * maxSaves; // 20 MB max
};

Contenu Généré Procéduralement (Estimations)
struct ProceduralContent {
    // Sprites générés à la volée
    int totalSprites = 10000;              // Types possibles
    int spritesInMemory = 500;             // Simultanément
    size_t spriteMemory = 2 * 1024 * 1024; // 2 MB
    
    // Sons générés
    int totalSounds = 5000;                // Variations possibles
    int soundsInMemory = 100;              // Cache audio
    size_t soundMemory = 10 * 1024 * 1024; // 10 MB
    
    // Musique adaptative
    int musicTracks = 50;                  // Combinaisons
    size_t musicMemory = 5 * 1024 * 1024;  // 5 MB streaming
    
    // Monde généré
    size_t worldSize = 6400 * 4800;        // Tiles
    size_t chunksInMemory = 9;             // 3x3 autour joueur
    size_t worldMemory = 50 * 1024 * 1024; // 50 MB max
};


CONCLUSION ET RÉSUMÉ EXÉCUTIF
Vue d'Ensemble du Projet
PIXEL DEPTHS représente un projet ambitieux de jeu de mining/action RPG entièrement généré par code. Cette approche unique permet une liberté créative totale et une optimisation poussée, tout en garantissant une expérience de jeu riche et rejouable.
Points Forts de l'Approche 100% Code
Contrôle Total : Chaque pixel, chaque son, chaque animation est généré avec précision
Optimisation Maximale : Pas de gaspillage de ressources avec des assets inutilisés
Contenu Infini : La génération procédurale permet une variété illimitée
Taille Minimale : Jeu complet en moins de 20 MB
Performance : Optimisé spécifiquement pour les besoins du jeu
Défis Techniques Relevés
Génération Pixel Art : Sprites de qualité professionnelle en temps réel
Audio Procédural : Sons et musiques adaptatives générés dynamiquement
Monde Infini : Terrain procédural cohérent avec streaming fluide
IA Avancée : Comportements d'ennemis complexes sans scripts externes
Interface Élégante : UI moderne entièrement codée
Livrable Final v1.0
Un jeu PC Windows complet de 30-60h de contenu, exécutable standalone de ~15MB avec :
Monde procédural de 6400x4800 tiles
5 biomes uniques avec écosystèmes
30+ types d'ennemis avec IA comportementale
150+ recettes de craft dans 8 stations
16 sorts magiques dans 4 écoles
Système de base building avec 60+ structures
Progression par compétences sur 4 arbres
45 quêtes incluant storyline principale
60 achievements et découvertes

