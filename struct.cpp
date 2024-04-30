#include <SplashKit.h>

struct Person {
    std::string name;
    int age;
};

int main() {

    Person person;

    person.name = "John";
    person.age = 30;

    write_line("Name: " + person.name);
    write_line("Age: " + std::to_string(person.age));

    return 0;
}