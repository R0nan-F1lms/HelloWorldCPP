#include "splashkit.h"
#include <iostream>
#include <string>

using namespace std;

double totalPetrolPrice = 0;
double totalLitres = 0, petrolPrice, litresFilled, transactionTotal, averagePricePerLitre;
int transactionCount = 0;
bool i = false;

int main() {
    
    bool continueTransaction = true;
    while (continueTransaction) {
        try {
        write_line("Enter the petrol price in cents per litre:");
        petrolPrice = convert_to_double(read_line());
        } catch (const std::invalid_argument& e) {
            throw("Please only enter a Bool" + string(e.what()));
        }
        try {
        write_line("Enter the number of litres filled:");
        litresFilled = convert_to_double(read_line());
        } catch (const std::invalid_argument& e) {
            throw("Please only enter a Bool" + string(e.what()));
        }
        transactionTotal = petrolPrice * litresFilled / 100;
        totalPetrolPrice += transactionTotal;
        totalLitres += litresFilled;
        transactionCount++;

        averagePricePerLitre = totalPetrolPrice / totalLitres;
        write_line("You spent $" + to_string(transactionTotal) + " on this transaction.");
        write_line("Your lifetime average price per litre is $" + to_string(averagePricePerLitre) + ".");
        write_line("Total amount spent on petrol: $" + to_string(totalPetrolPrice) + ".");

        do {
            write_line("Would you like to add another petrol transaction? (1/0)");
            string continueInput = read_line();

            if (continueInput == "1") {
                i = false;
                continue;
            } else if (continueInput == "0") {
                continueTransaction = false;
                write_line("Thank you for using the petrol price calculator.");
                i = false;
            } else {
                continueTransaction = false;
                write_line("Only enter 1 or 0");
                i = true;
            }
        } while (i);
    }

    return 0;
}
