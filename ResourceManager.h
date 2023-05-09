/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "texture.h"
#include "Shader.h"


// A static singleton ResourceManager class that hosts several
// functions to load texture_map and shader_map. Each loaded texture
// and/or Shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    shader_map;
    static std::map<std::string, Texture2D> texture_map;
    // loads (and generates) a Shader program from file loading vertex, fragment (and geometry) Shader's source code. If g_shader_file is not nullptr, it also loads a geometry Shader
    static Shader    load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name);
    // retrieves a stored Shader
    static Shader    get_shader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D load_texture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D get_texture(std::string name);
    // properly de-allocates all loaded resources
    static void      clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() = default;
    // loads and generates a Shader from file
    static Shader    load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file = nullptr);
    // loads a single texture from file
    static Texture2D load_texture_from_file(const char* file, bool alpha);
};

#endif