/*
Group Members:
Komal Waseem 20L-1114
Nisa Nadeem 20L-1141
Nabeeha Mudassir 20L-1080
*/

#include <iostream>
#include <fstream>

using namespace std;

class Employee;
class Customer;
class Bill;
class TariffTax;
class LESCO;

// class for static helper functions
class Helper
{
public:
	// allocates memory on the heap for a string and copies the contents of a temporary buffer into it
	static char* GetStringFromBuffer(char* buffer)
	{
		int strLen = strlen(buffer);
		char* NewStr = 0;

		if (strLen > 0)
		{
			NewStr = new char[strLen + 1];

			for (int i = 0; i < strLen; i++)
			{
				NewStr[i] = buffer[i];
			}
			NewStr[strLen] = '\0';
		}
		return NewStr;
	}

	// returns a substring from str starting from the index and the length sent in the parameters
	static char* Substring(char* str, int start_index, int length)
	{
		if (start_index > strlen(str) || length + start_index > strlen(str))
			return 0;

		char* substr = new char[length + 1];

		int j = 0;
		for (int i = start_index; j < length; i++, j++)
		{
			substr[j] = str[i];
		}
		substr[j] = '\0';

		return substr;
	}

	// returns the index of the first comma in a string after an index specified by the user
	static int FindNextCommaIndex(char* tempBuffer, int index)
	{
		for (int i = index; i < strlen(tempBuffer); i++)
		{
			if (tempBuffer[i] == ',')
				return i;
		}
		return strlen(tempBuffer);
	}

	// returns true if both c-strings are equal
	static bool IsEqual(char* str1, char* str2)
	{
		if (strlen(str1) != strlen(str2))
			return false;

		for (int i=0; i < strlen(str1); i++)
		{
			if (str1[i] != str2[i])
				return false;
		}

		return true;
	}
};

class Bill
{
private:
	char ID[5]; // Customer ID
	char Month[4]; // format: JAN-DEC
	int RegularReading;
	int PeakReading;
	char EntryDate[11]; // format: yyyy/dd/mm
	int Cost;
	double SalesTaxAmount;
	int FixedCharges;
	int TotalBilling;
	char DueDate[11];
	char* PaidStatus; // format: PAID/UNPAID
	char BillPaymentDate[11];

public:
	Bill(); // default constructor
	
	void SetBill(char*,char*,int,int,char*,char*,char*,char*,int); // sets bill values entered by employee (paramters) and also calculates bill and sets it

	char* GetID(); // getter for Customer ID

	void ReadFromFile(ifstream&); // loads bill data from file
	void SaveData(ofstream&); // writes bill data into file (to update data)

	void PrintBill(); // prints bill information


	void SetPaymentStatus()
	{
		char Temp[5] = "PAID";
		if (PaidStatus)
			delete [] PaidStatus;
		PaidStatus = Helper::GetStringFromBuffer(Temp);
	}

	void SetBillPaymentDate(char* NewDate)
	{
		strcpy(BillPaymentDate,NewDate);
	}

	static void CalculateAndDisplayBill(char*,char*,int,int); // calculates the bill whose meter reading customer has entered (not to be saved in file)

	~Bill(); // destructor
};

class Customer
{
private:
	int TotalCustomers; //Will be shifted in LESCO Class Later
	
	char ID[5]; //Username
	char DateOfBirth[11]; //Password //format = 
	char* Name;
	char* Address;
	char PhoneNumber[12];
	char CustomerType; // C for Commercial, D for Domestic
	char MeterType; //S for SinglePhase, T for ThreePhase
	char ConnectionDate[11];
	char* RegularUnits;
	char* PeakUnits;

	char PaymentStatus; //P means Paid, U means Unpaid

	Bill* CurrentBill; 
public:
	Customer();

	void AllocateBillSpace();
	void AssociateBill(Bill* B);
	void UpdatePaymentStatus();
	void PrintCustomerInfo();
	void PrintCustomerBill(); // prints current bill alongwith customer information
	void ViewCalculatedBill(char*, char*, int, int); // calculates and displays bill

	void ReadCustomerFile(ifstream&);
	void SaveData(ofstream&);

	// getters:
	char* GetID();
	char* GetDoB();
	Bill* GetCurrentBill()
	{
		return CurrentBill;
	}
	char GetCustomerType();
	char GetMeterType();

	void SetCustomer(char ID_[], char DOB_[], char Contact[], char* Name_, char* Address_, char* CDate,char C, char M);

	~Customer();
};

class Employee
{
private:
	char* Username;
	char* Password;

public:
	Employee(); // default constructor

	void ReadFromFile(ifstream&); // loads employee data from file

	void SetPassword(char*); // setter for password

	// getters:
	char* GetUsername();
	char* GetPassword();

	void SaveData(ofstream&); // writes employee data into file (for updating)

	~Employee(); // destructor
};

class TariffTax
{
private:
	char* MeterType; // 1PHASE/3PHASE
	int RegularUnitPrice;
	int PeakUnitPrice;
	int PercentageTax;
	int FixedCharges;

public:
	TariffTax(); // default constructor

	void ReadFromFile(ifstream& ReadFile); // loads tariff/tax data from file
	void UpdateTaxRecords(ofstream&); //Over-writes the Same File with Updated Information

	// getters:
	int GetRegularUnitPrice();
	int GetPeakUnitPrice();
	int GetPercentageTax();
	int GetFixedCharges();

	void SetRegularUnitPrice(int Unit);
	void SetPeakUnitPrice(int Unit);
	void SetFixedCharges(int Charges);
	void SetPercentageTax(int Tax);
	~TariffTax(); // destructor
};

class LESCO
{
private:
	Employee** AllEmployees;
	int TotalEmployees;
	Bill** AllBills;
	int  TotalBills;
	Customer** AllCustomer;
	int TotalCustomer;

	Employee* CurrentEmployee;
	Customer* CurrentCustomer;

public:
	static TariffTax* TariffTaxInformation[4];
	static void LoadTariffTaxData();

	LESCO();

	bool ProcessMenu();
	void LoadEmployeeData();
	void LoadCustomerData();
	void LoadBillData();
	void UpdateTariffTaxFile();
	void UpdatePaymentStatus();
	void GenerateNewTariffFile();
	
	void AssociateBillandCustomer();

	// searching functions that return required pointer:
	Employee* SearchEmployeeByUsername(char* Username);
	Customer* SearchCustomerByID(char* ID);
	Bill* SearchBillByID(char* ID);
	bool SearchCustomerPassword(char* Pass);
	bool SearchEmployeePassword(char* Pass);

	void ChangeEmployeePassword(); // re-enters username and current password, also prompts user for new password and changes it (also updates in file)
	void AddCustomer();

	void AddBill(); // adds a new bill in the bill file

	// for updating files:
	void UpdateEmployeeFile();
	void UpdateCustomerFile();
	void UpdateBillFile();
	// void UpdateTariffTaxFile();

	// for testing only:
	void DisplayAllBills();
	void DisplayAllCustomersData();

	void DisplayReport(); // prompts for customer ID and displays their report to Employee

