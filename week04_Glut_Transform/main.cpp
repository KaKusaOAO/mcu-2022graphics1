#include <cstdio>
#include <GL/glut.h>

float angle = 0, desiredAngle = 0, oldX = 0;
int width = 1, height = 1;

float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float Abs(float f) {
    return f > 0 ? f : -f;
}

void Render() {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ar = (float)width / (float)height;
    glFrustum(-ar, ar, -1, 1, 1, 2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslated(0, 0, -2);
    angle = Lerp(angle, desiredAngle, 0.075);
    if (Abs(desiredAngle - angle) < 0.05) {
        angle = desiredAngle;
    }
    glRotated(angle, 0, 0, 1.0);
    glutSolidTeapot(1);

    glutSwapBuffers();
}

void MouseMotion(int x, int y) {
    desiredAngle += x - oldX;
    oldX = x;
}

void Resized(int w, int h) {
    width = w;
    height = h;
}

void Idle() {
    glutPostRedisplay();
}

void MouseUpDown(int button, int state, int x, int y) {
    oldX = x;
}

void KeyDown(unsigned char c, int a, int b) {
    if(c == 'q') desiredAngle += 180;
    else if(c == 'w') desiredAngle -= 180;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Transform!?");
    glutDisplayFunc(Render);
    glutMotionFunc(MouseMotion);
    glutReshapeFunc(Resized);
    glutReshapeWindow(512, 512);
    glutMouseFunc(MouseUpDown);
    glutKeyboardFunc(KeyDown);
    glutIdleFunc(Idle);
    glutMainLoop();
}
