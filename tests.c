#include "tests.h"
#include "engine.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

TestResult testSetup(void)
{
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult result = {"", false};

  if (playerState.health != 10)
  {
    snprintf(result.errorMessage, 100, "Health is incorrect. Expected 10 but got %d\n", playerState.health);
    return result;
  }

  if (playerState.gold != 10)
  {
    snprintf(result.errorMessage, 100, "Gold is incorrect. Expected 10 but got %d\n", playerState.gold);
    return result;
  }

  if (gameState.currentPhase != PhaseShop)
  {
    strcpy(result.errorMessage, "Phase is incorrect. Expected shop\n");
    return result;
  }

  for (int x = 0; x < 9; x++)
  {
    if (gameState.basePets[x].health == 0)
    {
      strcpy(result.errorMessage, "Base pet is not setup\n");
      return result;
    }
  }

  result.didSucceed = true;
  return result;
}

TestResult testFillShop(void)
{
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);
  fillShop(gameState, &playerState);

  TestResult result = {"", false};

  for (int x = 0; x < gameState.shopPetSlotCount; x++)
  {
    if (playerState.shopSlots[x].isEmpty)
    {
      strcpy(result.errorMessage, "Missing shop pet");
      return result;
    }
  }

  for (int y = 0; y < gameState.shopItemSlotCount; y++)
  {
    if (playerState.itemSlots[y].isEmpty)
    {
      strcpy(result.errorMessage, "Missing shop item");
      return result;
    }
  }

  printGameState(gameState, playerState);

  result.didSucceed = true;
  return result;
}

