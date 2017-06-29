#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "ClientData.h"
#include "sec.h"
#include "primary.h"

using namespace std;
int enterChoice();
void createTextFile( fstream& );
void updateRecord( fstream& );
void newRecord( fstream& );
void deleteRecord( fstream& );
void BackupData();
void RestoreData();
void outputLine( ostream&, const ClientData & );
int getAccount( const char * const );
void Check();
void CreatPrmaryindex(fstream& ,fstream& );
void CreatSecandryindex(fstream&,fstream&);
void createPrimaryAndSecandryTextFile(fstream&,fstream&);

enum Choices { PRINT = 1, UPDATE, NEW, DELETE,Backup,Restoredata,check ,creatPrmaryindex ,creatSecandryindex ,
                CreatePrimaryAndSecandryTextFile , END };

int main()
{
    ClientData blankClient;
    primary p ;
    sec s ;
ofstream outCredit( "credit.dat", ios::out | ios::binary );
for ( int i = 0; i < 100; i++ )
       outCredit.write( reinterpret_cast< const char * >( &blankClient ),sizeof( ClientData ) );
ofstream Credit( "p.dat", ios::out | ios::binary );
ofstream ou( "s.dat", ios::out | ios::binary );

fstream inOutCredit( "credit.dat", ios::in | ios::out | ios::binary);
fstream inp( "p.dat", ios::in | ios::out | ios::binary);
fstream ins( "s.dat", ios::in | ios::out | ios::binary);

// exit program if fstream cannot open file
if ( !inOutCredit )
{
    cerr << "File could not be opened." << endl;
    exit (EXIT_FAILURE);
} // end if

int choice; // store user choice

// enable user to specify action
while ( ( choice = enterChoice() ) != END )
{
    switch ( choice )
    {
    case PRINT: // create text file from record file
        createTextFile( inOutCredit );
        break;
    case UPDATE: // update record
        updateRecord( inOutCredit );
        break;
    case NEW: // create record
        newRecord( inOutCredit );
        break;
    case DELETE: // delete existing record
        deleteRecord( inOutCredit );
        break;
    case Backup: // create Backup
            BackupData( );
            break;
    case Restoredata: // create RestoreData
            RestoreData();
            break;
    case check :
            Check();
            break;
    case creatPrmaryindex :
            CreatPrmaryindex(inOutCredit , inp );
            break;
    case creatSecandryindex:
            CreatSecandryindex(inOutCredit ,ins);
            break;
    case CreatePrimaryAndSecandryTextFile:
           createPrimaryAndSecandryTextFile(inp, ins);
            break;

    default: // display error if user does not select valid choice
        cerr << "Incorrect choice" << endl;
        break ;
    } // end switch

    inOutCredit.clear(); // reset end-of-file indicator
} // end while

return 0;
} // end main

// enable user to input menu choice
int enterChoice()
{
    // display available options
    cout << "\nEnter your choice" << endl
         << "1 - Store a formatted text file of accounts" << endl
         << "    called \"print.txt\" for printing" << endl
         << "2 - Update an account" << endl
         << "3 - Add a new account" << endl
         << "4 - Delete an account" << endl
         << "5 - BackupData  " << endl
         << "6 - RestoreData from BackupData" << endl
         << "7 - Check files" << endl
         << "8 - Create primary index file  "<< endl
         << "9 - Create secondary index file " << endl
         << "10- Create text files for Index Files " <<endl
         << "11- End program \n?" ;
    int menuChoice;
    cin >> menuChoice; // input menu selection from user
    return menuChoice;
} // end function enterChoice

// create formatted text file for printing
void createTextFile( fstream &readFromFile )
{
    // create text file
    ofstream outPrintFile( "print.txt", ios::out );

    // exit program if ofstream cannot create file
    if ( !outPrintFile )
    {
        cerr << "File could not be created." << endl;
        exit( EXIT_FAILURE);
    } // end if

    outPrintFile << left << setw( 10 ) << "Account" << setw( 16 )
                 << "Last Name" << setw( 11 ) << "First Name" << right
                 << setw( 10 ) << "Balance"<<right << setw( 16 )<<"Branch_ID"<< endl;

    // set file-position pointer to beginning of readFromFile
    readFromFile.seekg( 0 );

    // read first record from record file
    ClientData client;
    readFromFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );

    // copy all records from record file into text file
    while ( !readFromFile.eof() )
    {
        // write single record to text file
        if ( client.getAccountNumber() != 0 ) // skip empty records
            outputLine( outPrintFile, client );

        // read next record from record file
        readFromFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
    } // end while
} // end function createTextFile

