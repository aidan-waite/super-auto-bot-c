#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.h"
#include "tests.h"
#include "models.h"

void printTestResult(TestResult result, char *name)
{
  if (result.didSucceed)
  {
    printf("✅ %s succeeded\n", name);
  }
  else
  {
    printf("❌ %s failed: %s\n", name, result.errorMessage);
  }
}

void runAllTests(void)
{
  printf("--- START TESTS ---\n\n");
  printTestResult(testSetup(), "Setup test");
  printTestResult(testFillShop(), "Fill shop");
  printTestResult(testBuyPet(), "Buy pet");
  printTestResult(testBuyPetInsufficientGold(), "Buy pet insufficient gold");
  printTestResult(testClearShopSlots(), "Clear shop");
  printTestResult(testClearBoardSlots(), "Clear board");
  printTestResult(testSellPet(), "Sell pet");
  printTestResult(testBuyItem(), "Buy item");
  printTestResult(testBuyApple(), "Buy apple");
  printTestResult(testBuyHoney(), "Buy honey");
  printf("\n--- END TESTS ---\n");
}

int main()
{
  runAllTests();
  return 0;
}