	// viewing bill:
	void ViewBillToEmployee(char* CustomerID );
	void ViewBillToCustomer();

	// setters for current user:
	void SetCurrentCustomer(char*);
	void SetCurrentEmployee(char*);

	~LESCO();

};

TariffTax* LESCO::TariffTaxInformation[4] = {0};


// Bill functions: 

Bill::Bill()
{
	strcpy(ID, "");
	strcpy(Month, "");
	RegularReading = 0;
	PeakReading = 0;
	strcpy(EntryDate, "");
	Cost = 0;
	SalesTaxAmount = 0;
	FixedCharges = 0;
	TotalBilling = 0;
	strcpy(DueDate, "");
	PaidStatus = 0;
	strcpy(BillPaymentDate, "");
}

char* Bill::GetID()
{
	return ID;
}

void Bill::SetBill(char* _ID, char* _month, int _RegUnits, int _PeakUnits, char* _EntryDate, char* _DueDate, char* _Status, char* _PaymentDate, int TariffTaxIndex)
{
	cout<<ID;
	strcpy(ID , _ID);
	strcpy(Month , _month);
	RegularReading = _RegUnits;
	PeakReading = _PeakUnits;
	strcpy(EntryDate , _EntryDate);
	strcpy(DueDate , _DueDate);
	strcpy(BillPaymentDate , _PaymentDate);
	PaidStatus = Helper::GetStringFromBuffer(_Status);

	Cost = _RegUnits*(LESCO::TariffTaxInformation[TariffTaxIndex]->GetRegularUnitPrice()) + _PeakUnits*(LESCO::TariffTaxInformation[TariffTaxIndex]->GetPeakUnitPrice());
	FixedCharges =  LESCO::TariffTaxInformation[TariffTaxIndex]->GetFixedCharges();
	SalesTaxAmount = (LESCO::TariffTaxInformation[TariffTaxIndex]->GetPercentageTax())/100.0 * (double)Cost;
	TotalBilling = Cost + FixedCharges + SalesTaxAmount;
}

void Bill::ReadFromFile(ifstream& ReadFile)
{
	char** tempBuffer = 0;
	char FileLine[300];

	ReadFile.getline(FileLine,300); //Change Made here by Nabeeha
	//cout << "\nTesting File Line = "<<FileLine<<endl; //uncomment to confirm
	
		
	int previousComma = -1;
	tempBuffer = new char* [12]; // temporary array to store cstrings after being separated
	for (int i = 0; i < 12; i++)
	{
		int nextComma = Helper::FindNextCommaIndex(FileLine, previousComma + 1);
		if (nextComma == previousComma + 1 || nextComma == previousComma)
			tempBuffer[i] = 0;
		else
			tempBuffer[i] = Helper::Substring(FileLine, previousComma + 1, nextComma - previousComma - 1);
		previousComma = nextComma;
	}

	strcpy(ID, tempBuffer[0]);
	strcpy(Month, tempBuffer[1]);
	RegularReading = atoi(tempBuffer[2]);
	if (tempBuffer[3])
		PeakReading = atoi(tempBuffer[3]);
	strcpy(EntryDate, tempBuffer[4]);
	Cost = atoi(tempBuffer[5]);
	SalesTaxAmount = atoi(tempBuffer[6]);
	FixedCharges = atoi(tempBuffer[7]);
	TotalBilling = atoi(tempBuffer[8]);
	strcpy(DueDate, tempBuffer[9]);
	PaidStatus = Helper::GetStringFromBuffer(tempBuffer[10]);
	if (tempBuffer[11])
		strcpy(BillPaymentDate, tempBuffer[11]);

	for (int i = 0; i < 12; i++)
	{
		if (tempBuffer[i])
		{
			delete[] tempBuffer[i];
			tempBuffer[i] = 0;
		}
	}
}

void Bill::SaveData(ofstream& FOUT)
{
	FOUT<<endl<<ID<<","<<Month<<","<<RegularReading<<","<<PeakReading<<","<<EntryDate<<","<<Cost<<","<<SalesTaxAmount<<","<<FixedCharges<<","<<TotalBilling<<","<<DueDate<<","<<PaidStatus<<","<<BillPaymentDate;
}

void Bill::PrintBill()
{
	cout << "\n---Bill Information: " << endl;
	cout << "\t Billing month: " << Month << endl;
	cout << "\t Cost of Electricity: " << Cost << endl;
	cout << "\t Tax: " << SalesTaxAmount << endl;
	cout << "\t Fixed Charges: " << FixedCharges << endl;
	cout << "\t Total Amount Due: " << TotalBilling << endl;
	cout << "\t Due Date: " << DueDate << endl;
	cout << "\t Payment Status: " << PaidStatus << endl << endl;
}

void Bill::CalculateAndDisplayBill(char* Phases,char* CustomerType,int _RegularReading,int _PeakReading)
{
	Bill NewBill;

	int TariffTaxIndex;
	if (Helper::IsEqual(Phases,"S"))
	{
		if (Helper::IsEqual(CustomerType,"D"))
		{
			TariffTaxIndex = 0;
		}
		else
		{
			TariffTaxIndex = 1;
		}
	}
	else
	{
		if (Helper::IsEqual(CustomerType,"D"))
		{
			TariffTaxIndex = 2;
		}
		else
		{
			TariffTaxIndex = 3;
		}
	}

	NewBill.PeakReading = _PeakReading;
	NewBill.RegularReading = _RegularReading;
	NewBill.PaidStatus = Helper::GetStringFromBuffer("UNPAID");
	NewBill.Cost = _RegularReading*(LESCO::TariffTaxInformation[TariffTaxIndex]->GetRegularUnitPrice()) + _PeakReading*(LESCO::TariffTaxInformation[TariffTaxIndex]->GetPeakUnitPrice());
	NewBill.FixedCharges =  LESCO::TariffTaxInformation[TariffTaxIndex]->GetFixedCharges();
	NewBill.SalesTaxAmount = (LESCO::TariffTaxInformation[TariffTaxIndex]->GetPercentageTax())/100.0 * (double)NewBill.Cost;
	NewBill.TotalBilling = NewBill.Cost + NewBill.FixedCharges + NewBill.SalesTaxAmount;
	strcpy(NewBill.DueDate,"15/07/2021");
	strcpy(NewBill.Month,"JUN");
	NewBill.PrintBill();
}

Bill::~Bill()
{
	if (PaidStatus)
		PaidStatus = 0;
}


// Employee functions:

Employee::Employee()
{
	Username = 0;
	Password = 0;
}

void Employee::ReadFromFile(ifstream& ReadFile)
{
	char tempBuffer[200];

	ReadFile >> tempBuffer;

	int i = Helper::FindNextCommaIndex(tempBuffer, 0);
	Username = Helper::Substring(tempBuffer, 0, i);
	Password = Helper::Substring(tempBuffer, i + 1, strlen(tempBuffer) - i - 1);
}

void Employee::SetPassword(char* NewPassword)
{
	if (Password)
		delete[] Password;

	Password = Helper::GetStringFromBuffer(NewPassword);
}

