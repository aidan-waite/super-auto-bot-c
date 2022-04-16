#include <stdbool.h>

#ifndef MODELS
#define MODELS

struct TestResult
{
  char errorMessage[100];
  bool didSucceed;
};
typedef struct TestResult TestResult;

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
  // bool isEnabled;
};
typedef struct ShopPetSlot ShopPetSlot;

struct PlayerState
{
  int health;
  int gold;
  BoardSlot boardSlots[5];
  ShopPetSlot shopSlots[5];
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
  PetBase basePets[20];
};
typedef struct GameState GameState;

#endif