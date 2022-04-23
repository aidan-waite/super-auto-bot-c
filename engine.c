#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "models.h"
#include <string.h>

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

void printItemBase(ItemBase base)
{
  printf("%s %s\n", base.unicodeCodePoint, base.name);
}

void printGameState(GameState gameState, PlayerState playerState)
{
  char *phaseName = gameState.currentPhase == Shop ? "Shop" : "Battle";

  printf("\n*** GAME STATE ***\nCurrent tier:%d\nPet slot count:%d\nItem slot count:%d\nPhase:%s\nPlayer health:%d gold:%d\n",
         gameState.tier,
         gameState.shopPetSlotCount,
         gameState.shopItemSlotCount,
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
    printf("Pet shop slot #%d ", b);
    if (playerState.shopSlots[b].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printPetBase(playerState.shopSlots[b].pet);
    }
  }

  for (int i = 0; i < 2; i++)
  {
    printf("Item shop slot #%d ", i);
    if (playerState.itemSlots[i].isEmpty)
    {
      printf("Empty\n");
    }
    else
    {
      printItemBase(playerState.itemSlots[i].item);
    }
  }

  printf("*** / / / ***\n\n");
}

int randomItemBaseIndForTier(GameState gameState, int tier)
{
  // TODO: switch on tier

  // 1. Sum all probabilities
  int sum = 0;
  int x = 0;

  for (x = 0; x < 2; x++)
  {
    sum += gameState.baseItems[x].spawnChanceRound1;
  }

  // 2. Random between 0 and sum
  int r = rand() % sum;
  int tmp = 0;

  // 3. Walk through each item, adding chance until you're at > randomnum
  for (x = 0; x < 2; x++)
  {
    tmp += gameState.baseItems[x].spawnChanceRound1;
    if (tmp > r)
    {
      return x;
    }
  }

  // default to last index
  return 1;
}

ItemBase randomItemBase(GameState gameState)
{
  int ind = randomItemBaseIndForTier(gameState, 1);
  ItemBase i = {
    gameState.baseItems[ind].name,
    gameState.baseItems[ind].unicodeCodePoint,
    gameState.baseItems[ind].effect,
    gameState.baseItems[ind].spawnChanceRound1,
  };
  return i;
}

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

void fillShopWithItem(GameState gameState, PlayerState *playerState, int baseItemIndex)
{
  for (int s = 0; s < gameState.shopItemSlotCount; s++)
  {
    ShopItemSlot slot = {s, gameState.baseItems[baseItemIndex], false};
    playerState->itemSlots[s] = slot;
  }
}

void fillShop(GameState gameState, PlayerState *playerState)
{
  for (int x = 0; x < gameState.shopPetSlotCount; x++)
  {
    PetBase p = randomPetBase(gameState);
    ShopPetSlot s = {x, p, false};
    playerState->shopSlots[x] = s;
  }

  for (int s = 0; s < gameState.shopItemSlotCount; s++)
  {
    ItemBase item = randomItemBase(gameState);
    ShopItemSlot slot = {s, item, false};
    playerState->itemSlots[s] = slot;
  }
}

void populateBaseItems(ItemBase baseItems[2])
{
  int i = 0;

  baseItems[i].name = "apple";
  baseItems[i].unicodeCodePoint = "🍎";
  baseItems[i].effect = "perma-buff-1-1";
  baseItems[i++].spawnChanceRound1 = 50000000;

  baseItems[i].name = "honey";
  baseItems[i].unicodeCodePoint = "🍯";
  baseItems[i].effect = "status-bee";
  baseItems[i++].spawnChanceRound1 = 50000000;
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

  for (int s = 0; s < 2; s++)
  {
    playerState->itemSlots[s].isEmpty = true;
    playerState->itemSlots[s].position = s;
  }
}

void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, phase p)
{
  memset(playerState, 0, sizeof(PlayerState));
  memset(gameState, 0, sizeof(GameState));

  srand(time(0));
  rand();
  populateBasePets(gameState->basePets);
  populateBaseItems(gameState->baseItems);
  playerState->health = health;
  playerState->gold = gold;
  gameState->tier = tier;
  gameState->shopPetSlotCount = 3;
  gameState->shopItemSlotCount = 1;
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

OperationResult buyItem(GameState gameState, PlayerState *playerState, int buySlot, int targetSlot)
{
  OperationResult result = {"", false};

  if (playerState->gold < 3) {
    strcpy(result.errorMessage, "Not enough gold to buy item");
    return result;
  }

  if (playerState->itemSlots[buySlot].isEmpty) {
    strcpy(result.errorMessage, "Item slot is empty");
    return result;
  }

  if (playerState->boardSlots[targetSlot].isEmpty) {
    strcpy(result.errorMessage, "Target build slot is empty");
    return result;
  }

  if (strcmp(playerState->itemSlots[buySlot].item.effect, "perma-buff-1-1") == 0) {
    printf("Apply effect: increase attack and health by one\n");
    playerState->boardSlots[targetSlot].pet.attack += 1;
    playerState->boardSlots[targetSlot].pet.health += 1;
  }

  result.didSucceed = true;
  return result;
}

OperationResult buyPet(GameState gameState, PlayerState *playerState, int buySlot, int buildSlot)
{
  OperationResult result = {"", false};

  if (playerState->gold < 3)
  {
    strcpy(result.errorMessage, "Not enough gold to buy pet");
    return result;
  }

  if (playerState->shopSlots[buySlot].isEmpty)
  {
    strcpy(result.errorMessage, "Shop slot is empty");
    return result;
  }

  if (!playerState->boardSlots[buildSlot].isEmpty)
  {
    strcpy(result.errorMessage, "Build slot is not empty");
    return result;
  }

  playerState->boardSlots[buildSlot].pet.base = playerState->shopSlots[buySlot].pet;
  playerState->boardSlots[buildSlot].pet.attack = playerState->shopSlots[buySlot].pet.attack;
  playerState->boardSlots[buildSlot].pet.health = playerState->shopSlots[buySlot].pet.health;
  playerState->boardSlots[buildSlot].isEmpty = false;
  playerState->boardSlots[buildSlot].position = buildSlot;

  playerState->shopSlots[buySlot].isEmpty = true;
  playerState->gold -= 3;
  result.didSucceed = true;
  return result;
}

void sellPet(GameState gameState, PlayerState *playerState, int sellSlot)
{
  if (playerState->boardSlots[sellSlot].isEmpty)
  {
    printf("Board slot is already empty\n");
    return;
  }

  playerState->boardSlots[sellSlot].isEmpty = true;

  // TODO: I think you're supposed to get 1 gold per level
  playerState->gold += 1;
}
