#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
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
	int validateStewardCardEffect(
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
		int choice1: selection choice 1=+2 cards, 2=+2 coins, 3=trash 2 cards
		int choice2: index of first card to trash from hand
		int choice3: index of second card to trash from hand
		int *bonus: bonus coins pointer
		struct gameState *G: pointer to gameState object
		int detailedOutput: 1 = output status, anything else don't
		FILE* outputFile: pointer to output file (can be null)
		int expectedPassOrFail: value indicating expected function return value
	Returns:
		int 0 = failure, 1 = success
	Description:
		Will test the cardEffect for 'steward' card. Returns 0 if test
		fails, 1 if test is successful.
*/
int validateStewardCardEffect(
	int playerIndex,
	int indexHandCardPlayed,
	int handCardPlayed,
	int choice1, // 1 = +2 cards to hand, 2 = +2 coins, anything else = trash two cards
	int choice2, // index of card to discard
	int choice3, // index of card to discard
	int *bonus,
	struct gameState *G,
	int detailedOutput,//0 = no output, 1 = output
	FILE* outputFile,
	int expectedPassOrFail 
)
{
	//create gamestate for testing
	struct gameState tempG;

	//first copy gameState to test object
	memcpy(&tempG, G, sizeof(struct gameState));

	//assume we're gonna pass check
	int passesCheck = 1;

	//assume we're going to pass overall tests
	int overallTestPass = 1;

	//output string for print statements
	char *str = malloc(sizeof(char) * MAX_STRING_SIZE);

	//initialize output string
	initializeString(str, MAX_STRING_SIZE);

	//execute the card effect
	sprintf(str, "\tRunning cardEffect For Steward....");
	if(detailedOutput) customPrintf(str, outputFile);

	//reset check
	passesCheck = 1;

	//run the function
	int res = cardEffect(handCardPlayed, choice1, choice2, choice3, &tempG, indexHandCardPlayed, bonus);

	if (res != expectedPassOrFail)
	{
		passesCheck = overallTestPass = 0;
	}

	//output results to screen and file
	if (detailedOutput)
	{
		if (passesCheck)
		{
			sprintf(str, "Expected %d, function returned %d. Passed.\n", expectedPassOrFail, res);
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Expected %d, function returned %d. Failed!!!\n", expectedPassOrFail, res);
			customPrintf(str, outputFile);
		}
	}

	//reset check
	passesCheck = 1;

	//for loop variables
	int i = 0;
	int j = 0;

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tDetermining Users Selected Action....");
		customPrintf(str, outputFile);
	}

	//now validate card effect executed as expected
	if (choice1 == 1)
	{
		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "gain two cards.\n");
			customPrintf(str, outputFile);
		}

		//reset check
		passesCheck = 1;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tChecking if handCount increased correctly...");
			customPrintf(str, outputFile);
		}

		//check if player specified hand increased by one card
		//because original played card was put into discard pile (so +2 - 1 = 1)
		if (tempG.handCount[playerIndex] != G->handCount[playerIndex] + 1)
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
			sprintf(str, "\tVerifying cards received are valid cards...");
			customPrintf(str, outputFile);
		}

		//verify the two new cards in player's hand are valid cards
		for (i = G->handCount[playerIndex] - 1; i < tempG.handCount[playerIndex]; i++)
		{
			//check if card is within enum range
			if (tempG.hand[playerIndex][i] < curse || tempG.hand[playerIndex][i] > treasure_map)
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

		//reset check pass
		passesCheck = 1;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tVerifying first card came from player's deck...");
			customPrintf(str, outputFile);
		}

		//check that the first of two drawn cards came from the player's deck
		if (tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2] != G->deck[playerIndex][G->deckCount[playerIndex] - 1])
		{
			//didn't come from player's deck
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
			sprintf(str, "\tVerifying second card came from player's deck...");
			customPrintf(str, outputFile);
		}

		//check that the second of two drawn cards came from the player's deck
		if (tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1] != G->deck[playerIndex][G->deckCount[playerIndex] - 2])
		{
			//didn't come from player's deck
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
			sprintf(str, "\tChecking player's coins didn't change...");
			customPrintf(str, outputFile);
		}

		//now validate that player's coins didn't change
		if (tempG.coins != G->coins)
		{
			//coins were modified whent they shouldn't have
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
			sprintf(str, "\tChecking played card was removed from player's hand...");
			customPrintf(str, outputFile);
		}

		//validate that we discarded correct played card from hand
		int *newHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		int *originalHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		for (i = 0; i < treasure_map + 1; i++)
		{
			newHandCardsCount[i] = originalHandCardsCount[i] = 0;
		}

		//iterate through new hand and increment card counts
		for (i = 0; i < tempG.handCount[playerIndex]; i++)
		{
			newHandCardsCount[tempG.hand[playerIndex][i]]++;
		}

		//iterate through old hand and increment card counts
		for (i = 0; i < G->handCount[playerIndex]; i++)
		{
			originalHandCardsCount[G->hand[playerIndex][i]]++;
		}

		//verify that card removed from hand was the correct card
		if (newHandCardsCount[handCardPlayed] != originalHandCardsCount[handCardPlayed] - 1)
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
			sprintf(str, "\tChecking original cards in hand didn't change (except played card missing)...");
			customPrintf(str, outputFile);
		}

		//perform check
		for (i = 0; i < treasure_map + 1; i++)
		{
			//if we're on the cards that got added
			if (i == tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2] ||
				i == tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1] 
				)
			{
				if (tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2] ==
					tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1])
				{
					if (newHandCardsCount[i] != originalHandCardsCount[i] + 2)
					{
						//didn't find expected count
						passesCheck = overallTestPass = 0;
					}
				}
				else
				{
					if (newHandCardsCount[i] != originalHandCardsCount[i] + 1)
					{
						//didn't find expected count
						passesCheck = overallTestPass = 0;
					}
				}
				
			}
			//check if we're on the card that was played
			else if (i == handCardPlayed)
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i] - 1)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i])
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

		//free dynamic memory
		free(newHandCardsCount);
		free(originalHandCardsCount);

	}
	else if (choice1 == 2)
	{
		// output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "gain two coins.\n");
			customPrintf(str, outputFile);
		}

		//reset check
		passesCheck = 1;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tChecking player gained two coins...");
			customPrintf(str, outputFile);
		}

		//check if player state has increased coin count +2
		if (tempG.coins != G->coins + 2)
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
			sprintf(str, "\tChecking played card was removed from player's hand...");
			customPrintf(str, outputFile);
		}

		//validate that we discarded correct played card from hand
		int *newHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		int *originalHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		for (i = 0; i < treasure_map + 1; i++)
		{
			newHandCardsCount[i] = originalHandCardsCount[i] = 0;
		}

		//iterate through new hand and increment card counts
		for (i = 0; i < tempG.handCount[playerIndex]; i++)
		{
			newHandCardsCount[tempG.hand[playerIndex][i]]++;
		}

		//iterate through old hand and increment card counts
		for (i = 0; i < G->handCount[playerIndex]; i++)
		{
			originalHandCardsCount[G->hand[playerIndex][i]]++;
		}

		//verify that card removed from hand was the correct card
		if (newHandCardsCount[handCardPlayed] != originalHandCardsCount[handCardPlayed] - 1)
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
			sprintf(str, "\tChecking original cards in hand didn't change (except played card missing)...");
			customPrintf(str, outputFile);
		}

		//perform check
		for (i = 0; i < treasure_map + 1; i++)
		{
			
			//check if we're on the card that was played
			if (i == handCardPlayed)
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i] - 1)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i])
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

		free(newHandCardsCount);
		free(originalHandCardsCount);
	}
	else
	{
		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "trash two cards.\n");
			customPrintf(str, outputFile);
		}

		//reset check
		passesCheck = 1;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tChecking correct count of cards missing from player's hand...");
			customPrintf(str, outputFile);
		}

		if (tempG.handCount[playerIndex] != G->handCount[playerIndex] - 3)
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
			sprintf(str, "\tChecking correct card values are missing from player's hand...");
			customPrintf(str, outputFile);
		}

		//we've removed three cards (2 trash + 1 played), verify the correct cards are out of player's hand
		int *newHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		int *originalHandCardsCount = malloc(sizeof(int) * (treasure_map + 1));
		for (i = 0; i < treasure_map + 1; i++)
		{
			newHandCardsCount[i] = originalHandCardsCount[i] = 0;
		}

		//iterate through new hand and increment card counts
		for (i = 0; i < tempG.handCount[playerIndex]; i++)
		{
			newHandCardsCount[tempG.hand[playerIndex][i]]++;
		}

		//iterate through old hand and increment card counts
		for (i = 0; i < G->handCount[playerIndex]; i++)
		{
			originalHandCardsCount[G->hand[playerIndex][i]]++;
		}

		//determine card values that were to be trashed
		int card1ValueToTrash = G->hand[playerIndex][choice2];
		int card2ValueToTrash = G->hand[playerIndex][choice3];

		//check if card amounts were reduced by 1 for each card type selected (or 2 if same type card)
		if (card1ValueToTrash == card2ValueToTrash)
		{
			if (originalHandCardsCount[card1ValueToTrash] != newHandCardsCount[card1ValueToTrash] + 2)
			{
				//first card selected wasn't removed from hand correctly
				passesCheck = overallTestPass = 0;

			}
		}
		else
		{
			if (originalHandCardsCount[card1ValueToTrash] != newHandCardsCount[card1ValueToTrash] + 1)
			{
				//first card selected wasn't removed from hand correctly
				passesCheck = overallTestPass = 0;

			}
		}

		//check if second card amounts were changed correctly
		if (originalHandCardsCount[card2ValueToTrash] != newHandCardsCount[card2ValueToTrash] + 1)
		{
			//second card selected wasn't removed from hand correctly
			passesCheck = overallTestPass = 0;
		}
		
		//check that original played card amounts were changed correctly
		if (newHandCardsCount[handCardPlayed] != originalHandCardsCount[handCardPlayed] - 1)
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
			sprintf(str, "\tChecking cards in hand didn't change (except played card, and trashed missing)...");
			customPrintf(str, outputFile);
		}

		//perform check
		for (i = 0; i < treasure_map + 1; i++)
		{

			//check if we're on the card that was played
			if (i == handCardPlayed)
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i] - 1)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else if (i == G->hand[playerIndex][choice2] || i == G->hand[playerIndex][choice3])
			{
				if (card1ValueToTrash == card2ValueToTrash)
				{
					if (newHandCardsCount[i] != originalHandCardsCount[i] - 2)
					{
						passesCheck = overallTestPass = 0;
					}
				}
				else
				{
					if (newHandCardsCount[i] != originalHandCardsCount[i] - 1)
					{
						passesCheck = overallTestPass = 0;
					}
				}
			}
			else
			{
				if (newHandCardsCount[i] != originalHandCardsCount[i])
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

		free(newHandCardsCount);
		free(originalHandCardsCount);

		//reset check
		passesCheck = 1;

		//output status to screen and file
		if (detailedOutput)
		{
			sprintf(str, "\tChecking trashed cards didn't end up in discard deck...");
			customPrintf(str, outputFile);
		}

		int *newDiscardDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
		int *oldDiscardDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
		for (i = 0; i < treasure_map + 1; i++)
		{
			newDiscardDeckCounts[i] = oldDiscardDeckCounts[i] = 0;
		}

		//iterate through discard and increment card counts
		for (i = 0; i < tempG.discardCount[playerIndex]; i++)
		{
			if(tempG.discard[playerIndex][i] >= 0 && tempG.discard[playerIndex][i] < treasure_map + 1)
				newDiscardDeckCounts[tempG.discard[playerIndex][i]]++;
		}

		//iterate through discard and increment card counts
		for (i = 0; i < G->discardCount[playerIndex]; i++)
		{
			if (G->discard[playerIndex][i] >= 0 && G->discard[playerIndex][i] < treasure_map + 1)
				oldDiscardDeckCounts[G->discard[playerIndex][i]]++;
		}

		//check if trashed cards are in discard pile
		if (newDiscardDeckCounts[card1ValueToTrash] != oldDiscardDeckCounts[card1ValueToTrash] ||
			newDiscardDeckCounts[card2ValueToTrash] != oldDiscardDeckCounts[card2ValueToTrash])
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

		//free dynamic memory
		free(newDiscardDeckCounts);
		free(oldDiscardDeckCounts);
		
	}

	//output status to screen and file
	if (detailedOutput)
	{
		sprintf(str, "\tChecking played card ended up in discard pile...");
		customPrintf(str, outputFile);
	}

	int *newDiscardDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
	int *oldDiscardDeckCounts = malloc(sizeof(int) * (treasure_map + 1));
	for (i = 0; i < treasure_map + 1; i++)
	{
		newDiscardDeckCounts[i] = oldDiscardDeckCounts[i] = 0;
	}

	//iterate through discard and increment card counts
	for (i = 0; i < tempG.discardCount[playerIndex]; i++)
	{
		if (tempG.discard[playerIndex][i] >= 0 && tempG.discard[playerIndex][i] < treasure_map + 1)
			newDiscardDeckCounts[tempG.discard[playerIndex][i]]++;
	}

	//iterate through discard and increment card counts
	for (i = 0; i < G->discardCount[playerIndex]; i++)
	{
		if (G->discard[playerIndex][i] >= 0 && G->discard[playerIndex][i] < treasure_map + 1)
			oldDiscardDeckCounts[G->discard[playerIndex][i]]++;
	}

	//check if trashed cards are in discard pile
	if (newDiscardDeckCounts[handCardPlayed] != oldDiscardDeckCounts[handCardPlayed] + 1)
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

	//free dynamic memory
	free(newDiscardDeckCounts);
	free(oldDiscardDeckCounts);


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
		sprintf(str, "\tChecking player's coins weren't modified incorrectly..");
		customPrintf(str, outputFile);
	}

	if (choice1 != 2)
	{
		if (G->coins != tempG.coins)
		{
			passesCheck = overallTestPass = 0;
		}
	}
	else
	{
		if (G->coins + 2 != tempG.coins)
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

	//free dynamic memory
	free(str);

	//return final result
	return overallTestPass;
}

