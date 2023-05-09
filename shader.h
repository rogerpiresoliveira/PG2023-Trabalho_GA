/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>


// General purpose Shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // state
    unsigned int id;
    // constructor
    Shader() { }
    // sets the current Shader as active
    Shader& use();
    // compiles the Shader from given source code
    void    compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr); // note: geometry source code is optional 
    // utility functions
    void    set_float(const char* name, float value, bool use_shader = false);
    void    set_integer(const char* name, int value, bool use_shader = false);
    void    set_vector_2_f(const char* name, float x, float y, bool use_shader = false);
    void    set_vector_2_f(const char* name, const glm::vec2& value, bool use_shader = false);
    void    set_vector_3_f(const char* name, float x, float y, float z, bool use_shader = false);
    void    set_vector_3_f(const char* name, const glm::vec3& value, bool use_shader = false);
    void    set_vector_4_f(const char* name, float x, float y, float z, float w, bool use_shader = false);
    void    set_vector_4_f(const char* name, const glm::vec4& value, bool use_shader = false);
    void    set_matrix4(const char* name, const glm::mat4& matrix, bool use_shader = false);
private:
    // checks if compilation or linking failed and if so, print the error logs
    void    check_compile_errors(unsigned int object, std::string type);
};

#endif