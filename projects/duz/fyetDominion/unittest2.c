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
#define TESTFUNC "fullDeckCount()"

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

                printf("Player should have 3 estate card after game start.\n");
                testIssues += myAssert(fullDeckCount(n, estate, &testState), 3);

                printf("Player should have 7 copper card after game start.\n");
                testIssues += myAssert(fullDeckCount(n, copper, &testState), 7);

                printf("Player should have 0 province after game start.\n");
                testIssues += myAssert(fullDeckCount(n, province, &testState), 0);

                gainCard(estate, &testState, 1, n);
                printf("Player should have 4 estate card after gain one estate to deck.\n");
                testIssues += myAssert(fullDeckCount(n, estate, &testState), 4);

                gainCard(estate, &testState, 2, n);
                printf("Player should have 5 estate card after gain one estate to hand.\n");
                testIssues += myAssert(fullDeckCount(n, estate, &testState), 5);

                gainCard(estate, &testState, 0, n);
                printf("Player should have 6 estate card after gain one estate to discard.\n");
                testIssues += myAssert(fullDeckCount(n, estate, &testState), 6);

                gainCard(copper, &testState, 1, n);
                printf("Player should have 8 copper card after gain one copper to deck.\n");
                testIssues += myAssert(fullDeckCount(n, copper, &testState), 8);

                gainCard(copper, &testState, 2, n);
                printf("Player should have 9 copper card after gain one copper to hand.\n");
                testIssues += myAssert(fullDeckCount(n, copper, &testState), 9);

                gainCard(copper, &testState, 0, n);
                printf("Player should have 10 copper card after gain one copper to discard.\n");
                testIssues += myAssert(fullDeckCount(n, copper, &testState), 10);

                gainCard(baron, &testState, 1, n);
                printf("Player should have 1 baron card after gain one baron to deck.\n");
                testIssues += myAssert(fullDeckCount(n, baron, &testState), 1);

                gainCard(village, &testState, 2, n);
                printf("Player should have 1 village card after gain one village to hand.\n");
                testIssues += myAssert(fullDeckCount(n, village, &testState), 1);

                gainCard(province, &testState, 0, n);
                printf("Player should have 1 province card after gain one province to discard.\n");
                testIssues += myAssert(fullDeckCount(n, province, &testState), 1);
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
