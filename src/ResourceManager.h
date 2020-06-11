#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Main.h"

class ResourceManager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures2D;
    static Shader loadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    static Shader getShader(std::string name);
    static Texture2D loadTexture2D(const char *file, bool alpha, std::string name);
    static Texture2D getTexture2D(std::string name);
    static void flush();
private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Texture2D loadTexture2DFromFile(const char *file, bool alpha);
};

#endif