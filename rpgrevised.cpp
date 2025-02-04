#include <splashkit.h>
#include <string>

// Constants for screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constants for player and NPC dimensions
const int PLAYER_SIZE = 50;
const int NPC_SIZE = 30;

const double PLAYER_SPEED = 3; // Adjust player speed
const double NPC_SPEED = 0.5;    // Adjust NPC speed

const int MAX_NPCS = 20; // Maximum number of NPCs
const int NUM_NPC = 5;
const int WINNING_NUMBER = NUM_NPC * 3; // Threshold to spawn more NPCs
const int FPS = 60;


int lifetime = 1000;
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
    int lifetime;
    Target target;
    bool collided; // Flag to indicate if this NPC has collided in the current tick
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
bool check_collision(const NPC &npc, const NPC *npcs, int num_npcs);
void spawn_npc(Game &game);
void time_to_live(Game &game);


int main() {
    open_window("RPG Game", SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize player
    Player player;
    player.position = make_point(SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2);
    player.colour = COLOR_GREEN;

    // Initialize game
    Game game;
    game.player = player;
    game.num_npcs = NUM_NPC;
    game.state = Running;

    // Initialize NPCs
    for (int i = 0; i < game.num_npcs; ++i) {
        game.npcs[i].position = random_position();
        game.npcs[i].is_player_close = false;
        game.npcs[i].normal_colour = COLOR_RED;
        game.npcs[i].close_colour = COLOR_YELLOW;
        game.npcs[i].target.position = random_position();
        game.npcs[i].collided = false;
        game.npcs[i].lifetime = lifetime;
    }
// game loop
    do {
        process_events();
        handle_player_input(game.player);
        tick(game); 
        time_to_live(game);
    } while(game.state == Running);

    close_window("RPG Game");
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

// Function to check collision between NPCs
bool check_collision(const NPC &npc, const NPC *npcs, int num_npcs) {
    for (int i = 0; i < num_npcs; ++i) {
        if (&npc != &npcs[i]) { // Exclude checking collision with itself
            double dx = npc.position.x - npcs[i].position.x;
            double dy = npc.position.y - npcs[i].position.y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < NPC_SIZE) {
                return true; // Collision detected
            }
        }
    }
    return false; // No collision detected
}

// Procedure to update the game state
void tick(Game &game) {
    // Move NPCs
    for (int i = 0; i < game.num_npcs; ++i) {
        move_npc(game.npcs[i]);
    }

// Check collision between NPCs and change color to blue if needed
for (int i = 0; i < game.num_npcs; ++i) {
    if (check_collision(game.npcs[i], game.npcs, game.num_npcs)) {
        if (!game.npcs[i].collided) {
            game.npcs[i].normal_colour = COLOR_BLUE; // Change color to blue if collided
            game.npcs[i].collided = true;
            spawn_npc(game); // Spawn NPC if it hasn't already spawned after collision
        }
        break;
    } else {
        game.npcs[i].normal_colour = COLOR_RED; // Reset color to normal if not collided
        game.npcs[i].collided = false;
    }
}

    // Update NPCs' close status based on player's position
    for (int i = 0; i < game.num_npcs; ++i) {
        double distance = point_point_distance(game.player.position, game.npcs[i].position);
        game.npcs[i].is_player_close = (distance <= 100); // Assuming the close proximity is within 100 pixels
    }

    // Render the game
    clear_screen(COLOR_WHITE);

    // Display current number of NPCs
    //draw_text("Number of NPCs: " + std::to_string(game.num_npcs), COLOR_BLACK, 10, 10);

    // render player
    fill_rectangle(game.player.colour, game.player.position.x, game.player.position.y, PLAYER_SIZE, PLAYER_SIZE);

    // Draw NPCs with appropriate colour
    for (int i = 0; i < game.num_npcs; ++i) {
        if (game.npcs[i].is_player_close) {
            fill_triangle(game.npcs[i].close_colour, game.npcs[i].position.x, game.npcs[i].position.y, game.npcs[i].position.x + NPC_SIZE, game.npcs[i].position.y, game.npcs[i].position.x + (NPC_SIZE / 2), game.npcs[i].position.y - NPC_SIZE);
            game.npcs[i].lifetime = lifetime;
        } else {
            fill_triangle(game.npcs[i].normal_colour, game.npcs[i].position.x, game.npcs[i].position.y, game.npcs[i].position.x + NPC_SIZE, game.npcs[i].position.y, game.npcs[i].position.x + (NPC_SIZE / 2), game.npcs[i].position.y - NPC_SIZE);
        }
    }

    if (game.num_npcs <= 0) {
        game.state = Lost;
        write_line("Whoops The npc's died you lose");
    } else if (game.num_npcs >= WINNING_NUMBER) {
        game.state = Won;
        write_line("Congrats you maxed out the NPC's you win!");
    }

    refresh_screen(FPS);
}

void time_to_live(Game &game){
    // Update NPCs' lifetime and handle collisions with player
    for (int i = 0; i < game.num_npcs; ++i) {
        if (!game.npcs[i].collided) {
            game.npcs[i].lifetime--; // Decrease NPC's lifetime if it hasn't collided with the player
            // Check if NPC's lifetime has reached 0
            //write_line(game.npcs[i].lifetime);
            if (game.npcs[i].lifetime <= 0) {
                // Remove NPC if lifetime is 0
                write_line("NPC removed");
                for (int j = i; j < game.num_npcs - 1; ++j) {
                    game.npcs[j] = game.npcs[j + 1];
                }
                game.num_npcs--;
                i--; // Update loop index to recheck current index after removing NPC
            }
        }
    }
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

// Procedure to spawn a new NPC
void spawn_npc(Game &game) {
    if (game.num_npcs < MAX_NPCS) {
        game.npcs[game.num_npcs].position = random_position();
        game.npcs[game.num_npcs].is_player_close = false;
        game.npcs[game.num_npcs].normal_colour = COLOR_RED;
        game.npcs[game.num_npcs].close_colour = COLOR_YELLOW;
        game.npcs[game.num_npcs].target.position = random_position();
        game.npcs[game.num_npcs].collided = false;
        game.npcs[game.num_npcs].lifetime = lifetime;
        game.num_npcs++;
    }
}
