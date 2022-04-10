#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"

/* Game State */

PetBase basePets[9];
BoardSlot boardSlots[5];
ShopPetSlot shopSlots[3];
PlayerState playerState;
GameState gameState;

/* Printing */

void printPetBase(PetBase pet)
{
  printf("%s %s tier:%d debugCount:%d attack:%d health:%d\n",
         pet.unicodeCodePoint,
         pet.name,
         pet.tier,
         pet.debugCount,
         pet.attack,
         pet.health);
}

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

void printBasePets(void)
{
  for (int x = 0; x < 9; x++)
  {
    printPetBase(basePets[x]);
  }
}

void printBoardSlots(void)
{
  for (int x = 0; x < 5; x++)
  {
    printf("Board slot #%d ", x);
    if (boardSlots[x].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printPetBuilt(boardSlots[x].pet);
    }
  }
}

void printShopSlots(void)
{
  for (int x = 0; x < 3; x++)
  {
    printf("Shop slot #%d ", x);
    if (shopSlots[x].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printPetBase(shopSlots[x].pet);
    }
  }
}

void printGameState(void)
{
  char *phaseName = gameState.currentPhase == Shop ? "Shop" : "Battle";

  printf("\n*** GAME STATE ***\nCurrent tier:%d\nSlot count:%d\nPhase:%s\nPlayer health:%d gold:%d\n",
         gameState.tier,
         gameState.shopSlotCount,
         phaseName,
         playerState.health,
         playerState.gold);
  printBoardSlots();
  printShopSlots();
  printf("*** / / / ***\n\n");
}

/* Randoming */

int randomPetBaseIndForTier(int tier)
{
  // TODO: switch on tier

  // 1. Sum all probabilities
  int sum = 0;
  int x = 0;

  for (x = 0; x < 9; x++)
  {
    sum += basePets[x].spawnChanceRound1;
  }

  // 2. Random between 0 and sum
  int r = rand() % sum;
  int tmp = 0;
  int ind = 0;

  // 3. Walk through each item, adding chance until you're at > randomnum
  for (x = 0; x < 9; x++)
  {
    tmp += basePets[x].spawnChanceRound1;
    if (tmp > r)
    {
      return x;
    }
  }

  // default to last index
  return 8;
}

PetBase randomPetBase(int tier)
{
  int ind = randomPetBaseIndForTier(tier);
  PetBase b = {
      basePets[ind].name,
      basePets[ind].unicodeCodePoint,
      basePets[ind].tier,
      basePets[ind].debugCount,
      basePets[ind].spawnChanceRound1,
      basePets[ind].attack,
      basePets[ind].health};
  return b;
}

/* Testing */

void testRandomPets(void)
{
  for (int a = 0; a < 25000; a++)
  {
    basePets[randomPetBaseIndForTier(1)].debugCount += 1;
  }

  printBasePets();
}

void testBoardSlots(void)
{
  for (int x = 0; x < 5; x++)
  {
    PetBase base = randomPetBase(1);
    int bonusAttack = rand() % 5;
    int bonusHealth = rand() % 5;
    PetBuilt built = {base.attack + bonusAttack, base.health + bonusHealth, base};
    boardSlots[x].position = x;
    boardSlots[x].pet = built;
  }

  printBoardSlots();
}

void fillShop(void)
{
  for (int x = 0; x < gameState.shopSlotCount; x++)
  {
    PetBase p = randomPetBase(1);
    ShopPetSlot s = {x, p, false};
    shopSlots[x] = s;
  }
}

void testShopSlots(void)
{
  fillShop();
  printShopSlots();
}

void populateBasePets(void)
{
  printf("Populating Pets\n");

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

void setupBoardSlots(void)
{
  for (int x = 0; x < 5; x++)
  {
    boardSlots[x].isEmpty = true;
    boardSlots[x].position = 0;
  }
}

void setup(void)
{
  srand(time(0));
  rand();
  populateBasePets();
  setupBoardSlots();
  playerState.health = 10;
  playerState.gold = 10;
  gameState.tier = 1;
  gameState.shopSlotCount = 3;
  gameState.currentPhase = Shop;
}

void nextStep(void)
{
  printf("Next Step\n");
  switch (gameState.currentPhase)
  {
  case Shop:
    fillShop();
    playerState.gold = 10;
    break;
  case Battle:
    printf("prepare battle");
    break;
  }
}

void buyPet(int buySlot, int buildSlot)
{
  if (playerState.gold < 3)
  {
    printf("Not enough gold to buy pet\n");
    return;
  }

  if (shopSlots[buySlot].isEmpty)
  {
    printf("Shop slot %d is empty\n", buySlot);
    return;
  }

  if (!boardSlots[buildSlot].isEmpty)
  {
    printf("Build slot %d is not empty\n", buySlot);
    return;
  }

  printf("ACTION - Buy pet: ");
  printPetBase(shopSlots[buySlot].pet);

  boardSlots[buildSlot].pet.base = shopSlots[buySlot].pet;
  boardSlots[buildSlot].pet.attack = shopSlots[buySlot].pet.attack;
  boardSlots[buildSlot].pet.health = shopSlots[buySlot].pet.health;
  boardSlots[buildSlot].isEmpty = false;
  boardSlots[buildSlot].position = buildSlot;

  shopSlots[buySlot].isEmpty = true;
  playerState.gold -= 3;
}

int main()
{
  setup();
  nextStep();
  printGameState();
  buyPet(0, 0);
  printGameState();

  return 0;
}
