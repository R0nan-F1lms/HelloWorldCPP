/**
 * Commit: Added functionality to the Spider Catcher game
 * 
 * Compuslery Changes:
 * 1. Added constants for game : screen width, height, spider radius, spider speed, fly radius, and game timer name.
 * 2. Defined structs for SpiderData, FlyData, and GameData to represent game entities and state.
 * 3. Implemented functions to create new instances of spider and fly with initial positions and state.
 * 4. Implemented functions to handle player input, update game state, and draw game elements on the screen.
 * 5. Added functionality to make the fly appear on the screen at random intervals and disappear after a certain time.
 * 6. Implemented collision detection to check if the spider catches the fly.
 * 7. Displayed the player's score on the screen.
 * 8. Implemented a mechanism to end the game when the player presses the ESCAPE key.
 * 9. Modified the main function to initialise the game and run the game loop.
 * 10. Added comments to describe the purpose and usage of functions and structs.
 *
 * For Funsies Changes:
 * 1. Added a constant for the game duration.
 * 2. Added a variable to store the game start time.
 * 3. Modified the handle_input function to pass SpiderData by reference.
 * 4. Modified the update_game function to pass GameData by reference and added a parameter for the current time.
 * 5. Modified the draw_game function to pass GameData by const reference and added a new function to draw the timer on the screen.
 * 6. Modified the main function to initialise the game start time.
 * 7. Modified the main loop to include a check for the time-up condition.
 * 8. Added a new function to check if the game duration has elapsed.
 * 9. Modified the draw_game function to call draw_timer and display the timer on the screen.
 */


#include "splashkit.h"
#include <cstdlib> // for rand function
#include <ctime>   // for time function
#include <cmath>   // for sqrt and pow functions

using std::to_string;

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPIDER_RADIUS = 25;
const int SPIDER_SPEED = 1;
const int FLY_RADIUS = 15;
const string GAME_TIMER = "Game Timer";
const int GAME_DURATION = 60; // 60 seconds

// Structs

/** 
* Represents the position of the spider in the game.
*
* @field x The distance from the left side of the window.
* @field y The distance from the top of the window.
*/
struct SpiderData {
    int x;
    int y;
};

/** 
* Represents the fly in the game.
*
* @field appeared Indicates whether the fly is currently visible on the screen.
* @field x The x-coordinate of the fly's position.
* @field y The y-coordinate of the fly's position.
* @field appear_at_time The time at which the fly will appear on the screen.
* @field escape_at_time The time at which the fly will disappear from the screen.
*/
struct FlyData {
    bool appeared;
    int x;
    int y;
    long appear_at_time;
    long escape_at_time;
};

/** 
* Represents the game state.
*
* @field spider The position of the spider.
* @field fly The state of the fly.
*/
struct GameData {
    SpiderData spider;
    FlyData fly;
};

// Variables
static GameData game;
static bool IsPlaying = true;
static int score = 0;
static long game_start_time;

// Forward declarations
void handle_input(SpiderData &spider);
void update_game(GameData &game, long current_time);
void draw_game(const GameData &game);
void make_fly_appear(FlyData &fly);
void remove_fly(FlyData &fly);
void catch_fly();
bool circles_intersect(int x1, int y1, int r1, int x2, int y2, int r2);
SpiderData new_spider();
FlyData new_fly();
bool is_time_up();
void draw_timer(long start_time);

