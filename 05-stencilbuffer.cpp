#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/gl.h>

#define GL_PI 3.1415f

GLfloat xRot = 0.0f, yRot = 0.0f;
GLfloat OrthoLeft, OrthoRight, OrthoTop, OrthoBottom;
GLfloat x = 0.0f, y= 0.0f;
GLfloat xstep = 1.0f, ystep = 1.0f;
GLfloat rsize = 25.0f;
GLfloat windowWidth = 800.0f, windowHeight = 600.0f;

void SetupRC(void)
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set drawing color to green
    glColor3f(0.0f, 1.0f, 0.0f);
}

void RenderScene(void)
{
    GLdouble dRadius = 0.1; // Initial radius of the spiral
    GLdouble dAngle;


    // Clear blue window
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

    // Use 0 for clear stencil, enable stencil test
    glClearStencil(0.0f);
    glEnable(GL_STENCIL_TEST);

    // Clear color and stencil buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 
    // All drawing commands fail the stencil test, and are not
    // drawn, but increment the value in the stencil buffer
    glStencilFunc(GL_NEVER, 0x0, 0x0);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);

    // Spiral pattern will crate stencil pattern.
    // Draw the spiral pattern with white lines.
    // We make the lines white to demonstrate that the
    // stencil function prevents them from being drawn.
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_STRIP);
        // Plotting spiral circle line into the stencil buffer
        for(dAngle = 0.0; dAngle < 400.0; dAngle += 0.1)
        {
            glVertex2d(dRadius * cos(dAngle), dRadius * sin(dAngle));
            dRadius *= 1.002;
        }
    glEnd();
 
    // Now, allow drawing, except where the stencil pattern is 0x01,
    // and do not make any further changes to the stencil buffer
    glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(x, y, x + rsize, y - rsize);

    // Flush drawing commands
    glutSwapBuffers();
}

void TimerFunction(int value)
{
    static bool isReverseX = false;
    static bool isReverseY = false;

    // Reverse direction when you reach left or right edge
    if (x > OrthoRight - rsize || x < OrthoLeft)
    {
        isReverseX = true;
    } else {
        isReverseX = false;
    }

    // Reverse direction when you reach top or bottom edge
    if (y > OrthoTop || y < OrthoBottom + rsize)
    {
        isReverseY = true;
    } else {
        isReverseY = false;
    }


    if (isReverseX)
        xstep = -xstep;
    if (isReverseY)
        ystep = -ystep;

    // Actually move the square
    x += xstep;
    y += ystep;

    // Check bounds. This is in case the window is made smaller while the 
    // rectangle is bouncing and the rectangle suddenly finds itself
    // outside the new clipping volume 
    if(x > (windowWidth - rsize + xstep))
        x = windowWidth - rsize - 1;
    else if (x < -(windowWidth + xstep))
        x = -windowWidth -1;

    if(y > (windowHeight + ystep))
        y = windowHeight - 1;
    else if (y < -(windowHeight - rsize + ystep))
        y = -windowHeight + rsize -1;

    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void ChangeSize(GLsizei w, GLsizei h)
{
    GLfloat aspectRatio;

    if (h == 0)
    {
        h = 1;
    }

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
    {
        OrthoLeft = -100.0;
        OrthoRight = 100.0;
        OrthoBottom = -100.0 / aspectRatio;
        OrthoTop = 100.0 / aspectRatio;
    }
    else
    {
        OrthoLeft = -100.0 * aspectRatio;
        OrthoRight = 100.0 * aspectRatio;
        OrthoBottom = -100.0;
        OrthoTop = 100.0;
    }
    glOrtho(OrthoLeft, OrthoRight, OrthoBottom, OrthoTop, 1.0, -1.0);

    // Reset Model view matrix stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLStencilBuf");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    SetupRC();

    RenderScene(); // initial rendering after start-up

    glutMainLoop();

    return 0;
}