/*
	void testCardEffectSteward()
	Parameters:
		None
	Returns:
		Nothing
	Description:
		Will test the 'steward' cardEffect with varying inputs.
*/
void testCardEffectSteward()
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
	FILE *outputFile = fopen("cardtest1.out", "w");
	
	//output current test information
	sprintf(str, "Test %d: Playing Card 'steward' opting to gain two cards.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = estate;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = steward;
	
	//run the function and validate it
	res = validateStewardCardEffect(playerIndex, 4, steward, 1, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);
	
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
	sprintf(str, "Test %d: Playing Card 'steward' opting to gain two coins.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = estate;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = steward;
	
	//run the function and validate it
	res = validateStewardCardEffect(playerIndex, 4, steward, 2, -1, -1, &bonusCoins, &backupG, 1, outputFile, 0);
	
	//check the results and output status
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
	sprintf(str, "Test %d: Playing Card 'steward' opting to trash two cards.\n", testnum++);
	customPrintf(str, outputFile);

	//backup G for testing
	memcpy(&backupG, &G, sizeof(struct gameState));

	//now manipulate the cards in the player's hand to be known values
	backupG.handCount[playerIndex] = 0;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = copper;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = silver;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = gold;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = estate;
	backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = steward;
	
	//run the function and validate it
	res = validateStewardCardEffect(playerIndex, 4, steward, 3, 0, 1, &bonusCoins, &backupG, 1, outputFile, 0);
	
	//check the results and output status
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
	//for handling segmentation fault errors
	struct sigaction *sa;
	sa = malloc(sizeof(struct sigaction));
	sa->sa_handler = sig_segv_handler;
	sigaction(SIGSEGV, sa, NULL);

	testCardEffectSteward();
	return 0;
}








