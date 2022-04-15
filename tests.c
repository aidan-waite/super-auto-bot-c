#include "tests.h"
#include "engine.h"
#include <stdbool.h>
#include <stdio.h>

// TestResult testSetup(void)
// {
//   setup(10, 3, 1, Shop);
//   TestResult result = {"", false};

//   if (playerState.health != 10)
//   {
//     snprintf(result.errorMessage, 100, "Health is incorrect. Expected 10 but got %d\n", playerState.health);
//     result.didSucceed = false;
//     return result;
//   }

//   if (playerState.gold != 3)
//   {
//     snprintf(result.errorMessage, 100, "Gold is incorrect. Expected 3 but got %d\n", playerState.gold);
//     result.didSucceed = false;
//     return result;
//   }

//   if (gameState.currentPhase != Shop)
//   {
//     result.errorMessage = "Phase is incorrect. Expected shop\n";
//     result.didSucceed = false;
//     return result;
//   }

//   result.didSucceed = true;
//   return result;
// }

// TestResult testCleanup(void)
// {
//   setup(1, 1, 5, Shop);
//   fillShop();
//   buyPet(0, 0);
//   buyPet(2, 2);
//   setup(10, 3, 1, Shop);

//   // Setup should clean up the shop and board slots
//   for (int x = 0; x < 5; x++)
//   {
//     if (!boardSlots[x].isEmpty)
//     {
//         }
//   }
// }