void Employee::SaveData(ofstream& WriteFile)
{
	WriteFile<<endl<<Username<<","<<Password;
}

char* Employee::GetUsername()
{
	return Username;
}

char* Employee::GetPassword()
{
	return Password;
}

Employee::~Employee()
{
	if (Username)
		delete[] Username;
	if (Password)
		delete[] Password;
}


// Customer functions:

Customer::Customer()
{
	TotalCustomers = 0; //Remove Later;
	Name = Address = RegularUnits = PeakUnits = 0;
}

void Customer::AllocateBillSpace()
{
	CurrentBill = new Bill;
}

void Customer::AssociateBill(Bill* B)
{
	CurrentBill = B;
}

void Customer::PrintCustomerBill()
{
	PrintCustomerInfo();

	// CustomerBill -> PrintBill();
}

void Customer::SaveData(ofstream& FOUT)
{
	FOUT<<endl<<ID<<","<<DateOfBirth<<","<<Name<<","<<Address<<","<<PhoneNumber<<","<<CustomerType<<","<<MeterType<<","<<ConnectionDate<<","<<RegularUnits<<","<<PeakUnits<<","<<PaymentStatus;
}

void Customer::ViewCalculatedBill(char* _MeterType, char* _CustomerType, int _RegularReading, int _PeakReading)
{
	cout<<"Customer Information: "<<endl;
	PrintCustomerInfo();
	cout<<endl;

	Bill::CalculateAndDisplayBill(_MeterType,_CustomerType,_RegularReading,_PeakReading);
}

void Customer::ReadCustomerFile(ifstream& FIN)
{
	char Temp[500];
	FIN.getline(Temp,500);

	//Starting from T = 0;
	int T = 0; //Index Iterator for Temp

	int Comma = Helper::FindNextCommaIndex(Temp,T);
	int id = 0; //iterator for ID
	for (int j = T; j < Comma; j++)
	{
		ID[id] = Temp[j];
		id++;
	}
	ID[4] = '\0';
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T + 1);
	int dob = 0;
	for (int j = T + 1; j < Comma; j++)
	{
		DateOfBirth[dob] = Temp[j];
		dob++;
	}
	DateOfBirth[10] = '\0'; //Confirm Later - Fine
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	int name = 0;
	char TempName[50];
	for (int j = T + 1; j < Comma; j++)
	{
		TempName[name] = Temp[j];
		name++;
	}
	TempName[name] = '\0';
	Name = Helper::GetStringFromBuffer(TempName);
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	int address = 0;
	char TempAdd[50];
	for (int j = T + 1; j < Comma; j++)
	{
		TempAdd[address] = Temp[j];
		address++;
	}
	TempAdd[address] = '\0';
	Address = Helper::GetStringFromBuffer(TempAdd);
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T + 1);
	int phone = 0;
	for (int j = T + 1; j < Comma; j++)
	{
		PhoneNumber[phone] = Temp[j];
		phone++;
	}
	PhoneNumber[11] = '\0'; //Confirm Later - Fine
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	for (int j = T+1; j < Comma; j++)
	{
		CustomerType = Temp[j];
	}
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	for (int j = T+1; j < Comma; j++)
	{
		MeterType = Temp[j];
	}
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	int cd = 0;
	for (int j = T+1; j < Comma; j++)
	{
		ConnectionDate[cd] = Temp[j];
		cd++;
	}
	ConnectionDate[10] = '\0';
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	int ru = 0; char TempRegUnits[5];
	for (int j = T+1; j < Comma; j++)
	{
		TempRegUnits[ru] = Temp[j];
		ru++;
	}
	TempRegUnits[ru] = '\0';
	RegularUnits = Helper::GetStringFromBuffer(TempRegUnits);
	T = Comma;

	Comma = Helper::FindNextCommaIndex(Temp,T+1);
	int pu = 0; char TempPeakUnits[5];
	for (int j = T+1; j < Comma; j++)
	{
		TempPeakUnits[pu] = Temp[j];
		pu++;
	}
	TempPeakUnits[pu] = '\0';
	PeakUnits = Helper::GetStringFromBuffer(TempPeakUnits);
	T = Comma;

	//No comma to be found for the last field
	PaymentStatus = Temp[T+1];
}

char* Customer::GetID()
{
	return ID;
}

char* Customer::GetDoB()
{
	return DateOfBirth;
}

char Customer::GetCustomerType()
{
	return CustomerType;
}

char Customer::GetMeterType()
{
	return MeterType;
}

void Customer::PrintCustomerInfo()
{
	cout << "\n\t\t--Customer Info--";
	cout << "\nName: "<<"\t"<<Name;
	cout << "\nID: "<<"\t"<<ID;
	cout << "\nPhone Number: "<<"\t"<<PhoneNumber;
	cout << "\nAddress: "<<"\t"<<Address;
	cout << "\nDate of Birth: "<<"\t"<<DateOfBirth;
	cout << "\nCustomer Type: "<<"\t";
	if (CustomerType == 'C')
		cout << "Commercial";
	else
		cout << "Domestic";
	cout << "\nMeter Type: "<<"\t";
	if (MeterType =='S')
		cout << "Single-Phase Meter";
	else
		cout << "Three-Phase Meter";
	cout << "\nConnection Date: "<<"\t"<<ConnectionDate;
	cout << "\nRegular Hour Units Consumed: "<<"\t"<<RegularUnits;
	cout << "\nPeak Hour Units Consumed: "<<"\t"<<PeakUnits;
	cout << endl;
}

//Changes the status to PAID for the Current Bill
void Customer::UpdatePaymentStatus()
{
	char _Date[11];
	cout << "\nEnter the Bill Payment Date. "; //Must be in format
	cin >> _Date;
	
	CurrentBill ->SetBillPaymentDate(_Date); //Change the Date of Bill*
	

	CurrentBill -> SetPaymentStatus(); //Change Bill Pointer Data
	PaymentStatus = 'P'; //Change the Payment Status of Customer's own records

	cout << "\nBill of Customer " << ID << " of Date: " << _Date<<" has been changes to PAID. \n";

}

Customer::~Customer()
{
	if (Name)
		delete [] Name;
	if (Address)
		delete [] Address;
	if (RegularUnits)
		delete [] RegularUnits;
	if (PeakUnits)
		delete [] PeakUnits;
}

void Customer::SetCustomer(char ID_[], char DOB_[], char Contact[], char* Name_, char* Address_, char* CDate,char C, char M)
{
	strcpy(ID,ID_);
	strcpy(DateOfBirth,DOB_);
	strcpy(PhoneNumber,Contact);
	Name = Helper::GetStringFromBuffer(Name_);
	Address = Helper::GetStringFromBuffer(Address_);

	CustomerType = C;
	MeterType = M;
	strcpy(ConnectionDate,CDate);

	char Null[] = "0";
	RegularUnits = Helper::GetStringFromBuffer(Null);
	PeakUnits = Helper::GetStringFromBuffer(Null);

	PaymentStatus = 'U';

}



