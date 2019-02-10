#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

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
	int validateFullDeckCountFunction(
		int playerIndex,
		int cardToFind,
		struct gameState *G,
		int detailedOutput,
		FILE* outputFile,
		int expectedPassOrFail,
		int exitAfterFunctionFail
		)
	Parameters:
		int playerIndex: index of player whose turn it is
		int cardToFind: card value to find in decks
		struct gameState *G: gameState to use
		int detailedOutput: indicate if detailed output is needed
		FILE* outputFile: file to output results to (can be null)
		int expectedPassOrFail: what we expect the initial function call to return
		int exitAfterFunctionFail: if we should stop test if we don't get expected output
	Returns:
		int, 0 if fail, 1 if succes
	Description:
		This function can be used to perform a single test on a valid gameState object
		using the fullDeckCount function. It will find how many of a specified card
		exists in the player's hand, deck, and discard pile. This function will validate
		the function by performed tests and checks.
*/
int validateFullDeckCountFunction(
	int playerIndex,
	int cardToFind,
	struct gameState *G,
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail, //0 = expect fail, 1 = expect pass
	int exitAfterFunctionFail //1 = exit test if function != expected
)
{
	//gameState object to use
	struct gameState tempG;

	//create dynamic memory for output string
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize the string
	initializeString(str, MAX_STRING_SIZE);

	//first copy gameState to test object
	memcpy(&tempG, G, sizeof(struct gameState));

	//indicator for individual test pass
	int passesCheck = 1;

	//indiciator for all test pass
	int overallTestPass = 1;

	//counter variable
	int i = 0;
	int j = 0;

	//results stuff
	int expected = 0;
	int found = 0;

	//set check
	passesCheck = 1;

	//execute the card effect
	sprintf(str, "\tRunning function fullDeckCount()....");
	if (detailedOutput) customPrintf(str, outputFile);

	found = fullDeckCount(playerIndex, cardToFind, &tempG);
	expected = expectedPassOrFail;

	if (found != expected)
	{
		passesCheck = overallTestPass = 0;
	}

	//output status to screen and file
	if (detailedOutput)
	{
		if (passesCheck == 1)
		{
			sprintf(str, "Expected: %d, Found: %d. Passed.\n", expected, found);
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Expected: %d, Found: %d. Failed!!!!!\n", expected, found);
			customPrintf(str, outputFile);
		}
	}

	//checking function result
	if (found != expected && exitAfterFunctionFail == 1)
	{
		overallTestPass = 0;
		return overallTestPass; //break out of function early
	}


	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's coins weren't modified...");
		customPrintf(str, outputFile);
	}

	if (tempG.coins != G->coins)
	{
		passesCheck = overallTestPass = 0;
	}

	//output status to screen and file
	if (detailedOutput)
	{
		if (passesCheck == 1)
		{
			sprintf(str, "Passed.\n");
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Failed!!!!!\n");
			customPrintf(str, outputFile);
		}
	}


	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking ALL player's hands, decks, and discards weren't changed...");
		customPrintf(str, outputFile);
	}

	//verify all player's hands, decks, discard piles are identical to original values before function execution
	for (i = 0; i < tempG.numPlayers; i++)
	{
		//jump out of loop if something fails
		if (!passesCheck) break;

		//make sure hand counts are the same
		if (tempG.handCount[i] == G->handCount[i])
		{
			//check player's hand for changes
			for (j = 0; j < tempG.handCount[i]; j++)
			{
				//if the hand cards aren't the same
				if (tempG.hand[i][j] != G->hand[i][j])
				{
					//fails check...
					passesCheck = overallTestPass = 0;
					break;
				}
			}
		}
		else
		{
			//original hand and new hand aren't the same count
			passesCheck = overallTestPass = 0;
			break;
		}

		//make sure deck counts are the same
		if (tempG.deckCount[i] == G->deckCount[i])
		{
			//iterate through cards and make sure identical
			for (j = 0; j < tempG.deckCount[i]; j++)
			{
				//check if cards are the same
				if (tempG.deck[i][j] != G->deck[i][j])
				{
					//fails check...
					passesCheck = overallTestPass = 0;
					break;
				}
			}
		}
		else
		{
			//original deck count doesn't match new deck count
			passesCheck = 0;
			break;
		}

		//check discard piles are the same
		if (tempG.discardCount[i] == G->discardCount[i])
		{
			//iterate through discard piles and check for identical
			if (tempG.discard[i][j] != G->discard[i][j])
			{
				//fails check....
				passesCheck = overallTestPass = 0;
				break;
			}
		}
		else
		{
			//original discard pile doesn't match new one
			passesCheck = overallTestPass = 0;
			break;
		}

	}

	//output status to screen and file
	if (detailedOutput)
	{
		if (passesCheck == 1)
		{
			sprintf(str, "Passed.\n");
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Failed!!!!!\n");
			customPrintf(str, outputFile);
		}
	}

	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking current player's turn hasn't changed...");
		customPrintf(str, outputFile);
	}

	//also check that player's turn hasn't changed 
	if (tempG.whoseTurn != G->whoseTurn)
	{
		//function modified turn inside card effect (shouldn't happen)
		passesCheck = overallTestPass = 0;
	}

	//output status to screen and file
	if (detailedOutput)
	{
		if (passesCheck == 1)
		{
			sprintf(str, "Passed.\n");
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Failed!!!!!\n");
			customPrintf(str, outputFile);
		}
	}

	free(str);

	return overallTestPass;
}

