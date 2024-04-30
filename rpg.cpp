#include <splashkit.h>

// Constants for screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constants for player and NPC dimensions
const int PLAYER_SIZE = 50;
const int NPC_SIZE = 30;

const double SPEED = 0.7;

bool collision_in_radius(point_2d point, point_2d center, double radius);
bool have_characters_collided(point_2d player_pos, point_2d npc_pos);
bool point_in_rectangle(double px, double py, double rx, double ry, double rw, double rh);
bool point_in_circle(double px, double py, double cx, double cy, double radius);
bool rectangle_circle_collision(double rx, double ry, double rw, double rh, double cx, double cy, double radius);
double clamp(double value, double min, double max);
void draw_player(point_2d player_pos);
void draw_npc(point_2d npc_pos);
void render(point_2d player_pos, point_2d npc_pos, bool is_npc_alive);
void update_player_position_and_check_collision(point_2d &player_pos, const point_2d &npc_pos, bool &is_npc_alive);

int main() {
    open_window("RPG Game", SCREEN_WIDTH, SCREEN_HEIGHT);
    point_2d player_pos = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2};
    point_2d npc_pos = {100, 100};
    bool is_npc_alive = true;
    do {
    process_events();
    update_player_position_and_check_collision(player_pos, npc_pos, is_npc_alive);
    render(player_pos, npc_pos, is_npc_alive);
    } while(!window_close_requested("RPG Game"));
    return 0;
}

// Function to update player position and check collision
void update_player_position_and_check_collision(point_2d &player_pos, const point_2d &npc_pos, bool &is_npc_alive) {
    if (key_down(W_KEY) && player_pos.y > 0) {
        write_line("key_down(W_KEY) -- Moved Up");
        player_pos.y -= SPEED;
    }
    else if (key_down(A_KEY) && player_pos.x > 0) {
        write_line("key_down(A_KEY) -- Moved Left");
        player_pos.x -= SPEED;
    }
    else if (key_down(S_KEY) && player_pos.y < SCREEN_HEIGHT - PLAYER_SIZE) {
        write_line("key_down(S_KEY) -- Moved Down");
        player_pos.y += SPEED;
    }
    else if (key_down(D_KEY) && player_pos.x < SCREEN_WIDTH - PLAYER_SIZE) {
        write_line("key_down(D_KEY) -- Moved Right");
        player_pos.x += SPEED;
    }
    else if (key_down(ESCAPE_KEY)) {
        write_line("key_down(ESCAPE_KEY) -- Close game");
        write_line("Game ending...");
        write_line("PROCESS ENDED");
        close_window("RPG Game"); 
    }
    // Check collision and update NPC status
    if (collision_in_radius(player_pos, npc_pos, 100)) {
        if (have_characters_collided(player_pos, npc_pos)) {
            write_line(" -- Collided -- ");
            is_npc_alive = false;
        }
    } else {
        is_npc_alive = true;
    }
}
// Function to check if a point is within a collision circle
bool collision_in_radius(point_2d point, point_2d center, double radius) {
    return point_point_distance(point, center) <= radius;
}
// Function to check if two characters have collided
bool have_characters_collided(point_2d player_pos, point_2d npc_pos) {
    return rectangle_circle_collision(player_pos.x, player_pos.y, PLAYER_SIZE, PLAYER_SIZE, npc_pos.x, npc_pos.y, NPC_SIZE);
}
// Function to check if a point is within a rectangle
bool point_in_rectangle(double px, double py, double rx, double ry, double rw, double rh) {
    return (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
}
// Function to check if a point is within a circle
bool point_in_circle(double px, double py, double cx, double cy, double radius) {
    return ((px - cx) * (px - cx) + (py - cy) * (py - cy)) <= (radius * radius);
}
// Function to check if a rectangle and a circle have collided
bool rectangle_circle_collision(double rx, double ry, double rw, double rh, double cx, double cy, double radius) {
    double closestX = clamp(cx, rx, rx + rw);
    double closestY = clamp(cy, ry, ry + rh);
    return point_in_circle(closestX, closestY, cx, cy, radius);
}
// Function to clamp a value between a minimum and maximum
double clamp(double value, double min, double max) {
    return value < min ? min : (value > max ? max : value);
}
// Procedure to draw the player character
void draw_player(point_2d player_pos) {
    fill_rectangle(COLOR_GREEN, player_pos.x, player_pos.y, PLAYER_SIZE, PLAYER_SIZE);
}
// Procedure to draw the NPC
void draw_npc(point_2d npc_pos) {
    fill_triangle(COLOR_RED, npc_pos.x, npc_pos.y, npc_pos.x + NPC_SIZE, npc_pos.y, npc_pos.x + (NPC_SIZE / 2), npc_pos.y - NPC_SIZE);
}
// Procedure to render the game
void render(point_2d player_pos, point_2d npc_pos, bool is_npc_alive) {
    clear_screen(COLOR_WHITE);

    // Draw player character
    draw_player(player_pos);

    // Draw NPC if it's alive
    if (is_npc_alive) {
        draw_npc(npc_pos);
    }

    refresh_screen();
}