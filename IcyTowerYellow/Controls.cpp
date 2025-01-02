#include "Controls.h"
#include "Controls.h"
#include <GL/freeglut.h>
#include <iostream>

void Controls::SpecialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        if (!Game::instance || Game::instance->player.state == PlayerState::DEAD) { Game::instance = new Game(); Sound::PlayBackgroundMusic(); ;  return; }
        Game::instance->player.horizontalTargetSpeed = -Player::MAX_HORIZONTAL_SPEED;
        Game::instance->player.underControl = true;
        break;
    case GLUT_KEY_RIGHT:
        if (!Game::instance || Game::instance->player.state == PlayerState::DEAD) { Game::instance = new Game(); Sound::PlayBackgroundMusic(); ;  return; }
        Game::instance->player.horizontalTargetSpeed = Player::MAX_HORIZONTAL_SPEED;
        Game::instance->player.underControl = true;
        break;
    case GLUT_KEY_UP:
        if (!Game::instance || Game::instance->player.state == PlayerState::DEAD) { Game::instance = new Game(); Sound::PlayBackgroundMusic(); ;  return; }
        Game::instance->player.readyToJump = true;
        break;
    case GLUT_KEY_DOWN:
        if (!Game::instance || Game::instance->player.state == PlayerState::DEAD) { Game::instance = new Game(); Sound::PlayBackgroundMusic(); ;  return; }
        break;
    }
}

void Controls::KeyboardUpFunc(unsigned char key, int x, int y) {
    // Handle key releases if needed
    switch (key) {
    case ' ':  // Space key
        // Add any space key release handling if needed
        break;
    }
}

void Controls::SpecialUpFunc(int key, int x, int y) {
    if (!Game::instance) return;  // Don't process if game hasn't started

    Game::instance->player.underControl = false;
    switch (key) {
    case GLUT_KEY_LEFT:
        if (Game::instance->player.horizontalTargetSpeed < 0) {
            Game::instance->player.horizontalTargetSpeed = 0;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (Game::instance->player.horizontalTargetSpeed > 0) {
            Game::instance->player.horizontalTargetSpeed = 0;
        }
        break;
    case GLUT_KEY_UP:
        Game::instance->player.readyToJump = false;
        break;
    }
}