// TariffTax functions:

TariffTax::TariffTax()
{
	MeterType = 0;
	RegularUnitPrice = 0;
	PeakUnitPrice = 0;
	PercentageTax = 0;
	FixedCharges = 0;
}

void TariffTax::ReadFromFile(ifstream& ReadFile)
{
	char FileLine[50];
	char** tempBuffer = 0;
	ReadFile >> FileLine;

	MeterType = Helper::Substring(FileLine, 0, 6);

	int previousComma = 6;
	tempBuffer = new char* [5]; // temporary array to store cstrings after being separated
	for (int i = 0; i < 4; i++)
	{
		int nextComma = Helper::FindNextCommaIndex(FileLine, previousComma + 1);
		if (nextComma == previousComma + 1)
			tempBuffer[i] = 0;
		else
			tempBuffer[i] = Helper::Substring(FileLine, previousComma + 1, nextComma - previousComma - 1);
		previousComma = nextComma;
	}

	RegularUnitPrice = atoi(tempBuffer[0]);
	if (tempBuffer[1])
		PeakUnitPrice = atoi(tempBuffer[1]);
	PercentageTax = atoi(tempBuffer[2]);
	FixedCharges = atoi(tempBuffer[3]);

	for (int i = 0; i < 4; i++)
	{
		if (tempBuffer[i])
		{
			delete[] tempBuffer[i];
			tempBuffer[i] = 0;
		}
	}
}

int TariffTax::GetRegularUnitPrice()
{
	return RegularUnitPrice;
}

int TariffTax::GetPeakUnitPrice()
{
	return PeakUnitPrice;
}

int TariffTax::GetPercentageTax()
{
	return PercentageTax;
}

int TariffTax::GetFixedCharges()
{
	return FixedCharges;
}

void TariffTax::UpdateTaxRecords(ofstream& NewFile)
{
	
	NewFile << "1Phase," << RegularUnitPrice << "," << PeakUnitPrice << "," << PercentageTax;
		NewFile<< "," << FixedCharges << endl;
}

//Setters
void TariffTax::SetPeakUnitPrice(int Price)
{
	PeakUnitPrice = Price;
}

void TariffTax::SetRegularUnitPrice(int Price)
{
	RegularUnitPrice = Price;
}

void TariffTax::SetPercentageTax(int Tax)
{
	PercentageTax = Tax;
}

void TariffTax::SetFixedCharges(int Charges)
{
	FixedCharges = Charges;
}


TariffTax::~TariffTax()
{
	if (MeterType)
		delete[] MeterType;
}


// LESCO functions:

LESCO::LESCO()
{
    AllEmployees = 0;
	TotalEmployees = 0;
	AllBills = 0;
	TotalBills = 0;
	AllCustomer = 0;
	TotalCustomer = 0;
	CurrentEmployee = 0;
	CurrentCustomer = 0;
}

void LESCO::LoadEmployeeData()
{
	ifstream EmployeeFile;                         // Opening employee File
	EmployeeFile.open("EmployeeInfo.txt");

	if (!EmployeeFile)
	{
		cout << "File could NOT open!\n";
		return;
	}

	EmployeeFile >> TotalEmployees;

	AllEmployees = new Employee * [TotalEmployees];
	for (int i = 0; i < TotalEmployees; i++)
	{
		AllEmployees[i] = new Employee;
		AllEmployees[i]->ReadFromFile(EmployeeFile);
	}

	EmployeeFile.close();

}

void LESCO::DisplayAllBills()
{
	for (int i = 0; i < TotalBills; i++)
	{
		cout << "\n\t\tBill# "<<i+1<<endl;
		AllBills[i]->PrintBill();
	}
}

void LESCO::DisplayAllCustomersData()
{
	for (int i = 0; i < TotalCustomer; i++)
	{
		cout << "\n\t\tCustomer# "<<i+1<<endl;
		AllCustomer[i]->PrintCustomerInfo();
	}
}

void LESCO::AssociateBillandCustomer()
{
	//Heap Space Allocation for Bill* in Customer
	for (int i = 0; i < TotalCustomer; i++)
		AllCustomer[i]->AllocateBillSpace();

	//Placing Bill Pointer for every Customer
	for (int i = 0; i < TotalCustomer; i++)
	{
		Bill* BillID = 0;

		//Search All Bills 
		for (int j = 0; j < TotalBills; j++)
			BillID = SearchBillByID(AllBills[i]->GetID());

		if (BillID)
		{
			//cout << "\nAssociating for Customer"<<i+1<<endl;
			AllCustomer[i]->AssociateBill(BillID);
		}	
	}
}

void LESCO::LoadCustomerData()
{
	ifstream CustomerFile;
	CustomerFile.open("CustomerInfo.txt");
	if (!CustomerFile)
	{
		cout << "File could NOT open!\n";
		return;
	}

	CustomerFile >> TotalCustomer;
	//cout << "\nTotal Customers = "<<TotalCustomer<<endl;
	AllCustomer = new Customer * [TotalCustomer + 10];

	char TempBuffer[500];
	CustomerFile.getline(TempBuffer,500); //Comment 1
	//cout << "\nComment 1 = "<<TempBuffer;
	CustomerFile.getline(TempBuffer,500); //Comment 2
	//cout << "\nComment 2 = "<<TempBuffer;

	for (int i = 0; i < TotalCustomer; i++)
	{
		AllCustomer[i] = new Customer; //Change made here
		AllCustomer[i]->ReadCustomerFile(CustomerFile);
	}

	CustomerFile.close();
}

void LESCO::LoadBillData()
{
	ifstream BillFile;
	BillFile.open("BillInfo.txt");

	if (!BillFile)
	{
		cout << "File could NOT open!\n";
		return;
	}

	BillFile >> TotalBills;
	//cout << "\nTotal Bills Read = "<<TotalBills<<endl;
	AllBills = new Bill * [TotalBills + 10];

	char Comment[200];
	BillFile.getline(Comment,200);
	//cout << "\nTesting Comment 1 = "<<Comment<<endl;
	BillFile.getline(Comment,200);
	//cout << "\nTesting Comment 2 = "<<Comment<<endl;

	for (int i = 0; i < TotalBills; i++)
	{
		AllBills[i] = new Bill;
		AllBills[i]->ReadFromFile(BillFile);
	}

	BillFile.close();
}

void LESCO::LoadTariffTaxData()
{
	ifstream TariffFile;
	TariffFile.open("TariffTaxInfo.txt");

	if (!TariffFile)
	{
		cout << "File could NOT open!\n";
		return;
	}


	for (int i = 0; i < 4; i++)
	{
		TariffTaxInformation[i] = new TariffTax;
		TariffTaxInformation[i]->ReadFromFile(TariffFile);
	}

	TariffFile.close();
}

Employee* LESCO::SearchEmployeeByUsername(char* Username)
{
	for (int i = 0; i < TotalEmployees; i++)
	{
		int size = strlen(Username);
		char* iD = AllEmployees[i]->GetUsername();

		int j = 0;
		int matchSize = 0;
		while (iD[j] != '\0')
		{
			if (iD[j] == Username[j])
			{
				matchSize++;
			}
			j++;
		}

		if (matchSize == size)
		{
			return AllEmployees[i];
		}

	}

	return 0;
}

