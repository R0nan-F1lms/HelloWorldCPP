#include <splashkit.h>

// Constants for screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constants for player and NPC dimensions
const int PLAYER_SIZE = 50;
const int NPC_SIZE = 30;

const double PLAYER_SPEED = 0.7; // Adjust player speed
const double NPC_SPEED = 0.05;    // Adjust NPC speed

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
    NPC npc;
    GameState state;
};

// farward declaring
point_2d random_position();
point_2d make_point (double x, double y);
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

    // Initialize NPC
    NPC npc;
    npc.position = make_point(100, 100);
    npc.is_player_close = false;
    npc.normal_colour = COLOR_RED;
    npc.close_colour = COLOR_YELLOW;
    npc.target.position = random_position();

    // Initialize game
    Game game;
    game.player = player;
    game.npc = npc;
    game.state = Running;

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
    move_npc(game.npc);

    // Update NPC's close status based on player's position
    double distance = point_point_distance(game.player.position, game.npc.position);
    game.npc.is_player_close = (distance <= 100); // Assuming the close proximity is within 100 pixels

    // Render the game
    clear_screen(COLOR_WHITE);
    fill_rectangle(game.player.colour, game.player.position.x, game.player.position.y, PLAYER_SIZE, PLAYER_SIZE);

    // Draw NPC with appropriate colour
    if (game.npc.is_player_close) {
        fill_triangle(game.npc.close_colour, game.npc.position.x, game.npc.position.y, game.npc.position.x + NPC_SIZE, game.npc.position.y, game.npc.position.x + (NPC_SIZE / 2), game.npc.position.y - NPC_SIZE);
    } else {
        fill_triangle(game.npc.normal_colour, game.npc.position.x, game.npc.position.y, game.npc.position.x + NPC_SIZE, game.npc.position.y, game.npc.position.x + (NPC_SIZE / 2), game.npc.position.y - NPC_SIZE);
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