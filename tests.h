#ifndef TESTS
#define TESTS

#include "models.h"

TestResult testSetup(void);
TestResult testFillShop(void);
TestResult testBuyPet(void);
TestResult testBuyPetInsufficientGold(void);
TestResult testClearShopSlots(void);
TestResult testClearBoardSlots(void);
TestResult testSellPet(void);
TestResult testBuyItem(void);
TestResult testBuyApple(void);
TestResult testBuyHoney(void);
TestResult testSingleBattleRound(void);
TestResult testBattlePhaseSimple1(void);
TestResult testBattleAntTrigger(void);
TestResult testBattleCricketTrigger(void);
TestResult testBattleItemHoney(void);
TestResult testBattleSpawnWhenFull(void);
TestResult testBattleOneLargePetWins(void);
TestResult testBattleNoPets(void);

#endif // TESTS
