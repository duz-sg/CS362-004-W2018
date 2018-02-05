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
#define TESTCARD "sea_hag"

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

int getTokenCount(struct gameState *state) {
    int i = 0;
    int count = 0;
    for (i = 0; i <= treasure_map; i++) {
        if (state->embargoTokens[i]) {
            count++;
        }
    }
    return count;
}

int main() {
    int seed = 1000;
    int p, r;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int cost = 4;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, testG;
    int thisPlayer = 0;
    int thatPlayer = 1;
    int testIssues = 0;
    int newDiscard = 1;

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for (p = 0; p <= MAX_PLAYERS; p++) {
        printf("Test with %d players.\n", p);
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(p, k, seed, &G); // initialize a new game
        memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to test

        if ( r == 0 ) {
            cardEffect(sea_hag, choice1, choice2, choice3, &testG, handpos, &bonus);

            printf("Test 1: It cost 4 coins.\n");
            printf("This card cost %d coins, exptect %d.\n", getCost(sea_hag), cost);
            testIssues += myAssert(getCost(sea_hag), cost);

            printf("Test 2: Other player discard pile increase.\n");
            printf("Before using card discard count is %d.\n", G.discardCount[thatPlayer]);
            printf("After using card discard count is %d, exptect %d.\n", testG.discardCount[thatPlayer], newDiscard);
            testIssues += myAssert(testG.discardCount[thatPlayer], newDiscard);

            printf("Test 3: Other player deck count remain same.\n");
            printf("Before using card deck count is %d.\n", G.deckCount[thatPlayer]);
            printf("After using card deck count is %d, exptect %d.\n", testG.deckCount[thatPlayer], G.deckCount[thatPlayer]);
            testIssues += myAssert(testG.deckCount[thatPlayer], G.deckCount[thatPlayer]);

            printf("Test 4: Other player's new deck top is curse.\n");
            printf("After using card new deck top is %d, expected %d.\n", testG.deck[thatPlayer][testG.deckCount[thatPlayer]-1], curse);
            testIssues += myAssert(testG.deck[thatPlayer][testG.deckCount[thatPlayer]-1], curse);

            printf("Test 5: Other player's new discard top is original deck top.\n");
            printf("Before using card deck top is %d.\n", G.deck[thatPlayer][G.deckCount[thatPlayer]-1]);
            printf("After using card new discard top is %d, expected %d.\n", testG.discard[thatPlayer][testG.discardCount[thatPlayer]-1], G.deck[thatPlayer][G.deckCount[thatPlayer]-1]);
            testIssues += myAssert(testG.discard[thatPlayer][testG.discardCount[thatPlayer]-1], G.deck[thatPlayer][G.deckCount[thatPlayer]-1]);

            printf("Test 6: No state change should occur for current player.\n");
            printf("Other player expected %d in deck before and after card effect.\n", G.deckCount[thisPlayer]);
            testIssues += myAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
            printf("Other player expected %d in hand before and after card effect.\n", G.handCount[thisPlayer]);
            testIssues += myAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer]);
            printf("Other player expected %d in discard before and after card effect.\n", G.discardCount[thisPlayer]);
            testIssues += myAssert(testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);

            printf("Test 7: No state change should occur to the victory card piles and kingdom card piles.\n");
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
