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
#define TESTCARD "smithy"

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
    struct gameState G, testG;
    int thisPlayer = 0;
    int thatPlayer = 1;
    int testIssues = 0;
    int newCards = 3;
    int discard = 0;

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for (p = 0; p <= MAX_PLAYERS; p++) {
        printf("Test with %d players.\n", p);
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(p, k, seed, &G); // initialize a new game
        memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to test

        if ( r == 0 ) {
            smithyEffect(thisPlayer, &testG, 0);
        
            printf("Test 1: Get 3 cards.\n");
            printf("Before using card there are %d cards in hand.\n", G.handCount[thisPlayer]);
            printf("After using card there are %d cards in hand, exptect %d in hand.\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discard);
            testIssues += myAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discard);

            printf("Test 2: Cards from his own pile.\n");
            printf("Before using card there are %d cards in deck.\n", G.deckCount[thisPlayer]);
            printf("After using card there are %d cards in deck, exptect %d in deck.\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards);
            testIssues += myAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards);

            printf("Test 3: No state change should occur for other players.\n");
            printf("Other player expected %d in deck before and after card effect.\n", G.deckCount[thatPlayer]);
            testIssues += myAssert(testG.deckCount[thatPlayer], G.deckCount[thatPlayer]);
            printf("Other player expected %d in hand before and after card effect.\n", G.handCount[thatPlayer]);
            testIssues += myAssert(testG.handCount[thatPlayer], G.handCount[thatPlayer]);
            printf("Other player expected %d in discard before and after card effect.\n", G.discardCount[thatPlayer]);
            testIssues += myAssert(testG.discardCount[thatPlayer], G.discardCount[thatPlayer]);

            printf("Test 4: No state change should occur to the victory card piles and kingdom card piles.\n");
            printf("Estate pile expected %d in deck before and after card effect.\n", G.supplyCount[estate]);
            testIssues += myAssert(testG.supplyCount[estate], G.supplyCount[estate]);
            printf("Duchy pile expected %d in deck before and after card effect.\n", G.supplyCount[duchy]);
            testIssues += myAssert(testG.supplyCount[duchy], G.supplyCount[duchy]);
            printf("Province pile expected %d in deck before and after card effect.\n", G.supplyCount[province]);
            testIssues += myAssert(testG.supplyCount[province], G.supplyCount[province]);

            int j = 0;
            for (j = 0; j < 10; j++) {
                int kindomCard = k[j];
                printf("Kindom pile expected %d in deck before and after card effect.\n", G.supplyCount[kindomCard]);
                testIssues += myAssert(testG.supplyCount[kindomCard], G.supplyCount[kindomCard]);
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
