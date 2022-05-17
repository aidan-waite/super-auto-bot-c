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
  char *phaseName = gameState.currentPhase == PhaseShop ? "Shop" : "Battle";

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

void setupBattleSlot(PlayerState *playerState, int i) {
  playerState->boardSlots[i].pet.battleState.attack = playerState->boardSlots[i].pet.attack;
  playerState->boardSlots[i].pet.battleState.health = playerState->boardSlots[i].pet.health;
  playerState->boardSlots[i].pet.battleState.fainted = false;
}

/**
* Set a pet slot regardless of usual constraints such as gold. Used for testing.
*/
void setPet(PlayerState *playerState, PetBase base, int buildSlot)
{
  playerState->boardSlots[buildSlot].pet.base = base;
  playerState->boardSlots[buildSlot].pet.attack = base.attack;
  playerState->boardSlots[buildSlot].pet.health = base.health;
  playerState->boardSlots[buildSlot].isEmpty = false;
  playerState->boardSlots[buildSlot].position = buildSlot;
}

/**
* Summon a pet at or ahead of slotInd if possible.
*/
void summon(PlayerState *playerState, PetBase petBase, int slotInd) {
  for(int i = 0; i <= slotInd; i++) {
    if (playerState->boardSlots[i].isEmpty) {
      setPet(playerState, petBase, i);
      setupBattleSlot(playerState, i);
      return;
    }
  }

  printf("No valid slots to summon into");
}

void handleCricketTrigger(PlayerState *playerState, int slotInd)
{
  PetBase base = { "zombie cricket", "🦗", 1, 0, 11111111, 1, 1};
  summon(playerState, base, slotInd);
}

void handleAntTrigger(PlayerState *playerState, int slotInd)
{
    // Give a random friend +2/+1
    if (slotInd == 4) {
      printf("Ant was the last pet so its buff doesn't do anything\n");
      return;
    }

    int validBuffIndexes[5];
    memset(validBuffIndexes, -1, sizeof(int) * 5);
    int validBuffIndex = 0;

    for (int i = 0; i < 5; i++) {
      if (i != slotInd &&
        !playerState->boardSlots[i].isEmpty &&
        !playerState->boardSlots[i].pet.battleState.fainted) {
          validBuffIndexes[validBuffIndex++] = i;
        }
    }

    if (validBuffIndex == 0) {
      printf("There are no valid targets for ant buff\n");
      return;
    }

    int buffInd = validBuffIndexes[rand() % validBuffIndex];
    playerState->boardSlots[buffInd].pet.battleState.attack += 2;
    playerState->boardSlots[buffInd].pet.battleState.health += 1;
    return;
}

void handleBeeTrigger(PlayerState *playerState, int slotInd)
{
  PetBase base = { "bee", "🐝", 1, 0, 11111111, 1, 1};
  summon(playerState, base, slotInd);
}

void petDidFaint(PlayerState *playerState, int slotInd)
{
  if (strcmp(playerState->boardSlots[slotInd].pet.base.name, "ant") == 0) {
    handleAntTrigger(playerState, slotInd);
  }

  if (strcmp(playerState->boardSlots[slotInd].pet.base.name, "cricket") == 0) {
    handleCricketTrigger(playerState, slotInd);
  }

  if (playerState->boardSlots[slotInd].pet.equippedItem && strcmp(playerState->boardSlots[slotInd].pet.equippedItem, "bee") == 0) {
    handleBeeTrigger(playerState, slotInd);
  }
}

