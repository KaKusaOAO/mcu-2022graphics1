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
float desiredMx = 0, desiredMy = 0;
float scale = 1, desiredScale = 1;

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

    glPushMatrix();
        glTranslatef(0., 0., -5.);
        glRotatef(time, 1, sin(time/60.), 0);
        glScalef(scale, scale, scale);
        glutSolidTeapot(1.);
    glPopMatrix();


    glutSwapBuffers();
    time += 1;
}

void keyboard(unsigned char c, int a, int b) {
    exit(0);
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void motion(int x, int y) {
    desiredMx = x;
    desiredMy = y;
    mx = lerp(mx, desiredMx, 0.1);
    my = lerp(my, desiredMy, 0.1);

    float dx = x - oldX;
    float dy = y - oldY;
    desiredScale *= 1 + dx / (float)width;
    scale = lerp(scale, desiredScale, 0.01);

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
    glutCreateWindow("Light");
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
