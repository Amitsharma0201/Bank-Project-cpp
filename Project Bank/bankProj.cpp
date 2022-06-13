#include <iostream>
#include <map>
#include <fstream>
using namespace std;

#define Min_Balance = 500;

class notEnoughFund{};

class Account
{
    private:
        long accNum;
        string fName;
        string lName;
        float bal;
        static long nextAccNum;

    public:

        Account(){}

        Account(string fName, string lName, float bal)
        {
            nextAccNum++;
            accNum = nextAccNum;
            this->fName = fName;
            this->lName = lName;
            this->bal = bal;
        }
        
        long getAccNum() { return accNum; }
        string getFName() { return fName; }
        string getLName() { return lName; }
        float getBal() { return bal; }

        static long getLastAccNum () { return nextAccNum; }
        static void setLastAccNum ( long accNum);

        float Deposit(float amount);
        float withdraw(float amount );

        friend ofstream &operator<<(ofstream &ofs, Account &acc);
        friend ifstream &operator>>(ifstream &fis, Account &acc);
        friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::nextAccNum = 0;

class Bank
{
    private:
        map<long, Account> accounts;

    public:
        Bank();

        Account openAccount(string fName, string lName, float bal);
        Account deposit(long accNum, float amount);
        Account withdraw(long accNum, float amount);
        Account balEnquiry(long accNum);
        void closeAcc(long accNum);
        void seeAllAccoounts();

        ~Bank();
};

int main()
{

    Bank b;
    Account acc;

    string fName, lName;
    float bal;
    float amount;
    long accNum;
    int choice;

    

    cout<<"Welcome to J&M International Bank ltd."<<endl<<endl;

    
    do{
        cout<<endl;
        cout<<"\t1.Open an account."<<endl;
        cout<<"\t2.Deposit."<<endl;
        cout<<"\t3.Withdraw."<<endl;
        cout<<"\t4.Balance Enquiry."<<endl;
        cout<<"\t5.Close an account."<<endl;
        cout<<"\t6.See all accounts."<<endl;
        cout<<"\t7.Quit."<<endl;
        cout<<"Please choose an option : ";
        cin>>choice;
        cout<<endl;

        switch(choice)
        {
            case 1: 
                cout<<"Enter your First Name : ";
                cin>>fName;
                cout<<endl<<"Enter your Last Name : ";
                cin>>lName;
                cout<<endl<<"Enter the amount you want to deposit(must be greater than 500rs.) : ";
                cin>>bal;
                acc = b.openAccount(fName, lName, bal);
                cout<<"Account opened successfully."<<endl;
                cout<<"Welcome to J&M, Thank you!"<<endl;

                break;

            case 2:
                cout<<"Enter Account Number : ";
                cin>>accNum;
                cout<<endl<<"Enter the amount : ";
                cin>>amount;
                acc = b.deposit(accNum, amount);
                cout<<endl<<"Deposit Success."<<endl<<acc<<endl;

                break;

            case 3:
                cout<<"Enter Your Account Number : ";
                cin>>accNum;
                cout<<endl<<"Enter amount : ";
                cin>>amount;
                acc = b.withdraw(accNum, amount);
                cout<<endl<<"Withdraw Success."<<endl<<acc<<endl;

                break;

            case 4:
                cout<<"Enter Account Number : ";
                cin>>accNum;
                acc = b.balEnquiry(accNum);
                cout<<acc;

                break;

            case 5:
                cout<<endl<<"Enter Account Number : ";
                cin>>accNum;
                b.closeAcc(accNum);
                cout<<endl<<"Account closed Successfully."<<endl;

                break;

            case 6:
                b.seeAllAccoounts();
                break;

            case 7:
                
                break;

            default:
                cout<<"\t\tEnter Correct choice."<<endl;

        }
    }while(choice != 7);
}

float Account:: Deposit(float amount)
{
    bal += amount;
    return bal;
}

float Account :: withdraw(float amount)
{
    
    if(bal-amount <= 500)
    {
        throw notEnoughFund();
        return 0.0f;
    }
    return bal-=amount;
}

Bank :: Bank()
{
    Account acc;
    ifstream infile;
    infile.open("BankJ&M.data");
    if(!infile)
    {
        cout<<"Error in opening file. Maybe file doesnot exist."<<endl;
        return;
    }
    while(!infile.eof())
    {
        infile>>acc;
        accounts.insert(pair<long,Account>(acc.getAccNum(), acc));
    }
    Account::setLastAccNum(acc.getAccNum());

    infile.close();
}

Account Bank :: openAccount(string fName, string lName, float amount)
{
    ofstream outfile;
    Account acc(fName, lName, amount);

    accounts.insert(pair<long, Account>(acc.getAccNum(), acc));

    outfile.open("BankJ&M.data", ios:: trunc);

    map<long,Account> :: iterator itr;
    for(itr = accounts.begin(); itr!=accounts.end(); itr++)
    {
        outfile<<itr->second;
    }

    outfile.close();
    return acc;
}

Account Bank :: deposit(long accNum, float amount)
{
    map<long,Account> :: iterator itr = accounts.find(accNum);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank :: withdraw(long accNum, float amount)
{
    map<long, Account> :: iterator itr = accounts.find(accNum);
    itr->second.withdraw(amount);
    return itr->second;
}

Account Bank :: balEnquiry(long accNum)
{
    map<long, Account> :: iterator itr = accounts.find(accNum);
    return itr->second;
}

void Bank :: closeAcc(long accNum)
{
    map<long, Account> :: iterator itr = accounts.find(accNum);
    accounts.erase(itr);
}

void Bank :: seeAllAccoounts()
{
    map<long, Account> ::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout<<itr->second<<endl;
    }
}

Bank :: ~Bank()
{
    ofstream outfile;
    outfile.open("BankJ&M.data");

    map<long, Account> :: iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
}

void Account :: setLastAccNum(long accNum)
{
    nextAccNum = accNum;
}

ofstream & operator<<(ofstream &fos, Account &acc)
{
    fos<<acc.accNum<<endl;
    fos<<acc.fName<<endl;
    fos<<acc.lName<<endl;
    fos<<acc.bal<<endl<<endl;

    return fos;
}

ifstream & operator>>(ifstream &fis, Account &acc)
{
    fis>>acc.accNum;
    fis>>acc.fName;
    fis>>acc.lName;
    fis>>acc.bal;

    return fis;
}

ostream &operator<<(ostream &os, Account &acc)
{
    cout<<"Account Detail..."<<endl;
    cout<<"Name : "<<acc.fName<<" "<<acc.lName<<endl;
    cout<<"Account No. : "<<acc.accNum<<endl;
    cout<<"Balance : "<<acc.bal<<endl;

    return cout;
}
