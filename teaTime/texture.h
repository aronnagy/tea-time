#include "model.h"
#include "draw.h"

#include <SOIL/SOIL.h>
#include <GL/glut.h>
#include <stdio.h>

typedef GLubyte Pixel[3]; /*represents red green blue*/

GLuint load_texture(char* filename, Pixel* image);

GLuint initialize_texture(char* filename);
