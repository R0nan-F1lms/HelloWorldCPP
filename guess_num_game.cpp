#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include "splashkit.h" // SplashKit library

// Constants
const int MAX_GUESSES = 7;
const int MAX_NUMBER = 100;

// Forward declaring on functions
bool PerformGuess(int guessNumber, int target);
void PrintLine(int length);
void PlayGame();
int read_integer(string prompt);
void write_now(string message, int value);

// Main function to coordinate the program flow
int main() {
    char again;
    do {
        PlayGame();
        PrintLine(20);
        write("Play again? [Y or n]: ");
        again = read_char();
    } while (again != 'n' && again != 'N');

    return 0;
}

// Function to perform a single guess
bool PerformGuess(int guessNumber, int target) {
    int guess;
    write_now("Max Number of Guesses 7\nYou have guessed this many times:", guessNumber);
    guess = read_integer("Guess ");

    if (target < guess) {
        write_now("The number is less than: ", guess);
    } else if (target > guess) {
        write_now("The number is larger than: ", guess);
    } else {
        write_now("well done! the number was: ", guess);
        return true;
    }

    return false;
}

// Procedure to print a line of '-' characters
void PrintLine(int length) {
    for (int i = 0; i < length; ++i) {
        write("-");
    }
    write_line();
}

// Procedure to play the game
void PlayGame() {
    // Generate a random number
    srand(time(0));
    int myNumber = rand() % MAX_NUMBER + 1;
    int guessNumber = 0;
    bool gotIt = false;
    // Output starting text
    write_now("I am thinking of a number between 1 and ", MAX_NUMBER);

    // Repeat until the user guesses the number or runs out of guesses
    do {
        guessNumber++;
        gotIt = PerformGuess(guessNumber, myNumber);
    } while (guessNumber < MAX_GUESSES && !gotIt);

    // If the user ran out of guesses, output the answer
    if (!gotIt) {
        
        write_now("You ran out of guesses... the number was ", myNumber);
    }
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

void write_now(string message, int value) {
    // Convert the integer value to a string
    string valueAsString = std::to_string(value);

    // Concatenate the message and the string representation of the value
    string outputMessage = message + valueAsString;

    // Write the output message to the console
    write_line(outputMessage);
}