bool haveAllPetsFainted(PlayerState *playerState)
{
  for (int i = 0; i < 5; i++) {
    if (!playerState->boardSlots[i].isEmpty &&
        !playerState->boardSlots[i].pet.battleState.fainted) {
      return false;
    }
  }

  return true;
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

ItemBase appleBase() {
  ItemBase base = {"apple", "🍎", "perma-buff-1-1", 50000000};
  return base;
}

ItemBase honeyBase() {
  ItemBase base = {"honey", "🍯", "equip-bee", 50000000};
  return base;
}

void populateBaseItems(ItemBase baseItems[2])
{
  int i = 0;
  baseItems[i++] = appleBase();
  baseItems[i++] = honeyBase();
}

PetBase antBase() {
  PetBase base = { "ant", "🐜", 1, 0, 11111111, 2, 1 };
  return base;
}

PetBase beaverBase() {
  PetBase base = { "beaver", "🦫", 1, 0, 11111111, 2, 2};
  return base;
}

PetBase cricketBase() {
  PetBase base = { "cricket", "🦗", 1, 0, 11111111, 1, 2};
  return base;
}

PetBase duckBase() {
  PetBase base = { "duck", "🦆", 1, 0, 11111111, 1, 3};
  return base;
}

PetBase fishBase() {
  PetBase base = { "fish", "🐟", 1, 0, 11111111, 2, 3};
  return base;
}

PetBase horseBase() {
  PetBase base = { "horse", "🐎", 1, 0, 11111111, 2, 1};
  return base;
}

PetBase mosquitoBase() {
  PetBase base = { "mosquito", "🦟", 1, 0, 11111111, 2, 2};
  return base;
}

PetBase otterBase() {
  PetBase base = { "otter", "🦦", 1, 0, 11111111, 1, 2};
  return base;
}

PetBase pigBase() {
  PetBase base = { "pig", "🐖", 1, 0, 11111111, 3, 1};
  return base;
}

void populateBasePets(PetBase basePets[9])
{
  int i = 0;
  basePets[i++] = antBase();
  basePets[i++] = beaverBase();
  basePets[i++] = cricketBase();
  basePets[i++] = duckBase();
  basePets[i++] = fishBase();
  basePets[i++] = horseBase();
  basePets[i++] = mosquitoBase();
  basePets[i++] = otterBase();
  basePets[i++] = pigBase();
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

void setup(GameState *gameState, PlayerState *playerState, int health, int gold, int tier, Phase p)
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
  case PhaseShop:
    fillShop(gameState, &playerState);
    playerState.gold = 10;
    break;
  case PhaseBattle:
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
    playerState->boardSlots[targetSlot].pet.attack += 1;
    playerState->boardSlots[targetSlot].pet.health += 1;
  }

  if (strcmp(playerState->itemSlots[buySlot].item.effect, "equip-bee") == 0) {
    playerState->boardSlots[targetSlot].pet.equippedItem = "bee";
  }

  result.didSucceed = true;
  return result;
}

void setupBattleState(PlayerState *playerState)
{
  for (int i = 0; i < 5; i++) {
    if (!playerState->boardSlots[i].isEmpty) {
      setupBattleSlot(playerState, i);
    }
  }
}

OperationResult doBattleRound(PlayerState *playerState1, PlayerState *playerState2)
{
  OperationResult result = {"", false};

  int ind1 = -1;
  int ind2 = -1;

  for (int a = 0; a < 5; a++) {
    if (!playerState1->boardSlots[a].isEmpty &&
        !playerState1->boardSlots[a].pet.battleState.fainted) {
      ind1 = a;
      break;
    }
  }

  for (int b = 0; b < 5; b++) {
    if (!playerState2->boardSlots[b].isEmpty &&
      !playerState2->boardSlots[b].pet.battleState.fainted) {
      ind2 = b;
      break;
    }
  }

  if (ind1 == -1 || ind2 == -1) {
    strcpy(result.errorMessage, "Unable to find a pet that hasn't fainted");
    return result;
  }

  playerState1->boardSlots[ind1].pet.battleState.health -= playerState2->boardSlots[ind2].pet.battleState.attack;
  if (playerState1->boardSlots[ind1].pet.battleState.health <= 0) {
    playerState1->boardSlots[ind1].pet.battleState.fainted = true;
    playerState1->boardSlots[ind1].isEmpty = true;
    petDidFaint(playerState1, ind1);
  }

  playerState2->boardSlots[ind2].pet.battleState.health -= playerState2->boardSlots[ind1].pet.battleState.attack;
  if (playerState2->boardSlots[ind2].pet.battleState.health <= 0) {
    playerState2->boardSlots[ind2].pet.battleState.fainted = true;
    playerState2->boardSlots[ind2].isEmpty = true;
    petDidFaint(playerState2, ind2);
  }

  result.didSucceed = true;
  return result;
}

BattlePhaseResult doBattlePhase(PlayerState *playerState1, PlayerState *playerState2)
{
  while (!haveAllPetsFainted(playerState1) && !haveAllPetsFainted(playerState2)) {
    OperationResult result = doBattleRound(playerState1, playerState2);
    if (!result.didSucceed) {
      printf("Something went wrong while doing battle");
    }
  }

  bool p1 = haveAllPetsFainted(playerState1);
  bool p2 = haveAllPetsFainted(playerState2);
  if (p1 && p2) {
    return BattlePhaseResultTie;
  } else if (p1) {
    return BattlePhaseResultPlayer2Win;
  } else if (p2) {
    return BattlePhaseResultPlayer1Win;;
  }

  printf("If we get here something is very wrong!");
  return BattlePhaseResultTie;
}

/**
* Buy a pet using gold
*/
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

void executeRoundOfCombat(GameState *g1, PlayerState *p1, GameState *g2, PlayerState *p2)
{

}
