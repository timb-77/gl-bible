#include <math.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>

#define GL_PI 3.14159265f

GLfloat xRot = 0.0f, yRot = 0.0f;

void SetupRC(void)
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set drawing color to green
    glColor3f(0.0f, 1.0f, 0.0f);
}

void RenderScene(void)
{
    GLfloat x, y, z, angle;

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT);

    // Save matrix state and do the rotation
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Call only once for all remaining points
    glBegin(GL_POINTS);
        z = -50.0f;
        for (angle = 0.0f; angle <= (2.0f * GL_PI) * 3.0f; angle += 0.1f)
        {
            x = 50.0f * sin(angle);
            y = 50.0f * cos(angle);
 
            // Specify the point and move the Z value up a little
            glVertex3f(x, y, z);
            z += 0.5f;
        }
    // Done drawing points
    glEnd();

    // Restore transformation
    glPopMatrix();

    // Flush drawing commands
    glutSwapBuffers();
}

GLfloat OrthoLeft, OrthoRight, OrthoBottom, OrthoTop;
void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
    {
        OrthoLeft   = -100.0;
        OrthoRight  =  100.0;
        OrthoBottom = -100.0 / aspectRatio;
        OrthoTop    =  100.0 / aspectRatio;
    }
    else
    {
        OrthoLeft   = -100.0 * aspectRatio;
        OrthoRight  =  100.0 * aspectRatio;
        OrthoBottom = -100.0;
        OrthoTop    =  100.0;
    }
    glOrtho(OrthoLeft, OrthoRight, OrthoBottom, OrthoTop, 100.0, -100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLfloat rot_inc = GL_PI / 72.0f;
//GLfloat rot_inc = GL_PI / 36.0f;
void TimerFunction(int value)
{
    xRot += -rot_inc * 180.0f / GL_PI;
    yRot += rot_inc * 180.0f / GL_PI;

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLPoints");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    SetupRC();

    glutMainLoop();

    return 0;
}
