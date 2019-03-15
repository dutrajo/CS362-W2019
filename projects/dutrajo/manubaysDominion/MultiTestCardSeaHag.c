#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
	//check if the values match
	if (foundResult != expectedResult)
	{
		//they don't match, output bad result
		printf("%s", strBadResult);
		fprintf(outputFile, "%s", strBadResult);

		//reinitialize strings and return 0
		initializeString(strGoodResult, MAX_STRING_SIZE);
		initializeString(strBadResult, MAX_STRING_SIZE);
		return 0;
	}
	else
	{

		//values match, output good result
		printf("%s", strGoodResult);
		fprintf(outputFile, "%s", strGoodResult);
	}

	initializeString(strGoodResult, MAX_STRING_SIZE);
	initializeString(strBadResult, MAX_STRING_SIZE);
	return 1;
}

/*
	int int validateSeaHagCardEffect(
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
		int choice1: Not used, any int is fine
		int choice2: Not used, any int is fine
		int choice3: Not used, any int is fine
		int *bonus: bonus coins pointer
		struct gameState *G: pointer to gameState object
		int detailedOutput: 1 = output status, anything else don't
		FILE* outputFile: pointer to output file (can be null)
		int expectedPassOrFail: indicator if function was supposed to pass or fail
	Returns:
		int 0 = failure, 1 = success
	Description:
		Will test the cardEffect for 'Adventurer' card. Returns 0 if test
		fails, 1 if test is successful.
*/
int validateSeaHagCardEffect(
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

	//testing gameState
	struct gameState tempG;

	//first copy gameState to test object
	memcpy(&tempG, G, sizeof(struct gameState));

	//create dynamic memory for output string
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strPass = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strFail = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize output string
	initializeString(str, MAX_STRING_SIZE);
	initializeString(strPass, MAX_STRING_SIZE);
	initializeString(strFail, MAX_STRING_SIZE);

	//indicator for individual test pass
	int passesCheck = 1;

	//indiciator for all test pass
	int overallTestPass = 1;

	//counter variable
	int i = 0;

	//result values
	//int expected = 0;
	//int found = 0;

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
		if (G->deck[playerIndex][i] >= 0 && G->deck[playerIndex][i] < treasure_map + 1)
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
	sprintf(str, "\tRunning cardEffect For Sea Hag....");
	customPrintf(str, outputFile);

	//run the function
	int res = cardEffect(handCardPlayed, choice1, choice2, choice3, &tempG, indexHandCardPlayed, bonus);

	sprintf(strPass, "Expected %d, function returned %d. Passed.\n", expectedPassOrFail, res);
	sprintf(strFail, "Expected %d, function returned %d. Failed!!!\n", expectedPassOrFail, res);

	passesCheck = assertTrue(res, expectedPassOrFail, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//now fill new card counter values
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

	//output status to screen and file
	sprintf(str, "\tChecking player's hand count decreased by 1 card (played card)...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(tempG.handCount[playerIndex], G->handCount[playerIndex] - 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking played card was moved to discard pile...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");


	//check if played card was removed from hand
	passesCheck = assertTrue(newDiscardCounts[handCardPlayed] == origDiscardCounts[handCardPlayed] + 1, 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking player's deck count stayed the same...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(tempG.deckCount[playerIndex], G->deckCount[playerIndex], strPass, strFail, outputFile);
	overallTestPass &= passesCheck;


	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking supply counts didn't change...");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	int tempCheck = 1;

	for (i = 0; i < treasure_map + 1; i++)
	{
		if (tempG.supplyCount[i] != G->supplyCount[i])
		{
			tempCheck = 0;
			break;
		}
	}

	passesCheck = assertTrue(tempCheck, 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;


	//checking that no other player's cards were modified
	//initially assume we'll pass the check
	passesCheck = 1;

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//output status to screen and file
	sprintf(str, "\tChecking all other player's cards (+1 curse to all other players decks).\n");
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
			sprintf(str, "\tChecking player: %d, deck count didn't change (+1 curse, -1 top card)..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.deckCount[i], G->deckCount[i], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, deck cards did change (+1 card curse)..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			int *origPlayerDeck = malloc(sizeof(int) * (treasure_map + 1));
			int *newPlayerDeck = malloc(sizeof(int) * (treasure_map + 1));
			int j = 0;
			int checkTest = 1;
			for (j = 0; j < treasure_map + 1; j++) origPlayerDeck[j] = newPlayerDeck[j] = 0;

			for (j = 0; j <  tempG.deckCount[i]; j++)
			{
				newPlayerDeck[tempG.deck[i][j]]++;
			}
			for (j = 0; j < G->deckCount[i]; j++)
			{
				origPlayerDeck[G->deck[i][j]]++;
			}
			for (j = 0; j < treasure_map; j++)
			{
				if (j == curse)
				{
					if (newPlayerDeck[j] != origPlayerDeck[j] + 1)
					{
						checkTest = 0;
						break;
					}
				}
				else
				{
					if (newPlayerDeck[j] != origPlayerDeck[j])
					{
						//check if this card was discarded
						if (G->deck[i][G->deckCount[i] - 1] != tempG.discard[i][tempG.discardCount[i] - 1])
						{
							checkTest = 0;
							break;
						}
						
					}
				}
			}
			passesCheck = assertTrue(checkTest, 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			free(origPlayerDeck);
			free(newPlayerDeck);
			origPlayerDeck = newPlayerDeck = 0;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, discard count changed +1 card..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.discardCount[i], G->discardCount[i] + 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;

			//initially assume we'll pass the check
			passesCheck = 1;

			//output status to screen and file
			sprintf(str, "\tChecking player: %d, discard cards correctly changed..", i);
			customPrintf(str, outputFile);

			sprintf(strPass, "Passed.\n");
			sprintf(strFail, "Failed!!!\n");

			passesCheck = assertTrue(tempG.discard[i][tempG.discardCount[i] - 1], G->deck[i][G->deckCount[i] - 1], strPass, strFail, outputFile);
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

	//reset check
	passesCheck = 1;

	//output status to screen and file
	sprintf(str, "\tChecking player's actions were changed as expected..");
	customPrintf(str, outputFile);

	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	passesCheck = assertTrue(tempG.numActions == G->numActions, 1, strPass, strFail, outputFile);
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
	void generateRandomInputValuesForSeaHag(
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
void generateRandomInputValuesForSeaHag(int *playerIndex, int *choice1, int *choice2, int *choice3, int *bonusCoins, struct gameState *inputState)
{
	int i = 0;

	//bonus coins don't work for adventurer card
	*bonusCoins = 0;

	//max players = 4, this will generate value between 0 - 3
	*playerIndex = rand() % 4;

	//now randomly manipulate gamestate for player index chosen

	//first randomly choose number of cards in decks
	inputState->whoseTurn = *playerIndex;
	inputState->handCount[*playerIndex] = (rand() % (MAX_HAND - 1));
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

	//choice1 isn't used for smithy, and can be any integer
	*choice1 = rand() % (INT_MAX - 1);
	if (rand() % 2 == 0)
	{
		*choice1 = *choice1 * (-1);
	}

	//choice2 isn't used for smithy, and can be any integer
	*choice2 = rand() % (INT_MAX - 1);
	if (rand() % 2 == 0)
	{
		*choice2 = *choice2 * (-1);
	}

	//choice3 isn't used for smithy, and can be any integer
	*choice3 = rand() % (INT_MAX - 1);
	if (rand() % 2 == 0)
	{
		*choice3 = *choice3 * (-1);
	}


	updateCoins(*playerIndex, inputState, 0);

}

/*
	void multiTestCardEffectSmithy()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the adventurer card effect. It
		will call the function multiple times and peform numerous tests to try and
		locate any bugs that it may have.
*/
void multiTestCardEffectSeaHag()
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

	//loop counter
	int i = 0;

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
	FILE *outputFile = fopen("MultiTestCardSeaHag.out", "w");

	//RANDOM TEST SECTION
	int numberOfRandomTests = 50;
	for (i = 0; i < numberOfRandomTests; i++)
	{

		//backup game for testing
		memcpy(&backupG, &G, sizeof(struct gameState));

		//randomize inputs
		generateRandomInputValuesForSeaHag(&playerIndex, &choice1, &choice2, &choice3, &bonusCoins, &backupG);

		//now add the adventurer card to the last spot in the hand
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = sea_hag;

		//output current test information
		sprintf(str, "Test %d: Playing Card 'sea hag', playerIndex: %d, choice1: %d, choice2: %d, choice3: %d\n", testnum++, playerIndex, choice1, choice2, choice3);
		customPrintf(str, outputFile);

		//run the function and validate it
		res = validateSeaHagCardEffect(playerIndex, backupG.handCount[playerIndex] - 1, sea_hag, choice1, choice2, choice3, &bonusCoins, &backupG, outputFile, 0);

		sprintf(strPass, "------------- OVERALL TEST PASSED ---------------\n\n");
		sprintf(strFail, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");

		//check the result and output status
		assertTrue(res, 1, strPass, strFail, outputFile);
	}

	//MANUAL TESTS SECTION

	//make a backup of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//manually set inputs
	playerIndex = 1;
	backupG.whoseTurn = playerIndex;

	choice1 = 1;
	choice2 = 2;
	choice3 = 3;
	bonusCoins = 0;
	int t = 0;
	for (t = 0; t < MAX_PLAYERS; t++)
	{
		backupG.handCount[t] = backupG.deckCount[t] = backupG.discardCount[t] = 0;
		for (i = 0; i < MAX_HAND; i++) backupG.hand[t][i] = 0;
		for (i = 0; i < MAX_DECK; i++)
		{
			backupG.deck[t][i] = 0;
			backupG.discard[t][i] = 0;
		}

		//set cards 
		backupG.deck[t][backupG.deckCount[t]++] = copper;
		backupG.deck[t][backupG.deckCount[t]++] = silver;
		backupG.deck[t][backupG.deckCount[t]++] = gold;

		backupG.hand[t][backupG.handCount[t]++] = copper;
		backupG.hand[t][backupG.handCount[t]++] = silver;
		backupG.hand[t][backupG.handCount[t]++] = gold;
		
	}

	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = sea_hag;

	//output current test information
	sprintf(str, "Test %d: Playing Card 'sea hag', playerIndex: %d, choice1: %d, choice2: %d, choice3: %d\n", testnum++, playerIndex, choice1, choice2, choice3);
	customPrintf(str, outputFile);

	//run the function and validate it
	res = validateSeaHagCardEffect(playerIndex, backupG.handCount[playerIndex] - 1, sea_hag, choice1, choice2, choice3, &bonusCoins, &backupG, outputFile, 0);

	sprintf(strPass, "------------- OVERALL TEST PASSED ---------------\n\n");
	sprintf(strFail, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");

	//check the result and output status
	assertTrue(res, 1, strPass, strFail, outputFile);

	//make a backup of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//manually set inputs
	playerIndex = 1;
	backupG.whoseTurn = playerIndex;

	choice1 = 1;
	choice2 = 2;
	choice3 = 3;
	bonusCoins = 0;
	for (t = 0; t < MAX_PLAYERS; t++)
	{
		backupG.handCount[t] = backupG.deckCount[t] = backupG.discardCount[t] = 0;
		for (i = 0; i < MAX_HAND; i++) backupG.hand[t][i] = 0;
		for (i = 0; i < MAX_DECK; i++)
		{
			backupG.deck[t][i] = 0;
			backupG.discard[t][i] = 0;
		}

		//set cards 
		backupG.deck[t][backupG.deckCount[t]++] = estate;

		backupG.hand[t][backupG.handCount[t]++] = copper;

	}

	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = sea_hag;

	//output current test information
	sprintf(str, "Test %d: Playing Card 'sea hag', playerIndex: %d, choice1: %d, choice2: %d, choice3: %d\n", testnum++, playerIndex, choice1, choice2, choice3);
	customPrintf(str, outputFile);

	//run the function and validate it
	res = validateSeaHagCardEffect(playerIndex, backupG.handCount[playerIndex] - 1, sea_hag, choice1, choice2, choice3, &bonusCoins, &backupG, outputFile, 0);

	sprintf(strPass, "------------- OVERALL TEST PASSED ---------------\n\n");
	sprintf(strFail, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");

	//check the result and output status
	assertTrue(res, 1, strPass, strFail, outputFile);

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
		None.
	Returns:
		int 0
	Description:
		main launching point for the program.
*/
int main()
{
	srand(time((time_t*)NULL));
	multiTestCardEffectSeaHag();
	return 0;
}

