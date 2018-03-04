#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdbool.h>

#define DEBUG 0
// Borrowed coloring from https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

enum TESTS { returnCode=0, handCnt, deckCnt, gameCnt};
enum RESULTS { pass=0, fail, total };
int testCounter[4][3] = {{0}};

int myAssert(bool assertion, char* testName, int testNum, struct gameState *pre, struct gameState *post, int p) {
    if ( assertion ) {
        // printf(GRN "TEST PASSED");
        // printf(RESET "\n");
        testCounter[testNum][pass]++;
        testCounter[testNum][total]++;
        return 0;
    }
    else {
        // printf(RED "TEST FAILED, %s", testName);
        // printf(RESET "\n");
        testCounter[testNum][fail]++;
        testCounter[testNum][total]++;
        // printf ("adventurer PRE: p %d HC %d DeC %d DiC %d Deck: %d, %d \n",
        // p, pre->handCount[p], pre->deckCount[p], pre->discardCount[p],
        // pre->deck[p][pre->deckCount[p]-1],
        // pre->deck[p][pre->deckCount[p]-2]);
        // printf ("adventurer POST: p %d HC %d DeC %d DiC %d Hand: %d, %d Disc: %d \n",
        // p, post->handCount[p], post->deckCount[p], post->discardCount[p],
        // post->hand[p][post->handCount[p]-1],
        // post->hand[p][post->handCount[p]-2],
        // post->discard[p][post->discardCount[p]-1]);
        return 1;
    }
}

int showResults (){
    printf("---------- TEST RESULTS START ---------\n");
    printf("Return code passed: %d/%d, failed: %d/%d\n",
      testCounter[returnCode][pass], testCounter[returnCode][total],
      testCounter[returnCode][fail], testCounter[returnCode][total]);
    printf("Hand count passed: %d/%d, failed: %d/%d\n",
      testCounter[handCnt][pass], testCounter[handCnt][total],
      testCounter[handCnt][fail], testCounter[handCnt][total]);
    printf("DeckCount & discardCount passed: %d/%d, failed: %d/%d\n",
      testCounter[deckCnt][pass], testCounter[deckCnt][total],
      testCounter[deckCnt][fail], testCounter[deckCnt][total]);
    printf("Game state passed: %d/%d, failed: %d/%d\n",
      testCounter[gameCnt][pass], testCounter[gameCnt][total],
      testCounter[gameCnt][fail], testCounter[gameCnt][total]);
    printf("---------- TEST RESULTS END ---------\n");
}


int checkAdventurerCard(int p, struct gameState *post, int temphand[]) {
  struct gameState pre, pre_bk;
  memcpy (&pre, post, sizeof(struct gameState));
  memcpy (&pre_bk, post, sizeof(struct gameState));

  int r = adventurerEffect(post, p);

  if ( post->discardCount[p] - pre.discardCount[p] > 0) {
      pre.handCount[p] += 2;
      pre.hand[p][pre.handCount[p]-1] = copper;
      pre.hand[p][pre.handCount[p]-2] = copper;
      pre.deckCount[p] -= 2;
      pre.discard[p][pre.discardCount[p]-1] = estate;
  } else {
      pre.handCount[p] += 2;
      pre.hand[p][pre.handCount[p]-1] = copper;
      pre.hand[p][pre.handCount[p]-2] = copper;
      pre.deckCount[p] -= 2;
  }

  myAssert(r == 0, "Function return 0", returnCode, &pre_bk, post, p);
  myAssert(pre.handCount[p] == post->handCount[p], "handCount test", handCnt, &pre_bk, post, p);
  myAssert(pre.deckCount[p] + pre.discardCount[p] == post->deckCount[p] + post->discardCount[p], "deckCount & discardCount test", deckCnt, &pre_bk, post, p);
  myAssert(memcmp(&pre, post, sizeof(struct gameState)) == 0, "Game state test", gameCnt, &pre_bk, post, p);
}

int main () {

  int i, n, j, p, deckCount, discardCount, handCount;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

  struct gameState G;

  printf ("Testing smithy card.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
      for (i = 0; i < sizeof(struct gameState); i++) {
          ((char*)&G)[i] = floor(Random() * 256);
      }
      p = floor(Random() * 2);
      G.deckCount[p] = floor(Random() * MAX_DECK);
      G.discardCount[p] = floor(Random() * MAX_DECK);
      G.handCount[p] = floor(Random() * MAX_HAND);
      G.playedCardCount = floor(Random() * MAX_DECK);   // Need proper initialize to prevent segment fault
      G.deckCount[p] = 0;
      for (j = 0; j < 3; j++) {
        G.deck[p][j] = estate;
        G.deckCount[p]++;
      }
      for (j = 3; j < 10; j++) {
        G.deck[p][j] = copper;
        G.deckCount[p]++;
      }
      shuffle(p, &G);
      int temphand[MAX_HAND];
      checkAdventurerCard(p, &G, temphand);
  }

  showResults();

  exit(0);

  return 0;
}
