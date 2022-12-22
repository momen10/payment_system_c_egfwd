 /******************************************************************************
 *
 * Module: Terminal
 *
 * File Name: terminal.c
 *
 * Description: source file for the terminal module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#include "Terminal.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "standard_types.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	time_t now =time(NULL) ;

	struct  tm* current_time = localtime(&now);

	strftime(termData->transactionDate  , 11 , "%d/%m/%Y",current_time);
	puts (termData->transactionDate);
	uint8_t date_length = strlen(termData->transactionDate);
	if(date_length <10 || termData->transactionDate [2] !='/'\
			|| termData->transactionDate [5] !='/' )
	{
		return WRONG_DATE;
	}
	else
	{
		return TERMINAL_OK;
	}
//	printf("%d\n" , strlen(termData->transactionDate));
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData , ST_terminalData_t *termData)
{
	//get card expiry date (years) and current year
    uint8_t card_exp_year[3] ;
    card_exp_year[0] = cardData->cardExpirationDate[3];
    card_exp_year[1] = cardData->cardExpirationDate[4];
    card_exp_year[2]='\0';

    uint8_t card_exp_month[3] ;
    card_exp_month[0] = cardData->cardExpirationDate[0];
    card_exp_month[1] = cardData->cardExpirationDate[1];
    card_exp_month[2]='\0';

    uint8_t current_year[3] ;
    current_year[0] = termData->transactionDate[8];
    current_year[1] = termData->transactionDate[9];
    current_year[2]='\0';

    uint8_t current_month[3] ;
    current_month[0] = termData->transactionDate[3];
    current_month[1] = termData->transactionDate[4];
    current_month[2]='\0';

    uint8_t check_valid_exp_year =1;
    {
    	sint8_t compare_years ;
    	compare_years = strcmp(current_year,card_exp_year);
    	if(compare_years<0)
    	{
    		check_valid_exp_year =VALID_YEAR ;//expiry year has not come yet (valid)
    	}
    	else if(compare_years ==0)  //same year
    	{
      		check_valid_exp_year = SAME_YEAR;
    	}
    	else
    	{
      		check_valid_exp_year = INVALID_YEAR;
    	}
    }

    uint8_t check_valid_exp_month =1;
    {
    	if(strcmp(current_month,card_exp_month)>0)
    	{
    		check_valid_exp_month =0;
    	}
    	else
    	{
    		//valid month
    	}
    }

    if(check_valid_exp_year == VALID_YEAR )
    {
    	printf("Valid card\n");
    	return TERMINAL_OK;
    }
    else if(check_valid_exp_year == INVALID_YEAR)
    {
   	 	printf("Expired card\n");
    	return EXPIRED_CARD;
    }
    else if(check_valid_exp_year == SAME_YEAR)
    {
        if(check_valid_exp_month == 0)
        {
      	 	printf("Expired card\n");
      	    return EXPIRED_CARD;
        }
        else
        {
           	printf("Valid card\n");
            return TERMINAL_OK;
        }
    }

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	float trans_amount;
	scanf(" %f",&trans_amount);
	termData->transAmount = trans_amount;
//	printf("%.3f\n",trans_amount);
	if(trans_amount<= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

void getTransactionAmountTest(void)
{
	ST_terminalData_t my_terminal;

	printf("Tester Name: Mo'men Ahmed\nFunction Name: getTransactionAmount\n");
	printf("Test Case 1: \nInput Data: ");
	uint8_t result=getTransactionAmount(&my_terminal);
	printf("%.2f \n",my_terminal.transAmount);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);

	printf("Test Case 2: \nInput Data: ");
	result = getTransactionAmount(&my_terminal);
	printf("%.2f \n",my_terminal.transAmount);
    printf("Expected Result: %d\n",4); //invalid amount
    printf("Actual Result: %d\n",result);

	printf("Test Case 3: \nInput Data: ");
	result = getTransactionAmount(&my_terminal);
	printf("%.2f \n",my_terminal.transAmount);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	printf("max amount of transaction = %0.2f\n",termData->maxTransAmount);
	if(termData->transAmount > termData->maxTransAmount)
	{
		printf("exceeded max amount of transaction\n");
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		printf("valid amount\n");
		return TERMINAL_OK;
	}
}

void isBelowMaxAmountTest(void)
{
	ST_terminalData_t my_terminal;
	my_terminal.maxTransAmount= 5000;

	printf("Tester Name: Mo'men Ahmed\nFunction Name: isBelowMaxAmount\n");
	printf("Test Case 1: \nInput Data: ");
	uint8_t result;
	getTransactionAmount(&my_terminal);
	result= isBelowMaxAmount(&my_terminal);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);

	printf("Test Case 2: \nInput Data: ");
	getTransactionAmount(&my_terminal);
	result= isBelowMaxAmount(&my_terminal);
    printf("Expected Result: %d\n",5); //exceeded maximum amount
    printf("Actual Result: %d\n",result);

	printf("Test Case 3: \nInput Data: ");
	getTransactionAmount(&my_terminal);
	result= isBelowMaxAmount(&my_terminal);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
	termData->maxTransAmount = maxAmount;
	if(maxAmount < 0)
	{
		return 	INVALID_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

void setMaxAmountTest(void)
{
	ST_terminalData_t my_terminal;
	float max_amount;
	uint8_t result;

	printf("Tester Name: Mo'men Ahmed\nFunction Name: setMaxAmount\n");
	printf("Test Case 1: \nInput Data: ");
	scanf(" %f",&max_amount);
	result = setMaxAmount(&my_terminal, max_amount);
    printf("%0.2f\n",my_terminal.maxTransAmount);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);

	printf("Test Case 2: \nInput Data: ");
	scanf(" %f",&max_amount);
	result = setMaxAmount(&my_terminal, max_amount);
    printf("%0.2f\n",my_terminal.maxTransAmount);
    printf("Expected Result: %d\n",6); //invalid max amount
    printf("Actual Result: %d\n",result);

	printf("Test Case 3: \nInput Data: ");
	scanf(" %f",&max_amount);
	result = setMaxAmount(&my_terminal, max_amount);
    printf("%0.2f\n",my_terminal.maxTransAmount);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);
}

//luhn's algorithm to check last number of card pan
EN_terminalError_t isValidCardPan(ST_cardData_t *cardData)
{
 uint8_t my_pan[20];
 strcpy(my_pan, cardData->primaryAccountNumber);
 uint8_t my_pan_length = strlen(cardData->primaryAccountNumber);
// puts(my_pan);
 uint8_t sum=0;
 uint8_t c;
 uint8_t digit;
 for(uint8_t i=0 ; i<my_pan_length-1; i++)
 {
	 if(i%2 ==1) //if odd rank, don't multiply it by two , we begin the array from index 0
		         //so we opposite our logic
	 {
	   digit =(uint8_t)(my_pan[i])%48 ;    //converting ascii of digits to integers
//	   printf("%d ",digit);
       sum+= digit;
	 }
	 else if(i%2 == 0) //if even rank, multiply it by two
	 {
		   digit = (((uint8_t)(my_pan[i]))%48) *2;
//		   printf("%d ",digit); //converting ascii of digits to integers
		   if(digit>=10)     // take the sum of its two digits
		   {
              sum+= (digit%10);
              digit/=10;
              sum+= digit;
		   }
		   else
		   {
			   sum+= digit;
		   }
	 }
 }
 c = 10 - (sum%10);
 printf("check sum = %d\n",c);
 if(c != (((uint8_t)(my_pan[my_pan_length-1]))%48))
 {
	 printf("invalid PAN \n");
	 return INVALID_CARD;
 }
 else
 {
	 printf("valid PAN \n");
	 return TERMINAL_OK;
 }
}

void isValidCardPANTest(void)
{
	ST_cardData_t my_card;
	uint8_t pan[20];
	uint8_t result;
	gets(pan);
	strcpy(my_card.primaryAccountNumber,pan);

	printf("Tester Name: Mo'men Ahmed\nFunction Name: isValidPan\n");
	printf("Test Case 1: \nInput Data: ");
	puts(pan);
	result = isValidCardPan(&my_card);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);

	printf("Test Case 2: \nInput Data: ");
	gets(pan);
	strcpy(my_card.primaryAccountNumber,pan);
	puts(pan);
	result = isValidCardPan(&my_card);
    printf("Expected Result: %d\n",3); //invalid card
    printf("Actual Result: %d\n",result);

	printf("Test Case 3: \nInput Data: ");
	gets(pan);
	strcpy(my_card.primaryAccountNumber,pan);
	puts(pan);
	result = isValidCardPan(&my_card);
    printf("Expected Result: %d\n",0); //terminal ok
    printf("Actual Result: %d\n",result);
}
