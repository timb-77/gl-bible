#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/gl.h>

#define GL_PI 3.1415f

GLfloat OrthoLeft, OrthoRight, OrthoBottom, OrthoTop;
GLfloat rot_inc = GL_PI / 72.0f;

void SetupRC(void)
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set drawing color to green
    glColor3f(0.0f, 1.0f, 0.0f);

    // Set color shading model to flat
    glShadeModel(GL_FLAT);

    // Clockwise-wound polygons are front facing;
    // this is reversed because we are using triangle fans
//    glFrontFace(GL_CW);
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
    gluPerspective(100.0f, aspectRatio, 1.0, 400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TimerFunction(int value)
{
    glutPostRedisplay();
    glutTimerFunc(100, TimerFunction, 1);
}

void RenderScene(void)
{
    // Angle of revolution around the nucleus
    static GLfloat fElect1 = 0.0f;

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Reset the ModelView matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Translate the whole scene out and into view.
    // This is the initial viewing transformation.
    glTranslatef(0.0f, 0.0f, -200.0f);

    // Red nucleus
    glColor3ub(255, 0, 0);
    glutSolidSphere(10.0f, 15, 15); // Draw the nucleus


    // First electron orbit
    glPushMatrix(); // Save viewing transformation
        glColor3ub(0xff, 0xff, 0x00); // Yellow electron 1
        glRotatef(fElect1, 0.0f, 1.0f, 0.0f); // Rotate by angle of revolution
        glTranslatef(120.0f, 0.0f, 0.0f); // Translate out from origin to distance
        glutSolidSphere(6.0f, 15, 15); // Draw the electron
    glPopMatrix(); // Restore the viewing transformation

    // Second electron orbit
    glPushMatrix();
        glColor3ub(0xff, 0xcc, 0x00); // Orange electron 2
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // Rotate around Z axis
        glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
        glTranslatef(-120.0f, 0.0f, 0.0f); // Translate out FROM ORIGIN TO DISTANCE!
        glutSolidSphere(6.0f, 15, 15);
    glPopMatrix();

    // Third electron orbit
    glPushMatrix();
        glColor3ub(0xff, 0xcc, 0x99); // Peach electron 3
        glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // Rotate around Z axis
        glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 120.0f); // Translate out FROM ORIGIN TO DISTANCE!
        glutSolidSphere(6.0f, 15, 15);
    glPopMatrix(); 

    // Increment the angle of revolution
    fElect1 += 10.0f;
    if(fElect1 > 360.0f)
        fElect1 = 0.0f;
    
    // Show the image
    glutSwapBuffers();
} 

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLatom");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(100, TimerFunction, 1); // 10 times per second
/*
    glutCreateMenu(MenuCallback);
    glutAddMenuEntry("Culling (disabled)", 1);
    glutAddMenuEntry("Depth (disabled)", 2);
    glutAddMenuEntry("Outline (disabled)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMenuStatusFunc(ProcessMenuStatus);
*/
    SetupRC();

    RenderScene(); // initial rendering after start-up

    glutMainLoop();

    return 0;
}
