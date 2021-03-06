#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "models.h"
#include "bases.h"

void printPetBuilt(PetBuilt pet)
{
  printf("%s %s tier:%d debugCount:%d attack:%d health:%d battle state attack:%d health:%d\n",
         pet.base.unicodeCodePoint,
         pet.base.name,
         pet.base.tier,
         pet.base.debugCount,
         pet.attack,
         pet.health,
         pet.battleState.attack,
         pet.battleState.health);
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

/**
* Box is a simple x/x animal for testing with no triggers
*/
PetBase boxBase(int strength)
{
  PetBase base = {"box", "[]", 1, 0, 0, strength, strength};
  return base;
}

void setupBattleSlot(PlayerState *playerState, int i)
{
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
void summon(PlayerState *playerState, PetBase petBase, int slotInd)
{
  for (int i = 0; i <= slotInd; i++)
  {
    if (playerState->boardSlots[i].isEmpty)
    {
      setPet(playerState, petBase, i);
      setupBattleSlot(playerState, i);
      return;
    }
  }

  printf("No valid slots to summon into");
}

void handleCricketTrigger(PlayerState *playerState, int slotInd)
{
  PetBase base = {"zombie cricket", "????", 1, 0, 11111111, 1, 1};
  summon(playerState, base, slotInd);
}

void handleAntTrigger(PlayerState *playerState, int slotInd)
{
  // Give a random friend +2/+1
  if (slotInd == 4)
  {
    printf("Ant was the last pet so its buff doesn't do anything\n");
    return;
  }

  int validBuffIndexes[5];
  memset(validBuffIndexes, -1, sizeof(int) * 5);
  int validBuffIndex = 0;

  for (int i = 0; i < 5; i++)
  {
    if (i != slotInd &&
        !playerState->boardSlots[i].isEmpty &&
        !playerState->boardSlots[i].pet.battleState.fainted)
    {
      validBuffIndexes[validBuffIndex++] = i;
    }
  }

  if (validBuffIndex == 0)
  {
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
  summon(playerState, beeBase(), slotInd);
}

void petDidFaint(PlayerState *playerState, int slotInd)
{
  if (strcmp(playerState->boardSlots[slotInd].pet.base.name, "ant") == 0)
  {
    handleAntTrigger(playerState, slotInd);
  }

  if (strcmp(playerState->boardSlots[slotInd].pet.base.name, "cricket") == 0)
  {
    handleCricketTrigger(playerState, slotInd);
  }

  if (playerState->boardSlots[slotInd].pet.equippedItem && strcmp(playerState->boardSlots[slotInd].pet.equippedItem, "honey") == 0)
  {
    handleBeeTrigger(playerState, slotInd);
  }
}

bool haveAllPetsFainted(PlayerState *playerState)
{
  for (int i = 0; i < 5; i++)
  {
    if (!playerState->boardSlots[i].isEmpty &&
        !playerState->boardSlots[i].pet.battleState.fainted)
    {
      return false;
    }
  }

  return true;
}

int randomItemBaseIndForTier(GameState gameState, int tier)
{
  // There's an equal chance to get each item
  // so return a random item from the group of
  // all items <= current tier
  ItemBase baseItems[16];

    // Not sure if this is how you memset correctly
  memset(baseItems, 0, sizeof(PetBase * 16));

  int totalNumberOfItems = 16;
  int eligibleCount = 0;
  for (int x = 0; x < totalNumberOfItems; x++) {
    if (gameState.baseItems[x].tier <= tier) {
      baseItems[eligibleCount++] = gameState.baseItems[x];
    }
  }

  int randomInd = rand() % eligibleCount;
  return baseItems[randomInd];
}

ItemBase randomItemBase(GameState gameState)
{
  int ind = randomItemBaseIndForTier(gameState, 1);
  ItemBase i = {
      gameState.baseItems[ind].name,
      gameState.baseItems[ind].unicodeCodePoint,
  };
  return i;
}

int randomPetBaseIndForTier(GameState gameState, int tier)
{
  // There's an equal chance to get each pet
  // so return a random pet from the group of
  // all pets <= current tier
  PetBase basePets[58];

  // Not sure if this is how you memset correctly
  memset(basePets, 0, sizeof(PetBase * 58));

  int totalNumberOfPets = 58;
  int eligibleCount = 0;
  for (int x = 0; x < totalNumberOfPets; x++) {
    if (gameState.basePets[x].tier <= tier) {
      basePets[eligibleCount++] = gameState.basePets[x];
    }
  }

  int randomInd = rand() % eligibleCount;
  return basePets[randomInd];
}

PetBase randomPetBase(GameState gameState)
{
  int ind = randomPetBaseIndForTier(gameState, 1);
  PetBase b = {
      gameState.basePets[ind].name,
      gameState.basePets[ind].unicodeCodePoint,
      gameState.basePets[ind].tier,
      gameState.basePets[ind].debugCount,
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
  baseItems[i++] = appleBase();
  baseItems[i++] = honeyBase();
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

  if (playerState->gold < 3)
  {
    strcpy(result.errorMessage, "Not enough gold to buy item");
    return result;
  }

  if (playerState->itemSlots[buySlot].isEmpty)
  {
    strcpy(result.errorMessage, "Item slot is empty");
    return result;
  }

  if (playerState->boardSlots[targetSlot].isEmpty)
  {
    strcpy(result.errorMessage, "Target build slot is empty");
    return result;
  }

  if (strcmp(playerState->itemSlots[buySlot].item.name, "apple") == 0)
  {
    playerState->boardSlots[targetSlot].pet.attack += 1;
    playerState->boardSlots[targetSlot].pet.health += 1;
  }

  if (strcmp(playerState->itemSlots[buySlot].item.name, "honey") == 0)
  {
    playerState->boardSlots[targetSlot].pet.equippedItem = "honey";
  }

  result.succeeded = true;
  return result;
}

void setupBattleState(PlayerState *playerState)
{
  for (int i = 0; i < 5; i++)
  {
    if (!playerState->boardSlots[i].isEmpty)
    {
      setupBattleSlot(playerState, i);
    }
  }
}

BattleResult doBattleRound(PlayerState *playerState1, PlayerState *playerState2)
{
  BattleResult result = {BattleWinnerError, false, "Initial value"};

  // Find player 1's pet who will battle
  int ind1 = -1;
  for (int a = 0; a < 5; a++)
  {
    if (!playerState1->boardSlots[a].isEmpty &&
        !playerState1->boardSlots[a].pet.battleState.fainted)
    {
      ind1 = a;
      break;
    }
  }

  // Find player 2's pet who will battle
  int ind2 = -1;
  for (int b = 0; b < 5; b++)
  {
    if (!playerState2->boardSlots[b].isEmpty &&
        !playerState2->boardSlots[b].pet.battleState.fainted)
    {
      ind2 = b;
      break;
    }
  }

  // Players can start a battle with no pets
  if (ind1 == -1 && ind2 == -1) {
    result.winner = BattleWinnerTie;
    result.succeeded = true;
    return result;
  } else if (ind1 == -1) {
    result.winner = BattleWinnerPlayer2;
    result.succeeded = true;
    return result;
  } else if (ind2 == -1) {
    result.winner = BattleWinnerPlayer1;
    result.succeeded = true;
    return result;
  }

  bool p1fainted = false;
  bool p2fainted = false;

  // First deal the damage
  playerState1->boardSlots[ind1].pet.battleState.health -= playerState2->boardSlots[ind2].pet.battleState.attack;
  playerState2->boardSlots[ind2].pet.battleState.health -= playerState1->boardSlots[ind1].pet.battleState.attack;

  // Then handle the results
  if (playerState1->boardSlots[ind1].pet.battleState.health <= 0)
  {
    playerState1->boardSlots[ind1].pet.battleState.fainted = true;
    playerState1->boardSlots[ind1].isEmpty = true;
    petDidFaint(playerState1, ind1);
    p1fainted = true;
  }
  if (playerState2->boardSlots[ind2].pet.battleState.health <= 0)
  {
    playerState2->boardSlots[ind2].pet.battleState.fainted = true;
    playerState2->boardSlots[ind2].isEmpty = true;
    petDidFaint(playerState2, ind2);
    p2fainted = true;
  }

  if (p1fainted && p2fainted) {
    result.winner = BattleWinnerTie;
  } else if (p1fainted) {
    result.winner = BattleWinnerPlayer2;
  } else if (p2fainted) {
    result.winner = BattleWinnerPlayer1;
  }

  result.succeeded = true;
  return result;
}

BattleResult doBattlePhase(PlayerState *playerState1, PlayerState *playerState2)
{
  BattleResult phaseResult = {BattleWinnerError, false, ""};

  while (!haveAllPetsFainted(playerState1) && !haveAllPetsFainted(playerState2))
  {
    BattleResult roundResult = doBattleRound(playerState1, playerState2);
    if (!roundResult.succeeded)
    {
      printf("Something went wrong while doing battle phase");
      strcpy(phaseResult.errorMessage, roundResult.errorMessage);
      return phaseResult;
    }
  }

  bool p1 = haveAllPetsFainted(playerState1);
  bool p2 = haveAllPetsFainted(playerState2);
  if (p1 && p2)
  {
    phaseResult.winner = BattleWinnerTie;
  }
  else if (p1)
  {
    phaseResult.winner = BattleWinnerPlayer2;
  }
  else if (p2)
  {
    phaseResult.winner = BattleWinnerPlayer1;
  }

  phaseResult.succeeded = true;
  return phaseResult;
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
  result.succeeded = true;
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
