 /******************************************************************************
 *
 * Module: Terminal
 *
 * File Name: Terminal.h
 *
 * Description: header file for the terminal module
 *
 * Author: Mo'men Ahmed
 *
 *******************************************************************************/
#ifndef TERMINAL_H_
#define TERMINAL_H_

#define VALID_YEAR     1
#define SAME_YEAR      2
#define INVALID_YEAR   0

#include "standard_types.h"
#include "card.h"

typedef struct ST_terminalData_t
{
     float transAmount;
     float maxTransAmount;
     uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
     TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT,\
	 EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t ;

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData ,ST_terminalData_t *termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t isValidCardPan(ST_cardData_t *cardData);
void isValidCardPANTest(void);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount);
void getTransactionAmountTest(void);
void isBelowMaxAmountTest(void);
#endif /* TERMINAL_H_ */