Customer* LESCO::SearchCustomerByID(char* ID)
{
	for (int i = 0; i < TotalCustomer; i++)
	{
		int size = strlen(ID);
		char* iD = AllCustomer[i]->GetID();

		int j = 0;
		int matchSize = 0;
		while (iD[j] != '\0')
		{
			if (iD[j] == ID[j])
			{
				matchSize++;
			}
			j++;
		}

		if (matchSize == size)
		{
			return AllCustomer[i];
		}
	}

	return 0;
	
}

Bill* LESCO::SearchBillByID(char* ID)
{
	int matchSize;
	for (int i = 0; i < TotalBills; i++)
	{
		int size =strlen(ID);
		char* iD = AllBills[i]->GetID();

		int j = 0;
		matchSize = 0;
		while (iD[j] != '\0')
		{
			if (iD[j] == ID[j])
			{
				matchSize++;
			}
			j++;
		}

		if (matchSize == size)
		{
			return AllBills[i];
		}
	}
	return 0;
}

void LESCO::UpdateEmployeeFile()
{
	ofstream EmployeeFile("EmployeeInfo.txt"); // use same file -> overwrite

	EmployeeFile<<TotalEmployees;

	for (int i = 0; i < TotalEmployees; i++)
	{
		AllEmployees[i]->SaveData(EmployeeFile);
	}

	EmployeeFile.close();
}

void LESCO::UpdateCustomerFile()
{
	ofstream CustomerFile("CustomerInfo.txt"); // use same file -> overwrite

	CustomerFile<<TotalCustomer<<" //Total Customers";
	CustomerFile<<endl<<"// ID,DOB,Name,Address,PhoneNum,CustomerType,MeterType,ConnectionDate,RegularUnits,PeakHourUnits, PaymentStatus";

	for (int i = 0; i < TotalCustomer; i++)
	{
		AllCustomer[i]->SaveData(CustomerFile);
	}

	CustomerFile.close();
}

void LESCO::ChangeEmployeePassword()
{
	char New_Password[50], temp[200];

	cout<<"Re-enter your username: ";
	cin>>temp;

	if (!Helper::IsEqual(temp,CurrentEmployee->GetUsername()))
	{
		cout<<"Incorrect username! Cannot change password!"<<endl;
		return;
	}

	cout<<"Re-enter your current password: ";
	cin>>temp;

	if (!Helper::IsEqual(temp,CurrentEmployee->GetPassword()))
	{
		cout<<"Incorrect password! Cannot change password!"<<endl;
		return;
	}

	cout<<"Enter new password to change to: ";
	cin>>New_Password;

	CurrentEmployee->SetPassword(New_Password);

	UpdateEmployeeFile();
}

void LESCO::ViewBillToCustomer()
{
	char tempID[5], tempDoB[11], _MeterType[2], _CustomerType[2], test[5];
	int RegUnits, PeakUnits = 0;

	cout<<"Enter your 4-digit ID: ";
	cin>>tempID;
	if (!Helper::IsEqual(tempID,CurrentCustomer->GetID()))
	{
		cout<<"Incorrect ID! Cannot view bill!"<<endl;
		return;
	}

	cout<<"Enter your password (DoB): ";
	cin>>tempDoB;
	if (!Helper::IsEqual(tempDoB,CurrentCustomer->GetDoB()))
	{
		cout<<"Incorrect Password! Cannot view bill!"<<endl;
		return;
	}

	do
	{
		cout<<"Enter your meter type (S for Single Phase and T for Three Phase): ";
		cin>>_MeterType;
		if (!Helper::IsEqual(_MeterType,"S") && !Helper::IsEqual(_MeterType,"T"))
			cout<<"Incorrect value! Re-enter!"<<endl;
	}
	while (!Helper::IsEqual(_MeterType,"S") && !Helper::IsEqual(_MeterType,"T"));

	do
	{
		cout<<"Enter your customer type (D for Domestic and C for Commercial): ";
		cin>>_CustomerType;
		if (!Helper::IsEqual(_CustomerType,"C") && !Helper::IsEqual(_CustomerType,"D"))
			cout<<"Incorrect value! Re-enter!"<<endl;
	}
	while (!Helper::IsEqual(_CustomerType,"C") && !Helper::IsEqual(_CustomerType,"D"));

	cout<<"Enter meter reading: "<<endl;
	cout<<"\t Enter regular units: ";
	cin>>RegUnits;
	if (RegUnits < 0)
		RegUnits = -RegUnits;

	if (Helper::IsEqual(_MeterType,"T"))
	{
		cout<<"\t Enter peak units: ";
		cin>>PeakUnits;
		if (PeakUnits < 0)
			PeakUnits = -PeakUnits;
	}

	CurrentCustomer->ViewCalculatedBill(_MeterType,_CustomerType,RegUnits,PeakUnits);
}

void LESCO::SetCurrentCustomer(char* _ID)
{
	Customer* _customer = SearchCustomerByID(_ID);

	if (!_customer)
	{
		cout<<"Incorrect ID!"<<endl;
		return;
	}

	CurrentCustomer = _customer;
}

void LESCO::SetCurrentEmployee(char* _Username)
{
	Employee* _employee = SearchEmployeeByUsername(_Username);

	if (!_employee)
	{
		cout<<"Incorrect username!"<<endl;
		return;
	}

	CurrentEmployee = _employee;
}

void LESCO::DisplayReport()
{
	char ID[5];

	cout<<"Enter 4-digit customer ID to view their report: ";
	cin>>ID;

	Customer* _Customer = SearchCustomerByID(ID);

	if (!_Customer)
	{
		cout<<"Customer not found!"<<endl;
		return;
	}

	_Customer->PrintCustomerInfo();
	
	int matchSize;
	for (int i = 0; i < TotalBills; i++)
	{
		int size =strlen(ID);
		char* iD = AllBills[i]->GetID();

		int j = 0;
		matchSize = 0;
		while (iD[j] != '\0')
		{
			if (iD[j] == ID[j])
			{
				matchSize++;
			}
			j++;
		}

		if (matchSize == size)
		{
			AllBills[i]->PrintBill();
		}
	}
}

