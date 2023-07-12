#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/gl.h>

#define GL_PI 3.1415f

GLfloat xRot = 0.0f, yRot = 0.0f;
bool bCull = false, bDepth = false, bOutline = false;

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
    glFrontFace(GL_CW);
}

void RenderScene(void)
{
    GLfloat x, y, z, angle;
    int iPivot = 1; // Used to flag alternating colors

    // Clear the window (color) and the depth buffer 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Turn culling on if flag is set
    if(bCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    // Enable depth testing if flag is set
    if(bDepth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    // Draw the back side as a wireframe only, if flag is set
    if(bOutline)
        glPolygonMode(GL_BACK, GL_LINE);
    else
        glPolygonMode(GL_BACK, GL_FILL);

    // Save matrix state and do the rotation
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Begin a traingle fan
    glBegin(GL_TRIANGLE_FAN);
        // Pinnacle of cone is shared vertex for fan,
        // moved up the Z-axis
        // (to produce a cone instead of a circle)
        glVertex3f(0.0f, 0.0f, 75.0f);

        for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += (GL_PI/8.0f))
        {
            // Calculate X and Y position of the next vertex
            x = 50.0f * sin(angle);
            y = 50.0f * cos(angle);

            // Alternate color between red and green
            if((iPivot % 2) == 0)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(1.0f, 0.0f, 0.0f);

            iPivot++;
 
            // Specify the next for the triangle fan
            glVertex2f(x, y); // Z is automatically 0
        }
    // Done drawing fan for cone
    glEnd();


 
    // Begin a new triangle fan to cover the bottom
    glBegin(GL_TRIANGLE_FAN);

        // Center of fan is at the origin
        glVertex2f(0.0f, 0.0f);

        for(angle = 0.0f; angle <= (2.0f * GL_PI); angle += (GL_PI/8.0f))
        {
            // Calculate x and y position of the next vertex
            x = 50.0f * sin(angle);
            y = 50.0f * cos(angle);

            // Alternate color between red and green
            if((iPivot % 2) != 0)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(1.0f, 0.0f, 0.0f);

            iPivot++;

            glVertex2f(x,y);
        }
    glEnd(); // Done drawing the fan that covers the bottom
    
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
void TimerFunction(int value)
{
    //xRot += -rot_inc * 180.0f / GL_PI;
    yRot += rot_inc * 180.0f / GL_PI;

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void MenuCallback(int value)
{
    switch(value)
    {
        case 1: bCull = !bCull; break;
        case 2: bDepth = !bDepth; break;
        case 3: bOutline = !bOutline; break;
        default: break;
    }
}

void ProcessMenuStatus(int status, int x, int y)
{
    if(status != GLUT_MENU_IN_USE)
    {
        glutChangeToMenuEntry(1, bCull ? "Culling (enabled)": "Culling (disabled)", 1);
        glutChangeToMenuEntry(2, bDepth ? "Depth (enabled)": "Depth (disabled)", 2);
        glutChangeToMenuEntry(3, bOutline ? "Outline (enabled)": "Outline (disabled)", 3);
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLPoints");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1); // 33 ms

    glutCreateMenu(MenuCallback);
    glutAddMenuEntry("Culling (disabled)", 1);
    glutAddMenuEntry("Depth (disabled)", 2);
    glutAddMenuEntry("Outline (disabled)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMenuStatusFunc(ProcessMenuStatus);

    SetupRC();

    RenderScene(); // initial rendering after start-up

    glutMainLoop();

    return 0;
}
