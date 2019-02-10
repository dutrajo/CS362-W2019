#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

//set array size for output strings
#define MAX_STRING_SIZE 10000

//for handling crashes
volatile sig_atomic_t HAS_SIGSEGV = 0;
void sig_segv_handler(int sig_num)
{
	HAS_SIGSEGV = 1;
}

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
	int validateAdventurerCardEffect(
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
int validateAdventurerCardEffect(
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

	//testing gameState
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

	//result values
	int expected = 0;
	int found = 0;

	//indiciator for all test pass
	int overallTestPass = 1;

	//execute the card effect
	sprintf(str, "\tRunning cardEffect For Adventurer....");
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
	HAS_SIGSEGV = 0;

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
		sprintf(str, "\tChecking player's hand count has increased...");
		customPrintf(str, outputFile);
	}

	//perform check
	expected = 2;
	found = tempG.handCount[playerIndex] - G->handCount[playerIndex]; 


	//discard played card + 2 treasure cards = 1 card difference
	if (expected != found)
	{
		passesCheck = overallTestPass = 0;
	}

	//output status to screen and file
	if (detailedOutput)
	{
		if (passesCheck == 1)
		{
			sprintf(str, "Expceted: %d, Found; %d. Passed.\n",expected, found);
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Expected: %d, Found: %d. Failed!!!!!\n", expected, found);
			customPrintf(str, outputFile);
		}
	}

	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking player's hand received 2 treasure cards...");
		customPrintf(str, outputFile);
	}

	expected = 2;
	found = 0;

	//check change in treasure cards in hand
	
	found += newHandCounts[copper] - origHandCounts[copper];
	found += newHandCounts[silver] - origHandCounts[silver];
	found += newHandCounts[gold] - origHandCounts[gold];

	//validate we got exactly 2
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

	//reset check
	passesCheck = 1;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking that original hand cards weren't modified unexpectedly...");
		customPrintf(str, outputFile);
	}

	for (i = 0; i < treasure_map + 1; i++)
	{
		if (i != copper && i != silver && i != gold && i != handCardPlayed)
		{
			if (origHandCounts[i] != newHandCounts[i])
			{
				passesCheck = overallTestPass = 0;
			}
		}
		if (i == handCardPlayed)
		{
			if (origHandCounts[i] - 1 != newHandCounts[i])
			{
				//passesCheck = overallTestPass = 0;
				//this is checked again below, so ignored this time
			}
		}
		else
		{
			if (newHandCounts[i] < origHandCounts[i])
			{
				passesCheck = overallTestPass = 0;
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
		sprintf(str, "\tChecking played card was removed from hand...");
		customPrintf(str, outputFile);
	}

	//check if played card was removed from hand
	if (newHandCounts[handCardPlayed] != origHandCounts[handCardPlayed] - 1)
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
		sprintf(str, "\tChecking played card was moved to discard pile...");
		customPrintf(str, outputFile);
	}

	//check if played card was removed from hand
	if (newDiscardCounts[handCardPlayed] != origDiscardCounts[handCardPlayed] + 1)
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
		sprintf(str, "\tChecking cards added to discard + added to hand = lost from deck (none trashed)...");
		customPrintf(str, outputFile);
	}

	//check changes to decks
	int amountGainedToDiscard = tempG.discardCount[playerIndex] - G->discardCount[playerIndex];
	int amountLostFromDeck = G->deckCount[playerIndex] - tempG.deckCount[playerIndex];
	int amountGainedToHand = tempG.handCount[playerIndex] - G->handCount[playerIndex];

	
	if (amountLostFromDeck != amountGainedToHand + amountGainedToDiscard)
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
	void testCardEffectAdventurer()
	Parameters:
		None.
	Returns:
		None.
	Description:
		This function will perform several tests of the adventurer card effect. It
		will call the function multiple times and peform numerous tests to try and
		locate any bugs that it may have.
*/
void testCardEffectAdventurer()
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
	FILE *outputFile = fopen("cardtest4.out", "w");

	//output current test information
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has MAX_DECK (%d) treasure cards in it.\n", testnum++, MAX_DECK);
	customPrintf(str, outputFile);

	//backup game for testing
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
	int treasureLooper = 0;
	backupG.deckCount[playerIndex] = 0;
	for (i = 0; i < MAX_DECK; i++)
	{
		if (treasureLooper == 0)
		{
			backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = copper;
			treasureLooper++;
		}
		else if (treasureLooper == 1)
		{
			backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = silver;
			treasureLooper++;
		}
		else
		{
			backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = gold;
			treasureLooper = 0;
		}
	}
	

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has exactly 3 treasure cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup game for testing
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
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = gold;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = silver;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has exactly 2 treasure cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup game for testing
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
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = silver;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has exactly 1 treasure cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup game for testing
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
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has exactly 0 treasure cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup game for testing
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
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;
	backupG.deck[playerIndex][backupG.deckCount[playerIndex]++] = estate;

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'adventurer' when deck has zero cards in it.\n", testnum++);
	customPrintf(str, outputFile);

	//backup game for testing
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

	//run the function and validate it
	res = validateAdventurerCardEffect(playerIndex, 4, adventurer, -1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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

	//for handling segmentation fault errors
	struct sigaction *sa;
	sa = malloc(sizeof(struct sigaction));
	sa->sa_handler = sig_segv_handler;
	sigaction(SIGSEGV, sa, NULL);

	testCardEffectAdventurer();
	return 0;
}





