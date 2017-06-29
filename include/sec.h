
#include <string>
using namespace std;
class sec
{
    public :
    sec( string = "", int = 0 );

//     accessor functions for accountNumber
    void setAccountNumber(int);
    int getAccountNumber() const;

//     accessor functions for lastName
    void setLastName(const string &);
    string getLastName() const;


private:
    char lastName[ 15 ];
    int accountNumber;
};

