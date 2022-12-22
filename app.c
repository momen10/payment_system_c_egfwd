 /******************************************************************************
 *
 * Module: APP
 *
 * File Name: App.h
 *
 * Description: source file for the card module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#include "standard_types.h"
#include "card.h"
#include "Terminal.h"
#include <stdio.h>
#include "Server.h"
#include "App.h"

extern ST_accountsDB_t accounts_dataBase[255];
extern uint8_t account_index;
EN_cardError_t card_error;
EN_terminalError_t terminal_error;
EN_serverError_t server_error;

int main(void)
{
	appStart();
}
void appStart(void)
{
	ST_cardData_t my_card;

	//get card name from user
	printf("please enter your name \n");
	fflush(stdout);
	card_error =getCardHolderName(&my_card) ;
	while(card_error != CARD_OK)
	{
		printf("invalid name, please try again\n");
		fflush(stdout);
		card_error =getCardHolderName(&my_card);
	}

    // get card expiry date from user
	printf("please enter your card expiry date \n");
	fflush(stdout);
	card_error = getCardExpiryDate(&my_card);
	while(card_error!= CARD_OK)
	{
		printf("invalid date, please try again\n");
		fflush(stdout);
		card_error = getCardExpiryDate(&my_card);
	}

	// get card PAN from user
	printf("please enter your card PAN \n");
	fflush(stdout);
	card_error = getCardPAN(&my_card);
	while(card_error!= CARD_OK)
	{
		printf("invalid PAN, please try again\n");
		fflush(stdout);
		card_error = getCardPAN(&my_card);
	}


    ST_terminalData_t my_terminal ;
    my_terminal.maxTransAmount =5000.5;

	// get card expiry date from user
	printf("displaying transaction date (current date) \n");
	fflush(stdout);
	terminal_error = getTransactionDate(&my_terminal);
	fflush(stdout);
	while(terminal_error!= TERMINAL_OK)
	{
		printf("invalid date");
		fflush(stdout);
		terminal_error = getTransactionDate(&my_terminal);
	}

	//check expiry date of the card
	terminal_error = isCardExpired(&my_card, &my_terminal);
	fflush(stdout);
    if(terminal_error == EXPIRED_CARD)
    {
       return;
    }
    else
    {
    	//valid date, continue
    }

    //check valid PAN (luhn's algorithm)
    terminal_error =  isValidCardPan(&my_card);
    fflush(stdout);
    {
    	if (terminal_error == INVALID_CARD)
    	{
    		return;
    	}
    	else
    	{
    		//valid pan (continue)
    	}
    }

	// get transaction amount from user
	printf("please enter transaction amount: \n");
	fflush(stdout);
	terminal_error = getTransactionAmount(&my_terminal);
	fflush(stdout);
	while(terminal_error!= TERMINAL_OK)
	{
		printf("invalid amount");
		fflush(stdout);
		terminal_error = getTransactionAmount(&my_terminal);
	}

	//check transaction amount
	terminal_error = isBelowMaxAmount(&my_terminal);
	fflush(stdout);
    if(terminal_error == EXPIRED_CARD)
    {
       return;
    }
    else
    {
    	//valid amount, continue
    }

    //checking valid account
    server_error = isValidAccount(&my_card, accounts_dataBase);
	fflush(stdout);
    if(server_error == ACCOUNT_NOT_FOUND)
    {
    	return;
    }
    else
    {
    	//found account , continue
    }
    server_error = isBlockedAccount(&accounts_dataBase[account_index]);
	fflush(stdout);
    if(server_error == BLOCKED_ACCOUNT)
    {
    	return;
    }
    else
    {
    	//valid account , continue
    }


    server_error = isAmountAvailable(&my_terminal, accounts_dataBase);
	fflush(stdout);
    if(server_error== LOW_BALANCE)
    {
    	return;
    }
    else
    {
    	//available fund, continue
    }

    //saving transaction
    ST_transaction_t my_transaction;
    my_transaction.cardHolderData= my_card;
    my_transaction.terminalData=my_terminal;
    my_transaction.transState =recieveTransactionData(&my_transaction);
    saveTransaction(&my_transaction);
    if(my_transaction.transState != APPROVED)
    {
    	printf("declined transaction\n");
    }
    listSavedTransactions();

}
