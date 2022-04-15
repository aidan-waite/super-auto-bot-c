#include "models.h"

void printGameState(GameState gameState, PlayerState playerState);
void populateBasePets(PetBase basePets[9]);
int randomPetBaseIndForTier(GameState gameState, int tier);
PetBase randomPetBase(GameState gameState);
void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, phase p);
void buyPet(GameState gameState, PlayerState *playerState, int buySlot, int buildSlot);
void fillShop(GameState gameState, PlayerState *playerState);