#include "models.h"

#ifndef ENGINE
#define ENGINE

void printGameState(GameState gameState, PlayerState playerState);
void populateBasePets(PetBase basePets[9]);
int randomItemBaseIndForTier(GameState gameState, int tier);
ItemBase randomItemBase(GameState gameState);
int randomPetBaseIndForTier(GameState gameState, int tier);
PetBase randomPetBase(GameState gameState);
void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, phase p);
OperationResult buyPet(GameState gameState, PlayerState *playerState, int buySlot, int buildSlot);
void fillShop(GameState gameState, PlayerState *playerState);
void fillShopWithItem(GameState gameState, PlayerState *playerState, int baseItemIndex);
void clearShopSlots(GameState gameState, PlayerState *playerState);
void sellPet(GameState gameState, PlayerState *playerState, int sellSlot);
OperationResult buyItem(GameState gameState, PlayerState *playerState, int buySlot, int targetSlot);

#endif // ENGINE
