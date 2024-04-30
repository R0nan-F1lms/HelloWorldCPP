// Using statements "clang++ flycatch.cpp -l SplashKit -o flycatch
#include "splashkit.h"
#include <cstdlib> // for rand function
#include <ctime> // for time function

using std::stod;
using std::to_string;

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPIDER_RADIUS = 25;
const int SPIDER_SPEED = 2;
const int FLY_RADIUS = 15;
const string GAME_TIMER = "Game Timer";

// Variables
static int spiderX = SCREEN_WIDTH / 2;
static int spiderY = SCREEN_HEIGHT / 2;
static int flyX = 0;
static int flyY = 0;
static bool flyAppeared = false;
static long appearAtTime = 1000 + rnd(2000);
static long escapeAtTime = 0;
static bool IsPlaying = true;
static int score = 0;

// Forward declaring
void handle_input();
void update_game();
void draw_game();
void make_fly_appear();
void remove_fly();
void catch_fly();
bool circles_intersect(int x1, int y1, int r1, int x2, int y2, int r2);


int main() {
    // Open the window
    open_window("Spider Catcher", SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create and start the game timer
    create_timer(GAME_TIMER);
    start_timer(GAME_TIMER);

    // Game loop
    while (IsPlaying) {
        // Handle input
        handle_input();

        // Update the game
        update_game();

        // Draw the game
        draw_game();

        // Process events
        process_events();
    }

    // Close the window
    close_window("Spider Catcher");

    return 0;
}
void handle_input() {
    // Move the spider
    if (key_down(RIGHT_KEY) && spiderX + SPIDER_SPEED + SPIDER_RADIUS < SCREEN_WIDTH) {
        spiderX += SPIDER_SPEED;
    } else if (key_down(LEFT_KEY) && spiderX + SPIDER_SPEED + SPIDER_RADIUS > 0) {
        spiderX -= SPIDER_SPEED;
    } else if (key_down(UP_KEY) && spiderY + SPIDER_SPEED + SPIDER_RADIUS > 0) {
        spiderY -= SPIDER_SPEED;
    } else if (key_down(DOWN_KEY) && spiderY + SPIDER_SPEED + SPIDER_RADIUS < SCREEN_HEIGHT) {
        spiderY += SPIDER_SPEED;
    } else if (key_down(ESCAPE_KEY)) {
        write_line("Thanks for playing, your score is " + to_string(score));
        IsPlaying = false;
    }
}

void update_game() {
    // If the fly has not appeared and the timer has passed the appearAtTime, make the fly appear
    if (!flyAppeared && timer_ticks(GAME_TIMER) > appearAtTime) {
        make_fly_appear();
        escapeAtTime = timer_ticks(GAME_TIMER) + 2000 + rnd(5000);
    }
    // If the fly has appeared and the timer has passed the escapeAtTime, remove the fly
    else if (flyAppeared && timer_ticks(GAME_TIMER) > escapeAtTime) {
        remove_fly();
        appearAtTime = timer_ticks(GAME_TIMER) + 1000 + rnd(2000);
    }

    // If the fly has appeared and the spider and fly circles intersect, catch the fly
    if (flyAppeared && circles_intersect(spiderX, spiderY, SPIDER_RADIUS, flyX, flyY, FLY_RADIUS)) {
        catch_fly();
    }
}

void draw_game() {
    // Clear the screen
    clear_screen(COLOR_WHITE);

    // Draw the spider
    fill_circle(COLOR_BLACK, spiderX, spiderY, SPIDER_RADIUS);

    // Draw the fly if it has appeared
    if (flyAppeared) {
        fill_circle(COLOR_RED, flyX, flyY, FLY_RADIUS);
    }

    // Refresh the screen
    refresh_screen();
}

void make_fly_appear() {
    // Generate random position for the fly
    flyX = rnd(SCREEN_WIDTH - FLY_RADIUS * 2) + FLY_RADIUS;
    flyY = rnd(SCREEN_HEIGHT - FLY_RADIUS * 2) + FLY_RADIUS;
    flyAppeared = true;
}

void remove_fly() {
    // Remove the fly
    flyAppeared = false;
}

void catch_fly() {
    // Catch the fly
    score += 1;
    write_line("Caught +1 Fly\nScore: " + to_string(score));
    remove_fly();
}

bool circles_intersect(int x1, int y1, int r1, int x2, int y2, int r2) {
    // Calculate the distance between the centers of the circles
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    // Check if the distance is less than the sum of the radii
    return distance < r1 + r2;
}