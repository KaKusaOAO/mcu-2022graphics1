#include <GL/glut.h>
#include <cmath>
#include <cstdio>

int window;
float mouseX = 0, mouseY = 0;
int wWidth = 0, wHeight = 0;

float absf(float n) {
    return n > 0 ? n : -n;
}

void KHueNormalize(float* h) {
    float val = *h;

    if(val < 0) val += 360;
    while(val >= 360) {
        val -= 360;
    }

    *h = val;
}

// Hue: 0~360deg
// Saturation: 0~1
// Value: 0~1
void KColorHSL(float h, float s, float l) {
    float c = 1 - absf(2 * l - 1) * s;
    float x = c * (1 - absf(fmod(h / 60.0, 2.0) - 1));
    float m = l - c / 2;

    float r, g, b;
    if(h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if(h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if(h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if(h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if(h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    glColor3f(r + m, g + m, b + m);
}

void KHexColor(int hex) {
    float r = (float)((hex >> 16) & 0xff) / 255.0;
    float g = (float)((hex >> 8) & 0xff) / 255.0;
    float b = (float)(hex & 0xff) / 255.0;
    glColor3f(r, g, b);
}

void resized(int width, int height) {
    float ar = (float)width / (float)height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1, 1, 2, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float hue = 0;

void render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 2, -8);
    glRotatef(hue, 0, 1, 0);

    // Rainbow color
    hue += 1.0;
    KHueNormalize(&hue);
    KColorHSL(hue, 1, 0.5);

    // Beautiful teapot
    glutSolidTeapot(.1);

    glutSwapBuffers();
}

void mouseUpdate(int button, int state, int x, int y) {
    mouseX = x;
    mouseY = y;
}

void keyboardUpdate(unsigned char key, int a, int b) {

}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    window = glutCreateWindow("Water!");
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardUpdate);
    glutReshapeFunc(resized);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
