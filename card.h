 /******************************************************************************
 *
 * Module: Card
 *
 * File Name: Card.h
 *
 * Description: header file for the card module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#ifndef CARD_H_
#define CARD_H_

#define MINIMUM_PAN_LENGTH     16
#define MAXIMUM_PAN_LENGTH     19
#define MINIMUM_NAME_LENGTH    20
#define MAXIMUM_NAME_LENGTH    24

#include "standard_types.h"

typedef struct ST_cardData_t
{
     uint8_t cardHolderName[25];
     uint8_t primaryAccountNumber[20];
     uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
     CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
void getCardHolderNameTest(void);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
void getCardExpiryDateTest (void);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
void getCardPANTest (void);


#endif /* CARD_H_ */
