#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.h"

int main()
{
  PetBase basePets[9];
  BoardSlot boardSlots[5];
  ShopPetSlot shopSlots[5];
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 10, 1, Shop);

  printGameState(gameState, playerState);

  fillShop(gameState, &playerState);

  printGameState(gameState, playerState);

  return 0;
}
