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
#define TESTCARD "adventurer"

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

int myAssertGreater(int left, int right) {
    if ( left > right ) {
        printf(GRN "TEST PASSED");
        printf(RESET "\n");
        return 0;
    }
    else {
        printf(RED "TEST FAILED, expected %d > %d", left, right);
        printf(RESET "\n");
        return 1;
    }
}

int getTreasureCardCount(int player, struct gameState *state) {
    int i = 0;
    int count = 0;
    for (i = 0; i < state->handCount[player]; i++) {
        //printf("HERE HAS: %d", state->hand[player][i]);
        if ( state->hand[player][i] == copper || state->hand[player][i] == silver || state->hand[player][i] == gold) {
            count++;
        }
    }
    return count;
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
    int newCards = 2;
    int discard = 1;
    int temphand[MAX_HAND];

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for (p = 0; p <= MAX_PLAYERS; p++) {
        printf("Test with %d players.\n", p);
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(p, k, seed, &G); // initialize a new game
        memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to test

        if ( r == 0 ) {
            adventurerEffect(thisPlayer, &testG, temphand);
        
            printf("Test 1: Draw 2 treasure cards.\n");
            printf("Before using card there are %d treasure cards in hand.\n", getTreasureCardCount(thisPlayer, &G));
            printf("After using card there are %d cards in hand, exptect %d in hand.\n", getTreasureCardCount(thisPlayer, &testG), getTreasureCardCount(thisPlayer, &G)+newCards);
            testIssues += myAssert(getTreasureCardCount(thisPlayer, &G)+newCards, getTreasureCardCount(thisPlayer, &testG));

            printf("Test 2: Discard non treasure cards.\n");
            printf("Before using card there are %d cards in discard.\n", G.discardCount[thisPlayer]);
            printf("After using card there are more in discard.\n");
            // The number of discard cards could be random, cannot be fixed in test
            testIssues += myAssertGreater(testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);

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
