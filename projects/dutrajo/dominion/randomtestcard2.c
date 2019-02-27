#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

//set array size for output strings
#define MAX_STRING_SIZE 10000

/*
	void initializeString(
						char* str,
						int size
						)
	Parameters:
		char* str: char* array to initialize
		int size: size of char* array
	Returns:
		Nothing
	Description:
		Will set all of the char array to '\0' characters
		to ensure a proper formatted string.
*/
void initializeString(char *str, int size)
{
	//validate pointer isn't null
	if (str != NULL)
	{
		//initialize array to all '\0' characters
		memset(str, '\0', sizeof(char) * size);
	}
}

/*
	void customPrintf(
						char* line,
						FILE* outputFile
						)
	Parameters:
		char* line: string to output,
		FILE* outputFile: file pointer to output file (can be null)
	Returns:
		Nothing
	Description:
		Will output a string (ends with '\0') to the console as well
		as to the output file if the pointer isn't null.
*/
void customPrintf(char* line, FILE* outputFile)
{
	//output the line to the console
	printf("%s", line);

	//check if the file pointer is null
	if (outputFile != NULL)
	{
		//not null, so output to file
		fprintf(outputFile, "%s", line);
	}

	//re-initialize string 
	initializeString(line, MAX_STRING_SIZE);
}

/*
	void assertTrue(
		int foundResult, int expectedResult, char *strGoodResult, char *strBadResult, FILE *outputFile)
	Parameters:
		int foundResult: result returned from a function call
		int expectedResult: result to compare foundResult to
		char *strGoodResult: message to output if values match
		char *strBadResult: message to output if values differ
		FILE *outputFile: file pointer to output message to
	Returns:
		int, 0 = fail, 1 = pass
	Description:
		Will compare two provided inputes and display a message (and write to file) depending on
		if the two values match or not.
*/
int assertTrue(int foundResult, int expectedResult, char *strGoodResult, char *strBadResult, FILE *outputFile)
{
	//check if values match
	if (foundResult != expectedResult)
	{
		//values don't match output bad result string
		printf("%s", strBadResult);
		fprintf(outputFile, "%s", strBadResult);

		//reinitialize strings
		initializeString(strGoodResult, MAX_STRING_SIZE);
		initializeString(strBadResult, MAX_STRING_SIZE);
		return 0;
	}
	else
	{
		//values match, output good result string
		printf("%s", strGoodResult);
		fprintf(outputFile, "%s", strGoodResult);
	}

	initializeString(strGoodResult, MAX_STRING_SIZE);
	initializeString(strBadResult, MAX_STRING_SIZE);
	return 1;
}


