/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// Borrowed coloring from https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define TESTFUNC "isGameOver()"

// set NOISY_TEST to 0 to remove printfs from output

int myAssert(int left, int right) {
    if ( left == right ) {
        printf(GRN "TEST PASSED");
        printf(RESET "\n");
        return 0;
    }
    else {
        printf(RED "TEST FAILED, expected %d to be %d", left, right);
        printf(RESET "\n");
        return 1;
    }
}

int main() {
    int seed = 1000;
    int p, r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState testState;
    int testIssues = 0;

    printf("----------------- Testing Function: %s ----------------\n", TESTFUNC);
    for (p = 0; p <= MAX_PLAYERS; p++) {
        printf("Test with %d players.\n", p);
        memset(&testState, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(p, k, seed, &testState); // initialize a new game
        
        if ( r == 0 ) {
            printf("isGameOver() return expected 0 after game start.\n");
            testIssues += myAssert(isGameOver(&testState), 0);

            testState.supplyCount[province] = 0;
            printf("isGameOver() return expected 1 when province pile drain to 0.\n");
            testIssues += myAssert(isGameOver(&testState), 1);

            testState.supplyCount[province] = 3;
            printf("isGameOver() return expected 0 when province pile has cards.\n");
            testIssues += myAssert(isGameOver(&testState), 0);

            testState.supplyCount[copper] = 0;
            testState.supplyCount[silver] = 0;
            testState.supplyCount[gold] = 0;
            printf("isGameOver() return expected 1 when any three piles drain to empty.\n");
            testIssues += myAssert(isGameOver(&testState), 1);

            testState.supplyCount[gold] = 10;
            printf("isGameOver() return expected 0 when only two piles are empty.\n");
            testIssues += myAssert(isGameOver(&testState), 0);

            testState.supplyCount[salvager] = 0;
            printf("isGameOver() return expected 1 when three piles are empty, edge case 1.\n");
            testIssues += myAssert(isGameOver(&testState), 1);

            testState.supplyCount[salvager] = 10;
            printf("isGameOver() return expected 1 when three piles are empty, edge case 1 restore.\n");
            testIssues += myAssert(isGameOver(&testState), 0);

            testState.supplyCount[sea_hag] = 0;
            printf("isGameOver() return expected 1 when three piles are empty, edge case 2.\n");
            testIssues += myAssert(isGameOver(&testState), 1);
        }
        else {
            printf("Test skipped because game could not be initialized with %d players.\n", p);
        }
    }

    if (testIssues) {
        printf("Some test failed!\n");
    }
    else {
        printf("All tests passed!\n");
    }

    return 0;
}
