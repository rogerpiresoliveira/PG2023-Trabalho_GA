/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Shader.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader& Shader::use()
{
    glUseProgram(this->id);
    return *this;
}

void Shader::compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
    unsigned int sVertex, sFragment, gShader;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertex_source, NULL);
    glCompileShader(sVertex);
    check_compile_errors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragment_source, NULL);
    glCompileShader(sFragment);
    check_compile_errors(sFragment, "FRAGMENT");
    // if geometry Shader source code is given, also compile geometry Shader
    if (geometry_source != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometry_source, NULL);
        glCompileShader(gShader);
        check_compile_errors(gShader, "GEOMETRY");
    }
    // Shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, sVertex);
    glAttachShader(this->id, sFragment);
    if (geometry_source != nullptr)
        glAttachShader(this->id, gShader);
    glLinkProgram(this->id);
    check_compile_errors(this->id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometry_source != nullptr)
        glDeleteShader(gShader);
}

void Shader::set_float(const char* name, float value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
}
void Shader::set_integer(const char* name, int value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}
void Shader::set_vector_2_f(const char* name, float x, float y, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void Shader::set_vector_2_f(const char* name, const glm::vec2& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void Shader::set_vector_3_f(const char* name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}
void Shader::set_vector_3_f(const char* name, const glm::vec3& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void Shader::set_vector_4_f(const char* name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}
void Shader::set_vector_4_f(const char* name, const glm::vec4& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}
void Shader::set_matrix4(const char* name, const glm::mat4& matrix, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}


void Shader::check_compile_errors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}