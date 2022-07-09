#ifndef ENGINE
#define ENGINE

#include "models.h"

PetBase antBase();
PetBase beaverBase();
PetBase cricketBase();
PetBase duckBase();
PetBase fishBase();
PetBase horseBase();
PetBase mosquitoBase();
PetBase otterBase();
PetBase pigBase();
PetBase beeBase();

ItemBase appleBase();
ItemBase honeyBase();

void printGameState(GameState gameState, PlayerState playerState);
void populateBasePets(PetBase basePets[9]);
int randomItemBaseIndForTier(GameState gameState, int tier);
ItemBase randomItemBase(GameState gameState);
int randomPetBaseIndForTier(GameState gameState, int tier);
PetBase randomPetBase(GameState gameState);
void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, Phase p);
OperationResult buyPet(GameState gameState, PlayerState *playerState, int buySlot, int buildSlot);
void fillShop(GameState gameState, PlayerState *playerState);
void fillShopWithItem(GameState gameState, PlayerState *playerState, int baseItemIndex);
void clearShopSlots(GameState gameState, PlayerState *playerState);
void sellPet(GameState gameState, PlayerState *playerState, int sellSlot);
OperationResult buyItem(GameState gameState, PlayerState *playerState, int buySlot, int targetSlot);
void setPet(PlayerState *playerState, PetBase base, int buildSlot);
void setupBattleState(PlayerState *playerState);
OperationResult doBattleRound(PlayerState *playerState1, PlayerState *playerState2);
BattlePhaseResult doBattlePhase(PlayerState *playerState1, PlayerState *playerState2);
bool haveAllPetsFainted(PlayerState *playerState);
void petDidFaint(PlayerState *playerState, int slotIndex);
void summon(PlayerState *playerState, PetBase petBase, int slotInd);
void handleCricketTrigger(PlayerState *playerState, int slotInd);
void handleAntTrigger(PlayerState *playerState, int slotInd);
void setupBattleSlot(PlayerState *playerState, int i);
void printPetBase(PetBase base);

#endif // ENGINE
