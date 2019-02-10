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
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail //0 = expect fail, 1 = expect pass
)
{
	//create back gamestate for testing
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
	sprintf(str, "\tRunning cardEffect For Remodel....");
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

	//reset check
	passesCheck = 1;

	//checking trashed card isn't card played
	if (detailedOutput)
	{
		sprintf(str, "\tChecking trashed card isn't remodel card played...");
		customPrintf(str, outputFile);
	}

	//if card to trash is same as one played, maybe we had two in hand?
	if (handCardPlayed == G->hand[playerIndex][choice1])
	{
		if (origHandCounts[handCardPlayed] < 2)
		{
			//we didn't have two of the remodel card, 
			//can't trash same card as played card
			passesCheck = overallTestPass = 0;
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

	//test purchase price of card to receive
	int costOfCardTrashed = getCost(G->hand[playerIndex][choice1]);
	int costOfCardToBuy = getCost(choice2);

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking purchased card is within purchase price (%d + 2 >= %d)...", costOfCardTrashed, costOfCardToBuy);
		customPrintf(str, outputFile);
	}

	//doing actual price check
	if (costOfCardTrashed + 2 < costOfCardToBuy)
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

	//checking purchased card didn't go to player's hand
	if (detailedOutput)
	{
		sprintf(str, "\tChecking purchased card didn't go into player's hand...");
		customPrintf(str, outputFile);
	}

	//make sure we're not trashing the same card as the type played and received
	if (handCardPlayed != choice2 && handCardPlayed != G->hand[playerIndex][choice1])
	{
		if (origHandCounts[choice2] != newHandCounts[choice2])
		{
			passesCheck = overallTestPass = 0;
		}
	}
	else
	{
		if (origHandCounts[choice2] - 2 != newHandCounts[choice2])
		{
			passesCheck = overallTestPass = 0;
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking purchased card didn't go into player's deck...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (origDeckCounts[choice2] != newDeckCounts[choice2])
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking purchased card went into player's discard pile...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (origDiscardCounts[choice2] + 1 != newDiscardCounts[choice2])
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking purchased card came from supply...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (G->supplyCount[choice2] - 1 != tempG.supplyCount[choice2])
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking card to trash isn't in player's hand...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (origHandCounts[G->hand[playerIndex][choice1]] - 1 != newHandCounts[G->hand[playerIndex][choice1]])
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking card to trash isn't in player's deck...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (origDeckCounts[G->hand[playerIndex][choice1]]  != newDeckCounts[G->hand[playerIndex][choice1]])
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking card to trash didn't end up in supply..");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	//make sure deck bought isn't same as one trashed first
	if (choice2 != G->hand[playerIndex][choice1])
	{
		if (G->supplyCount[G->hand[playerIndex][choice1]] != tempG.supplyCount[G->hand[playerIndex][choice1]])
		{
			passesCheck = overallTestPass = 0;
		}
	}
	else
	{
		//trashed card and bought card are the same, i expect supply to drop by 1 card
		if (G->supplyCount[G->hand[playerIndex][choice1]] - 1 != tempG.supplyCount[G->hand[playerIndex][choice1]])
		{
			passesCheck = overallTestPass = 0;
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

	//checking purchased card didn't go to player's deck
	if (detailedOutput)
	{
		sprintf(str, "\tChecking card to trash isn't in player's discard pile...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	//check if card played was same as one trashed...
	if (G->hand[playerIndex][choice1] != handCardPlayed)
	{
		if (origDiscardCounts[G->hand[playerIndex][choice1]] != newDiscardCounts[G->hand[playerIndex][choice1]])
		{
			passesCheck = overallTestPass = 0;
		}
	}
	else
	{
		//otherwise i expect these to be different by 1
		if (origDiscardCounts[G->hand[playerIndex][choice1]] + 1 != newDiscardCounts[G->hand[playerIndex][choice1]])
		{
			passesCheck = overallTestPass = 0;
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

	//checking played card went into discard pile
	if (detailedOutput)
	{
		sprintf(str, "\tChecking card played went into discard pile...");
		customPrintf(str, outputFile);
	}

	//check that deck count for card purchased didn't increase (indicating went into deck)
	if (origDeckCounts[handCardPlayed] + 1 != newDeckCounts[handCardPlayed])
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

	//checking that no other player's cards were modified
	//initially assume we'll pass the check
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

	//initialize the gameState object
	int res = initializeGame(playerCount, k, randomSeed, &G);
	//this should always pass, if it doesn't we need to exit
	assert(res == 0);

	//initialze a string of all '\0' chars
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);
	memset(str, '\0', sizeof(char) * MAX_STRING_SIZE);

	//create a file pointer to output file
	FILE *outputFile = fopen("cardtest2.out", "w");

	//output current test information
	sprintf(str, "Test %d: Playing Card 'remodel', trashing a 'cutpurse' and gaining an 'adventurer' (should pass).\n", testnum++);
	customPrintf(str, outputFile);

	//backup gamestate to test state
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = remodel;

	//run the function and validate it (trash cutpurse, gain adventurer)
	res = validateRemodelCardEffect(playerIndex, 4, remodel, 3, adventurer, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'remodel', trashing a 'cutpurse' and gaining a 'copper' (should pass).\n", testnum++);
	customPrintf(str, outputFile);

	//backup gamestate to test state
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = remodel;

	//run the function and validate it (trash cutpurse, gain adventurer)
	res = validateRemodelCardEffect(playerIndex, 4, remodel, 3, copper, -1, &bonusCoins, &backupG, 1, outputFile, 0);

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
	sprintf(str, "Test %d: Playing Card 'remodel', trashing a 'copper' and gaining an 'adventurer' (should fail).\n", testnum++);
	customPrintf(str, outputFile);

	//backup gamestate to test state
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = remodel;

	//run the function and validate it (trash copper, gain adventurer)
	res = validateRemodelCardEffect(playerIndex, 4, remodel, 0, adventurer, -1, &bonusCoins, &backupG, 1, outputFile, -1);

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
	sprintf(str, "Test %d: Playing Card 'remodel', trashing a 'copper' and gaining an 'adventurer' when supply is empty (should fail).\n", testnum++);
	customPrintf(str, outputFile);

	//backup gamestate to test state
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = cutpurse;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = remodel;

	backupG.supplyCount[adventurer] = 0;

	//run the function and validate it (trash copper, gain adventurer)
	res = validateRemodelCardEffect(playerIndex, 4, remodel, 0, adventurer, -1, &bonusCoins, &backupG, 1, outputFile, -1);

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
		None
	Returns:
		int 0
	Description:
		Will test the 'steward' cardEffect with varying inputs.
*/
int main()
{
	testCardEffectRemodel();
	return 0;
}
	

	
	
