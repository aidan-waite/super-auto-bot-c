#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct PetBase
{
  char *name;
  char *unicodeCodePoint;
  int tier;
  int count;
  int spawnChanceRound1;
  int attack;
  int health;
};
typedef struct PetBase PetBase;

PetBase basePets[9];

void printPetBase(PetBase pet)
{
  printf("%s %s tier:%d count:%d attack:%d health:%d\n",
         pet.unicodeCodePoint,
         pet.name,
         pet.tier,
         pet.count,
         pet.attack,
         pet.health);
}

void populateBasePets(void)
{
  printf("Populating Pets\n");

  int i = 0;

  basePets[i].name = "ant";
  basePets[i].unicodeCodePoint = "🐜";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 1;

  basePets[i].name = "beaver";
  basePets[i].unicodeCodePoint = "🦫";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 2;

  basePets[i].name = "cricket";
  basePets[i].unicodeCodePoint = "🦗";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 2;

  basePets[i].name = "duck";
  basePets[i].unicodeCodePoint = "🦆";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 3;

  basePets[i].name = "fish";
  basePets[i].unicodeCodePoint = "🐟";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 3;

  basePets[i].name = "horse";
  basePets[i].unicodeCodePoint = "🐎";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 1;

  basePets[i].name = "mosquito";
  basePets[i].unicodeCodePoint = "🦟";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 2;
  basePets[i++].health = 2;

  basePets[i].name = "otter";
  basePets[i].unicodeCodePoint = "🦦";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 1;
  basePets[i++].health = 2;

  basePets[i].name = "pig";
  basePets[i].unicodeCodePoint = "🐖";
  basePets[i].tier = 1;
  basePets[i].count = 0;
  basePets[i].spawnChanceRound1 = 11111111;
  basePets[i].attack = 3;
  basePets[i++].health = 1;
}

int randomPetForTier(int tier)
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

void printBasePets(void)
{
  for (int x = 0; x < 9; x++)
  {
    printPetBase(basePets[x]);
  }
}

void testRandomPets()
{
  for (int a = 0; a < 25000; a++)
  {
    basePets[randomPetForTier(1)].count += 1;
  }

  printBasePets();
}

int main()
{
  populateBasePets();
  printBasePets();

  printf("Random Pets\n");

  srand(time(0));

  return 0;
}