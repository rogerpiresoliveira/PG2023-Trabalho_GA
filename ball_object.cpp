/******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ball_object.h"

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true) { }

glm::vec2 BallObject::move(const float dt, const unsigned int window_width, const unsigned int window_height)
{
    // if not stuck to player
    if (!this->stuck)
    {
        // Move the ball
        this->position += this->velocity * dt;
        if (this->position.y <= 0.0f) // Did hit the bottom?
        {
            this->velocity.y = -this->velocity.y; // Reverse velocity
            this->position.y = 0.0f;
        }
        else if (this->position.y + this->size.y >= window_height) // Did hit the top?
        {
            this->velocity.y = -this->velocity.y; // Reverse velocity
            this->position.y = window_height - this->size.y;
        }
    }
    return this->position;
}

// Resets the ball to a specific position and set the stuck mode to true
void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
    this->position = position;
    this->velocity = velocity;
    this->stuck = true;
}