/*
	void testFullDeckCountFunction()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the fullDeckCount function
		by calling the test harness function above. Test results will be
		both output to screen and to a text file.
*/
void testFullDeckCountFunction()
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

	//random seed
	int randomSeed = 1;

	//loop counter
	int i = 0;

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("unittest3.out", "w");

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for copper, with zero in any of player's decks.\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//also manipulate discardDeck (should be zero anyways)
	backupG.discardCount[playerIndex] = 0;

	res = validateFullDeckCountFunction(playerIndex, copper, &backupG, 1, outputFile, 0, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);


	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for copper, with only 1 in player's hand.\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//also manipulate discardDeck (should be zero anyways)
	backupG.discardCount[playerIndex] = 0;

	res = validateFullDeckCountFunction(playerIndex, copper, &backupG, 1, outputFile, 1, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for copper, with 1 in player's hand, and 1 in deck.\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = copper;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//also manipulate discardDeck (should be zero anyways)
	backupG.discardCount[playerIndex] = 0;

	res = validateFullDeckCountFunction(playerIndex, copper, &backupG, 1, outputFile, 2, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for copper, with 1 in player's hand, 1 in deck, and 1 in discard.\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = copper;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//also manipulate discardDeck 
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;


	res = validateFullDeckCountFunction(playerIndex, copper, &backupG, 1, outputFile, 3, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for copper, with MAX_HAND (%d) in player's hand.\n", testnum++, MAX_HAND);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	for (i = 0; i < MAX_HAND; i++)
	{
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	}

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	}

	//also manipulate discardDeck 
	backupG.discardCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	}


	res = validateFullDeckCountFunction(playerIndex, copper, &backupG, 1, outputFile, MAX_HAND, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for estate, with %d in player's hand, and %d in deck.\n", testnum++, MAX_HAND, MAX_DECK);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	for (i = 0; i < MAX_HAND; i++)
	{
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = estate;
	}

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	}

	//also manipulate discardDeck 
	backupG.discardCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	}

	res = validateFullDeckCountFunction(playerIndex, estate, &backupG, 1, outputFile, MAX_HAND + MAX_DECK, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//output current test information
	sprintf(str, "Test %d: testing fullDeckCount, looking for adventurer, with %d in player's hand, %d in deck, and %d in discard.\n", testnum++, MAX_HAND, MAX_DECK, MAX_DECK);
	customPrintf(str, outputFile);

	//make copy of G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	for (i = 0; i < MAX_HAND; i++)
	{
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;
	}

	//also manipulate player's deck to be known values
	//so that we enter all parts of the test function
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = adventurer;
	}

	//also manipulate discardDeck 
	backupG.discardCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = adventurer;
	}

	res = validateFullDeckCountFunction(playerIndex, adventurer, &backupG, 1, outputFile, MAX_HAND + MAX_DECK + MAX_DECK, 1);

	//check the result and output status
	if (res == 1)
	{
		sprintf(str, "------------- OVERALL TEST PASSED ---------------\n\n");
	}
	else
	{
		sprintf(str, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");
	}
	customPrintf(str, outputFile);

	//close the output file
	fclose(outputFile);

	//free dynamic memory
	free(str);
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
	testFullDeckCountFunction();
	return 0;
}
