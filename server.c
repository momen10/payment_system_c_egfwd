 /******************************************************************************
 *
 * Module: Server
 *
 * File Name: Server.c
 *
 * Description: source file for the server module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#include "stdio.h"
#include "Terminal.h"
#include "card.h"
#include "Server.h"
#include "String.h"
#include "standard_types.h"

//global array of accounts structure (255 accounts as data base of server)
	ST_accountsDB_t accounts_dataBase [255] ={{500,RUNNING, "4117394994098308" },\
			                                {1500.5,BLOCKED, "8399411957015952"}} ;

uint8_t account_index=0;

ST_transaction_t transactions_dataBase [255]={0};
uint8_t number_of_saved_transactions=0;

extern EN_cardError_t card_error;
extern EN_terminalError_t terminal_error;
extern EN_serverError_t server_error;

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
	for (uint8_t i=0; i<255; i++)
	{
		if (strcmp(cardData->primaryAccountNumber,accountRefrence[i].primaryAccountNumber)==0)
		{
			printf("found account\n");
			account_index = i;
			return SERVER_OK;
		}
		else
		{
           continue;
		}
	}
	printf("not found account\n");
	return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest(void)
{
	printf("Tester Name: Mo'men Ahmed\nFunction Name: isValidAccount\n");
    printf("Test Case 1:\n");
	ST_cardData_t my_card;
	strcpy(my_card.primaryAccountNumber,"12345678901234567890");
	printf("Input Data: ");
	puts(my_card.primaryAccountNumber);
	printf("Expected Result: 0\n"); //server ok
	printf("Actual Result: %d\n",isValidAccount(&my_card, accounts_dataBase));

    printf("Test Case 2:\n");
	strcpy(my_card.primaryAccountNumber,"00000111112222233333");
	printf("Input Data: ");
	puts(my_card.primaryAccountNumber);
	printf("Expected Result: 3\n"); //account not found
	printf("Actual Result: %d\n",isValidAccount(&my_card, accounts_dataBase));

    printf("Test Case 3:\n");
	strcpy(my_card.primaryAccountNumber,"78910678901234567890");
	printf("Input Data: ");
	puts(my_card.primaryAccountNumber);
	printf("Expected Result: 0\n"); //server ok
	printf("Actual Result: %d\n",isValidAccount(&my_card, accounts_dataBase));
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
	if(accountRefrence->state ==BLOCKED)
	{
		printf("Blocked account\n");
		return BLOCKED_ACCOUNT;
	}
	else
	{
		printf("Valid account\n");
		return SERVER_OK;
	}
}

void isBlockedAccountTest(void)
{
	printf("Tester Name: Mo'men Ahmed\nFunction Name: isBlockedAccount\n");
    printf("Test Case 1:\n");
	printf("Input Data: ");
	printf("%d\n",accounts_dataBase[0].state);
	printf("Expected Result: 0\n"); //server ok
	printf("Actual Result: %d\n",isBlockedAccount(&accounts_dataBase[0]));

    printf("Test Case 2:\n");
	printf("Input Data: ");
	printf("%d\n",accounts_dataBase[1].state);
	printf("Expected Result: 5\n"); //blocked account
	printf("Actual Result: %d\n",isBlockedAccount(&accounts_dataBase[1]));

    printf("Test Case 3:\n");
	printf("Input Data: ");
	printf("%d\n",accounts_dataBase[2].state);
	printf("Expected Result: 0\n"); //server ok
	printf("Actual Result: %d\n",isBlockedAccount(&accounts_dataBase[2]));
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
	if(termData->transAmount > accountRefrence->balance)
	{
		printf("account balance before withdrawal = %0.2f\n",accountRefrence[account_index].balance);
		printf("insufficient fund\n");
		return LOW_BALANCE;
	}
	else
	{
		printf("available fund\n");
		printf("account balance before withdrawal = %0.2f\n",accountRefrence[account_index].balance);
		accountRefrence[account_index].balance -= termData->transAmount;
		printf("new account balance = %0.2f\n",accountRefrence[account_index].balance);
		fflush(stdout);
		return SERVER_OK;
	}
}

void isAmountAvailableTest(void)
{
	ST_terminalData_t my_terminal;
	my_terminal.transAmount=2000;
	printf("Tester Name: Mo'men Ahmed\nFunction Name: isAmountAvailableTest\n");
    printf("Test Case 1:\n");
	printf("Input Data: ");
	printf("%0.2f\n",accounts_dataBase[0].balance);
	printf("Expected Result: 4\n"); //low balance
	printf("Actual Result: %d\n",isAmountAvailable(&my_terminal, &accounts_dataBase[0]));

    printf("Test Case 2:\n");
	printf("Input Data: ");
	printf("%0.2f\n",accounts_dataBase[1].balance);
	printf("Expected Result: 4\n"); //low balance
	printf("Actual Result: %d\n",isAmountAvailable(&my_terminal, &accounts_dataBase[1]));

    printf("Test Case 3:\n");
	printf("Input Data: ");
	printf("%0.2f\n",accounts_dataBase[2].balance);
	printf("Expected Result: 0\n"); //server ok
	printf("Actual Result: %d\n",isAmountAvailable(&my_terminal, &accounts_dataBase[2]));
	printf("balance after withdrawal: %0.2f\n",accounts_dataBase[2].balance);

}


ST_cardData_t my_card;
ST_terminalData_t my_terminal;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{

	uint8_t transaction_state=0;

	if(server_error==ACCOUNT_NOT_FOUND)
	{
		transaction_state= FRAUD_CARD;
	}
	else if(server_error==LOW_BALANCE)
	{
		transaction_state= DECLINED_INSUFFECIENT_FUND;
	}
	else if(server_error==BLOCKED_ACCOUNT)
	{
		transaction_state= DECLINED_STOLEN_CARD;
	}
	else if(server_error== SAVING_FAILED)
	{
		transaction_state = INTERNAL_SERVER_ERROR;
	}
	else
	{
		transaction_state= APPROVED;
	}
	return transaction_state;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	for (uint8_t i=0 ; i<255 ; i++)
	{
		if(transactions_dataBase[i].cardHolderData.primaryAccountNumber[0]=='\0')  //empty place in database
		{
			number_of_saved_transactions ++;
           transData->transactionSequenceNumber = i ;
           transactions_dataBase[i] = *transData;     //save transaction in database
           return SERVER_OK;
		}
		else
		{
			continue;
		}
	}
	//if there is no place in database
	return SAVING_FAILED;
}

void saveTransactionTest(void)
{
	ST_transaction_t my_transaction;
	strcpy(my_transaction.cardHolderData.primaryAccountNumber,"01234567891023456789");
	saveTransaction(&my_transaction);
	puts(transactions_dataBase[0].cardHolderData.primaryAccountNumber);
}

void recieveTransactionDataTest(void)
{
	ST_transaction_t my_transaction;

	printf("Tester Name: Mo'men Ahmed\nFunction Name: recieveTransactionData\n");
    printf("Test Case 1:\n");
	printf("Input Data: ");
	printf("insufficient fund\n");  //account has only 500$ and transaction amount is 2000$
	printf("Expected Result: 1\n"); //low balance
	printf("Actual Result: %d\n",recieveTransactionData(&my_transaction));
}

void listSavedTransactions(void)
{
	for (uint8_t i=0 ; i<number_of_saved_transactions ;i++)
	{
       printf("#########################\n");
       printf("Transaction Sequence Number: %d\n",transactions_dataBase[i].transactionSequenceNumber);
       printf("Transaction Date: ");
       puts(transactions_dataBase[i].terminalData.transactionDate);
       printf("Transaction Amount: %.2f\n",transactions_dataBase[i].terminalData.transAmount);
       printf("Transaction State: %d\n",transactions_dataBase[i].transState);
       printf("Terminal Max Amount: %.2f\n",transactions_dataBase[i].terminalData.maxTransAmount);
       printf("card holder name : ");
       puts(transactions_dataBase[i].cardHolderData.cardHolderName);
       printf("PAN: ");
       puts(transactions_dataBase[i].cardHolderData.primaryAccountNumber);
       printf("Card Expiration Date: ");
       puts(transactions_dataBase[i].cardHolderData.cardExpirationDate);
       printf("#########################");
	}
}

void listSavedTransactionsTest(void)
{
	ST_transaction_t trans1 ;
	trans1.transactionSequenceNumber=0;
	getTransactionDate(&my_terminal);
	strcpy(trans1.terminalData.transactionDate,my_terminal.transactionDate);
	trans1.terminalData.transAmount=750;
	trans1.transState= RUNNING;
	trans1.terminalData.maxTransAmount=5000;
	strcpy(trans1.cardHolderData.cardHolderName,"Mo'men Ahmed");
	strcpy(trans1.cardHolderData.primaryAccountNumber,"12345123451234512345");
	strcpy(trans1.cardHolderData.cardExpirationDate,"12/23");

	saveTransaction(&trans1);

	listSavedTransactions();
}
