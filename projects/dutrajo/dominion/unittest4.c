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
	int validateIsGameOverFunction(
		int *kingdomCards,
		int kingdomCardsSize,
		struct gameState *G,
		int detailedOutput,
		FILE* outputFile,
		int expectedPassOrFail,
		int exitAfterFunctionFail
		)
	Parameters:
		int *kingdomCards: pointer to an array of kingdom cards
		int kingdomCardsSize: int size of array pointed to 
		struct gameState *G: gameState to use
		int detailedOutput: indicate if detailed output is needed
		FILE* outputFile: file to output results to (can be null)
		int expectedPassOrFail: what we expect the initial function call to return
		int exitAfterFunctionFail: if we should stop test if we don't get expected output
	Returns:
		int, 0 if fail, 1 if succes
	Description:
		This function can be used to perform a single test on a valid gameState object
		using the isGameOver function. It will determine if the current game is over
		by calling the isGameOver function and then testing to ensure that the function
		didn't modify anything it wasn't supposed to. It will also test to ensure the
		game over calcuation is done correctly.
*/
int validateIsGameOverFunction(
	int *kingdomCards,
	int kingdomCardsSize,
	struct gameState *G,
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail, //0 = expect fail, 1 = expect pass
	int exitAfterFunctionFail //1 = exit test if function != expected
)
{

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
	sprintf(str, "\tRunning function isGameOver()....");
	if (detailedOutput) customPrintf(str, outputFile);

	found = isGameOver(&tempG);
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
		sprintf(str, "\tChecking manually if game is over and comparing against function...");
		customPrintf(str, outputFile);
	}

	int gameOverVal = 0;
	int supplyPilesEmpty = 0;
	for (i = 0; i < kingdomCardsSize; i++)
	{
		if (tempG.supplyCount[kingdomCards[i]] <= 0)
		{
			supplyPilesEmpty++;
		}
	}

	if (tempG.supplyCount[province] <= 0 || supplyPilesEmpty >= 3)
	{
		gameOverVal = 1;
	}

	if (gameOverVal != found)
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
	void testIsGameOverFunction()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the isGameOver function
		by calling the test harness function above. Test results will be
		both output to screen and to a text file.
*/
void testIsGameOverFunction()
{
	//create two gameState objects
	struct gameState G, backupG;

	//set the initial player count to 4
	int playerCount = 4;

	//variable to keep track of test number
	int testnum = 1;

	//initial supply cards
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, remodel, smithy, council_room };

	//random seed
	int randomSeed = 1;

	int i = 0;

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("unittest4.out", "w");

	//output current test information
	sprintf(str, "Test %d: testing isGameOver function on newly initialized game (game not over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing isGameOver function with all supply counts set to 1 card (game not over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	for (i = 0; i < treasure_map + 1; i++) backupG.supplyCount[i] = 1;
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing isGameOver function with two supply counts set to 0 (game not over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	backupG.supplyCount[k[0]] = backupG.supplyCount[k[1]] = 0;
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing isGameOver function with three supply counts set to 0 (game is over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	backupG.supplyCount[k[0]] = backupG.supplyCount[k[1]] = backupG.supplyCount[k[2]] = 0;
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 1, 1);

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

	sprintf(str, "Test %d: testing isGameOver function with full supply but no province cards left (game is over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	backupG.supplyCount[province] = 0;
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 1, 1);

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

	sprintf(str, "Test %d: testing isGameOver function with all supply counts set to zero (game is over).\n", testnum++);
	customPrintf(str, outputFile);

	//make copy of gamestate for testing
	memcpy(&backupG, &G, sizeof(struct gameState));
	for(i = 0; i < treasure_map + 1; i++) backupG.supplyCount[i] = 0;
	res = validateIsGameOverFunction(k, 10, &backupG, 1, outputFile, 1, 1);

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
	testIsGameOverFunction();
	return 0;
}
