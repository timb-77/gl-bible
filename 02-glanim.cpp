#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>

// Initial square position and size
GLfloat X1 = 0.0f;
GLfloat Y1 = 0.0f;
GLfloat rsize = 25;

// Step size in x and y directions
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Keep track of windows changing width and height
GLfloat windowWidth = 800;
GLfloat windowHeight = 600;

GLfloat OrthoLeft, OrthoRight, OrthoTop, OrthoBottom;

void DrawHUD()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glRasterPos3f(OrthoLeft, OrthoBottom, 0.0f);

#define BUF_SIZE 128
    char buf[BUF_SIZE];
    memset(buf, 0x00, BUF_SIZE * sizeof(buf[0]));

    snprintf(buf, BUF_SIZE, "X1: %3.3f, Y1: %3.3f", X1, Y1);

    for(char* p = buf; *p; ++p)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);
    }
}

void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set current drawing color to red
    glColor3f(1.0f, 0.0f, 0.0f);

    // Draw a filled rectangle with current color
    glRectf(X1, Y1, X1 + rsize, Y1 - rsize);

    // Debug print
    DrawHUD();

    // Flush drawing commands and swap
    glutSwapBuffers();
}

/*****************************************************************************
 * Called by GLUT library when idle (window not being resized or moved)
 */
void TimerFunction(int value)
{

    static bool isReverseX = false;
    static bool isReverseY = false;

    // Reverse direction when you reach left or right edge
    if (X1 > OrthoRight - rsize || X1 < OrthoLeft)
    {
        isReverseX = true;
    } else {
        isReverseX = false;
    }

    // Reverse direction when you reach top or bottom edge
    if (Y1 > OrthoTop || Y1 < OrthoBottom + rsize)
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
    X1 += xstep;
    Y1 += ystep;

    // Check bounds. This is in case the window is made smaller while the 
    // rectangle is bouncing and the rectangle suddenly finds itself
    // outside the new clipping volume 
    if(X1 > (windowWidth - rsize + xstep))
        X1 = windowWidth - rsize - 1;
    else if (X1 < -(windowWidth + xstep))
        X1 = -windowWidth -1;

    if(Y1 > (windowHeight + ystep))
        Y1 = windowHeight - 1;
    else if (Y1 < -(windowHeight - rsize + ystep))
        Y1 = -windowHeight + rsize -1;

    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLBounce");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    SetupRC();

    glutMainLoop();

    return 0;
}
