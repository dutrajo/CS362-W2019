#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
//#include <unistd.h>
#include <signal.h>
#include <time.h>

//set array size for output strings
#define MAX_STRING_SIZE 10000


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
	if(foundResult != expectedResult)
	{
		//don't match, output bad result string
		printf("%s",strBadResult);
		fprintf(outputFile,"%s",strBadResult);

		//reinit strings
		initializeString(strGoodResult,MAX_STRING_SIZE);
		initializeString(strBadResult,MAX_STRING_SIZE);
		return 0;
	}
	else
	{
		//values match output good result string
		printf("%s",strGoodResult);
		fprintf(outputFile, "%s", strGoodResult);
	}
	
	initializeString(strGoodResult,MAX_STRING_SIZE);
	initializeString(strBadResult,MAX_STRING_SIZE);
	return 1;
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
	char *strPass = malloc(sizeof(char) * MAX_STRING_SIZE);
	char *strFail = malloc(sizeof(char) * MAX_STRING_SIZE);
	
	//initialize output string
	initializeString(str, MAX_STRING_SIZE);
	initializeString(strPass, MAX_STRING_SIZE);
	initializeString(strFail, MAX_STRING_SIZE);

	//execute the card effect
	sprintf(str, "\tRunning cardEffect For Steward....");
	customPrintf(str, outputFile);

	//reset check
	passesCheck = 1;

	//run the function
	int res = cardEffect(handCardPlayed, choice1, choice2, choice3, &tempG, indexHandCardPlayed, bonus);
	
	sprintf(strPass, "Expected %d, function returned %d. Passed.\n", expectedPassOrFail, res);
	sprintf(strFail, "Expected %d, function returned %d. Failed!!!\n", expectedPassOrFail, res);
	

	passesCheck = assertTrue(res, expectedPassOrFail, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;

	//for loop variables
	int i = 0;
	//int j = 0;

	//output status to screen and file
	sprintf(str, "\tDetermining Users Selected Action....");
	customPrintf(str, outputFile);

	//now validate card effect executed as expected
	if (choice1 == 1)
	{
		//output status to screen and file
		sprintf(str, "gain two cards.\n");
		customPrintf(str, outputFile);

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking if handCount increased correctly...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		//check if player specified hand increased by one card
		//because original played card was put into discard pile (so +2 - 1 = 1)
		passesCheck = assertTrue(tempG.handCount[playerIndex], G->handCount[playerIndex] + 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tVerifying first card received are valid cards...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		passesCheck = assertTrue((tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2] >= curse && tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2] <= treasure_map), 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tVerifying second card received are valid cards...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		passesCheck = assertTrue((tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1] >= curse && tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1] <= treasure_map), 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		passesCheck = assertTrue(passesCheck, 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check pass
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tVerifying first card came from player's deck...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//check that the first of two drawn cards came from the player's deck
		passesCheck = assertTrue(tempG.hand[playerIndex][tempG.handCount[playerIndex] - 2], G->deck[playerIndex][G->deckCount[playerIndex] - 2], strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tVerifying second card came from player's deck...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//check that the second of two drawn cards came from the player's deck
		passesCheck = assertTrue(tempG.hand[playerIndex][tempG.handCount[playerIndex] - 1], G->deck[playerIndex][G->deckCount[playerIndex] - 1], strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;
		
		//output status to screen and file
		sprintf(str, "\tChecking player's coins didn't change...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		//now validate that player's coins didn't change
		passesCheck = assertTrue(tempG.coins, G->coins, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

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

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking played card was removed from player's hand...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//verify that card removed from hand was the correct card
		passesCheck = assertTrue(newHandCardsCount[handCardPlayed], originalHandCardsCount[handCardPlayed] - 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking original cards in hand didn't change (except played card missing)...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		passesCheck = assertTrue(
			memcmp(&tempG.hand[playerIndex][0], &G->hand[playerIndex][0], sizeof(int) * (G->handCount[playerIndex] - 1)), 0, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;


		//free dynamic memory
		free(newHandCardsCount);
		free(originalHandCardsCount);

	}
	else if (choice1 == 2)
	{
		// output status to screen and file
		sprintf(str, "gain two coins.\n");
		customPrintf(str, outputFile);

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking player gained two coins...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		//check if player state has increased coin count +2
		passesCheck = assertTrue(tempG.coins, G->coins + 2, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

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

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking played card was removed from player's hand...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		//verify that card removed from hand was the correct card
		passesCheck = assertTrue(newHandCardsCount[handCardPlayed], originalHandCardsCount[handCardPlayed] - 1, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking original cards in hand didn't change (except played card missing)...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		passesCheck = assertTrue(
			memcmp(&tempG.hand[playerIndex][0], &G->hand[playerIndex][0], sizeof(int) * (tempG.handCount[playerIndex] - 1)), 0, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;


		free(newHandCardsCount);
		free(originalHandCardsCount);
	}
	else
	{
		//output status to screen and file
		sprintf(str, "trash two cards.\n");
		customPrintf(str, outputFile);

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking correct count of cards missing from player's hand...");
		customPrintf(str, outputFile);

		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		passesCheck = assertTrue(tempG.handCount[playerIndex], G->handCount[playerIndex] - 3, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;

		//reset check
		passesCheck = 1;


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

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking card counts in player's hand for first card to trash were changed correctly..");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//check if card amounts were reduced by 1 for each card type selected (or 2 if same type card)
		if (card1ValueToTrash == card2ValueToTrash)
		{
			passesCheck = assertTrue(originalHandCardsCount[card1ValueToTrash], newHandCardsCount[card1ValueToTrash] + 2, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else
		{
			passesCheck = assertTrue(originalHandCardsCount[card1ValueToTrash], newHandCardsCount[card1ValueToTrash] + 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking card counts in player's hand for second card to trash were changed correctly..");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//check if card amounts were reduced by 1 for each card type selected (or 2 if same type card)
		if(card1ValueToTrash == card2ValueToTrash)
		{
			//check if second card amounts were changed correctly
			passesCheck = assertTrue(originalHandCardsCount[card2ValueToTrash], newHandCardsCount[card2ValueToTrash] + 2, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else
		{
			//check if second card amounts were changed correctly
			passesCheck = assertTrue(originalHandCardsCount[card2ValueToTrash], newHandCardsCount[card2ValueToTrash] + 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		
		//output status to screen and file
		sprintf(str, "\tChecking card counts in player's hand for original played card changed correctly..");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		//check that original played card amounts were changed correctly
		if(card1ValueToTrash == handCardPlayed && handCardPlayed == card2ValueToTrash)
		{
			passesCheck = assertTrue(newHandCardsCount[handCardPlayed], originalHandCardsCount[handCardPlayed] - 3, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else if(card1ValueToTrash == handCardPlayed || handCardPlayed == card2ValueToTrash)
		{
			passesCheck = assertTrue(newHandCardsCount[handCardPlayed], originalHandCardsCount[handCardPlayed] - 2, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else
		{
			passesCheck = assertTrue(newHandCardsCount[handCardPlayed], originalHandCardsCount[handCardPlayed] - 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}

		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");

		//reset check
		passesCheck = 1;
		
		sprintf(str, "\tChecking cards in hand didn't change (except played card, and trashed missing)...");
		customPrintf(str, outputFile);


		//perform check
		for (i = 0; i < treasure_map + 1; i++)
		{
			
			if(i == handCardPlayed && handCardPlayed == card1ValueToTrash && handCardPlayed == card2ValueToTrash && card1ValueToTrash == card2ValueToTrash)
			{
				
				if(newHandCardsCount[i] != originalHandCardsCount[i] - 3)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else if((i == handCardPlayed || i == card1ValueToTrash || i == card2ValueToTrash) && (handCardPlayed == card1ValueToTrash || handCardPlayed == card2ValueToTrash || card1ValueToTrash == card2ValueToTrash))
			{
				if(newHandCardsCount[i] != originalHandCardsCount[i] - 2)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else if(i == handCardPlayed || i == card1ValueToTrash || i == card2ValueToTrash)
			{
				if(newHandCardsCount[i] != originalHandCardsCount[i] - 1)
				{
					passesCheck = overallTestPass = 0;
				}
			}
			else
			{
				if(newHandCardsCount[i] != originalHandCardsCount[i])
				{
					passesCheck = overallTestPass = 0;
				}
			}
		}
		
		if(assertTrue(passesCheck, 1, strPass, strFail, outputFile) == 0)
		{
			passesCheck = overallTestPass = 0;
		}


		free(newHandCardsCount);
		free(originalHandCardsCount);

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

		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking first trashed card didn't end up in discard pile...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		if(handCardPlayed == card1ValueToTrash)
		{
			passesCheck = assertTrue(newDiscardDeckCounts[card1ValueToTrash], oldDiscardDeckCounts[card1ValueToTrash] + 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else
		{
			passesCheck = assertTrue(newDiscardDeckCounts[card1ValueToTrash], oldDiscardDeckCounts[card1ValueToTrash], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		
		//reset check
		passesCheck = 1;

		//output status to screen and file
		sprintf(str, "\tChecking second trashed card didn't end up in discard pile...");
		customPrintf(str, outputFile);
		
		sprintf(strPass, "Passed.\n");
		sprintf(strFail, "Failed!!!\n");
		
		if(handCardPlayed == card2ValueToTrash)
		{
			passesCheck = assertTrue(newDiscardDeckCounts[card2ValueToTrash], oldDiscardDeckCounts[card2ValueToTrash] + 1, strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}
		else
		{
			passesCheck = assertTrue(newDiscardDeckCounts[card2ValueToTrash], oldDiscardDeckCounts[card2ValueToTrash], strPass, strFail, outputFile);
			overallTestPass &= passesCheck;
		}

		//free dynamic memory
		free(newDiscardDeckCounts);
		free(oldDiscardDeckCounts);
		
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
	
	//reset check
	passesCheck = 1;
	
	//output status to screen and file
	sprintf(str, "\tChecking played card ended up in discard pile...");
	customPrintf(str, outputFile);
	
	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	//check if trashed cards are in discard pile
	passesCheck = assertTrue(newDiscardDeckCounts[handCardPlayed], oldDiscardDeckCounts[handCardPlayed] + 1, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//free dynamic memory
	free(newDiscardDeckCounts);
	free(oldDiscardDeckCounts);

	//output status to screen and file
	sprintf(str, "\tChecking all other player's cards for any modifications.\n");
	customPrintf(str, outputFile);
	
	for(i = 0; i < tempG.numPlayers; i++)
	{
		if(i != playerIndex)
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
	passesCheck = assertTrue(tempG.whoseTurn, G->whoseTurn, strPass, strFail, outputFile);
	overallTestPass &= passesCheck;

	//reset check
	passesCheck = 1;
	
	//output status to screen and file
	sprintf(str, "\tChecking player's coins weren't modified incorrectly..");
	customPrintf(str, outputFile);
	
	sprintf(strPass, "Passed.\n");
	sprintf(strFail, "Failed!!!\n");

	if(choice1 != 2)
	{
		passesCheck = assertTrue(G->coins, tempG.coins, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}
	else
	{
		passesCheck = assertTrue(G->coins + 2, tempG.coins, strPass, strFail, outputFile);
		overallTestPass &= passesCheck;
	}

	//free dynamic memory
	free(str);
	free(strPass);
	free(strFail);

	//return final result
	return overallTestPass;
}

/*
	void generateRandomInputValuesForSteward(
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
void generateRandomInputValuesForSteward(int *playerIndex, int *choice1, int *choice2, int *choice3, int *bonusCoins, struct gameState *inputState)
{
	int i = 0;

	//add or subtract coins ( -5 to +5)
	*bonusCoins = (rand() % 11) - 5;

	//possible choices = 1, 2, 3
	*choice1 = (rand() % 3) + 1;

	//max players = 4, this will generate value between 0 - 3
	*playerIndex = rand() % 4;

	//now randomly manipulate gamestate for player index chosen

	//first randomly choose number of cards in decks
	inputState->whoseTurn = *playerIndex;
	inputState->handCount[*playerIndex] = rand() % MAX_HAND;
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

	//now we need to determine if we're trashing two cards
	if (*choice1 != 1 && *choice1 != 2)
	{
		//we're trashing two cards, so we need to select two cards from gameState
		if (inputState->handCount[*playerIndex] > 1)
		{
			*choice2 = rand() % inputState->handCount[*playerIndex];
			*choice3 = rand() % inputState->handCount[*playerIndex];
		}
		else
		{
			*choice2 = 0;
			*choice3 = -1;
		}


		//if the two indexes are the same, reselect second choice till they're not
		while (*choice2 == *choice3)
		{
			*choice3 = rand() % inputState->handCount[*playerIndex];
		}
	}

	updateCoins(*playerIndex, inputState, 0);

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
	FILE *outputFile = fopen("randomtestcard1.out", "w");

	int numberOfRandomTests = 1000;
	int loopCounter = 0;

	for (loopCounter = 0; loopCounter < numberOfRandomTests; loopCounter++)
	{

		//backup G for testing
		memcpy(&backupG, &G, sizeof(struct gameState));

		//randomize inputs
		
		generateRandomInputValuesForSteward(&playerIndex, &choice1, &choice2, &choice3, &bonusCoins, &backupG);

		while (choice1 > 2 && (choice2 == -1 || choice3 == -1))
		{
			generateRandomInputValuesForSteward(&playerIndex, &choice1, &choice2, &choice3, &bonusCoins, &backupG);
		}

		//output current test information
		if (choice1 == 1)
		{
			sprintf(str, "Test %d: Playing Card 'steward' opting to gain two cards, playing as player: %d.\n", testnum++, playerIndex);
			customPrintf(str, outputFile);
		}
		else if (choice1 == 2)
		{
			sprintf(str, "Test %d: Playing Card 'steward' opting to gain two coins, playing as player: %d.\n", testnum++, playerIndex);
			customPrintf(str, outputFile);
		}
		else
		{
			sprintf(str, "Test %d: Playing Card 'steward' opting to trash two cards, playing as player: %d.\n", testnum++, playerIndex);
			customPrintf(str, outputFile);
		}
		

		//now make sure we add the steward card as last index in hand to play
		backupG.hand[playerIndex][backupG.handCount[playerIndex]++] = steward;

		//run the function and validate it
		res = validateStewardCardEffect(playerIndex, backupG.handCount[playerIndex] - 1, steward, choice1, choice2, choice3, &bonusCoins, &backupG, outputFile, 0);

		sprintf(strPass, "------------- OVERALL TEST PASSED ---------------\n\n");
		sprintf(strFail, "------------- OVERALL TEST FAILED!!!! ---------------\n\n");

		assertTrue(res, 1, strPass, strFail, outputFile);
	}

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
	srand(time((time_t*)NULL));
	//for handling segmentation fault errors
	/*struct sigaction *sa;
	sa = malloc(sizeof(struct sigaction));
	sa->sa_handler = sig_segv_handler;
	sigaction(SIGSEGV, sa, NULL);*/

	testCardEffectSteward();
	return 0;
}








