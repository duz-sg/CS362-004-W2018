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

enum TESTS { returnCode=0, coinCnt, handCnt, tokenCnt, token, playedCnt, gameCnt};
enum RESULTS { pass=0, fail, total };
int testCounter[7][3] = {{0}};

int myAssert(bool assertion, char* testName, int testNum, struct gameState *pre, struct gameState *post, int p, int choice1, int choice2) {
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
        // printf ("smithy PRE: p %d HC %d DeC %d DiC %d Token: %d, %d \n",
        // p, pre->handCount[p], pre->deckCount[p], pre->discardCount[p],
        // pre->embargoTokens[choice1],
        // pre->embargoTokens[choice1]);
        // printf ("smithy POST: p %d HC %d DeC %d DiC %d Token: %d, %d \n",
        // p, post->handCount[p], post->deckCount[p], post->discardCount[p],
        // post->embargoTokens[choice1],
        // post->embargoTokens[choice1]);
        return 1;
    }
}

int getTokenCount(struct gameState *pre, struct gameState *post) {
    int i = 0;
    int count = 0;
    for (i = 0; i <= treasure_map; i++) {
        if (pre->embargoTokens[i] != post->embargoTokens[i]) {
            count++;
        }
    }
    return count;
}

int showResults (){
    printf("---------- TEST RESULTS START ---------\n");
    printf("Return code passed: %d/%d, failed: %d/%d\n",
      testCounter[returnCode][pass], testCounter[returnCode][total],
      testCounter[returnCode][fail], testCounter[returnCode][total]);
    printf("Coin count passed: %d/%d, failed: %d/%d\n",
      testCounter[coinCnt][pass], testCounter[coinCnt][total],
      testCounter[coinCnt][fail], testCounter[coinCnt][total]);
    printf("Hand count passed: %d/%d, failed: %d/%d\n",
      testCounter[handCnt][pass], testCounter[handCnt][total],
      testCounter[handCnt][fail], testCounter[handCnt][total]);
    printf("Embargo token passed: %d/%d, failed: %d/%d\n",
      testCounter[tokenCnt][pass], testCounter[tokenCnt][total],
      testCounter[tokenCnt][fail], testCounter[tokenCnt][total]);
    printf("Embargo token count passed: %d/%d, failed: %d/%d\n",
      testCounter[token][pass], testCounter[token][total],
      testCounter[token][fail], testCounter[token][total]);
    printf("Game state passed: %d/%d, failed: %d/%d\n",
      testCounter[gameCnt][pass], testCounter[gameCnt][total],
      testCounter[gameCnt][fail], testCounter[gameCnt][total]);
    printf("---------- TEST RESULTS END ---------\n");
}


int checkEmbargoCard(int p, struct gameState *post, int handPos, int choice1, int choice2) {
  struct gameState pre, pre_bk;
  memcpy (&pre, post, sizeof(struct gameState));
  memcpy (&pre_bk, post, sizeof(struct gameState));

  int r = embargoEffect(p, post, handPos, choice1, choice2);

  pre.coins += 2;
  pre.handCount[p]--;
  pre.embargoTokens[choice1]++;
  pre.hand[p][handPos] = -1;

  myAssert(r == 0, "Function return 0", returnCode, &pre_bk, post, p, choice1, choice2);
  myAssert(pre.coins == post->coins, "Coins test", coinCnt, &pre_bk, post, p, choice1, choice2);
  myAssert(pre.handCount[p] == post->handCount[p], "Hand count test", handCnt, &pre_bk, post, p, choice1, choice2);
  myAssert(pre.embargoTokens[choice1] == post->embargoTokens[choice1], "Token test", tokenCnt, &pre_bk, post, p, choice1, choice2);
  myAssert(getTokenCount(&pre, post) == 0, "Token count test", token, &pre_bk, post, p, choice1, choice2);
  myAssert(pre.hand[p][handPos] == post->hand[p][handPos], "Played card test", playedCnt, &pre_bk, post, p, choice1, choice2);
  myAssert(memcmp(&pre, post, sizeof(struct gameState)) == 0, "Game state test", gameCnt, &pre_bk, post, p, choice1, choice2);
}

int main () {

  int i, n, r, p, deckCount, discardCount, handCount;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

  struct gameState G;

  printf ("Testing embargo card.\n");

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
      int handPos = G.handCount[p]-1;
      int choice1 = floor(Random() * treasure_map);
      int choice2 = floor(Random() * treasure_map);
      checkEmbargoCard(p, &G, handPos, choice1, choice2);
  }

  showResults();

  exit(0);

  return 0;
}