// update balance in record
void updateRecord( fstream &updateFile )
{
    // obtain number of account to update
    int accountNumber = getAccount( "Enter account to update" );

    // move file-position pointer to correct record in file
    updateFile.seekg( ( accountNumber - 1 ) * sizeof( ClientData ) );

    // read first record from file
    ClientData client;
    updateFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );

    // update record
    if ( client.getAccountNumber() != 0 )
    {
        outputLine( cout, client ); // display the record

        // request user to specify transaction
        cout << "\nEnter charge (+) or payment (-): ";
        double transaction; // charge or payment
        cin >> transaction;



        // update record balance
        double oldBalance = client.getBalance();
        client.setBalance( oldBalance + transaction );
        outputLine( cout, client ); // display the record

        // move file-position pointer to correct record in file
        updateFile.seekp( ( accountNumber - 1 ) * sizeof( ClientData ) );

        // write updated record over old record in file
        updateFile.write( reinterpret_cast< const char * >( &client ),sizeof( ClientData ) );
    } // end if
    else // display error if account does not exist
        cerr << "Account #" << accountNumber
             << " has no information." << endl;
} // end function updateRecord

// create and insert record
void newRecord( fstream &insertInFile )
{
    // obtain number of account to create
    int accountNumber = getAccount( "Enter new account number" );

    // move file-position pointer to correct record in file
    insertInFile.seekg( ( accountNumber - 1 ) * sizeof( ClientData ) );

    // read record from file
    ClientData client;
    insertInFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );

    // create record, if record does not previously exist
    if ( client.getAccountNumber() == 0 )
    {
        string lastName;
        string firstName;
        double balance;
        double Branch_ID;

        // user enters last name, first name and balance
        cout << "Enter lastname, firstname, balance,BranchID\n? ";
        cin >> setw( 15 ) >> lastName;
        cin >> setw( 10 ) >> firstName;
        cin >> balance ;
        cin >> Branch_ID ;

        // use values to populate account values
        client.setLastName( lastName );
        client.setFirstName( firstName );
        client.setBalance( balance );
        client.setAccountNumber( accountNumber );
        client.setBranch_ID(Branch_ID);

        // move file-position pointer to correct record in file
        insertInFile.seekp( ( accountNumber - 1 ) * sizeof( ClientData ) );

        // insert record in file
        insertInFile.write( reinterpret_cast< const char * >( &client ),sizeof( ClientData ) );
    } // end if
    else // display error if account already exists
        cerr << "Account #" << accountNumber
             << " already contains information." << endl;
} // end function newRecord

// delete an existing record
void deleteRecord( fstream &deleteFromFile )
{
    // obtain number of account to delete
    int accountNumber = getAccount( "Enter account to delete" );

    // move file-position pointer to correct record in file
    deleteFromFile.seekg( ( accountNumber - 1 ) * sizeof( ClientData ) );

    // read record from file
    ClientData client;
    deleteFromFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );

    // delete record, if record exists in file
    if ( client.getAccountNumber() != 0 )
    {
        ClientData blankClient; // create blank record

        // move file-position pointer to correct record in file
        deleteFromFile.seekp( ( accountNumber - 1 ) *sizeof( ClientData ) );

        // replace existing record with blank record
        deleteFromFile.write(reinterpret_cast< const char * >( &blankClient ),sizeof( ClientData ) );

        cout << "Account #" << accountNumber << " deleted.\n";
    } // end if
    else // display error if record does not exist
        cerr << "Account #" << accountNumber << " is empty.\n";
} // end deleteRecord

// display single record
void outputLine( ostream &output, const ClientData &record )
{
    output << left << setw( 10 ) << record.getAccountNumber()
           << setw( 16 ) << record.getLastName()
           << setw( 11 ) << record.getFirstName()
           << setw( 10 ) << setprecision( 2 ) << right << fixed
           << showpoint << record.getBalance()
           << setw( 16 ) << record.getBranch_ID()<< endl;
} // end function outputLine

// obtain account-number value from user
int getAccount( const char * const prompt )
{
    int accountNumber;

    // obtain account-number value
    do
    {
        cout << prompt << " (1 - 100): ";
        cin >> accountNumber;
    }
    while ( accountNumber < 1 || accountNumber > 100 );

    return accountNumber;
} // end function getAccount

 void BackupData(){

  fstream back("backup.dat",ios::out | ios::binary);
  ifstream inCredit( "credit.dat", ios::in | ios::binary );

     ClientData client;
    inCredit.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
   while(!inCredit.eof()){
    cout<<"done"<<endl;
    back.write(reinterpret_cast< const char * >( &client ),sizeof( ClientData ) );
   inCredit.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
}

back.close();
}
void RestoreData(){

   fstream back ("backup.dat",ios::in| ios::binary);
   ClientData client;
     if ( !back )
    {
    cerr << "File could not be opened." << endl;

    }
    else{
        ofstream outCredit( "credit.dat", ios::out | ios::binary );
        back.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );

       while(!back.eof()){
          outCredit.write(reinterpret_cast< const char * >( &client ),sizeof( ClientData ) );
          cout<<"done"<<endl;

   back.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
      }
      back.close();
 }
}
void Check(){
ifstream outCredit("Credit.dat", ios::in| ios::binary);
   if ( !outCredit)
    {
    ofstream outCredit("Credit.dat",ios::out);
   ClientData blankClient;

    for ( int i = 0; i < 100; i++ )
               outCredit.write( reinterpret_cast< const char * >( &blankClient ),sizeof( ClientData ) );

outCredit.close();
    }

ifstream outbackup("backup.dat", ios::in);
    if ( !outbackup)
        {
        cout<<"an error on restore\n";
        }
}

