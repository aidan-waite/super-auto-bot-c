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
TestResult testAntTrigger(void);
TestResult testCricketTrigger(void);
TestResult testBeeTrigger(void);

#endif // TESTS