void LESCO::AddBill()
{
	char ID[5], Month[4], DueDate[11], Paid[10], PaymentDate[11], EntryDate[11];
	int RegUnits,PeakUnits;
	
	cout<<"Enter ID of Customer to add new bill: ";
	cin>>ID;

	Customer* _Customer = SearchCustomerByID(ID);

	if (!_Customer)
	{
		cout<<"Customer not found!"<<endl;
		return;
	}
	
	cout<<"Enter first three letters of Billing Month in uppercase: ";
	cin>>Month;
	cout<<"Enter Regular Meter Reading: ";
	cin>>RegUnits;
	cout<<"Enter Peak Meter Reading (0 if N/A): ";
	cin>>PeakUnits;
	cout<<"Enter date of entry: ";
	cin>>EntryDate;
	cout<<"Enter Due Date: ";
	cin>>DueDate;
	cout<<"Enter Paid Status (PAID/UNPAID): ";
	cin>>Paid;

	if (Helper::IsEqual(Paid,"PAID"))
	{
		cout<<"Enter Payment Date: ";
		cin>>PaymentDate;
	}
	else
		strcpy(PaymentDate,"");

	char CustomerType = _Customer->GetCustomerType();
	char MeterType = _Customer->GetMeterType();
	int TariffTaxIndex;

	if (MeterType == 'S')
	{
		if (CustomerType == 'D')
		{
			TariffTaxIndex = 0;
		}
		else
		{
			TariffTaxIndex = 1;
		}
	}
	else
	{
		if (CustomerType == 'D')
		{
			TariffTaxIndex = 2;
		}
		else
		{
			TariffTaxIndex = 3;
		}
	}

	AllBills[TotalBills] = new Bill;
	AllBills[TotalBills]->SetBill(ID,Month,RegUnits,PeakUnits,EntryDate,DueDate,Paid,PaymentDate,TariffTaxIndex);

	TotalBills++;

	cout<<endl<<"Bill added to file."<<endl;

	UpdateBillFile();
}

void LESCO::AddCustomer()
{

    char DOB[11], NAME[40], ADDRESS[200], PHONEnumber[12];
	char CType, MType;
	char ConnectionDATE[11], RegularUNITS [20], PeakUNITS[20];
	cout<<"Generating a Unique ID for Customer.. \n";
	char* ID = AllCustomer[TotalCustomer-1]->GetID();

	// generating new ID:
	int val = atoi(ID);
	val++;
	char tempID[5], newID[5];
		itoa(val,tempID,10);
	int noOfZeros = 4 - strlen(tempID), i;
	for (i=0; i < noOfZeros; i++)
	newID[i] = '0';
	for (int j=i, k=0; j < 4; j++, k++)
	newID[j] = tempID[k];
	newID[4] = '\0';


	cout<<"Unique ID generated is:\t"<<newID<<endl<<"Enter Date of Birth of NEW Customer:\t";
	cin>>DOB;

	cout<<"\n\nEnter Name of the NEW Customer:\t";
	cin.ignore();
	cin.getline(NAME,40);

	cout<<"\n\nEnter Address of the NEW Customer:\t";
	cin.ignore();
	cin.getline(ADDRESS,200);

	cout<<"\n\nEnter Phone number of the NEW Customer:\t";
	cin>>PHONEnumber;

	cout<<"\n\nEnter Customer Type of the NEW Customer:\t";
	cin>>CType;

	cout<<"\n\nEnter Meter Type of the NEW Customer:\t";
	cin>>MType;

	cout<<"\n\nEnter Connection Date of the NEW Customer:\t";
	cin>>ConnectionDATE;

	AllCustomer[TotalCustomer] = new Customer;
	AllCustomer[TotalCustomer]->SetCustomer(newID,DOB,PHONEnumber,NAME,ADDRESS,ConnectionDATE,CType,MType);
	TotalCustomer++;

	UpdateCustomerFile();

	cout<<"New customer added in file."<<endl;

}

