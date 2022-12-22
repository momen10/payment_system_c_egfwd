 /******************************************************************************
 *
 * Module: Card
 *
 * File Name: Card.c
 *
 * Description: source file for the card module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#include "card.h"
#include "standard_types.h"
#include <string.h>
#include <ctype.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	uint8_t holder_name [25];
	gets(holder_name);

    strcpy(cardData ->cardHolderName , holder_name);

	if (!(strcmp(holder_name,"\0")) ||strlen(holder_name) < MINIMUM_NAME_LENGTH \
		                            || strlen(holder_name) >MAXIMUM_NAME_LENGTH )
   {
	   return WRONG_NAME;
   }
   else
   {
	   return CARD_OK;
   }
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
  uint8_t date [6];
  gets(date);

  strcpy(cardData->cardExpirationDate , date);
  uint8_t check_valid_month =1;
  {
	  uint8_t month[3];
	  month[0]=date[0];
	  month[1]=date[1];
	  month[2]='\0';
	  if(strcmp(month,"12") >0)
	  {
		  check_valid_month = 0;       //invalid month
	  }
	  else
	  {
            // valid month
	  }
  }

  uint8_t check_date_characters =0;
  for(uint8_t i=0;i<strlen(date); i++)
  {
	  uint8_t check_digit = isdigit(date[i]);
	  if(!check_digit)
	  {
		  check_date_characters++;
	  }
	  else
	  {

	  }
  }
  if(strlen(date)!=5 || date[2] != '/' || check_date_characters != 1 || !check_valid_month)
  {
	  return WRONG_EXP_DATE;
  }
  else
  {
	  return CARD_OK;
  }
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	uint8_t card_pan [20];
	gets(card_pan);
    strcpy(cardData ->primaryAccountNumber , card_pan);
    uint8_t check_pan_digits=1;
    for (uint8_t i=0 ; i<strlen(card_pan); i++)
    {
    	if(!isdigit(card_pan[i]))
    	{
    		check_pan_digits =0;
    		break;
    	}
    }

    uint8_t pan_length = strlen(card_pan);
    if (pan_length <MINIMUM_PAN_LENGTH || pan_length >MAXIMUM_PAN_LENGTH || !check_pan_digits)
    {
    	return WRONG_PAN;
    }
    else
    {
    	return CARD_OK;
    }
}

void getCardHolderNameTest(void)
{
	ST_cardData_t my_card ;
	uint8_t valid;
	valid=getCardHolderName(&my_card);
	printf("Tester Name: Mo'men ahmed \nFunction Name: getCardHolderName \nTest Case 1:\nInput Data:");
    puts(my_card.cardHolderName);
    printf("Expected Result: %d\n",CARD_OK);
    printf("Actual Result: %d\n",valid);

	valid= getCardHolderName(&my_card);
	printf("Test Case 2:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.cardHolderName,WRONG_NAME,valid);

	valid=getCardHolderName(&my_card);
	printf("Test Case 3:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.cardHolderName,WRONG_NAME,valid);

	valid=getCardHolderName(&my_card);
	printf("Test Case 4:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.cardHolderName,WRONG_NAME,valid);
}

void getCardExpiryDateTest (void)
{
	ST_cardData_t my_card ;
	uint8_t valid;
	valid=getCardExpiryDate(&my_card);
	printf("Tester Name: Mo'men ahmed \nFunction Name: getCardExpiryDate \nTest Case 1:\nInput Data:");
	puts(my_card.cardExpirationDate);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",valid);

	valid= getCardExpiryDate(&my_card);
	printf("Test Case 2:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.cardExpirationDate,WRONG_EXP_DATE,valid);

	valid= getCardExpiryDate(&my_card);
	printf("Test Case 3:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
				, my_card.cardExpirationDate,WRONG_EXP_DATE,valid);


	valid= getCardExpiryDate(&my_card);
	printf("Test Case 4:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
				, my_card.cardExpirationDate,WRONG_EXP_DATE,valid);

}

void getCardPANTest (void)
{
	ST_cardData_t my_card ;
	uint8_t valid;
	valid=getCardPAN(&my_card);
	printf("Tester Name: Mo'men ahmed \nFunction Name: getCardPan \nTest Case 1:\nInput Data:");
    puts(my_card.primaryAccountNumber);
    printf("Expected Result: %d\n",CARD_OK);
    printf("Actual Result: %d\n",valid);

	valid=getCardPAN(&my_card);
	printf("Test Case 2:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.primaryAccountNumber,WRONG_PAN,valid);

	valid=getCardPAN(&my_card);
	printf("Test Case 3:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.primaryAccountNumber,WRONG_PAN,valid);

	valid=getCardPAN(&my_card);
	printf("Test Case 4:\nInput Data:%s \nExpected Result: %d \nActual Result: %d\n"\
			, my_card.primaryAccountNumber,WRONG_PAN,valid);
}
