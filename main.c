#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"

/* Game State */

PetBase basePets[9];
BoardSlot boardSlots[5];
ShopPetSlot shopSlots[3];

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
    printPetBuilt(boardSlots[x].pet);
  }
}

void printShopSlots(void)
{
  for (int x = 0; x < 5; x++)
  {
    printf("Shop slot #%d ", x);
    printPetBase(basePets[x]);
  }
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

void testShopSlots(void)
{
  for (int x = 0; x < 3; x++)
  {
    PetBase p = randomPetBase(1);
    ShopPetSlot s = {x, p};
    shopSlots[x] = s;
  }

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

int main()
{
  populateBasePets();
  printBasePets();
  srand(time(0));

  testBoardSlots();

  return 0;
}