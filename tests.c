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
    result.didSucceed = false;
    return result;
  }

  if (playerState.gold != 10)
  {
    snprintf(result.errorMessage, 100, "Gold is incorrect. Expected 10 but got %d\n", playerState.gold);
    result.didSucceed = false;
    return result;
  }

  if (gameState.currentPhase != Shop)
  {
    strcpy(result.errorMessage, "Phase is incorrect. Expected shop\n");
    result.didSucceed = false;
    return result;
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

  for (int x = 0; x < gameState.shopSlotCount; x++)
  {
    if (playerState.shopSlots[x].isEmpty)
    {
      strcpy(result.errorMessage, "Missing shop pet");
      return result;
    }
  }

  result.didSucceed = true;
  return result;
}