TestResult testBuyPet(void)
{
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};
  fillShop(gameState, &playerState);
  OperationResult buyResult = buyPet(gameState, &playerState, 0, 0);

  if (!buyResult.didSucceed)
  {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  if (!playerState.shopSlots[0].isEmpty)
  {
    strcpy(testResult.errorMessage, "Expected shop slot to be empty after buy pet");
    return testResult;
  }

  if (playerState.boardSlots[0].isEmpty)
  {
    strcpy(testResult.errorMessage, "Expected board slot to not be empty after buy pet");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBuyPetInsufficientGold(void)
{
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 2, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  OperationResult buyResult = buyPet(gameState, &playerState, 0, 0);

  if (buyResult.didSucceed)
  {
    strcpy(testResult.errorMessage, "Buy pet succeeded with insufficient gold");
    return testResult;
  }

  if (playerState.gold != 2)
  {
    strcpy(testResult.errorMessage, "Gold balance changed when it shouldn't have");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testClearShopSlots(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  clearShopSlots(gameState, &playerState);

  for (int x = 0; x < 5; x++)
  {
    if (!playerState.shopSlots[x].isEmpty)
    {
      strcpy(testResult.errorMessage, "Shop pet slot should be empty");
      return testResult;
    }
  }

  for (int y = 0; y < 2; y++)
  {
    if (!playerState.itemSlots[y].isEmpty)
    {
      strcpy(testResult.errorMessage, "Shop item slot should be empty");
      return testResult;
    }
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testClearBoardSlots(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  clearShopSlots(gameState, &playerState);

  for (int x = 0; x < 5; x++)
  {
    if (!playerState.boardSlots[x].isEmpty)
    {
      strcpy(testResult.errorMessage, "Board slot should be empty");
      return testResult;
    }
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testSellPet(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  buyPet(gameState, &playerState, 2, 2);
  sellPet(gameState, &playerState, 2);

  if (playerState.gold != 8)
  {
    printf("gold is:%d\n", playerState.gold);
    strcpy(testResult.errorMessage, "Expected gold to be 8");
    return testResult;
  }

  if (!playerState.boardSlots[2].isEmpty)
  {
    strcpy(testResult.errorMessage, "Expected sold slot to be empty");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBuyItem(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  buyPet(gameState, &playerState, 2, 2);

  OperationResult buyResult = buyItem(gameState, &playerState, 0, 2);

  if (!buyResult.didSucceed)
  {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBuyApple(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  fillShopWithItem(gameState, &playerState, 0);
  buyPet(gameState, &playerState, 2, 2);

  int initialAttack = playerState.boardSlots[2].pet.attack;
  int initialHealth = playerState.boardSlots[2].pet.health;

  OperationResult buyResult = buyItem(gameState, &playerState, 0, 2);

  if (!buyResult.didSucceed)
  {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  if (playerState.boardSlots[2].pet.attack != (initialAttack + 1))
  {
    strcpy(testResult.errorMessage, "Expected intitial attack to go up by 1");
    return testResult;
  }

  if (playerState.boardSlots[2].pet.health != (initialHealth + 1))
  {
    strcpy(testResult.errorMessage, "Expected intitial attack to go up by 1");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBuyHoney(void)
{
  GameState gameState;
  PlayerState playerState;

  setup(&gameState, &playerState, 10, 10, 1, PhaseShop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  fillShopWithItem(gameState, &playerState, 1);
  buyPet(gameState, &playerState, 2, 2);
  OperationResult buyResult = buyItem(gameState, &playerState, 0, 2);

  if (!buyResult.didSucceed)
  {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  if (strcmp(playerState.boardSlots[2].pet.equippedItem, "bee") != 0)
  {
    strcpy(testResult.errorMessage, "Expected status to be bee");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testSingleBattleRound(void)
{
  TestResult testResult = {"", false};
  PetBase beaver = beaverBase();

  GameState gameState1;
  PlayerState playerState1;
  setup(&gameState1, &playerState1, 10, 10, 1, PhaseBattle);
  setPet(&playerState1, beaver, 0);
  setupBattleState(&playerState1);

  GameState gameState2;
  PlayerState playerState2;
  setup(&gameState2, &playerState2, 10, 10, 1, PhaseBattle);
  setPet(&playerState2, beaver, 0);
  setupBattleState(&playerState2);

  OperationResult result = doBattleRound(&playerState1, &playerState2);
  if (!result.didSucceed)
  {
    strcpy(testResult.errorMessage, result.errorMessage);
    return testResult;
  }

  // Beaver is a 2/2 so both pets should now be fainted
  if (!playerState1.boardSlots[0].pet.battleState.fainted ||
      !playerState2.boardSlots[0].pet.battleState.fainted)
  {
    strcpy(testResult.errorMessage, "Expected both pets to be fainted");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBattlePhaseSimple1(void)
{
  TestResult testResult = {"", false};

  GameState gameState1;
  PlayerState playerState1;
  setup(&gameState1, &playerState1, 10, 10, 1, PhaseBattle);
  setPet(&playerState1, beaverBase(), 0);
  setupBattleState(&playerState1);

  GameState gameState2;
  PlayerState playerState2;
  setup(&gameState2, &playerState2, 10, 10, 1, PhaseBattle);
  setPet(&playerState2, beeBase(), 0);
  setupBattleState(&playerState2);

  BattlePhaseResult result = doBattlePhase(&playerState1, &playerState2);
  if (result != BattlePhaseResultPlayer1Win)
  {
    strcpy(testResult.errorMessage, "Expected player 1 to win battle phase");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testAntTrigger(void)
{
  TestResult testResult = {"", false};

  GameState gameState1;
  PlayerState playerState1;
  setup(&gameState1, &playerState1, 10, 10, 1, PhaseBattle);
  setPet(&playerState1, antBase(), 0);
  setPet(&playerState1, mosquitoBase(), 1);
  setupBattleState(&playerState1);

  GameState gameState2;
  PlayerState playerState2;
  setup(&gameState2, &playerState2, 10, 10, 1, PhaseBattle);
  setPet(&playerState2, otterBase(), 0);
  setPet(&playerState2, beaverBase(), 1);
  setupBattleState(&playerState2);

  // Do a round of battle
  // The ant and otter should both faint
  // The ant buff should be applied to the mosquito
  doBattleRound(&playerState1, &playerState2);

  if (playerState1.boardSlots[1].pet.battleState.attack != 4 ||
      playerState1.boardSlots[1].pet.battleState.health != 3)
  {
    strcpy(testResult.errorMessage, "Ant buff was not applied correctly");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testCricketTrigger(void)
{
  TestResult testResult = {"", false};

  GameState gameState1;
  PlayerState playerState1;
  setup(&gameState1, &playerState1, 10, 10, 1, PhaseBattle);
  setPet(&playerState1, cricketBase(), 0);
  setPet(&playerState1, mosquitoBase(), 1);
  setupBattleState(&playerState1);

  GameState gameState2;
  PlayerState playerState2;
  setup(&gameState2, &playerState2, 10, 10, 1, PhaseBattle);
  setPet(&playerState2, pigBase(), 0);
  setPet(&playerState2, beaverBase(), 1);
  setupBattleState(&playerState2);

  // Do a round of battle
  // The cricket and the pig should both faint
  // Then the cricket should spawn a zombie cricket
  doBattleRound(&playerState1, &playerState2);

  if (strcmp(playerState1.boardSlots[0].pet.base.name, "zombie cricket") != 0 ||
      playerState1.boardSlots[0].pet.battleState.attack != 1 ||
      playerState1.boardSlots[0].pet.battleState.health != 1)
  {
    strcpy(testResult.errorMessage, "Expected a 1/1 zombie cricket in slot 0");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}

TestResult testBeeTrigger(void)
{
  TestResult testResult = {"", false};

  GameState gameState1;
  PlayerState playerState1;
  setup(&gameState1, &playerState1, 10, 10, 1, PhaseBattle);
  setPet(&playerState1, mosquitoBase(), 0);
  playerState1.boardSlots[0].pet.equippedItem = "bee";
  setupBattleState(&playerState1);

  GameState gameState2;
  PlayerState playerState2;
  setup(&gameState2, &playerState2, 10, 10, 1, PhaseBattle);
  setPet(&playerState2, fishBase(), 0);
  setupBattleState(&playerState2);

  // Do a round of battle
  // The cricket and the pig should both faint
  // Then the cricket should spawn a zombie cricket
  doBattleRound(&playerState1, &playerState2);

  if (strcmp(playerState1.boardSlots[0].pet.base.name, "bee") != 0 ||
      playerState1.boardSlots[0].pet.battleState.attack != 1 ||
      playerState1.boardSlots[0].pet.battleState.health != 1)
  {
    strcpy(testResult.errorMessage, "Expected a 1/1 bee in slot 0");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}
