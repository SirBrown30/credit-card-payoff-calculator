// Brown_C_Lab_1 -- Credit Card Payoff Schedule Program
// Christopher Brown
// CSIS 112-D03
// Sources:
// https://cplusplus.com/reference/iomanip/
// https://cplusplus.com/reference/limits/numeric_limits/
// Program Purpose: This program demonstrates recursion by creating and
// displaying credit card payoff schedules for two payment amounts so the
// user can compare how long payoff takes and how much interest is charged.

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

// Function prototypes
void clearCin();
void pauseUntilUserIsReady();
bool isValidIntInRange(int _value, int _min, int _max, const string& _errorMessage);
bool isValidInterestRate(int _value);
int getValidatedInt(const string& _prompt, int _min, int _max, const string& _errorMessage, bool _useSeriesCheck = false);
double roundTo5DecimalPlaces(double _valueToRound);
double convertAnnualToMonthlyRate(int _annualInterestRate);
void displayScheduleColumnHeaders();
int ccPayoffSchedule(int _currentPeriod, double _currentBalance, int _actualPayment, double _monthlyRate);
double calculateTotalInterestRec(double _currentBalance, int _actualPayment, double _monthlyRate);
double calculateRoundedInterest(double _currentBalance, double _monthlyRate);

int main()
{
    cout << "Christopher Brown -- Lab 1 - Recursion" << endl << endl;

    // Required input order
    int initialBalance = getValidatedInt(
        "Enter the initial credit card balance (500 - 2000): ",
        500,
        2000,
        "Error: Balance must be a whole number between 500 and 2000 inclusive."
    );

    int annualInterestRate = getValidatedInt(
        "Enter the annual interest rate (11, 12, 13, or 14): ",
        11,
        14,
        "Error: Interest rate must be 11, 12, 13, or 14.",
        true
    );

    int minimumPayment = getValidatedInt(
        "Enter the minimum payment (25 - 45): ",
        25,
        45,
        "Error: Minimum payment must be a whole number between 25 and 45 inclusive."
    );

    int maximumPayment = getValidatedInt(
        "Enter the maximum payment (65 - 99): ",
        65,
        99,
        "Error: Maximum payment must be a whole number between 65 and 99 inclusive."
    );

    pauseUntilUserIsReady();

    double monthlyRate = convertAnnualToMonthlyRate(annualInterestRate);

    int monthsMin = 0;
    int monthsMax = 0;
    double totalInterestMin = 0.0;
    double totalInterestMax = 0.0;

    // First pass = minimum payment, second pass = maximum payment
    for (int pass = 1; pass <= 2; pass++)
    {
        int actualPayment = (pass == 1) ? minimumPayment : maximumPayment;
        string paymentLabel = (pass == 1) ? "Minimum Payment Schedule" : "Maximum Payment Schedule";

        cout << endl;
        cout << "============================================================" << endl;
        cout << paymentLabel << endl;
        cout << "============================================================" << endl;
        cout << fixed << setprecision(2);
        cout << "Initial Balance: $" << initialBalance << endl;
        cout << "Annual Interest Rate: " << annualInterestRate << "%" << endl;
        cout << "Monthly Interest Rate: " << setprecision(5) << monthlyRate << endl;
        cout << setprecision(2);
        cout << "Payment Amount: $" << actualPayment << endl << endl;

        displayScheduleColumnHeaders();

        int months = ccPayoffSchedule(1, static_cast<double>(initialBalance), actualPayment, monthlyRate);
        double totalInterest = calculateTotalInterestRec(static_cast<double>(initialBalance), actualPayment, monthlyRate);

        cout << endl;
        cout << "Total months to payoff: " << months << endl;
        cout << "Total interest charged: $" << fixed << setprecision(2) << totalInterest << endl;

        if (pass == 1)
        {
            monthsMin = months;
            totalInterestMin = totalInterest;
        }
        else
        {
            monthsMax = months;
            totalInterestMax = totalInterest;
        }

        pauseUntilUserIsReady();
    }

    cout << endl;
    cout << "============================================================" << endl;
    cout << "Summary" << endl;
    cout << "============================================================" << endl;
    cout << fixed << setprecision(2);
    cout << "Initial credit card balance: $" << initialBalance << endl;
    cout << "Annual interest rate: " << annualInterestRate << "%" << endl;
    cout << "Minimum payment amount: $" << minimumPayment << endl;
    cout << "Maximum payment amount: $" << maximumPayment << endl << endl;

    cout << "Minimum payment results:" << endl;
    cout << "Months to payoff: " << monthsMin << endl;
    cout << "Total interest charged: $" << totalInterestMin << endl << endl;

    cout << "Maximum payment results:" << endl;
    cout << "Months to payoff: " << monthsMax << endl;
    cout << "Total interest charged: $" << totalInterestMax << endl << endl;

    cout << "Difference using maximum instead of minimum payment:" << endl;
    cout << "Months saved: " << (monthsMin - monthsMax) << endl;
    cout << "Interest saved: $" << (totalInterestMin - totalInterestMax) << endl;

    pauseUntilUserIsReady();

    return 0;
} // main()