int main()
{
    // Open the window
    open_window("Spider Catcher", SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create and start the game timer
    create_timer(GAME_TIMER);
    start_timer(GAME_TIMER);

    // Start/Initialise the game data
    game.spider = new_spider();
    game.fly = new_fly();
    game_start_time = timer_ticks(GAME_TIMER);

    // Game loop
    while (IsPlaying)
    {
        // Handle input
        handle_input(game.spider);

        // Update the game
        update_game(game, timer_ticks(GAME_TIMER));

        // Draw the game
        draw_game(game);

        // Check if time is up
        if (is_time_up())
        {
            write_line("Time's up! Your score is: " + to_string(score));
            IsPlaying = false;
        }

        // Process events
        process_events();
    }

    // Close the window
    close_window("Spider Catcher");

    return 0;
}

/** 
* Handles input from the player to move the spider.
*
* @param spider The spider data to be updated based on player input.
*/
void handle_input(SpiderData &spider)
{
    // Move the spider
    if (key_down(RIGHT_KEY))
    {
        spider.x = (spider.x + SPIDER_SPEED) % SCREEN_WIDTH;
    }
    else if (key_down(DOWN_KEY))
    {
        spider.y = (spider.y + SPIDER_SPEED) % SCREEN_HEIGHT;
    }
    else if (key_down(LEFT_KEY))
    {
        spider.x = (spider.x - SPIDER_SPEED + SCREEN_WIDTH) % SCREEN_WIDTH;
    }
    else if (key_down(UP_KEY))
    {
        spider.y = (spider.y - SPIDER_SPEED + SCREEN_HEIGHT) % SCREEN_HEIGHT;
    }
    else if (key_down(ESCAPE_KEY))
    {
        write_line("Thanks for playing, your score is " + to_string(score));
        IsPlaying = false;
    }
}

/** 
* Updates the game state based on the passage of time.
*
* @param game The current game state to be updated.
* @param current_time The current time in milliseconds.
*/
void update_game(GameData &game, long current_time)
{
    // If the fly has not appeared and the timer has passed the appearAtTime, make the fly appear
    if (!game.fly.appeared && current_time > game.fly.appear_at_time)
    {
        make_fly_appear(game.fly);
        game.fly.escape_at_time = current_time + 2000 + rnd(5000);
    }
    // If the fly has appeared and the timer has passed the escapeAtTime, remove the fly
    else if (game.fly.appeared && current_time > game.fly.escape_at_time)
    {
        remove_fly(game.fly);
        game.fly.appear_at_time = current_time + 1000 + rnd(2000);
    }

    // If the fly has appeared and the spider and fly circles intersect, catch the fly
    if (game.fly.appeared && circles_intersect(game.spider.x, game.spider.y, SPIDER_RADIUS, game.fly.x, game.fly.y, FLY_RADIUS))
    {
        catch_fly();
    }
}

/** 
* Draws the game elements on the screen.
*
* @param game The current game state to be drawn.
*/
void draw_game(const GameData &game)
{
    // Clear the screen
    clear_screen(COLOR_WHITE);

    // Draw the spider
    fill_circle(COLOR_BLACK, game.spider.x, game.spider.y, SPIDER_RADIUS);

    // Draw the fly if it has appeared
    if (game.fly.appeared)
    {
        fill_circle(COLOR_RED, game.fly.x, game.fly.y, FLY_RADIUS);
    }

    // Draw the score in the top-left corner
    draw_text("Score: " + to_string(score), COLOR_BLACK, 10, 10);

    // Draw the timer in the top-right corner
    draw_timer(game_start_time);

    // Refresh the screen
    refresh_screen();
}

/** 
* Makes the fly appear on the screen at a random position.
*
* @param fly The fly data to be updated.
*/
void make_fly_appear(FlyData &fly)
{
    // Generate random position for the fly
    fly.x = rnd(SCREEN_WIDTH - FLY_RADIUS * 2) + FLY_RADIUS;
    fly.y = rnd(SCREEN_HEIGHT - FLY_RADIUS * 2) + FLY_RADIUS;
    fly.appeared = true;
}

/** 
* Removes the fly from the screen.
*
* @param fly The fly data to be updated.
*/
void remove_fly(FlyData &fly)
{
    // Remove the fly
    fly.appeared = false;
}

/** 
* Updates the score and removes the fly when the spider catches the fly.
*/
void catch_fly()
{
    // Catch the fly
    score += 1;
    write_line("Caught +1 Fly\nScore: " + to_string(score));
    remove_fly(game.fly);
}

/** 
* Checks whether two circles intersect.
*
* @param x1 The x-coordinate of the center of the first circle.
* @param y1 The y-coordinate of the center of the first circle.
* @param r1 The radius of the first circle.
* @param x2 The x-coordinate of the center of the second circle.
* @param y2 The y-coordinate of the center of the second circle.
* @param r2 The radius of the second circle.
* @return True if the circles intersect, otherwise false.
*/
bool circles_intersect(int x1, int y1, int r1, int x2, int y2, int r2)
{
    // Calculate the distance between the centers of the circles
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    // Check if the distance is less than the sum of the radii
    return distance < r1 + r2;
}

/** 
* Creates a new spider with an initial position at the center of the screen.
*
* @return The newly created spider data.
*/
SpiderData new_spider()
{
    SpiderData spider;
    spider.x = SCREEN_WIDTH / 2;
    spider.y = SCREEN_HEIGHT / 2;
    return spider;
}

/** 
* Creates a new fly with an initial position at a random location.
*
* @return The newly created fly data.
*/
FlyData new_fly()
{
    FlyData fly;
    fly.appeared = false;
    fly.x = rnd(SCREEN_WIDTH - FLY_RADIUS * 2) + FLY_RADIUS;
    fly.y = rnd(SCREEN_HEIGHT - FLY_RADIUS * 2) + FLY_RADIUS;
    fly.appear_at_time = timer_ticks(GAME_TIMER) + 1000 + rnd(2000);
    fly.escape_at_time = 0;
    return fly;
}

/** 
* Checks if the game duration has elapsed.
*
* @return True if the game duration has elapsed, otherwise false.
*/
bool is_time_up()
{
    return timer_ticks(GAME_TIMER) - game_start_time >= GAME_DURATION * 1000;
}

/** 
* Draws the countdown timer on the screen.
*
* @param start_time The starting time of the game.
*/
void draw_timer(long start_time)
{
    long elapsed_time = timer_ticks(GAME_TIMER) - start_time;
    int remaining_time = GAME_DURATION - elapsed_time / 1000;

    draw_text("Time: " + to_string(remaining_time), COLOR_BLACK, SCREEN_WIDTH - 100, 10);
}