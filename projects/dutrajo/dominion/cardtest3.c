#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
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
	int validateSmithyCardEffect(
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
int validateSmithyCardEffect(
	int playerIndex,
	int indexHandCardPlayed,
	int handCardPlayed,
	int choice1,
	int choice2,
	int choice3,
	int *bonus,
	struct gameState *G,
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail //0 = expect fail, 1 = expect pass
)
{
	//make backup gamestate for testing
	struct gameState tempG;

	//first copy gameState to test object
	memcpy(&tempG, G, sizeof(struct gameState));

	//create dynamic memory for output string
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize the string
	initializeString(str, MAX_STRING_SIZE);

	//indicator for individual test pass
	int passesCheck = 1;

	//counter variable
	int i = 0;
	int j = 0;

	//indiciator for all test pass
	int overallTestPass = 1;

	//execute the card effect
	sprintf(str, "\tRunning cardEffect For Smithy....");
	if (detailedOutput) customPrintf(str, outputFile);

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

	//run the function
	int res = cardEffect(handCardPlayed, choice1, choice2, choice3, &tempG, indexHandCardPlayed, bonus);

	//check we got expected value
	if (res != expectedPassOrFail)
	{
		passesCheck = overallTestPass = 0;
	}

	//output results to screen and file
	if (detailedOutput)
	{
		if (passesCheck)
		{
			sprintf(str, "Expected: %d, function returned: %d. Passed.\n", expectedPassOrFail, res);
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Expected: %d, function returned: %d. Failed!!!\n", expectedPassOrFail, res);
			customPrintf(str, outputFile);
		}
	}

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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's hand count has increased by 3 cards...");
		customPrintf(str, outputFile);
	}

	//expect new hand to be +3 cards from original
	if (tempG.handCount[playerIndex] != G->handCount[playerIndex] + 2)
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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's hand actually received 3 cards...");
		customPrintf(str, outputFile);
	}

	//iterate through original hand and previous see how many added
	int totalAdded = 0;
	for (i = 0; i < treasure_map + 1; i++)
	{
		if (i != handCardPlayed)
		{
			totalAdded += newHandCounts[i] - origHandCounts[i];
		}
		else
		{
			//we discarded smithy card played, and may have gained another smithy...
			if (newHandCounts[i] - origHandCounts[i] < 0)
			{
				//we discarded a smithy and didn't get one back
				//but don't increment totalAdded
			}
			else if(newHandCounts[i] - origHandCounts[i] == 0)
			{
				//we discarded a smithy and received one back from draw cards
				totalAdded += 1;
			}
			else
			{
				totalAdded += newHandCounts[i];
			}
		}
		
	}

	//check that we increased by 3
	if (totalAdded != 3)
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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking cards player received are valid...");
		customPrintf(str, outputFile);
	}

	//check all cards in player's hand are valid
	for (i = 0; i < tempG.handCount[playerIndex]; i++)
	{
		if (tempG.hand[playerIndex][i] < 0 || tempG.hand[playerIndex][i] >= treasure_map + 1)
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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking received cards didn't come from supply...");
		customPrintf(str, outputFile);
	}

	//iterate through suppply and check for changes
	for (i = 0; i < treasure_map + 1; i++)
	{
		if (tempG.supplyCount[i] != G->supplyCount[i])
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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking received cards came from player's deck...");
		customPrintf(str, outputFile);
	}

	//iterate through original deck and new deck
	int totalCardsMissing = 0;

	//iterate through suppply and check for changes
	for (i = 0; i < treasure_map + 1; i++)
	{
		totalCardsMissing += origDeckCounts[i] - newDeckCounts[i];
	}

	if (totalCardsMissing != 3)
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

	//set check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking played card went to discard pile...");
		customPrintf(str, outputFile);
	}

	//perform check
	if (origDiscardCounts[handCardPlayed] + 1 != newDiscardCounts[handCardPlayed])
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
	void testCardEffectSmithy()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the smithy card effect. It
		will call the function multiple times and peform numerous tests to try and
		locate any bugs that it may have.
*/
void testCardEffectSmithy()
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

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("cardtest3.out", "w");

	//output current test information
	sprintf(str, "Test %d: Playing Card 'smithy' when player's deck is full (MAX_DECK: %d).\n", testnum++, MAX_DECK);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = smithy;

	//manipulate player's deck
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	}

	//run the function and validate it
	res = validateSmithyCardEffect(playerIndex, 4, smithy, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'smithy' when deck has exactly 4 cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = smithy;

	//manipulate player's deck
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = gold;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = silver;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = copper;

	//run the function and validate it
	res = validateSmithyCardEffect(playerIndex, 4, smithy, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'smithy' when deck has exactly 3 cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = smithy;

	//manipulate player's deck
	backupG.deckCount[playerIndex] = 0;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = gold;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = silver;

	//run the function and validate it
	res = validateSmithyCardEffect(playerIndex, 4, smithy, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'smithy' when deck has exactly 0 cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = smithy;

	//manipulate player's deck
	backupG.deckCount[playerIndex] = 0;

	//run the function and validate it
	res = validateSmithyCardEffect(playerIndex, 4, smithy, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	testCardEffectSmithy();
	return 0;
}

