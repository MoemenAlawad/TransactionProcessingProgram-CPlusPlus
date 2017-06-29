#include "primary.h"
#include <string>
using namespace std;
// default ClientData constructor
primary::primary(int accountNumberValue , int a)
{
    setAccountNumber(accountNumberValue);
    setbos(a);

} // end ClientData constructor

// get account-number value
int primary::getAccountNumber() const
{
    return accountNumber;
} // end function getAccountNumber

// set account-number value
void primary::setAccountNumber(int accountNumberValue)
{
    accountNumber = accountNumberValue; // should validate
} // end function setAccountNumber

// get account-number value
int primary::getbos() const
{
    return bos;
} // end function getAccountNumber

// set account-number value
void primary::setbos(int a)
{
    bos = a; // should validate
} // end function setAccountNumber
