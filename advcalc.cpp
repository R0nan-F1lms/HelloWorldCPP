/*
AUTHOR: RONAN WINFIELD
CREATED: Tue, 9th April 2024
LAST EDITED: Tue, 9th April 2024
*/

// Using statements "clang++ advcalc.cpp -l SplashKit -o advcalc
#include "splashkit.h"
#include <string>

// This stuff not sure whats its actually there for but it works DO NOT REMOVE
using std::stod;
using std::to_string;

int read_integer(string prompt);
bool valid(int a, int b);
void calc_change(int a, int b);

int main()
{
    // Defining variables.
    bool isRunning = true;
    int cost, amountPaid, ans;
    bool isValid, t;

    // While isRunning is true, will continue to run the program.
    do
    {
        cost = read_integer("Please enter the cost of the item: ");

        amountPaid = read_integer("Please enter the amount paid for the item: ");

        isValid = valid(cost, amountPaid);
        while (isValid)
        {
            calc_change(cost, amountPaid);
            do
            {
                ans = read_integer("Would you like to continue (1) or end this session (0)");

                if (ans == 0)
                {
                    isRunning = false;
                    write_line("Thank you for using this program");
                    return 0;
                }
                else if (ans == 1)
                {
                    break;
                }
                else
                {
                    write_line("Please only enter 1 or 0");
                    t = true;
                }
            } while (t);
            break;
        }
        if (cost == amountPaid)
        {
            write_line("No change returned - cost and amount paid were equal.");
        }
        else if (cost > amountPaid)
        {
            write_line("The cost must not be more than the amount paid");
        } else {
        write_line("");
        continue;
        }
        
    } while (isRunning);
    return 0;
}

int read_integer(string prompt)
{
    string result;
    int res;
    write_line(prompt);
    do
    {
        result = read_line();

        if (!isdigit(result[0]))
        {
            write_line("Please enter the correct syntax:\nSyntax is Integer");
        }
    } while (!isdigit(result[0]));
    res = stoi(result);
    return res;
}

bool valid(int a, int b)
{
    bool myBool;
    myBool = a < b;
    return myBool;
}

void calc_change(int a, int b)
{
    int changeNeeded = b - a;

    write_line("Change needed: $" + to_string(changeNeeded / 100) + " . " + to_string(changeNeeded % 100));

    int denominations[] = {10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10, 5};
    string labels[] = {"$100 notes", "$50 notes", "$20 notes", "$10 notes", "$5 notes", "$2 coins", "$1 coins", "50c coins", "20c coins", "10c coins", "5c coins"};

    for (int i = 0; i < sizeof(denominations) / sizeof(denominations[0]); i++)
    {
        int numCoins = changeNeeded / denominations[i];
        changeNeeded %= denominations[i];
        write_line("Give " + labels[i] + ": " + to_string(numCoins));
    }
}