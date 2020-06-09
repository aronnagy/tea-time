#include "model.h"
#include "draw.h"

#include <SOIL/SOIL.h>
#include <GL/glut.h>
#include <stdio.h>
#include "texture.h"

Pixel*images[3];
GLuint texture_names[3];

void initialize_texture()
{
    unsigned int i;

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    char texture_filenames[][32] =
    {
        "house.png",
        "Board.png"
    };

    for(i=0; i < 2; i++)
    {
        texture_names[i] = load_texture(texture_filenames[i],images[i]);
    }

    int width;
    int height;

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

    glEnable(GL_TEXTURE_2D);
}

GLuint load_texture(char* filename, Pixel* image)
{
    GLuint texture_name;
    glGenTextures(1, &texture_name);

    int width;
    int height;

    image = (Pixel*)SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (Pixel*)image);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    return texture_name;
}

