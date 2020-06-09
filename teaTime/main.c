#include "model.h"
#include "draw.h"

#include <SOIL/SOIL.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Model model;
typedef GLubyte Pixel[3]; /*represents red green blue*/
// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f, z=5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
/*lights*/
GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_diffuse[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light_specular[] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_position[] = {0, 1.0, 0};
/*help*/
int help = 0;
int pause = 0;

Pixel*images[3];
GLuint texture_names[3];

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
void initialize_texture()
{
    unsigned int i;

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    char texture_filenames[][32] =
    {
        "board.png",
        "house.png",
        "grass.png"
    };

    for(i=0; i < 3; i++)
    {
        texture_names[i] = load_texture(texture_filenames[i],images[i]);
    }
    
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);
}
void changeSize(GLsizei w, GLsizei h)
{
		if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
void drawTeapot()
{
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,texture_names[0]);
	glTranslatef(deltaMove, 0, deltaAngle-8);
	glPushMatrix();
	glTranslatef(0.0f ,-0.25f, 0.0f);
	glColor3f(1.0f ,0.0f, 0.0f);
	glRotatef(0, 180, 1.0, 0);
	glutSolidTeapot(1.0f);
	glPopMatrix();
}
void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}
void drawText(const char *text, int lenght, int x, int y){
	glColor3f(1,1,1);
	glMatrixMode(GL_PROJECTION);
	double matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX,matrix);
	glLoadIdentity();
	glOrtho(0,800,0,600,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
  int i = 0;
	for(i; i<lenght; i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
void renderScene(void)
{
    if(help == 1) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();
    char helpRow1[] = "In this program your can drive a teapot between houses.";
    char helpRow2[] = "HAPPY TEA TIME! :D";
    char helpRow3[] = "You can drive the teapot with your arrows.";
    char helpRow4[] = "You can also switch lights with + and - operators.";
    char helpRow5[] = "Press F1 for return!";

    drawText(helpRow1,55,250,500);
    drawText(helpRow2,18,250,450);
    drawText(helpRow3,42,250,400);
    drawText(helpRow4,50,250,350);
    drawText(helpRow5,20,250,300);
  }else{
    if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, z,
				x+lx, 1.0f,  z+lz,
				0.0f, 1.0f,  0.0f);

    // Draw ground
    glBindTexture(GL_TEXTURE_2D,texture_names[2]);
	glBegin(GL_QUADS);
		/*Ground*/
		glTexCoord2f(0.0,0.0);
		glVertex3f(-50.0f, 0.0f, -50.0f);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-50.0f, 0.0f,  50.0f);
		glTexCoord2f(1.0,1.0);
		glVertex3f( 50.0f, 0.0f,  50.0f);
		glTexCoord2f(0.0,1.0);
		glVertex3f( 50.0f, 0.0f, -50.0f);
    glEnd();

	glMatrixMode(GL_MODELVIEW);

    //Draw houses	
	glBindTexture(GL_TEXTURE_2D,texture_names[1]);
    for(int i = -5; i < 5; i++){
		for(int j=-5; j < 5; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			draw_model(&model);
			glPopMatrix();
		}
	}
	drawTeapot();
	}

	
  if(help == 1 && pause == 0) {
    glutSwapBuffers();
    pause = 1;
  }
  else if(help == 0) glutSwapBuffers();
}
void changeLight()
{
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 27) exit(0);

  switch(key) {
    case 43:
        if (light_ambient[0] < 3.5) {
          light_ambient[0] += 0.5;
          light_ambient[1] += 0.5;
          light_ambient[2] += 0.5;
          light_ambient[3] += 0.5;
        }
        changeLight();
        break;
    case 45:
    if (light_ambient[0] > 0.0) {
      light_ambient[0] -= 0.5;
      light_ambient[1] -= 0.5;
      light_ambient[2] -= 0.5;
      light_ambient[3] -= 0.5;
    }
    changeLight();
    break;
  }
}
void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
		 case GLUT_KEY_F1:
      if(help == 1) {
        help=0;
        pause = 0;
      }
  		else {
        help = 1;
      } break;
		glutPostRedisplay();
	}
}
void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
		glutPostRedisplay();
	}
}
void initialize()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 1.0, 1.0, 0.0);

    initialize_texture();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
/**
 * Main function
 */
int main(int argc, char* argv[])
{    
    glutInit(&argc, argv);

    load_model("house.obj", &model);
    scale_model(&model,0.05,0.05,0.05);
    print_bounding_box(&model);

	glutInitWindowSize(1920, 1080);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	int window = glutCreateWindow("Tea Time");

    // register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

    glutSpecialFunc(pressKey);
    initialize();

    // here are the new entries
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);
	glutKeyboardFunc(processNormalKeys);
  	glutKeyboardUpFunc(processNormalKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

    glutMainLoop();
	
    return 0;
}
