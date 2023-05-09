/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "Shader.h"


class SpriteRenderer
{
public:
    // Constructor (init shader/shapes)
    SpriteRenderer(Shader shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void draw_sprite(Texture2D texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    // render state
    Shader       shader_;
    unsigned int quad_vao_;
    // Initializes and configures the quad's buffer and vertex attributes
    void init_render_data();
};

#endif