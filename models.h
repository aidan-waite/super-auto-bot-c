#ifndef MODELS
#define MODELS

#include <stdbool.h>

struct TestResult
{
  char errorMessage[200];
  bool succeeded;
};
typedef struct TestResult TestResult;

enum BattleWinner
{
  BattleWinnerPlayer1, // Player 1's pet lived and player 2's bet fainted
  BattleWinnerPlayer2, // Player 2's pet lived and player 1's bet fainted
  BattleWinnerTie, // Both pets lived
  BattleWinnerError // Something went wrong: check the errorMessage for details
};
typedef enum BattleWinner BattleWinner;

struct BattleResult
{
  BattleWinner winner;
  bool succeeded;
  char errorMessage[200];
};
typedef struct BattleResult BattleResult;

struct OperationResult
{
  char errorMessage[200];
  bool succeeded;
};
typedef struct OperationResult OperationResult;

struct ItemBase
{
  char *name;
  char *unicodeCodePoint;
};
typedef struct ItemBase ItemBase;

struct PetBase
{
  char *name;
  char *unicodeCodePoint;
  int tier;
  int debugCount;
  int attack;
  int health;
};
typedef struct PetBase PetBase;

struct PetBattleState
{
  int attack;
  int health;
  bool fainted;
};
typedef struct PetBattleState PetBattleState;

struct PetBuilt
{
  int attack;
  int health;
  PetBase base;
  PetBattleState battleState;
  char *equippedItem;
};
typedef struct PetBuilt PetBuilt;

struct BoardSlot
{
  int position;
  PetBuilt pet;
  bool isEmpty;
};
typedef struct BoardSlot BoardSlot;

struct ShopItemSlot
{
  int position;
  struct ItemBase item;
  bool isEmpty;
};
typedef struct ShopItemSlot ShopItemSlot;

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
  BoardSlot boardSlots[5];
  ShopPetSlot shopSlots[5];
  ShopItemSlot itemSlots[2];
};
typedef struct PlayerState PlayerState;

enum Phase
{
  PhaseShop,
  PhaseBattle
};
typedef enum Phase Phase;

struct GameState
{
  int tier;
  int shopPetSlotCount;
  int shopItemSlotCount;
  Phase currentPhase;
  PetBase basePets[20];
  ItemBase baseItems[2];
};
typedef struct GameState GameState;

#endif // MODELS
