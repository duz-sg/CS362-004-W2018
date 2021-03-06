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
#define TESTFUNC "scoreFor"

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
    int p, r, n;
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
            for (n = 0; n < p; n++) {
                printf("Test with player %d.\n", n);

                printf("Player should have 3 score after game start.\n");
                testIssues += myAssert(scoreFor(n, &testState), 3);

                gainCard(copper, &testState, 0, n);
                printf("Player should have 3 score after gain one copper to discard.\n");
                testIssues += myAssert(scoreFor(n, &testState), 3);

                gainCard(copper, &testState, 1, n);
                printf("Player should have 3 score after gain one copper to deck.\n");
                testIssues += myAssert(scoreFor(n, &testState), 3);

                gainCard(copper, &testState, 2, n);
                printf("Player should have 3 score after gain one copper to hand.\n");
                testIssues += myAssert(scoreFor(n, &testState), 3);

                // Bug in scoreFor cannot count score for player's deck
                // Any more tests will just be duplicates
            }
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
