
#include <string>
using namespace std;
class primary
{
    public :
    primary(int = 0, int = 0);

    void setAccountNumber(int);
    int getAccountNumber() const;

    void setbos(int);
    int getbos() const;

private :
    int accountNumber;
    int bos ;
};

