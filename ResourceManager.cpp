/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>

#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::texture_map;
std::map<std::string, Shader>       ResourceManager::shader_map;


Shader ResourceManager::load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name)
{
    shader_map[name] = load_shader_from_file(v_shader_file, f_shader_file, g_shader_file);
    return shader_map[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    return shader_map[name];
}

Texture2D ResourceManager::load_texture(const char* file, bool alpha, std::string name)
{
    // Save the texture with the given name to the map
    texture_map[name] = load_texture_from_file(file, alpha);
    return texture_map[name];
}

Texture2D ResourceManager::get_texture(std::string name)
{
    return texture_map[name];
}

void ResourceManager::clear()
{
    // (properly) delete all shaders	
    for (auto iter : shader_map)
        glDeleteProgram(iter.second.id);
    // (properly) delete all textures
    for (auto iter : texture_map)
        glDeleteTextures(1, &iter.second.id);
}

Shader ResourceManager::load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(v_shader_file);
        std::ifstream fragmentShaderFile(f_shader_file);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry Shader path is present, also load a geometry Shader
        if (g_shader_file != nullptr)
        {
            std::ifstream geometryShaderFile(g_shader_file);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read Shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create Shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, g_shader_file != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}