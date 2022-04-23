#include "tests.h"
#include "engine.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

TestResult testSetup(void)
{
  PlayerState playerState;
  GameState gameState;

  setup(&gameState, &playerState, 10, 10, 1, Shop);

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

  if (gameState.currentPhase != Shop)
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

  setup(&gameState, &playerState, 10, 10, 1, Shop);
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

  for (int y = 0; y < gameState.shopItemSlotCount; y++) {
    if (playerState.itemSlots[y].isEmpty) {
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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

  TestResult testResult = {"", false};
  fillShop(gameState, &playerState);
  OperationResult buyResult = buyPet(gameState, &playerState, 0, 0);

  if (!buyResult.didSucceed) {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  if (!playerState.shopSlots[0].isEmpty)
  {
    printf("c\n");
    strcpy(testResult.errorMessage, "Expected shop slot to be empty after buy pet");
    return testResult;
  }

  if (playerState.boardSlots[0].isEmpty)
  {
    printf("d\n");
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

  setup(&gameState, &playerState, 10, 2, 1, Shop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  OperationResult buyResult = buyPet(gameState, &playerState, 0, 0);

  if (buyResult.didSucceed) {
    strcpy(testResult.errorMessage, "Buy pet succeeded with insufficient gold");
    return testResult;
  }

  if (playerState.gold != 2) {
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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

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

  for (int y = 0; y < 2; y++) {
    if (!playerState.itemSlots[y].isEmpty) {
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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  buyPet(gameState, &playerState, 2, 2);

  OperationResult buyResult = buyItem(gameState, &playerState, 0, 2);

  if (!buyResult.didSucceed) {
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

  setup(&gameState, &playerState, 10, 10, 1, Shop);

  TestResult testResult = {"", false};

  fillShop(gameState, &playerState);
  fillShopWithItem(gameState, &playerState, 0);
  buyPet(gameState, &playerState, 2, 2);

  int initialAttack = playerState.boardSlots[2].pet.attack;
  int initialHealth = playerState.boardSlots[2].pet.health;

  OperationResult buyResult = buyItem(gameState, &playerState, 0, 2);

  if (!buyResult.didSucceed) {
    strcpy(testResult.errorMessage, buyResult.errorMessage);
    return testResult;
  }

  if (playerState.boardSlots[2].pet.attack != (initialAttack + 1)) {
    strcpy(testResult.errorMessage, "Expected intitial attack to go up by 1");
    return testResult;
  }

  if (playerState.boardSlots[2].pet.health != (initialHealth + 1)) {
    strcpy(testResult.errorMessage, "Expected intitial attack to go up by 1");
    return testResult;
  }

  testResult.didSucceed = true;
  return testResult;
}
