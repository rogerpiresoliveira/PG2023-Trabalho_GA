#include "game.h"
#include "ResourceManager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include <iostream>
#include <GLFW/glfw3.h>

// Game-related state data
SpriteRenderer* renderer;
GameObject* player1;
GameObject* player2;
BallObject* ball;

using namespace std;

// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

Game::Game(const unsigned int width, const unsigned int height)
    : keys(), width(width), height(height)
{

}

void Game::init()
{
    // load shaders
    ResourceManager::load_shader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

    // projection
    const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

    // configure shaders
    ResourceManager::get_shader("sprite").use().set_integer("image", 0);
    ResourceManager::get_shader("sprite").set_matrix4("projection", projection);

    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

    // load textures
    ResourceManager::load_texture("textures/mesa.jpg", false, "background");
    ResourceManager::load_texture("textures/ball.png", true, "ball");
    ResourceManager::load_texture("textures/paddle.png", true, "paddle");

    // configure game object for player1
    const glm::vec2 player1Pos = glm::vec2(0, this->height / 2.0f - player_size.y / 2.0f);
    player1 = new GameObject(player1Pos, player_size, ResourceManager::get_texture("paddle"));

    // configure game object for player2
    const glm::vec2 player2Pos = glm::vec2(this->width - player_size.x, this->height / 2.0f - player_size.y / 2.0f);
    player2 = new GameObject(player2Pos, player_size, ResourceManager::get_texture("paddle"));

    // configure ball object
    const glm::vec2 ballPos = player1Pos + glm::vec2(player_size.x, player_size.y / 2 - ball_radius);
    ball = new BallObject(ballPos, ball_radius, initial_ball_velocity, ResourceManager::get_texture("ball"));
}

void Game::update(float dt)
{
    // update objects
    ball->move(dt, this->width, this->height);

    // check for collisions
    this->do_collisions(ball, player1);
    this->do_collisions(ball, player2);

    // check loss condition
    if (ball->position.x <= 0.0f) // Did the ball pass player1?
    {
        this->reset_player();
        ball->reset(player2->position + glm::vec2(-ball_radius * 2, player_size.y / 2 - ball_radius), initial_ball_velocity);
    }
    if (ball->position.x + ball->radius >= width) // Did the ball pass player2?
    {
        this->reset_player();
        ball->reset(player1->position + glm::vec2(player_size.x, player_size.y / 2 - ball_radius), initial_ball_velocity);
    }
}

void Game::process_input(float dt)
{
    const float velocity = player_velocity * dt;

    // player1 movement
    if (this->keys[GLFW_KEY_W]) // Up
    {
        if (player1->position.y >= 0.0f)
        {
            player1->position.y -= velocity;
            if (ball->stuck && ball->position.x <= this->width / 2)
                ball->position.y -= velocity;
        }
    }
    if (this->keys[GLFW_KEY_S]) // Down
    {
        if (player1->position.y <= this->height - player1->size.y)
        {
            player1->position.y += velocity;
            if (ball->stuck && ball->position.x <= this->width / 2)
                ball->position.y += velocity;
        }
    }

    // player2 movement
    if (this->keys[GLFW_KEY_UP]) // up
    {
        if (player2->position.y >= 0.0f)
        {
            player2->position.y -= velocity;
            if (ball->stuck && ball->position.x >= this->width / 2)
                ball->position.y -= velocity;
        }
    }
    if (this->keys[GLFW_KEY_DOWN]) // down
    {
        if (player2->position.y <= this->height - player2->size.y)
        {
            player2->position.y += velocity;
            if (ball->stuck && ball->position.x >= this->width / 2)
                ball->position.y += velocity;
        }
    }

    // defines that the ball can move
    if (this->keys[GLFW_KEY_SPACE])
        ball->stuck = false;
}

void Game::render()
{
    // draw background
    renderer->draw_sprite(ResourceManager::get_texture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);

    // draw player1
    player1->draw(*renderer);

    // draw player2
    player2->draw(*renderer);

    // draw ball
    ball->draw(*renderer);
}

void Game::reset_player()
{
    // reset player1 stats
    player1->size = player_size;
    player1->position = glm::vec2(0, this->height / 2.0f - player_size.y / 2.0f);

    // reset player2 stats
    player2->size = player_size;
    player2->position = glm::vec2(this->width - player_size.x, this->height / 2.0f - player_size.y / 2.0f);
}

// collision detection
bool check_collision(GameObject& one, GameObject& two);
Collision check_collision(BallObject& one, GameObject& two);
direction vector_direction(glm::vec2 closest);

void Game::do_collisions(BallObject* ball, GameObject* player)
{
    Collision result = check_collision(*ball, *player);
    if (!ball->stuck && std::get<0>(result)) // check collisions for player pad (unless stuck)
    {
        // check where it hit the board, and change velocity based on where it hit the board
        const float center_board = player->position.y + player->size.y / 2.0f; // Calculate the coordinate that represents the center of the racket.
        const float distance = (ball->position.y + ball->radius) - center_board;
        const float percentage = distance / (player->size.y / 2.0f);
        
        // then move accordingly
        constexpr float strength = 2.0f;
        const glm::vec2 old_velocity = ball->velocity;
        ball->velocity.y = initial_ball_velocity.y * percentage * strength;
        ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        
        // fix sticky paddle
        if (ball->position.x < this->width / 2) {
            ball->velocity.x = 1.0f * abs(ball->velocity.x);
        }
        else {
            ball->velocity.x = -1.0f * abs(ball->velocity.x);
        }
    }
}

bool check_collision(const GameObject& one, const GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    const bool collision_x = one.position.x + one.size.x >= two.position.x && two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    const bool collision_y = one.position.y + one.size.y >= two.position.y && two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collision_x && collision_y;
}

Collision check_collision(BallObject& one, GameObject& two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) <= one.radius)
        return std::make_tuple(true, vector_direction(difference), difference);
    else
        return std::make_tuple(false, up, glm::vec2(0.0f, 0.0f));
}

// calculates which direction a vector is facing (N,E,S or W)
direction vector_direction(glm::vec2 closest)
{
    constexpr glm::vec2 compass[] = {
			  glm::vec2(0.0f, 1.0f),	// up
			  glm::vec2(1.0f, 0.0f),	// right
			  glm::vec2(0.0f, -1.0f),	// down
			  glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(closest), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (direction)best_match;
}