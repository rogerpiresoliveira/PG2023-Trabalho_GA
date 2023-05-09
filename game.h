#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game_object.h"
#include "ball_object.h"

// Represents the four possible (collision) directions
enum direction {
    up,
    right,
    down,
    left
};

// Initial size of the player paddle
constexpr glm::vec2 player_size(20.0f, 100.0f);
// Initial velocity of the player paddle
constexpr float player_velocity(1000.0f);
// Initial velocity of the ball
constexpr glm::vec2 initial_ball_velocity(1500.0f, -150.0f);
// radius of the ball object
constexpr float ball_radius = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    bool                    keys[1024];
    unsigned int            width, height;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);

    // initialize game state (load all shaders/textures/levels)
    void init();

    // game loop
    void process_input(float dt);
    void update(float dt);
    void render();
    void do_collisions(BallObject* ball, GameObject* player);

    // reset
    void reset_player();
};

#endif