/*
	int validateRemodelCardEffect(
							int playerIndex,
							int indexHandCardPlayed,
							int handCardPlayed,
							int choice1,
							int choice2,
							int choice3,
							int *bonus,
							struct gameState *G,
							int detailedOutput,
							FILE* outputFile,
							int expectedPassOrFail
						)
	Parameters:
		int playerIndex: index of player who is playing card
		int indexHandCardPlayed: index of the played card in player's hand
		int handCardPlayed: value of the card that was played
		int choice1: index of card in player's hand to trash
		int choice2: card type of card to purchase from supply
		int choice3: not used (any int is fine)
		int *bonus: bonus coins pointer
		struct gameState *G: pointer to gameState object
		int detailedOutput: 1 = output status, anything else don't
		FILE* outputFile: pointer to output file (can be null)
		int expectedPassOrFail: indicator if function was supposed to pass or fail
	Returns:
		int 0 = failure, 1 = success
	Description:
		Will test the cardEffect for 'steward' card. Returns 0 if test
		fails, 1 if test is successful.
*/
int validateRemodelCardEffect(
	int playerIndex,
	int indexHandCardPlayed,
	int handCardPlayed,
	int choice1,
	int choice2,
	int choice3,
	int *bonus,
	struct gameState *G,
	FILE* outputFile,
	int expectedPassOrFail //0 = expect fail, 1 = expect pass
)
{
	//create back gamestate for testing
	struct gameState tempG;

	//first copy gameState to test object
	memcpy(&tempG, G, sizeof(struct gameState));

	//create dynamic memory for output strings
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strPass = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strFail = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize output string
	initializeString(str, MAX_STRING_SIZE);
	initializeString(strPass, MAX_STRING_SIZE);
	initializeString(strFail, MAX_STRING_SIZE);

	//indicator for individual test pass
	int passesCheck = 1;

	//counter variable
	int i = 0;

	//indiciator for all test pass
	int overallTestPass = 1;

	//arrays to hold card counts (all used just for playerIndex)
	int *origDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *newDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *origDiscardCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *newDiscardCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *origHandCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *newHandCounts = malloc(sizeof(int) * (treasure_map + 1));

	//initializing counters
	for (i = 0; i < treasure_map + 1; i++)
	{
		origDeckCounts[i] = newDeckCounts[i] = origDiscardCounts[i] = newDiscardCounts[i] = origHandCounts[i] = newHandCounts[i] = 0;
	}

	//now fill original counters (validate values are in valid range)
	for (i = 0; i < G->deckCount[playerIndex]; i++)
	{
		if(G->deck[playerIndex][i] >= 0 && G->deck[playerIndex][i] < treasure_map + 1)
			origDeckCounts[G->deck[playerIndex][i]]++;
	}

	for (i = 0; i < G->discardCount[playerIndex]; i++)
	{
		if (G->discard[playerIndex][i] >= 0 && G->discard[playerIndex][i] < treasure_map + 1)
			origDiscardCounts[G->discard[playerIndex][i]]++;
	}

	for (i = 0; i < G->handCount[playerIndex]; i++)
	{
		if (G->hand[playerIndex][i] >= 0 && G->hand[playerIndex][i] < treasure_map + 1)
			origHandCounts[G->hand[playerIndex][i]]++;
	}

	//set check
	passesCheck = 1;

	//execute the card effect
	sprintf(str, "\tRunning cardEffect For Remodel....");
	customPrintf(str, outputFile);

	//run the function
	int res = cardEffect(handCardPlayed, choice1, choice2, choice3, &tempG, indexHandCardPlayed, bonus);

	sprintf(strPass, "Expected %d, function returned %d. Passed.\n", expectedPassOrFail, res);
	sprintf(strFail, "Expected %d, function returned %d. Failed!!!\n", expectedPassOrFail, res);

	passesCheck = assertTrue(res, expectedPassOrFail, strPass, strFail, outputFile);
	overallTestPass &= 0;


	//now check deck changes
	for (i = 0; i < tempG.deckCount[playerIndex]; i++)
	{
		if (tempG.deck[playerIndex][i] >= 0 && tempG.deck[playerIndex][i] < treasure_map + 1)
			newDeckCounts[tempG.deck[playerIndex][i]]++;
	}

	for (i = 0; i < tempG.discardCount[playerIndex]; i++)
	{
		if (tempG.discard[playerIndex][i] >= 0 && tempG.discard[playerIndex][i] < treasure_map + 1)
			newDiscardCounts[tempG.discard[playerIndex][i]]++;
	}

	for (i = 0; i < tempG.handCount[playerIndex]; i++)
	{
		if (tempG.hand[playerIndex][i] >= 0 && tempG.hand[playerIndex][i] < treasure_map + 1)
			newHandCounts[tempG.hand[playerIndex][i]]++;
	}

	//reset check
	passesCheck = 1;

	//checking trashed card isn't card played
	sprintf(str, "\tChecking trashed card isn't remodel card played...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//if card to trash is same as one played, maybe we had two in hand?
	passesCheck = assertTrue(handCardPlayed == G->hand[playerIndex][choice1] && origHandCounts[handCardPlayed] < 2, 0, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//test purchase price of card to receive
	int costOfCardTrashed = getCost(G->hand[playerIndex][choice1]);
	int costOfCardToBuy = getCost(choice2);

	sprintf(str, "\tChecking purchased card is within purchase price (%d + 2 >= %d)...", costOfCardTrashed, costOfCardToBuy);
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//doing actual price check
	passesCheck = assertTrue(costOfCardTrashed + 2 >= costOfCardToBuy, 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's hand
	sprintf(str, "\tChecking purchased card didn't go into player's hand...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//make sure we're not trashing the same card as the type played and received
	if (handCardPlayed != choice2 && handCardPlayed != G->hand[playerIndex][choice1])
	{
		passesCheck = assertTrue(origHandCounts[choice2] == newHandCounts[choice2], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}
	else
	{
		passesCheck = assertTrue(origHandCounts[choice2] - 2 == newHandCounts[choice2], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking purchased card didn't go into player's deck...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check that deck count for card purchased didn't increase (indicating went into deck)
	passesCheck = assertTrue(origDeckCounts[choice2] == newDeckCounts[choice2], 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking purchased card went into player's discard pile...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(origDiscardCounts[choice2] + 1 == newDiscardCounts[choice2], 1, strPass, strFail, outputFile);
	overallTestPass &= 0;

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking purchased card came from supply...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(G->supplyCount[choice2] - 1 == tempG.supplyCount[choice2], 1, strPass, strFail, outputFile);
	overallTestPass &= 0;


	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking card to trash isn't in player's hand...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check that deck count for card purchased didn't increase (indicating went into deck)
	passesCheck = assertTrue(origHandCounts[G->hand[playerIndex][choice1]] - 1 == newHandCounts[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking card to trash isn't in player's deck...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check that deck count for card purchased didn't increase (indicating went into deck)
	passesCheck = assertTrue(origDeckCounts[G->hand[playerIndex][choice1]] == newDeckCounts[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking card to trash didn't end up in supply..");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check that deck count for card purchased didn't increase (indicating went into deck)
	//make sure card bought isn't same as one trashed first
	if (choice2 != G->hand[playerIndex][choice1])
	{
		passesCheck = assertTrue(G->supplyCount[G->hand[playerIndex][choice1]] == tempG.supplyCount[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}
	else
	{
		//trashed card and bought card are the same, i expect supply to drop by 1 card
		passesCheck = assertTrue(G->supplyCount[G->hand[playerIndex][choice1]] - 1 == tempG.supplyCount[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}

	//reset check
	passesCheck = 1;

	//checking purchased card didn't go to player's deck
	sprintf(str, "\tChecking card to trash isn't in player's discard pile...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");


	//check that deck count for card purchased didn't increase (indicating went into deck)
	//check if card played was same as one trashed...
	if (G->hand[playerIndex][choice1] != handCardPlayed)
	{
		passesCheck = assertTrue(origDiscardCounts[G->hand[playerIndex][choice1]] == newDiscardCounts[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}
	else
	{
		//otherwise i expect these to be different by 1
		passesCheck = assertTrue(origDiscardCounts[G->hand[playerIndex][choice1]] + 1 == newDiscardCounts[G->hand[playerIndex][choice1]], 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}
	


	//reset check
	passesCheck = 1;

	//checking played card went into discard pile
	sprintf(str, "\tChecking card played went into discard pile...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check that deck count for card purchased didn't increase (indicating went into deck)
	passesCheck = assertTrue(origDeckCounts[handCardPlayed] + 1 == newDeckCounts[handCardPlayed], 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;


	//checking that no other player's cards were modified
	//initially assume we'll pass the check
	passesCheck = 1;

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//output status to screen and file
	sprintf(str, "\tChecking all other player's cards for any modifications.\n");
	customPrintf(str, outputFile);

	for (i = 0; i < tempG.numPlayers; i++)
	{
		if (i != playerIndex)
		{
			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, hand count didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.handCount[i], G->handCount[i], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, hand cards didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(memcmp(&tempG.hand[i][0], &G->hand[i][0], sizeof(int) * tempG.handCount[i]), 0, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, deck count didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.deckCount[i], G->deckCount[i], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, deck cards didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(memcmp(&tempG.deck[i][0], &G->deck[i][0], sizeof(int) * tempG.deckCount[i]), 0, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, discard count didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.discardCount[i], G->discardCount[i], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, discard cards didn't change..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(memcmp(&tempG.discard[i][0], &G->discard[i][0], sizeof(int) * tempG.discardCount[i]), 0, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

		}
	}

	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking current player's turn hasn't changed...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//also check that player's turn hasn't changed 
	passesCheck = assertTrue(tempG.whoseTurn == G->whoseTurn, 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking player's coins weren't changed..");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(G->coins == tempG.coins, 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;


	//freeing dynamic memory
	free(origDeckCounts);
	free(newDeckCounts);
	free(origDiscardCounts);
	free(newDiscardCounts);
	free(origHandCounts);
	free(newHandCounts);
	free(str);
	free(strPass);
	free(strFail);

	return overallTestPass;
}

/*
	void generateRandomInputValuesForRemodel(
					(int *playerIndex, int *choice1, int *choice2, int *choice3, int *bonusCoins, struct gameState *inputState)
	Parameters:
		int *playerIndex: pointer to integer representing player index
		int *choice1: pointer to integer representing user choice1
		int *choice2: pointer to integer representing user choice2
		int *choice3: pointer to integer representing user choice3
		int *bonusCoins: pointer to integer representing bonus coins to received
		struct gameState *inputState: pointer to gameState to randomize
	Returns:
		Nothing
	Description:
		Will create random values for playerIndex, choice1, choice2, cohice3, bonusCoins based on card type played.
		Will also randomize the gameState object provided.
*/
void generateRandomInputValuesForRemodel(int *playerIndex, int *choice1, int *choice2, int *choice3, int *bonusCoins, struct gameState *inputState)
{
	int i = 0;

	//bonus coins don't work for remodel card
	*bonusCoins = 0;

	//max players = 4, this will generate value between 0 - 3
	*playerIndex = rand() % 4;

	//now randomly manipulate gamestate for player index chosen

	//first randomly choose number of cards in decks
	inputState->whoseTurn = *playerIndex;
	inputState->handCount[*playerIndex] = (rand() % (MAX_HAND - 1)) + 1;
	inputState->deckCount[*playerIndex] = rand() % MAX_DECK;
	inputState->discardCount[*playerIndex] = rand() % MAX_DECK;

	//now fill decks with random cards
	for (i = 0; i < inputState->handCount[*playerIndex]; i++)
	{
		inputState->hand[*playerIndex][i] = rand() % (treasure_map + 1);
	}

	for (i = 0; i < inputState->deckCount[*playerIndex]; i++)
	{
		inputState->deck[*playerIndex][i] = rand() % (treasure_map + 1);
	}

	for (i = 0; i < inputState->discardCount[*playerIndex]; i++)
	{
		inputState->discard[*playerIndex][i] = rand() % (treasure_map + 1);
	}

	//choice1 is card from hand to trash
	*choice1 = rand() % inputState->handCount[*playerIndex];

	//choice2 is card to gain from supply
	*choice2 = -1;
	while (*choice2 == -1)
	{
		int tempChoice2 = rand() % (treasure_map + 1);
		if (inputState->supplyCount[tempChoice2] > 0)
		{
			*choice2 = tempChoice2;
		}
	}

	//choice3 isn't used for remodel, and can be any integer
	*choice3 = rand() % (INT_MAX - 1);
	if (rand() % 2 == 0)
	{
		*choice3 = *choice3 * (-1);
	}


	updateCoins(*playerIndex, inputState, 0);

}

/*
	void testCardEffectRemodel()
	Parameters:
		None
	Returns:
		Nothing
	Description:
		Will test the 'remodel' cardEffect with varying inputs and initial conditions.
*/
void testCardEffectRemodel()
{
	//create two gameState objects
	struct gameState G, backupG;

	//set the initial player count to 4
	int playerCount = 4;

	//setting the playerIndex to 0'th player
	int playerIndex = 0;

	//variable to keep track of test number
	int testnum = 1;

	//initial supply cards
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, remodel, smithy, council_room };

	//bonus coins
	int bonusCoins = 0;

	//random seed
	int randomSeed = 1;

	//choices to try
	int choice1 = -1;
	int choice2 = -1;
	int choice3 = -1;

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strPass = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strFail = malloc(sizeof(char) * MAX_STRING_SIZE);

	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);
	memset(strPass, '\0', sizeof(char) * MAX_STRING_SIZE);
	memset(strFail, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("randomtestcard2.out", "w");

	int numberOfTestsToTry = 1000;
	int i = 0;


	for (i = 0; i < numberOfTestsToTry; i++)
	{
		//output current test information
		sprintf(str, "Test %d: Playing Card 'remodel', choice1: %d, choice2: %d, choice3: %d, playerIndex: %d\n", testnum++, choice1, choice2, choice3, playerIndex);
		customPrintf(str, outputFile);

		//backup gamestate to test state
		memcpy(&backupG, &G, sizeof(struct gameState));

		//randomize inputs
		generateRandomInputValuesForRemodel(&playerIndex, &choice1, &choice2, &choice3, &bonusCoins, &backupG);

		//now manipulate the cards in the player's hand to be known values
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = remodel;

		//run the function and validate it (trash cutpurse, gain adventurer)
		res = validateRemodelCardEffect(playerIndex, backupG.handCount[playerIndex] - 1, remodel, choice1, choice2, choice3, &bonusCoins, &backupG, outputFile, 0);

		sprintf(strPass, "------------- OVERALL TEST PASSED ---------------\n\n");
		sprintf(strFail, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");

		 assertTrue(res, 1, strPass, strFail, outputFile);
	}

	
	//close the output file
	fclose(outputFile);

	//free dynamic memory
	free(str);
	free(strPass);
	free(strFail);
}

/*
	int main()
	Parameters:
		None
	Returns:
		int 0
	Description:
		Will test the 'steward' cardEffect with varying inputs.
*/
int main()
{
	srand(time((time_t*)NULL));
	testCardEffectRemodel();
	return 0;
}
	

	
	
