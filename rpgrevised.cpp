#include <splashkit.h>

// Constants for screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constants for player and NPC dimensions
const int PLAYER_SIZE = 50;
const int NPC_SIZE = 30;

const double PLAYER_SPEED = 0.7; // Adjust player speed
const double NPC_SPEED = 0.05;    // Adjust NPC speed

const int MAX_NPCS = 20; // Maximum number of NPCs

// Player
struct Player {
    point_2d position;
    color colour;
};

// Target
struct Target {
    point_2d position;
};

// NPC
struct NPC {
    point_2d position;
    bool is_player_close;
    color normal_colour;
    color close_colour;
    Target target;
};

// Enum for Game state
enum GameState {
    Running,
    Won,
    Lost
};

// Game
struct Game {
    Player player;
    NPC npcs[MAX_NPCS]; // Array to hold NPCs
    int num_npcs; // Number of NPCs currently active
    GameState state;
};

// Forward declaring
point_2d random_position();
point_2d make_point(double x, double y);
void set_new_random_target(NPC &npc);
void move_npc(NPC &npc);
void tick(Game &game);
void handle_player_input(Player &player);


int main() {
    open_window("RPG Game", SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize player
    Player player;
    player.position = make_point(SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2);
    player.colour = COLOR_GREEN;

    // Initialize game
    Game game;
    game.player = player;
    game.num_npcs = rnd(MAX_NPCS); // Random number of NPCs between 0 and MAX_NPCS
    game.state = Running;

    // Initialize NPCs
    for (int i = 0; i < game.num_npcs; ++i) {
        game.npcs[i].position = random_position();
        game.npcs[i].is_player_close = false;
        game.npcs[i].normal_colour = COLOR_RED;
        game.npcs[i].close_colour = COLOR_YELLOW;
        game.npcs[i].target.position = random_position();
    }

    do {
        process_events();
        handle_player_input(game.player);
        tick(game);
    } while(!window_close_requested("RPG Game"));

    return 0;
}

// Function to create a point_2d struct
point_2d make_point(double x, double y) {
    point_2d result;
    result.x = x;
    result.y = y;
    return result;
}

// Function to generate a random position within the screen bounds
point_2d random_position() {
    double x = rnd(SCREEN_WIDTH);
    double y = rnd(SCREEN_HEIGHT);
    return make_point(x, y);
}

// Procedure to set a new random target for the NPC
void set_new_random_target(NPC &npc) {
    npc.target.position = random_position();
}

// Procedure to move the NPC towards its current target
void move_npc(NPC &npc) {
    double dx = npc.target.position.x - npc.position.x;
    double dy = npc.target.position.y - npc.position.y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance > 1.0) { // Adjust threshold as needed
        double ratio = NPC_SPEED / distance;
        npc.position.x += dx * ratio;
        npc.position.y += dy * ratio;
    } else {
        set_new_random_target(npc);
    }
}

// Procedure to update the game state
void tick(Game &game) {
    // Move NPCs
    for (int i = 0; i < game.num_npcs; ++i) {
        move_npc(game.npcs[i]);
    }

    // Update NPCs' close status based on player's position
    for (int i = 0; i < game.num_npcs; ++i) {
        double distance = point_point_distance(game.player.position, game.npcs[i].position);
        game.npcs[i].is_player_close = (distance <= 100); // Assuming the close proximity is within 100 pixels
    }

    // Render the game
    clear_screen(COLOR_WHITE);
    fill_rectangle(game.player.colour, game.player.position.x, game.player.position.y, PLAYER_SIZE, PLAYER_SIZE);

    // Draw NPCs with appropriate colour
    for (int i = 0; i < game.num_npcs; ++i) {
        if (game.npcs[i].is_player_close) {
            fill_triangle(game.npcs[i].close_colour, game.npcs[i].position.x, game.npcs[i].position.y, game.npcs[i].position.x + NPC_SIZE, game.npcs[i].position.y, game.npcs[i].position.x + (NPC_SIZE / 2), game.npcs[i].position.y - NPC_SIZE);
        } else {
            fill_triangle(game.npcs[i].normal_colour, game.npcs[i].position.x, game.npcs[i].position.y, game.npcs[i].position.x + NPC_SIZE, game.npcs[i].position.y, game.npcs[i].position.x + (NPC_SIZE / 2), game.npcs[i].position.y - NPC_SIZE);
        }
    }

    refresh_screen();
}

// Procedure to handle player input
void handle_player_input(Player &player) {
    if (key_down(W_KEY) && player.position.y > 0) {
        player.position.y -= PLAYER_SPEED;
    }
    else if (key_down(A_KEY) && player.position.x > 0) {
        player.position.x -= PLAYER_SPEED;
    }
    else if (key_down(S_KEY) && player.position.y < SCREEN_HEIGHT - PLAYER_SIZE) {
        player.position.y += PLAYER_SPEED;
    }
    else if (key_down(D_KEY) && player.position.x < SCREEN_WIDTH - PLAYER_SIZE) {
        player.position.x += PLAYER_SPEED;
    }
    else if (key_down(ESCAPE_KEY)) {
        close_window("RPG Game"); 
    }
}