bool isValidIntInRange(int _value, int _min, int _max, const string& _errorMessage)
{
    bool returnValue = true;

    if (cin.fail() || _value < _min || _value > _max)
    {
        cout << _errorMessage << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        returnValue = false;
    }

    return returnValue;
} // isValidIntInRange()

bool isValidInterestRate(int _value)
{
    bool returnValue = true;

    if (cin.fail() || (_value != 11 && _value != 12 && _value != 13 && _value != 14))
    {
        cout << "Error: Interest rate must be 11, 12, 13, or 14." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        returnValue = false;
    }

    return returnValue;
} // isValidInterestRate()

int getValidatedInt(const string& _prompt, int _min, int _max, const string& _errorMessage, bool _useSeriesCheck)
{
    int userValue = 0;
    bool validEntry = false;

    do
    {
        cout << _prompt;
        cin >> userValue;

        if (_useSeriesCheck)
        {
            validEntry = isValidInterestRate(userValue);
        }
        else
        {
            validEntry = isValidIntInRange(userValue, _min, _max, _errorMessage);
        }

    } while (!validEntry);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return userValue;
} // getValidatedInt()

double roundTo5DecimalPlaces(double _valueToRound)
{
    const int ROUNDING_VALUE = 100000;
    double temp = static_cast<int>(_valueToRound * ROUNDING_VALUE + 0.5);
    double temp2 = temp / ROUNDING_VALUE;
    return temp2;
} // roundTo5DecimalPlaces()

double convertAnnualToMonthlyRate(int _annualInterestRate)
{
    double annualRate = (_annualInterestRate * 1.0) / 100.0;
    double monthlyRate = annualRate / 12.0;
    return roundTo5DecimalPlaces(monthlyRate);
} // convertAnnualToMonthlyRate()

void displayScheduleColumnHeaders()
{
    cout << left
        << setw(10) << "Period"
        << setw(16) << "Start Bal"
        << setw(14) << "Interest"
        << setw(14) << "Payment"
        << setw(16) << "End Bal"
        << endl;

    cout << left
        << setw(10) << "------"
        << setw(16) << "---------"
        << setw(14) << "--------"
        << setw(14) << "-------"
        << setw(16) << "-------"
        << endl;
} // displayScheduleColumnHeaders()

double calculateRoundedInterest(double _currentBalance, double _monthlyRate)
{
    double interest = _currentBalance * _monthlyRate;
    return roundTo5DecimalPlaces(interest);
} // calculateRoundedInterest()

int ccPayoffSchedule(int _currentPeriod, double _currentBalance, int _actualPayment, double _monthlyRate)
{
    double interest = calculateRoundedInterest(_currentBalance, _monthlyRate);
    double paymentToUse = _actualPayment;
    double endBalance = _currentBalance + interest - paymentToUse;
    bool adjusted = false;

    if (endBalance <= 0.0)
    {
        paymentToUse = _currentBalance + interest;
        endBalance = 0.0;
        adjusted = true;
    }

    cout << fixed << setprecision(2);
    cout << left
        << setw(10) << _currentPeriod
        << "$" << setw(15) << _currentBalance
        << "$" << setw(13) << interest
        << "$" << setw(13) << paymentToUse
        << "$" << setw(15) << endBalance;

    if (adjusted)
    {
        cout << "<--Adjusted";
    }

    cout << endl;

    if (endBalance <= 0.0)
    {
        return _currentPeriod;
    }

    return ccPayoffSchedule(_currentPeriod + 1, endBalance, _actualPayment, _monthlyRate);
} // ccPayoffSchedule()

double calculateTotalInterestRec(double _currentBalance, int _actualPayment, double _monthlyRate)
{
    double interest = calculateRoundedInterest(_currentBalance, _monthlyRate);
    double paymentToUse = _actualPayment;
    double endBalance = _currentBalance + interest - paymentToUse;

    if (endBalance <= 0.0)
    {
        paymentToUse = _currentBalance + interest;
        endBalance = 0.0;
        return interest;
    }

    return interest + calculateTotalInterestRec(endBalance, _actualPayment, _monthlyRate);
} // calculateTotalInterestRec()

void clearCin()
{
    if (cin.rdbuf()->in_avail() > 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
} // clearCin()

void pauseUntilUserIsReady()
{
    cout << endl << "Press ENTER to continue...";
    clearCin();
    cin.get();
    clearCin();
    cout << endl;
} // pauseUntilUserIsReady()