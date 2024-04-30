#include <stdio.h>
#include <string>

using std::string;

typedef struct room_struct *room_ptr;

typedef struct path_struct
{
  string    description;
  room_ptr  destination;
} path_data;

typedef struct room_struct
{
  string title;
  string description;
} room_data;

// Forward declarations
room_data new_room(string title, string description);
path_data new_path(string description, room_ptr destination);
void print_path(int idx, const path_data &path);
void print_room(room_ptr room);
void move_player(room_ptr &current_room, const path_data &path);
int read_integer(const string &prompt);

int main()
{
  room_data r1 = new_room("Room 1", "This is room 1, it is empty nothing is in here, just like me");
  room_data r2 = new_room("Room 2", "This is room 2 it's just like room 1 but called room 2 it is also empty");
  room_data r3 = new_room("Room 3", "This is room 3 this one is like room 2 and 1 but is called room 3 this is not empty this as you in it");

  path_data p1 = new_path("A Door", &r2);
  path_data p2 = new_path("Yet another Door", &r3);

  room_ptr current_room = &r1;

  while (1) {
    print_room(current_room);
    print_path(0, p1);
    print_path(1, p2);

    int option = read_integer("Option: ");

    if (option == 0 && p1.destination != nullptr)
    {
      move_player(current_room, p1);
    }
    else if (option == 1 && p2.destination != nullptr)
    {
      move_player(current_room, p2);
    }
    else
    {
      printf("Invalid option or path unavailable. Please choose again.\n");
    }

    // Check if the user wants to quit
    printf("Do you want to continue (quit/any thing else is a continue)? ");
    char answer[10];
    scanf("%s", option);
    if (strcmp(answer, "quit") == 0) {
      break;
    }
  }

  return 0;
}

// create new room with title and description
room_data new_room(string title, string description)
{
  room_data result = { title, description };
  return result;
}

//create new path with description and destination room
path_data new_path(string description, room_ptr destination)
{
  path_data result = { description, destination };
  return result;
}

// Prints the path
void print_path(int idx, const path_data &path)
{
  printf("%d - %s\n", idx, path.description.c_str());
}

//Output the room's title and description to the terminal.
void print_room(room_ptr room)
{
  printf("%s\n-----\n%s\n", room->title.c_str(), room->description.c_str());
}

//Move the player through the selected path, setting the player's current room
// to the destination of the path.
void move_player(room_ptr &current_room, const path_data &path)
{
  current_room = path.destination;
}

// Reads an integer value from the user. If the user enters a non-integer value,
//the function will continue to prompt the user until they enter a valid value.
int read_integer(const string &prompt)
{
  int result = 0;

  printf("%s", prompt.c_str());
  while (scanf(" %d", &result) != 1) // Read value, and try to convert to int
  {
    // Convert failed, as input was not a number
    scanf("%*[^\n]"); // Read past the end of the current line
    printf("Please enter a whole number.\n");
    printf("%s", prompt.c_str());
  }

  return result;
}