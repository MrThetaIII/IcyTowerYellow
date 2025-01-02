#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <windows.h>
#include "GameClasses.h"
#include "Controls.h"

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

// Window and coordinate system constants
const float WORLD_LEFT = -10.0f;
const float WORLD_RIGHT = 110.0f;
const float WORLD_BOTTOM = 0.0f;
const float WORLD_TOP = 150.0f;

// Window dimensions and aspect ratio
const float ASPECT_RATIO_WIDTH = 12.0f;
const float ASPECT_RATIO_HEIGHT = 15.0f;
const int INITIAL_WINDOW_WIDTH = 660;
const int INITIAL_WINDOW_HEIGHT = 825;

// Refresh rate settings
const int FRAME_RATE = 60;
const int FRAME_TIME = 1000 / FRAME_RATE;

// Time tracking
float deltaTime = 0.0f;
int lastFrameTime = 0;

void reshape(int width, int height) {
    float aspectRatio = ASPECT_RATIO_WIDTH / ASPECT_RATIO_HEIGHT;
    int newWidth = width;
    int newHeight = width * (ASPECT_RATIO_HEIGHT / ASPECT_RATIO_WIDTH);

    if (newHeight > height) {
        newHeight = height;
        newWidth = height * (ASPECT_RATIO_WIDTH / ASPECT_RATIO_HEIGHT);
    }

    int viewportX = (width - newWidth) / 2;
    int viewportY = (height - newHeight) / 2;

    glViewport(viewportX, viewportY, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(WORLD_LEFT, WORLD_RIGHT, WORLD_BOTTOM, WORLD_TOP, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastFrameTime) / 1000.0f;
    lastFrameTime = currentTime;

    if (Game::instance && Game::instance->player.state != PlayerState::DEAD) {
        Game::instance->Update(deltaTime);
    }
    else {
        // Draw "Press Enter to Start" message when no game is running
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(33.0f, 75.0f);
        const char* message = "Press An Arrow to Start";
        for (const char* c = message; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

        glutSwapBuffers();
    }

    glutPostRedisplay();
}

void display() {
    if (Game::instance && Game::instance->player.state != PlayerState::DEAD) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(0.0f, 0.0f, 0.0f);

        Game::instance->Draw();

        glutSwapBuffers();
    }
}

void timer(int value) {
    update();
    glutTimerFunc(FRAME_TIME, timer, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

void cleanup() {
    delete Game::instance;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    glutCreateWindow("Icy Tower Yellow");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(FRAME_TIME, timer, 0);

    glutSpecialFunc(Controls::SpecialFunc);
    glutKeyboardUpFunc(Controls::KeyboardUpFunc);
    glutSpecialUpFunc(Controls::SpecialUpFunc);

    glutMainLoop();

    atexit(cleanup);
    return 0;
}