// Shakya Abisheik Buwanekabahu, G21328022
// Milestone: 5 (62 mark)

#include<fstream>						// Allows the use to read/input in text files
#include<iomanip>						// Allows you to manipulate inputs and outputs
#include<cctype>						// To find number of spaces in text file. (Sometimes they are tab spaces thats different from regualr spaces)
#include<iostream>
#include<string>						// Enables string manipulations
#include<vector>						// Allows use of dynamic arrays (vectors)
using namespace std;

const int decimalPlaces = 2;					// Stores number of decimal places for outputting

// Holds employee information
struct info
{
	string name;
	string ID;
	string rate;
};

// Calculating salary before and after tax
void calculateEmployeePay(vector<info> &vec, vector<string> &vec2 ,int &storePosition); 		
							

int main()
{		
	int currentPosition = 0;				// Tracks current position in employeeDetailsVector	
	ifstream jan25File("Jan25.txt");			// Opens "Jan25.txt" for reading
	vector<string> jan25Vector;				// Vector to store work hours from the file
	string hoursDetails = " ";				// Temporary string to store each line from the file
	// Checks if file is available 
	if (!jan25File) {
		cout << "Error: Could not open Jan25.txt" << endl;
	}
	
	while(getline(jan25File, hoursDetails))			// Stores lines from "jan25.txt" to variable hoursDetails
	{
		int janOldPos = 0;				// Stores the previous position of the last space in the line loop
		int janNewPos = 0;				// Stores the current position of the space in the line loop
		int janSpaceCount = 0;				// Keeps track of the number of spaces
		string strHours = "";				// Stores extracted hours from line
		
		// Iterate through the line character by character
		for (; janNewPos  < hoursDetails.length(); janNewPos ++)	
		{
		// Checks if that character is a space
			if (isspace(hoursDetails[janNewPos]))	
			{
				janSpaceCount ++;
				switch (janSpaceCount)
				{
				case 1:
				janNewPos++;
				janOldPos  = janNewPos;
				break;
				
				case 2:
				strHours = hoursDetails.substr(janOldPos, janNewPos - janOldPos);
				janNewPos++;
				janOldPos  = janNewPos;
				break;
				}
			}		
		}
		jan25Vector.push_back(strHours);		// Store extracted work hours into vector
	}
	jan25File.close();		
	
	
	ifstream employeesFile("employees.txt");		// Reading contents of txt file "employees"
	vector<info> employeeDetailsVector;			// Vector to store employee details
	info employees;						// Instance of struct info
	string workerDetails = " ";				// Stores lines from employees file
	// Checks if file is available 
	if (!employeesFile) {
		cout << "Error: Could not open employees.txt" << endl;
	}
	
	// Stores lines from employee to workerDetails string
	while (getline(employeesFile, workerDetails))		
	{	
		int employeeOldPos  = 0;				
		int employeeSpaceCount = 0;			
		
		// Temporary storage for ID, name & rate 
		string strID;
		string strName;
		string strRate;
		
		for (int employeeNewPos = 0; employeeNewPos  < workerDetails.length(); employeeNewPos ++)	
		{
			if (isspace(workerDetails[employeeNewPos]))
			{
				employeeSpaceCount ++;
				switch (employeeSpaceCount)
				{
				case 1:
				// First space: extract ID
				strID = workerDetails.substr(employeeOldPos , employeeNewPos  - employeeOldPos );
				employeeNewPos++;
				employeeOldPos = employeeNewPos;
				break;
				
				case 2:
				// Second space: extract name
				strName = workerDetails.substr(employeeOldPos , employeeNewPos  - employeeOldPos );
				employeeNewPos++;
				employeeOldPos  = employeeNewPos;
				break;
				
				// Third space: extract pay rate
				case 3:
				strRate = workerDetails.substr(employeeOldPos);
				break;
				}
			}		
		}
		 // Store extracted employee details into struct and push into vector
		employees.name = strName;
		employees.ID = strID;
		employees.rate = strRate;
		employeeDetailsVector.push_back(employees);
	}
	employeesFile.close();	
	calculateEmployeePay(employeeDetailsVector, jan25Vector ,currentPosition);
}
		
			
void calculateEmployeePay(vector<info> &vec, vector<string> &vec2 , int &storePosition)
{
	// Constants for tax calculations
	const float tax_Rate = 0.20;				// 20% tax rate
	const float months_in_year = 12;			// Number of months in a year
	const float tax_Free_Allowance = 12570;			// Annual tax-free allowance (income not taxed)

	float incomeSalary = 0;					// Monthly salary before tax deduction
	float finalSalary = 0;					// Monthly salary after tax deduction
	float annualIncome = 0;					// Income per year before tax
	float taxableIncome = 0;				// Taxable portion of the annual income after deducting the tax-free allowance
	float annualIncomeTax = 0;				// Total tax for the year
	float monthlyTaxDeduction = 0;				// Monthly tax deduction 
	
	float floatRate = 0;					// Rate of pay converted to float
	float floatHours = 0;					// No of hours converted to float
	int SIZE = vec.size();					// Size of vector that is placed into the function
	
	ofstream janOutFile("jan25_output.txt");		// Open output file to write salary details
	// Checks if file can be created or not
	if (!janOutFile) 
	{
		cout << "Error: Could not find jan25_output.txt" << endl;
	}
	
	// Iterate through each employee and calculate salary details
	for(int iterate = storePosition; iterate < SIZE ; iterate ++)
	{
		cout << "ID: " << vec[iterate].ID 
         	<< ", Name: " << vec[iterate].name 
         	<< ", No of Hours: "<< vec2[iterate]
         	<< ", Rate: " << vec[iterate].rate << endl;
		
		// Convert string rate and hours to float values
		floatRate = stof(vec[iterate].rate);
		floatHours = stof(vec2[iterate]);
		
		// Calculate salaries
		incomeSalary = floatRate * floatHours;
		annualIncome = incomeSalary * months_in_year;
		taxableIncome = annualIncome - tax_Free_Allowance;
		
		// Ensure taxable income is never negative
		if (taxableIncome < 0) taxableIncome = 0;
		
		// Calculate tax
		annualIncomeTax = taxableIncome * tax_Rate;
		monthlyTaxDeduction = annualIncomeTax / months_in_year;
		finalSalary = incomeSalary - monthlyTaxDeduction;
		
		// Output Salary before and after deduction
		cout<<fixed<<setprecision(decimalPlaces);
		cout<<"Your monthly salary: $"<<incomeSalary<<endl;
		cout<<"Your monthly tax deduction: $"<<monthlyTaxDeduction<<endl;
		cout<<"Your monthly salary after tax: $"<<finalSalary<<endl;
		cout<<"\n"<<endl;
		
		// Write salary details to output file
		janOutFile<<"Employee ID: "<<vec[iterate].ID<<" | Before Tax: "<<incomeSalary	
			  <<" | After Tax: "<<finalSalary<<endl;
	}
	janOutFile.close();	
}


