#include <stdbool.h>

/* Structs */

struct PetBase
{
  char *name;
  char *unicodeCodePoint;
  int tier;
  int debugCount;
  int spawnChanceRound1;
  int attack;
  int health;
};
typedef struct PetBase PetBase;

struct PetBuilt
{
  int attack;
  int health;
  PetBase base;
};
typedef struct PetBuilt PetBuilt;

struct BoardSlot
{
  int position;
  PetBuilt pet;
  bool isEmpty;
};
typedef struct BoardSlot BoardSlot;

struct ShopPetSlot
{
  int position;
  struct PetBase pet;
  bool isEmpty;
};
typedef struct ShopPetSlot ShopPetSlot;

struct PlayerState
{
  int health;
  int gold;
};
typedef struct PlayerState PlayerState;

enum phase
{
  Shop,
  Battle
};
typedef enum phase phase;

struct GameState
{
  int tier;
  int shopSlotCount;
  phase currentPhase;
};
typedef struct GameState GameState;

void printPetBase(PetBase pet);
void printPetBuilt(PetBuilt pet);
void printBasePets(void);
void printShopSlots(void);
void printBoardSlots(void);

void populateBasePets(void);
int randomPetBaseIndForTier(int tier);
PetBase randomPetBase(int tier);
void testRandomPets(void);
void testBoardSlots(void);
void testShopSlots(void);
