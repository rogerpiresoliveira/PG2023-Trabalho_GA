#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


// Ball Object inherit from GameObject and contains some extra specific functionality.
class BallObject : public GameObject
{
public:
    // ball state
    float   radius;
    bool    stuck;

    // constructor(s)
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    // moves the ball
    glm::vec2 move(float dt, unsigned int window_width, unsigned int window_height);

    // resets the ball to specific position
    void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif