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
	int validateShuffleFunction(
		int playerIndex,
		struct gameState *G,
		int detailedOutput,
		FILE* outputFile,
		int expectedPassOrFail,
		int exitAfterFunctionFail
		)
	Parameters:
		int playerIndex: index of player whose turn it is
		struct gameState *G: gameState to use
		int detailedOutput: indicate if detailed output is needed
		FILE* outputFile: file to output results to (can be null)
		int expectedPassOrFail: what we expect the initial function call to return
		int exitAfterFunctionFail: if we should stop test if we don't get expected output
	Returns:
		int, 0 if fail, 1 if succes
	Description:
		This function can be used to perform a single test on a valid gameState object
		using the shuffle function. It checks that the shuffle function
		properly shuffles the player's hand and nothing else.
*/
int validateShuffleFunction(
	int playerIndex,
	struct gameState *G,
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail, //0 = expect fail, 1 = expect pass
	int exitAfterFunctionFail //1 = exit test if function != expected
)
{
	//create gameState for testing
	struct gameState tempG;
	memcpy(&tempG, G, sizeof(struct gameState));

	//create dynamic memory for output string
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize the string
	initializeString(str, MAX_STRING_SIZE);

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
	sprintf(str, "\tRunning function Shuffle()....");
	if (detailedOutput) customPrintf(str, outputFile);

	found = shuffle(playerIndex, &tempG);
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

	if (found != expected && exitAfterFunctionFail == 1)
	{
		overallTestPass = 0;
		return overallTestPass; //break out of function early
	}

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
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player deck count hasn't changed...");
		customPrintf(str, outputFile);
	}

	if (tempG.deckCount[playerIndex] != G->deckCount[playerIndex])
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
		sprintf(str, "\tChecking player's deck cards haven't changed, just the order...");
		customPrintf(str, outputFile);
	}

	for (i = 0; i < treasure_map + 1; i++)
	{
		if (newDeckCounts[i] != origDeckCounts[i])
		{
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

	//only do this test if there is more than 5 cards in it
	//random shuffle might not re-order smaller sized decks due to
	//propability stuff
	if (tempG.deckCount[playerIndex] > 5)
	{
		//reset check
		passesCheck = 0;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tChecking player's deck cards were re-ordered...");
			customPrintf(str, outputFile);
		}

		for (i = 0; i < tempG.deckCount[playerIndex]; i++)
		{
			if (tempG.deck[playerIndex][i] != G->deck[playerIndex][i])
			{
				passesCheck = 1;
				break;
			}
		}

		if (!passesCheck) overallTestPass = 0;

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
	}

	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's hand cards weren't shuffled or modified...");
		customPrintf(str, outputFile);
	}

	
	if (tempG.handCount[playerIndex] != G->handCount[playerIndex])
	{
		passesCheck = overallTestPass = 0;
	}

	for (i = 0; i < treasure_map + 1; i++)
	{
		if (newHandCounts[i] != origHandCounts[i])
		{
			passesCheck = overallTestPass = 0;
			break;
		}
	}

	for (i = 0; i < tempG.handCount[playerIndex]; i++)
	{
		if (tempG.hand[playerIndex][i] != G->hand[playerIndex][i])
		{
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
		sprintf(str, "\tChecking player's discard cards weren't shuffled or modified...");
		customPrintf(str, outputFile);
	}


	if (tempG.discardCount[playerIndex] != G->discardCount[playerIndex])
	{
		passesCheck = overallTestPass = 0;
	}

	for (i = 0; i < treasure_map + 1; i++)
	{
		if (newDiscardCounts[i] != origDiscardCounts[i])
		{
			passesCheck = overallTestPass = 0;
			break;
		}
	}

	for (i = 0; i < tempG.discardCount[playerIndex]; i++)
	{
		if (tempG.discard[playerIndex][i] != G->discard[playerIndex][i])
		{
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
		sprintf(str, "\tChecking other player's hands, decks, and discards weren't changed...");
		customPrintf(str, outputFile);
	}

	//verify all other player's hands, decks, discard piles are identical to original values before function execution
	for (i = 0; i < tempG.numPlayers; i++)
	{
		//jump out of loop if something fails
		if (!passesCheck) break;

		//skip checks for playerIndex, we'll check those decks seperately
		if (i != playerIndex)
		{
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

	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's coins weren't changed..");
		customPrintf(str, outputFile);
	}

	//check that coins weren't changed
	if (G->coins != tempG.coins)
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


	//freeing dynamic memory
	free(origDeckCounts);
	free(newDeckCounts);
	free(origDiscardCounts);
	free(newDiscardCounts);
	free(origHandCounts);
	free(newHandCounts);
	free(str);

	return overallTestPass;
}

/*
	void testShuffleFunction()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the shuffle function
		by calling the test harness function above. Test results will be
		both output to screen and to a text file.
*/
void testShuffleFunction()
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
	//int bonusCoins = 0;

	//random seed
	int randomSeed = 1;

	//loop counter
	int i = 0;
	int j = 0;

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("unittest1.out", "w");

	//output current test information
	sprintf(str, "Test %d: testing shuffle on newly initialized game.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing shuffle on ascending sorted deck.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
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
	for (i = 0; i < treasure_map + 1; i++)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = i;
	}

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = gold;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = estate;

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing shuffle on descending sorted deck.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
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
	for (i = treasure_map; i >= 0; i--)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = i;
	}

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = gold;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = estate;

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing shuffle on a full deck (%d cards).\n", testnum++, MAX_DECK);
	customPrintf(str, outputFile);

	//copy G into backup for testing
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
	j = 0;
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		if (j > treasure_map) j = 0;
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = j++;
	}

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = gold;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = estate;

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing shuffle on a deck with a single card.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
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

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = gold;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = estate;

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, 0, 1);

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
	sprintf(str, "Test %d: testing shuffle on a deck with zero cards.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
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

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = copper;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = silver;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = gold;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = estate;

	res = validateShuffleFunction(playerIndex, &backupG, 1, outputFile, -1, 1);

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
	sprintf(str, "Test %d: testing randomness of shuffle function using two cards in deck.\n", testnum++);
	customPrintf(str, outputFile);

	//copy G into backup for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = adventurer;

	//manipulate the discard pile too
	backupG.discardCount[playerIndex] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = 0;
	backupG.discard[playerIndex][backupG.discardCount[playerIndex]++] = 1;

	int cardValue0PositionCounts[2] = { 0,0 };
	int numberOfTestsToTry = 1000000;

	for (i = 0; i < numberOfTestsToTry; i++)
	{
		//initialize deck to shuffle
		backupG.deckCount[playerIndex] = 0;
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = 0;
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = 1;

		//now shuffle deck of 2 cards
		res = shuffle(playerIndex, &backupG);
		assert(res == 0);

		//now see where card value 0 ended up
		if (backupG.deck[playerIndex][0] == 0)
		{
			cardValue0PositionCounts[0]++;
		}
		else
		{
			cardValue0PositionCounts[1]++;
		}

	}

	int passesCheck = 1; //assume test will fail

	//now check statistics
	double cardValue0Position0Percent = ((double)cardValue0PositionCounts[0] / (double)numberOfTestsToTry) * 100.0;
	double cardValue0Position1Percent = ((double)cardValue0PositionCounts[1] / (double)numberOfTestsToTry) * 100.0;
	double percentageExpected = 50.0;
	double wiggleRoom = 2.0; //percent statistics can be off by and still pass test

	if (cardValue0Position0Percent < percentageExpected - wiggleRoom && cardValue0Position0Percent > percentageExpected + wiggleRoom)
	{
		passesCheck = 0;
	}

	if (cardValue0Position1Percent < percentageExpected - wiggleRoom && cardValue0Position1Percent > percentageExpected + wiggleRoom)
	{
		passesCheck = 0;
	}

	sprintf(str, "Found randomness of shuffle with 2 cards created a %2.2f : %2.2f distribution using %d tests.\n", cardValue0Position0Percent, cardValue0Position1Percent, numberOfTestsToTry);
	customPrintf(str, outputFile);


	//check the result and output status
	if (passesCheck == 1)
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
	testShuffleFunction();
	return 0;
}
