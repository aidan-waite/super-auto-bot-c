#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.h"
#include "tests.c"
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
  printf("\n--- END TESTS ---\n");
}

int main()
{
  runAllTests();
  return 0;
}