void CreatPrmaryindex( fstream &insertInFile ,fstream &onsertInFile  )
{
    insertInFile.seekg(0);

    ClientData client;
    primary pri ;
    insertInFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
    // copy all records from record file into text file
    while ( !insertInFile.eof() )
    {
        pri.setAccountNumber(client.getAccountNumber());
        pri.setbos(    ( client.getAccountNumber() - 1 ) * sizeof( ClientData )     );
        //cout << pri.getAccountNumber()<<endl ;
        if(pri.getAccountNumber() != 0  )
            {
        // move file-position pointer to correct record in file
        onsertInFile.seekp( ( pri.getAccountNumber() - 1 ) * sizeof( primary ) );
        // insert record in file
        onsertInFile.write( reinterpret_cast< const char * >( &pri ),sizeof( primary) );
            }
        insertInFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
    }

 }

void CreatSecandryindex( fstream &insertInFile ,fstream &onsertInFile  )
{
    insertInFile.seekg(0);

    ClientData client;
    sec s ;
    insertInFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
    // copy all records from record file into text file
    while ( !insertInFile.eof() )
    {
        s.setAccountNumber(client.getAccountNumber());
        s.setLastName(client.getLastName());
        //cout << pri.getAccountNumber()<<endl ;
        if(s.getAccountNumber() != 0  )
            {
                // move file-position pointer to correct record in file
                onsertInFile.seekp( ( s.getAccountNumber() - 1 ) * sizeof( sec ) );
                // insert record in file
                onsertInFile.write( reinterpret_cast< const char * >( &s ),sizeof( sec) );
            }
        insertInFile.read( reinterpret_cast< char * >( &client ),sizeof( ClientData ) );
    }
 }
void outputPriLine( ostream &output, const primary &record )
{
    output << left << setw( 16 ) << record.getAccountNumber()
           << setw( 10 ) << record.getbos() << endl ;
} // end function outputLine
void outputSecLine( ostream &output, const sec &record )
{
    output << left << setw( 16 ) << record.getLastName()
           << setw( 10 ) <<record.getAccountNumber()<< endl;
} // end function outputLine

void createPrimaryAndSecandryTextFile( fstream &readFromFilep ,fstream &readFromFiles )
{
    // create text file
    ofstream outPriPrintFile( "PrimaryIndex.txt", ios::out );
    // create text file
    ofstream outSecPrintFile( "ScenadryIndex.txt", ios::out );

    // exit program if ofstream cannot create file
    if ( !outSecPrintFile && !outPriPrintFile )
    {
        cerr << "Files could not be created." << endl;
        exit( EXIT_FAILURE);
    } // end if

    outSecPrintFile << left << setw( 16 ) << "Last Name" << setw( 10 ) << "Account" <<endl;
    outPriPrintFile << left << setw( 16 ) << "Account" << setw( 10 ) << "Byte offset" <<endl;

    // set file-position pointer to beginning of readFromFile
    readFromFilep.seekg( 0 );
    readFromFiles.seekg( 0 );

    // read first record from record file
    primary pri;
    readFromFilep.read( reinterpret_cast< char * >( &pri ),sizeof( primary ) );

    sec s;
    readFromFiles.read( reinterpret_cast< char * >( &s ),sizeof( sec) );

    // copy all records from record file into text file
    while ( !readFromFilep.eof() )
    {
        // write single record to text file
        if (  pri.getAccountNumber()!=0  ) // skip empty records
              outputPriLine( outPriPrintFile, pri );

        // read next record from record file
        readFromFilep.read( reinterpret_cast< char * >( &pri ),sizeof( primary ) );
    } // end while
    // copy all records from record file into text file
    while ( !readFromFiles.eof() )
    {
        // write single record to text file
        if ( s.getAccountNumber()!= 0   ) // skip empty records
                {
                    outputSecLine( outSecPrintFile, s );
                }

        // read next record from record file
        readFromFiles.read( reinterpret_cast< char * >( &s ),sizeof( sec ) );
    } // end while
}
