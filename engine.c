#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "models.h"

/* Printing */

void printPetBuilt(PetBuilt pet)
{
  printf("%s %s tier:%d debugCount:%d attack:%d health:%d\n",
         pet.base.unicodeCodePoint,
         pet.base.name,
         pet.base.tier,
         pet.base.debugCount,
         pet.attack,
         pet.health);
}

void printPetBase(PetBase base)
{
  printf("%s %s tier:%d debugCount:%d attack:%d health:%d\n",
         base.unicodeCodePoint,
         base.name,
         base.tier,
         base.debugCount,
         base.attack,
         base.health);
}

void printGameState(GameState gameState, PlayerState playerState)
{
  char *phaseName = gameState.currentPhase == Shop ? "Shop" : "Battle";

  printf("\n*** GAME STATE ***\nCurrent tier:%d\nSlot count:%d\nPhase:%s\nPlayer health:%d gold:%d\n",
         gameState.tier,
         gameState.shopSlotCount,
         phaseName,
         playerState.health,
         playerState.gold);

  for (int a = 0; a < 5; a++)
  {
    printf("Board slot #%d ", a);
    if (playerState.boardSlots[a].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printPetBuilt(playerState.boardSlots[a].pet);
    }
  }

  for (int b = 0; b < 5; b++)
  {
    printf("Shop slot #%d ", b);
    if (playerState.shopSlots[b].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printPetBase(playerState.shopSlots[b].pet);
    }
  }

  printf("*** / / / ***\n\n");
}

/* Randoming */

int randomPetBaseIndForTier(GameState gameState, int tier)
{
  // TODO: switch on tier

  // 1. Sum all probabilities
  int sum = 0;
  int x = 0;

  for (x = 0; x < 9; x++)
  {
    sum += gameState.basePets[x].spawnChanceRound1;
  }

  // 2. Random between 0 and sum
  int r = rand() % sum;
  int tmp = 0;

  // 3. Walk through each item, adding chance until you're at > randomnum
  for (x = 0; x < 9; x++)
  {
    tmp += gameState.basePets[x].spawnChanceRound1;
    if (tmp > r)
    {
      return x;
    }
  }

  // default to last index
  return 8;
}

PetBase randomPetBase(GameState gameState)
{
  int ind = randomPetBaseIndForTier(gameState, 1);
  PetBase b = {
      gameState.basePets[ind].name,
      gameState.basePets[ind].unicodeCodePoint,
      gameState.basePets[ind].tier,
      gameState.basePets[ind].debugCount,
      gameState.basePets[ind].spawnChanceRound1,
      gameState.basePets[ind].attack,
      gameState.basePets[ind].health};
  return b;
}

void fillShop(GameState gameState, PlayerState *playerState)
{
  for (int x = 0; x < gameState.shopSlotCount; x++)
  {
    PetBase p = randomPetBase(gameState);
    ShopPetSlot s = {x, p, false};
    playerState->shopSlots[x] = s;
  }
}

void populateBasePets(PetBase basePets[9])
{
  int i = 0;

  basePets[i].name = "ant";
  basePets[i].unicodeCodePoint = "🐜";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 1;

  basePets[i].name = "beaver";
  basePets[i].unicodeCodePoint = "🦫";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 2;

  basePets[i].name = "cricket";
  basePets[i].unicodeCodePoint = "🦗";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 2;

  basePets[i].name = "duck";
  basePets[i].unicodeCodePoint = "🦆";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 3;

  basePets[i].name = "fish";
  basePets[i].unicodeCodePoint = "🐟";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 3;

  basePets[i].name = "horse";
  basePets[i].unicodeCodePoint = "🐎";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 1;

  basePets[i].name = "mosquito";
  basePets[i].unicodeCodePoint = "🦟";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 2;

  basePets[i].name = "otter";
  basePets[i].unicodeCodePoint = "🦦";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 2;

  basePets[i].name = "pig";
  basePets[i].unicodeCodePoint = "🐖";
  basePets[i].tier = 1;
  basePets[i].debugCount = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 3;
  basePets[i++].health = 1;
}

void clearBoardSlots(PlayerState *playerState)
{
  for (int x = 0; x < 5; x++)
  {
    playerState->boardSlots[x].isEmpty = true;
    playerState->boardSlots[x].position = x;
  }
}

void clearShopSlots(GameState gameState, PlayerState *playerState)
{
  for (int x = 0; x < 5; x++)
  {
    playerState->shopSlots[x].isEmpty = true;
    playerState->shopSlots[x].position = x;
  }
}

void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, phase p)
{
  srand(time(0));
  rand();
  populateBasePets(gameState->basePets);
  playerState->health = health;
  playerState->gold = gold;
  gameState->tier = tier;
  gameState->shopSlotCount = 3;
  gameState->currentPhase = p;
  clearBoardSlots(playerState);
  clearShopSlots(*gameState, playerState);
}

void nextStep(GameState gameState, PlayerState playerState)
{
  printf("Next Step\n");
  switch (gameState.currentPhase)
  {
  case Shop:
    fillShop(gameState, &playerState);
    playerState.gold = 10;
    break;
  case Battle:
    printf("prepare battle");
    break;
  }
}

void buyPet(GameState gameState, PlayerState *playerState, int buySlot, int buildSlot)
{
  if (playerState->gold < 3)
  {
    printf("Not enough gold to buy pet\n");
    return;
  }

  if (playerState->shopSlots[buySlot].isEmpty)
  {
    printf("Shop slot %d is empty\n", buySlot);
    return;
  }

  if (!playerState->boardSlots[buildSlot].isEmpty)
  {
    printf("Build slot %d is not empty\n", buySlot);
    return;
  }

  printf("ACTION - Buy pet: ");
  printPetBase(playerState->shopSlots[buySlot].pet);

  playerState->boardSlots[buildSlot].pet.base = playerState->shopSlots[buySlot].pet;
  playerState->boardSlots[buildSlot].pet.attack = playerState->shopSlots[buySlot].pet.attack;
  playerState->boardSlots[buildSlot].pet.health = playerState->shopSlots[buySlot].pet.health;
  playerState->boardSlots[buildSlot].isEmpty = false;
  playerState->boardSlots[buildSlot].position = buildSlot;

  playerState->shopSlots[buySlot].isEmpty = true;
  playerState->gold -= 3;
}