void LESCO::UpdateTariffTaxFile()
{
	int choice = 0;
	cout << "\nPress 1: To Update Information of Domestic Customers.";
	cout << "\nPress 2: To Update Information of Commercial Customers.";
	cout << "\nYour choice: ";
	cin >> choice;

	while (choice!=1 && choice!=2)
	{
		cout << "\nOnly Enter 1 or 2. ";
		cout << "\nYour choice: ";
		cin >> choice;
	}
	
	//Choices for DOMESTIC Customer
	if (choice == 1) 
	{
		int Option = 0;
		cout << "\nYour Choice: Update Domestic Customer Info.";
		cout << "\nPress 1: To Update Single-Phase Information.";
		cout << "\nPress 2: To Update Three-Phase Information.";
		cout << "\nYour Choice: ";
		cin >> Option;

		while (Option!=1 && Option!=2)
		{
			cout << "\nOnly Enter either 1 or 2. ";
			cout << "\nYour choice: ";
			cin >> Option;
		}
		if (Option == 1) //Single Phase Domestic Customer
		{
			int Pressed = 0;
			cout << "\nPress 1: To Update Regular Unit Charges. ";
			cout << "\nPress 2: To Update Percentage of Tax. ";
			cout << "\nPress 3: To Update Fixed Charges.";

			while (Pressed!=1 && Pressed!=2 && Pressed!=3)
			{
				cout << "\nOnly Enter either 1 or 2. ";
				cout << "\nYour choice: ";
				cin >> Pressed;
			}
			if (Pressed == 1) //Regular Charges
			{
				cout << "\nYou have chose to Update Regular Charges.";
				int NewCharges = 0;
				cout << "\nEnter the New Regular Unit Price: ";
				cin >> NewCharges;
				while (NewCharges < 0)
				{
					cout << "\nNew Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[0]->SetRegularUnitPrice(NewCharges);
			}
			else if (Pressed == 2) //Tax %
			{
				cout << "\nYou have chose to Update Tax Percentage.";
				int NewCharges = 0;
				cout << "\nEnter the New Tax Percentage: ";
				cin >> NewCharges;
				while (NewCharges < 0)
				{
					cout << "\nTax Percentage cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[0]->SetPercentageTax(NewCharges);
			}
			else //Fixed Charges
			{
				cout << "\nYou have chose to Update Fixed Charges.";
				int NewCharges = 0;
				cout << "\nEnter the New Fixes Charges: ";
				cin >> NewCharges;
				while (NewCharges < 0)
				{
					cout << "\nFixed Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[0]->SetFixedCharges(NewCharges);
			}
		}
		else //Three Phase Domestic Customer
		{
			int Pressed = 0;
			cout << "\nPress 1: To Update Regular Unit Charges. ";
			cout << "\nPress 2: To Update Peak Unit Charges. ";
			cout << "\nPress 3: To Update Percentage of Tax. ";
			cout << "\nPress 4: To Update Fixed Charges.";

			while (Pressed!=1 && Pressed!=2 && Pressed!=3 && Pressed!=4)
			{
				cout << "\nOnly Enter either 1 or 2 or 3 or 4 only. ";
				cout << "\nYour choice: ";
				cin >> Pressed;
			}

			if (Pressed == 1) //Regular Unit
			{
				cout << "\nYou have chosen to Update the Regular Charges.";
				int Charges = 0;
				cout << "\nEnter New Regular Unit Charges: ";
				cin >> Charges;

				while (Charges < 0)
				{
					cout << "\nCHarges cannot be Negative. Enter again: ";
					cin >> Charges;
				}

				TariffTaxInformation[2]->SetRegularUnitPrice(Charges);
			}
			else if (Pressed == 2) //Peak Unit
			{
				cout << "\nYou have chosen to Update the Peak Charges.";
				int Charges = 0;
				cout << "\nEnter New Peak Unit Charges: ";
				cin >> Charges;

				while (Charges < 0)
				{
					cout << "\nCharges cannot be Negative. Enter again: ";
					cin >> Charges;
				}

				TariffTaxInformation[2]->SetPeakUnitPrice(Charges);

			}
			else if (Pressed == 3) //Percentage Tax
			{
				cout << "\nYou have chosen to Update the Percentage Tax.";
				int Charges = 0;
				cout << "\nEnter New Percentage Tax: ";
				cin >> Charges;

				while (Charges < 0)
				{
					cout << "\nTax cannot be Negative. Enter again: ";
					cin >> Charges;
				}

				TariffTaxInformation[2]->SetPercentageTax(Charges);
			}
			else //Fixed Charges
			{
				cout << "\nYou have chosen to Update the Fixed Charges.";
				int Charges = 0;
				cout << "\nEnter New Fixed Charges: ";
				cin >> Charges;

				while (Charges < 0)
				{
					cout << "\nCharges cannot be Negative. Enter again: ";
					cin >> Charges;
				}

				TariffTaxInformation[2]->SetFixedCharges(Charges);
			}
		}

	}
	//Choices for COMMERCIAL Customer
	else
	{
		int Pressed = 0;
		cout << "\nAs a Commercial Customer: ";
		cout << "\nPress 1: To Update the Single-Phase Meter.";
		cout << "\nPress 2: To Update the Three-Phase Meter.";
		cout << "\nYour Choice: ";
		cin >> Pressed;
		
		while (Pressed!=1 && Pressed!=2)
		{
			cout << "\nEnter only 1 or 2: ";
			cin >> Pressed;
		}
		if (Pressed == 1) //SINGLE PHASE COMMERCIAL
		{
			int P = 0;
			cout << "\nPress 1: To Update the Regular Unit Charges.";
			cout << "\nPress 2: To Update the Peak Hour Unit Charges.";
			cout << "\nPress 3: To Update the Percentage Tax Charges.";
			cout << "\nPress 4: To Update the Fixed Charges.";

			cout << "\nYour Choice: ";
			cin >> P;

			while (P!=1 && P!=2 && P!=3 && P!=4)
			{
				cout << "\nOnly enter 1 or 2 or 3 or 4. Enter Again: ";
				cin >> P;
			}
			if (P == 1) //Regular unit
			{
				cout << "\nYou have chosen to Update Regular Units.";
				int NewCharges = 0;
				cout << "\nEnter New Regular Units: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nNew Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[1]->SetRegularUnitPrice(NewCharges);
			}
			else if (P == 2) //Peak Hour Unit
			{
				cout << "\nYou have chosen to Update Peak Units.";
				int NewCharges = 0;
				cout << "\nEnter New Peak Hour Units: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nPeak Units cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[1]->SetPeakUnitPrice(NewCharges);
			}
			else if (P==3) //Percentage Tax 
			{
				cout << "\nYou have chosen to Update Percentage Tax.";
				int NewCharges = 0;
				cout << "\nEnter New Percentage Tax: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nTax cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[1]->SetPercentageTax(NewCharges);
			}
			else //Fixed Charges
			{
				cout << "\nYou have chosen to Update Fixed Charges.";
				int NewCharges = 0;
				cout << "\nEnter New Fixed Charges: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nFixed Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[1]->SetFixedCharges(NewCharges);
			}
		}
		else //THREE PHASE COMMERCIAL
		{
			cout << "\nYou have chosen Three-Phase.";
			int P = 0;
			cout << "\nPress 1: To Update the Regular Unit Charges.";
			cout << "\nPress 2: To Update the Peak Hour Unit Charges.";
			cout << "\nPress 3: To Update the Percentage Tax Charges.";
			cout << "\nPress 4: To Update the Fixed Charges.";

			cout << "\nYour Choice: ";
			cin >> P;

			while (P!=1 && P!=2 && P!=3 && P!=4)
			{
				cout << "\nOnly enter 1 or 2 or 3 or 4. Enter Again: ";
				cin >> P;
			}
			if (P == 1) //Regular unit
			{
				cout << "\nYou have chosen to Update Regular Units.";
				int NewCharges = 0;
				cout << "\nEnter New Regular Units: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nNew Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[3]->SetRegularUnitPrice(NewCharges);
			}
			else if (P == 2) //Peak Hour Unit
			{
				cout << "\nYou have chosen to Update Peak Units.";
				int NewCharges = 0;
				cout << "\nEnter New Peak Hour Units: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nPeak Units cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[3]->SetPeakUnitPrice(NewCharges);
			}
			else if (P==3) //Percentage Tax 
			{
				cout << "\nYou have chosen to Update Percentage Tax.";
				int NewCharges = 0;
				cout << "\nEnter New Percentage Tax: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nTax cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[3]->SetPercentageTax(NewCharges);
			}
			else //Fixed Charges
			{
				cout << "\nYou have chosen to Update Fixed Charges.";
				int NewCharges = 0;
				cout << "\nEnter New Fixed Charges: ";
				cin >> NewCharges;

				while (NewCharges < 0)
				{
					cout << "\nFixed Charges cannot be Negative. Enter again: ";
					cin >> NewCharges;
				}
				TariffTaxInformation[3]->SetFixedCharges(NewCharges);
			}
		}
	}

}

void LESCO::GenerateNewTariffFile()
{
	ofstream NewFile("TariffTaxInfo.txt"); // same file is overwritten
	for (int i = 0; i < 4; i++)
	{
		TariffTaxInformation[i]->UpdateTaxRecords(NewFile);
	}

	cout << "\nTariffTaxInfo.txt has been updated. \n";
}

void LESCO::UpdatePaymentStatus()
{
	cout << "\nEnter the 4-Digit Customer ID for which you want to Update the Current Payment Status.";
	cout << "\nCustomer ID: ";
	char CustomerID[5];
	cin >> CustomerID;

	//Match that this Customer ID actually exists or not
	bool Exists = false;
	Customer* _Customer = 0;
	for (int i = 0; i < TotalCustomer; i++)
	{
		_Customer = SearchCustomerByID(CustomerID);
		if (_Customer) //!=0 means a pointer to a valid ID has been returned
		{
			Exists = true;
			break;
		}
	}
	while (Exists == false)
	{
		cout << "\nCustomer ID "<<CustomerID<<" is not part of LESCO's Customer Database. ";
		cout << "\nEnter a Valid Customer ID. ";

		cin >> CustomerID;

		for (int i = 0; i < TotalCustomer; i++)
			{
				_Customer = SearchCustomerByID(CustomerID);
				if (_Customer) //!=0 means a pointer to a valid ID has been returned
				{
					Exists = true;
					break;
				}
			}
	}
	
	//In case ID Exists: 
	char* TempID = Helper::GetStringFromBuffer(CustomerID);
	_Customer ->UpdatePaymentStatus();
}

void LESCO::UpdateBillFile()
{
	ofstream WriteFile("BillInfo.txt"); // use same file -> overwrite

	WriteFile<<TotalBills<<" //Total Bills";
	WriteFile<<endl<<"//ID,Month,RegularReading,Peak Reading,EntryDate,Cost, SalesTaxAmount, FixedCharges,TotalBilling,DueDate, PaidStatus,BillPaymentDate";

	for (int i = 0; i < TotalBills; i++)
	{
		AllBills[i]->SaveData(WriteFile);
	}

	WriteFile.close();
}

LESCO::~LESCO()
{
	if (AllEmployees != 0)
	{
		for (int i = 0; i < TotalEmployees; i++)
		{
			delete AllEmployees[i];
		}
		delete[] AllEmployees;
	}

	if (AllCustomer != 0)
	{
		for (int i = 0; i < TotalCustomer; i++)
		{
			delete AllCustomer[i];
		}
		delete[] AllCustomer;
	}

	if (AllBills != 0)
	{
		for (int i = 0; i < TotalBills; i++)
		{
			delete AllBills[i];
		}
		delete[] AllBills;
	}
}

bool LESCO::ProcessMenu()
{
	cout<<"\t\t\tChoose an Option:\n      1)Log in as a Employee (Press 1)      2)Login as a Customer (Press 2)      3) Exit (Press 3)\n\n                   Your Option:\t";
	int option;
	cin>>option;
	cout<<endl<<endl<<endl;

							//Log in as Employee							
	if(option == 1) 
	{
		cout<<"\n\tEnter Username:\t";
		char username[50];
		cin>>username;
		cout<<"\n\tEnter Password:\t";
		char password[30];
		cin>>password;

		char* Password = Helper::GetStringFromBuffer(password);
		char* UserName = Helper::GetStringFromBuffer(username);

		Employee* Check = SearchEmployeeByUsername(UserName);
		if(Check == 0 || !Helper::IsEqual(Check->GetPassword(),Password))
		{
			cout<<"\nWrong Username or Password\n\n";
			return false;
		}

		SetCurrentEmployee(UserName);

		delete[] Password;
		delete[] UserName;

		int Option1;
		do
		{
			cout<<"-------------------------------------------------------------------------------------------------------------------------------------------------------------------";
			
			cout <<"\n\n\t\t\tWelcome to Home Page\n\n\t\t\t";
			cout << "Choose an option:\n\n\t";
			cout << "1) View Bill of Customer (Press 1)\t";
			cout << "2) Add New Customer (Press 2)\t";
			cout << "3)Add New Bill (Press 3)\t";
			cout << "4)Update Tariff Tax (Press 4)";
			cout <<"\n\n\t\t\t5) Update Payement Status (Press 5)\t";
			cout << "6) Change Password (Press 6)\t ";
			cout << "7)Display Customer Report (Press 7)\t";
			cout << "8)Logout (Press 8)\n\n\tYour Option:\t";
		
			cin >> Option1;

			if(Option1 == 1) //View Bill
			{
				cout<<"\nEnter Customer ID:\t";
				char CustomerID[5];
				cin >> CustomerID;
		
				 ViewBillToEmployee(CustomerID); //uncomment
			}

			else if(Option1 == 2) //Add New Customer
			{
				AddCustomer();
			}

			else if(Option1 == 3) //Add New Bill
			{
				AddBill();
			}

			else if(Option1 == 4) //Update Tariff Tax
			{
				UpdateTariffTaxFile();
				GenerateNewTariffFile();
			}

			else if(Option1 == 5) //Update Payment Status
			{
				UpdatePaymentStatus();
				UpdateBillFile();
				UpdateCustomerFile();
			}

			else if(Option1 == 6) //Reset Employee Password
			{
				ChangeEmployeePassword();
			}

			else if(Option1 == 7) //Reset Employee Password
			{
				DisplayReport();
			}

			else if(Option1 == 8)
			{
				cout<<"\n\n---------------------------------------------------------------------------------------------";
				cout<<"\n\t\tYour account is Logged Out\n\n";
				return false;
			}

		}
		while (Option1 != 8);
	}
	else
	if(option == 2)
	{
		cout<<"\n\tEnter Username:\t";
		char username[50];
		cin>>username;
		cout<<"\n\tEnter Password:\t";
		char password[30];
		cin>>password;

		char* Password = Helper::GetStringFromBuffer(password);
		char* UserName = Helper::GetStringFromBuffer(username);

		Customer* Check = SearchCustomerByID(UserName);
		if(Check == 0 || !Helper::IsEqual(Check->GetDoB(),Password))
		{
			cout<<"\nWrong Username or Password\n\n";
			return false;
		}

		SetCurrentCustomer(UserName);

		delete[] UserName;
		delete[] Password;

		int Option2;
		do
		{
			cout <<"\n\n\t\t\t Welcome to Home Page\n\n\t\t\tChoose an option:\n\t1) Check Bill (Press 1)\t\t2) Logout (Press 2)\n\n\tYour Option:\t";
		
			cin>>Option2;

			if(Option2 == 1)
			{
				ViewBillToCustomer();
				cout<<endl;
			}

			else if(Option2 == 2)
			{
				cout<<"Your account is Logged out\n";
				return false;
			}

			else
			{
				cout<<"Invalid Option entered!"<<endl;
			}
		}
		while(Option2 != 2);



	}
	else
	if(option == 3)
	{
		return true;
	}
	else
	{
		cout<<"Please choose the correct option!\n";
    	return false;
	}
}

bool LESCO::SearchCustomerPassword(char* Pass)	
{	
	for (int i = 0; i < TotalCustomer; i++)	
	{	
		int size = strlen(Pass);	
		char* Pass_= AllCustomer[i]->GetDoB();	
		int j = 0;	
		int matchSize = 0;	
		while (Pass_[j] != '\0')	
		{	
			if (Pass_[j] == Pass[j])	
			{	
				matchSize++;	
			}	
			j++;	
		}	
		if (matchSize == size)	
		{	
			return true;	
		}	
	}	
	return false;	
		
}	

bool LESCO::SearchEmployeePassword(char* Pass)	
{	
	for (int i = 0; i < TotalEmployees; i++)	
	{	
		int size = strlen(Pass);	
		char* Pass_= AllEmployees[i]->GetPassword();	
		int j = 0;	
		int matchSize = 0;	
		while (Pass_[j] != '\0')	
		{	
			if (Pass_[j] == Pass[j])	
			{	
				matchSize++;	
			}	
			j++;	
		}	
		if (matchSize == size)	
		{	
			return true;	
		}	
	}	
	return false;	
		
}	

void LESCO::ViewBillToEmployee(char* CustomerID )
{
	Customer* _Customer = SearchCustomerByID(CustomerID);

	Bill* CustomerBill =  _Customer->GetCurrentBill();
	CustomerBill->PrintBill();
}

int main()
{
	LESCO L1;

	L1.LoadCustomerData();
	L1.LoadBillData();
	L1.LoadTariffTaxData();
	L1.LoadEmployeeData();
	L1.AssociateBillandCustomer();

	while(L1.ProcessMenu() == 0) 
	{

	}

	cout<<"\t\t\t------------------------------------------------EXIT-------------------------------------------\n\n";

	return 0;
}
