#include "sec.h"
#include <string>
using namespace std;
// default ClientData constructor
sec::sec( string lastNameValue , int accountNumberValue)
{
    setAccountNumber(accountNumberValue);
    setLastName(lastNameValue);
} // end ClientData constructor

// get account-number value
int sec::getAccountNumber() const
{
    return accountNumber;
} // end function getAccountNumber

// set account-number value
void sec::setAccountNumber(int accountNumberValue)
{
    accountNumber = accountNumberValue; // should validate
} // end function setAccountNumber

// get last-name value
string sec::getLastName() const
{
    return lastName;
} // end function getLastName

// set last-name value
void sec::setLastName(const string &lastNameString)
{
    // copy at most 15 characters from string to lastName
    int length = lastNameString.size();
    length = (length < 15 ? length : 14);
    lastNameString.copy(lastName, length);
    lastName[ length ] = '\0'; // append null character to lastName
} // end function setLastName


