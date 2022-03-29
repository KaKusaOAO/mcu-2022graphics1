#include <cstdio>
#include <cmath>
#include <GL/glut.h>

typedef struct {
    GLfloat ambient[4]; // = { 0., 0., 0., 1. };
    GLfloat diffuse[4]; // = { 1., 1. ,1., 1. };
    GLfloat specular[4]; // = { 1., 1., 1., 1. };
    GLfloat position[4]; // = { 2., 5., 5., 0. };
} LightData;

typedef struct {
    GLfloat ambient[4]; // = { 0., 0., 0., 1. };
    GLfloat diffuse[4]; // = { 1., 1. ,1., 1. };
    GLfloat specular[4]; // = { 1., 1., 1., 1. };
    GLfloat shininess[1]; // = 100.;
} Material;

int width = 0, height = 0;
float time = 0;
float mx = 0, my = 0, oldX, oldY;

float ox = 0, oy = 0, desiredOx = 0, desiredOy = 0;
float angle = 0, desiredAngle = 0;
float scale = 1, desiredScale = 1;

int mode = 1;

LightData light = {
    { 0., 0., 0., 1. },
    { 1., 1., 1., 1. },
    { 1., 1. ,1., 1. },
    { 2., 5., 5., 0. }
};

Material mat = {
    { 0.7, 0.7, 0.7, 1. },
    { 0.8, 0.8, 0.8, 1. },
    { 1., 1. ,1., 1. },
    { 100. }
};

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float max(float a, float b) {
    return a > b ? a : b;
}

float min(float a, float b) {
    return a > b ? b : a;
}

void resize(int w, int h) {
    width = w;
    height = h;
    float ar = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1, 1, 2, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0xff / 255., 0x11 / 255., 0x4f / 255.);

    scale = lerp(scale, desiredScale, 0.1);
    angle = lerp(angle, desiredAngle, 0.1);
    ox = lerp(ox, desiredOx, 0.1);
    oy = lerp(oy, desiredOy, 0.1);

    glPushMatrix();
        glTranslatef(ox, oy, -5.);
        glRotatef(angle, 0, 1, 0);
        glScalef(scale, scale, scale);
        glutSolidTeapot(1.);
    glPopMatrix();

    glutSwapBuffers();
    time += 1;
}

void keyboard(unsigned char c, int a, int b) {
    if (c == '1' || c == 'w' || c == 'W') mode = 1;
    else if (c == '2' || c == 'e' || c == 'E') mode = 2;
    else if (c == '3' || c == 'r' || c == 'R') mode = 3;
    else exit(0);
}

void motion(int x, int y) {
    float dx = x - oldX;
    float dy = y - oldY;

    switch (mode) {
    case 1:
        desiredOx += dx / 256;
        desiredOy -= dy / 256;
        break;
    case 2:
        desiredAngle += dx;
        break;
    case 3:
        desiredScale *= 1 + dx / (float)width;
        break;
    }

    desiredOx = max(-2, min(2, desiredOx));
    desiredOy = max(-2, min(2, desiredOy));
    desiredScale = max(0.5, min(desiredScale, 2));

    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    oldX = x;
    oldY = y;
    motion(x, y);
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Light + Transform + Mouse + Keyboard");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutReshapeWindow(512, 512);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light.position);

    glLightfv(GL_FRONT, GL_AMBIENT, mat.ambient);
    glLightfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glLightfv(GL_FRONT, GL_SPECULAR, mat.specular);
    glLightfv(GL_FRONT, GL_SHININESS, mat.shininess);

    glutMainLoop();
}
