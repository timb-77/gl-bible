#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include "libs/math3d.h"
#include "libs/gltools.h"

#define  GL_PI        3.1415f
#define  NUM_SPHERES  50

GLfloat OrthoLeft, OrthoRight, OrthoBottom, OrthoTop;
GLfloat rot_inc = GL_PI / 72.0f;

class GLFrame {
private:
public:
};

GLFrame spheres[NUM_SPHERES];
GLFrame frameCamera;

void SetupRC(void)
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set drawing color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set color shading model to flat
    glShadeModel(GL_FLAT);

    // Clockwise-wound polygons are front facing;
    // this is reversed because we are using triangle fans
    glFrontFace(GL_CCW);
}

void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

    // Reset coordinate system
    gluPerspective(30.0f, aspectRatio, 1.0, 400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TimerFunction(int value)
{
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void RenderScene(void)
{
    M3DMatrix44f transformationMatrix; // For Y-axis rotation, and Z-axis translation (in one step/matrix)
    M3DMatrix44f rotationMatrix; // For Z-axis rotation
    static GLfloat yRot = 0.0f; // Rotation angle
    static GLfloat zRot = 0.0f; // Rotation angle

    yRot += 1.0f;
    zRot += 1.0f;

    // Turn culling on if flag is set
    glEnable(GL_CULL_FACE);

    // Enable depth testing if flag is set
    glEnable(GL_DEPTH_TEST);

    // Draw the back side as a wireframe only, if flag is set
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
    // Clear window (w/ current clearing color)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
 
        // Build the transformation matrix:
        //   a) yRot degrees around Y-axis (Rotation)
        m3dRotationMatrix44(transformationMatrix, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
        //   b) position torus a little down the Z-axis (Translation)
        transformationMatrix[12] =  0.0f;
        transformationMatrix[13] =  0.0f;
        transformationMatrix[14] = -2.5f;
    
        glMultMatrixf(transformationMatrix);
    
        m3dRotationMatrix44(rotationMatrix, m3dDegToRad(zRot), 0.0f, 0.0f, 1.0f);
    
        glMultMatrixf(rotationMatrix);
    
        gltDrawTorus(0.35, 0.15, 40, 20);
    
    glPopMatrix();

    // Show the image
    glutSwapBuffers();
} 

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLtorus");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1); // (1000 / 33) ms --> 30 frames per second 

    SetupRC();

    RenderScene(); // initial rendering after start-up

    glutMainLoop();

    return 0;
}
