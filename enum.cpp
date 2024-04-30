#include <SplashKit.h>

// Enum to represent gender
enum Gender {
    MALE,
    FEMALE
};

// Struct to represent a person
struct Person {
    std::string name;
    int age;
    Gender gender;
};

// Function to convert Gender enum to string
std::string genderToString(Gender gender) {
    return (gender == MALE) ? "Male" : "Female";
}

// Function to get gender from user input
Gender getGenderFromInput() {
    char userInput;
    write_line("Enter gender (M/F): ");
    userInput = read_char();
    return (userInput == 'M' || userInput == 'm') ? MALE : FEMALE;
}

int main() {
    // Create an instance of the Person struct
    Person person;

    // Set the values for the person
    person.name = "John";
    person.age = 30;
    person.gender = MALE; // Default value for demonstration

    // Get gender from user input
    person.gender = getGenderFromInput();

    // Print out the person's information
    write_line("Name: " + person.name);
    write_line("Age: " + std::to_string(person.age));
    write_line("Gender: " + genderToString(person.gender));

    return 0;
}