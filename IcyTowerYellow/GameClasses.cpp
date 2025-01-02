#include "GameClasses.h"
#include "GameClasses.h"
#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

// Platform implementation
const float Platform::HEIGHT = 5.0f;
const float Platform::PEBBLE_SPACING_TOLERANCE = 2.0f;  // Adjust as needed

void Platform::GeneratePebbles() {
    // Calculate number of pebbles based on width/height ratio
    numPebbles = static_cast<int>(width / HEIGHT);
    pebbles.clear();

    // Calculate base spacing between pebbles
    float baseSpacing = width / (numPebbles + 1); 

    for (int i = 0; i < numPebbles; i++) {
        Pebble pebble;

        // Calculate x position with spacing and random variation
        float baseX = baseSpacing * (i + 1);
        float xVariation = RandomFloat(-PEBBLE_SPACING_TOLERANCE, PEBBLE_SPACING_TOLERANCE);
        pebble.xPos = std::min(std::max(baseX + xVariation, HEIGHT / 3), width - HEIGHT / 3);

        // Y position remains between HEIGHT/3 and 2*HEIGHT/3
        pebble.yPos = RandomFloat(HEIGHT / 3, 2 * HEIGHT / 3);

        // Generate random number of points (5-8)
        int numPoints = RandomInt(5, 8);

        // Generate points with random r and theta
        for (int j = 0; j < numPoints; j++) {
            PebblePoint point;
            point.theta = RandomFloat(0, 360) * M_PI / 180.0f;
            point.r = RandomFloat(HEIGHT / 5, HEIGHT / 3);
            pebble.points.push_back(point);
        }

        // Sort points by theta
        std::sort(pebble.points.begin(), pebble.points.end(),
            [](const PebblePoint& a, const PebblePoint& b) {
                return a.theta < b.theta;
            });

        pebbles.push_back(pebble);
    }
}

float Platform::RandomFloat(float min, float max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int Platform::RandomInt(int min, int max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

Platform::Platform(float x, float y, float w, float speed)
    : beginX(x), yCoord(y), width(w), horizontalSpeed(speed), visited(false)
{
    GeneratePebbles();
}

void Platform::DrawPebble(const Pebble& pebble) const {
    glBegin(GL_POLYGON);
    for (const auto& point : pebble.points) {
        float x = pebble.xPos + point.r * cos(point.theta);
        float y = pebble.yPos + point.r * sin(point.theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void Platform::Draw() const {
    // Draw main platform
    glColor3f(0.2f, 0.6f, 0.8f);  // Green color
    glBegin(GL_QUADS);
    glVertex2f(beginX, yCoord);
    glVertex2f(beginX + width, yCoord);
    glVertex2f(beginX + width, yCoord - HEIGHT);
    glVertex2f(beginX, yCoord - HEIGHT);
    glEnd();

    if (visited) {
        glColor3f(0.2f, 0.8f, 0.6f);  // Green color
        glBegin(GL_QUADS);
        glVertex2f(beginX, yCoord);
        glVertex2f(beginX + HEIGHT / 3, yCoord);
        glVertex2f(beginX + HEIGHT / 3, yCoord - HEIGHT);
        glVertex2f(beginX, yCoord - HEIGHT);

        glVertex2f(beginX + width - HEIGHT / 3, yCoord);
        glVertex2f(beginX + width, yCoord);
        glVertex2f(beginX + width, yCoord - HEIGHT);
        glVertex2f(beginX + width - HEIGHT / 3, yCoord - HEIGHT);
        glEnd();
    }

    // Draw pebbles
    glPushMatrix();
    glTranslatef(beginX, yCoord - HEIGHT, 0);  // Transform to platform's bottom-left corner

    glColor3f(0.6f, 0.7f, 0.9f);  // Gray color for pebbles
    for (const auto& pebble : pebbles) {
        DrawPebble(pebble);
    }

    glPopMatrix();
}

// Player implementation
const float Player::GRAVITY = -300.0f;
const float Player::HORIZONTAL_SPEED_DECAY = 0.98f;
const float Player::MAX_HORIZONTAL_SPEED = 100.0f;
const float Player::INITIAL_JUMP_SPEED = 170.0f;
const float Player::RADIUS = 5.0f;

Player::Player(float x, float y)
    : xCoord(x), yCoord(y), horizontalSpeed(0), playerVerticalSpeed(0), horizontalTargetSpeed(0), underControl(false), readyToJump(false),
    state(PlayerState::ON_PLATFORM), currentPlatform(nullptr), score(0) {}

void Player::Draw() {
    // Draw main circle
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xCoord, yCoord);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(xCoord + cos(angle) * RADIUS,
            yCoord + sin(angle) * RADIUS);
    }
    glEnd();

    // Calculate normalized speed vector for pupil position
    float speedMagnitude = sqrt(horizontalSpeed * horizontalSpeed +
        playerVerticalSpeed * playerVerticalSpeed);
    float normalizedX = speedMagnitude > 0 ? horizontalSpeed / speedMagnitude : 0;
    float normalizedY = speedMagnitude > 0 ? playerVerticalSpeed / speedMagnitude : 0;

    // Draw eyes and pupils
    float eyeRadius = RADIUS * 0.3f;
    float pupilRadius = eyeRadius * 0.4f;
    float eyeOffset = RADIUS * 0.4f;

    // Left eye
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    float leftEyeX = xCoord - eyeOffset;
    float eyeY = yCoord + eyeOffset;
    glVertex2f(leftEyeX, eyeY);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(leftEyeX + cos(angle) * eyeRadius,
            eyeY + sin(angle) * eyeRadius);
    }
    glEnd();

    // Right eye
    glBegin(GL_TRIANGLE_FAN);
    float rightEyeX = xCoord + eyeOffset;
    glVertex2f(rightEyeX, eyeY);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(rightEyeX + cos(angle) * eyeRadius,
            eyeY + sin(angle) * eyeRadius);
    }
    glEnd();

    // Pupils
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    float leftPupilX = leftEyeX + normalizedX * (eyeRadius - pupilRadius);
    float pupilY = eyeY + normalizedY * (eyeRadius - pupilRadius);
    glVertex2f(leftPupilX, pupilY);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(leftPupilX + cos(angle) * pupilRadius,
            pupilY + sin(angle) * pupilRadius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    float rightPupilX = rightEyeX + normalizedX * (eyeRadius - pupilRadius);
    glVertex2f(rightPupilX, pupilY);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(rightPupilX + cos(angle) * pupilRadius,
            pupilY + sin(angle) * pupilRadius);
    }
    glEnd();
}

// Wall Pebbles Implementation
const float WallPebbles::WALL_WIDTH = 10.0f;  // Width of the walls (-10 to 0 and 100 to 110)
const float WallPebbles::PEBBLE_SPACING_TOLERANCE = 2.0f;

float WallPebbles::RandomFloat(float min, float max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int WallPebbles::RandomInt(int min, int max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

WallPebble WallPebbles::GenerateNewPebble(float yPos) const {
    WallPebble pebble;
    pebble.yPos = yPos;

    // Generate random number of points (5-8)
    int numPoints = RandomInt(5, 8);

    // Generate points with random r and theta
    for (int j = 0; j < numPoints; j++) {
        PebblePoint point;
        point.theta = RandomFloat(0, 360) * M_PI / 180.0f;
        point.r = RandomFloat(WALL_WIDTH / 5, WALL_WIDTH / 3);
        pebble.points.push_back(point);
    }

    // Sort points by theta
    std::sort(pebble.points.begin(), pebble.points.end(),
        [](const PebblePoint& a, const PebblePoint& b) {
            return a.theta < b.theta;
        });

    return pebble;
}

WallPebbles::WallPebbles() {
    // Calculate number of pebbles based on height
    int numPebbles = static_cast<int>((160 + 10) / WALL_WIDTH);  // Total height / wall width
    float baseSpacing = (160 + 10) / (numPebbles + 1);

    // Generate initial pebbles for both walls
    for (int i = 0; i < numPebbles; i++) {
        float baseY = -10 + baseSpacing * (i + 1);
        float yVariation = RandomFloat(-PEBBLE_SPACING_TOLERANCE, PEBBLE_SPACING_TOLERANCE);

        // Left wall pebbles
        leftWallPebbles.push_back(GenerateNewPebble(baseY + yVariation));

        // Right wall pebbles
        rightWallPebbles.push_back(GenerateNewPebble(baseY + yVariation));
    }
}

void WallPebbles::Update(float deltaTime, float gameSpeed) {
    // Move pebbles down with game speed
    for (auto& pebble : leftWallPebbles) {
        pebble.yPos -= gameSpeed * deltaTime;
    }
    for (auto& pebble : rightWallPebbles) {
        pebble.yPos -= gameSpeed * deltaTime;
    }

    // Check and replace pebbles that went below -10
    auto replacePebble = [this](WallPebble& pebble) {
        if (pebble.yPos < -10) {
            pebble = GenerateNewPebble(160 + RandomFloat(-PEBBLE_SPACING_TOLERANCE, PEBBLE_SPACING_TOLERANCE));
            return true;
        }
        return false;
        };

    for (auto& pebble : leftWallPebbles) {
        replacePebble(pebble);
    }
    for (auto& pebble : rightWallPebbles) {
        replacePebble(pebble);
    }
}

void WallPebbles::Draw() const {
    // Draw left wall pebbles
    glPushMatrix();
    glTranslatef(-10, 0, 0);  // Transform to left wall
    glColor3f(0.4f, 0.4f, 0.4f);  // Slightly darker than the walls

    for (const auto& pebble : leftWallPebbles) {
        glBegin(GL_POLYGON);
        for (const auto& point : pebble.points) {
            float x = WALL_WIDTH / 2 + point.r * cos(point.theta);
            float y = pebble.yPos + point.r * sin(point.theta);
            glVertex2f(x, y);
        }
        glEnd();
    }
    glPopMatrix();

    // Draw right wall pebbles
    glPushMatrix();
    glTranslatef(100, 0, 0);  // Transform to right wall

    for (const auto& pebble : rightWallPebbles) {
        glBegin(GL_POLYGON);
        for (const auto& point : pebble.points) {
            float x = WALL_WIDTH / 2 + point.r * cos(point.theta);
            float y = pebble.yPos + point.r * sin(point.theta);
            glVertex2f(x, y);
        }
        glEnd();
    }
    glPopMatrix();
}

// Game implementation
const float Game::MAX_PLATFORM_WIDTH = 40.0f;
const float Game::MIN_PLATFORM_WIDTH = 20.0f;
const float Game::MAX_PLATFORM_SPEED = 10.0f;

Game* Game::instance = nullptr;

float Game::RandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Game::Game() : player(50.0f, Player::RADIUS + Platform::HEIGHT), gameVerticalSpeed(0) {
    // Create initial platforms
    Platform base = Platform(0, Platform::HEIGHT, 100, 0);
    base.visited = true;
    platforms.push_back(base);
    player.currentPlatform = &base;
    float spacing = (140.0f - 30.0f) / (PLATFORMS_NUMBER - 1);
    for (int i = 0; i < PLATFORMS_NUMBER; i++) {
        CreatePlatform(30.0f + i * spacing);
    }
}

void Game::CreatePlatform(float yCoord) {
    float x = RandomFloat(0, 100 - MAX_PLATFORM_WIDTH);
    float width = RandomFloat(MIN_PLATFORM_WIDTH, MAX_PLATFORM_WIDTH);
    float speed = RandomFloat(-MAX_PLATFORM_SPEED, MAX_PLATFORM_SPEED);
    platforms.emplace_back(x, yCoord, width, speed);
}

void Game::Draw() {
    // Draw background
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.7f, 1.0f);  // Light blue at bottom
    glVertex2f(-10.0f, 0.0f);     // Bottom left
    glVertex2f(110.0f, 0.0f);     // Bottom right

    glColor3f(0.0f, 0.4f, 0.8f);  // Darker blue at top
    glVertex2f(110.0f, 150.0f);   // Top right
    glVertex2f(-10.0f, 150.0f);   // Top left
    glEnd();

    // Draw left wall
    glColor3f(0.5f, 0.5f, 0.5f);  // Gray color for walls
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 0.0f);    // Bottom-left
    glVertex2f(0.0f, 0.0f);      // Bottom-right
    glVertex2f(0.0f, 150.0f);    // Top-right
    glVertex2f(-10.0f, 150.0f);  // Top-left
    glEnd();

    // Draw right wall
    glBegin(GL_QUADS);
    glVertex2f(100.0f, 0.0f);    // Bottom-left
    glVertex2f(110.0f, 0.0f);    // Bottom-right
    glVertex2f(110.0f, 150.0f);  // Top-right
    glVertex2f(100.0f, 150.0f);  // Top-left
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);  // White color for text
    glRasterPos2f(2.0f, 145.0f);  // Position near top-left
    std::string scoreText = "Score: " + std::to_string(player.score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    wallPebbles.Draw();

    for (auto& platform : platforms) {
        platform.Draw();
    }
    player.Draw();
}

const float Player::SPEED_INTERPOLATION_FACTOR = 0.03f;  // Adjust as needed
const float Player::PLATFORM_COLLISION_TOLERANCE_SPEED_FACTOR = 0.01f;

void Game::Update(float deltaTime) {
    // Update player horizontal speed (weighted average)
    player.horizontalSpeed = player.horizontalSpeed * (1 - Player::SPEED_INTERPOLATION_FACTOR) +
        player.horizontalTargetSpeed * Player::SPEED_INTERPOLATION_FACTOR;

    // State-specific updates
    switch (player.state) {
    case PlayerState::ON_PLATFORM:
        if (!player.underControl) {
            player.horizontalTargetSpeed = player.currentPlatform->horizontalSpeed;
        }

        // Check if player is still on platform
        if (player.xCoord < player.currentPlatform->beginX ||
            player.xCoord > player.currentPlatform->beginX + player.currentPlatform->width) {
            player.state = PlayerState::JUMPING;
            player.currentPlatform = nullptr;
        }

        if (player.readyToJump) {
            Sound::PlayJump();
            player.playerVerticalSpeed = Player::INITIAL_JUMP_SPEED;
            player.state = PlayerState::JUMPING;
            player.currentPlatform = nullptr;
        }
        break;

    case PlayerState::JUMPING:
        player.playerVerticalSpeed += Player::GRAVITY * deltaTime;

        // Check platform collisions
        for (auto& platform : platforms) {
            if (player.xCoord >= platform.beginX &&
                player.xCoord <= platform.beginX + platform.width) {

                float platformCollisionY = platform.yCoord + Player::RADIUS;
                if (std::abs(player.yCoord - platformCollisionY) <= std::abs(player.playerVerticalSpeed) * Player::PLATFORM_COLLISION_TOLERANCE_SPEED_FACTOR &&
                    player.playerVerticalSpeed < 0) {

                    player.state = PlayerState::ON_PLATFORM;
                    player.currentPlatform = &platform;
                    player.playerVerticalSpeed = 0;
                    player.yCoord = platformCollisionY;

                    if (!platform.visited) {
                        player.score += 10;
                        platform.visited = true;
                    }
                    break;
                }
            }
        }

        player.yCoord += player.playerVerticalSpeed * deltaTime;
        break;

    case PlayerState::DEAD:
        return;
    }

    // Wall collisions
    if (player.xCoord < Player::RADIUS) {
        player.xCoord = Player::RADIUS;
        player.horizontalSpeed *= -1;
    }
    else if (player.xCoord > 100 - Player::RADIUS) {
        player.xCoord = 100 - Player::RADIUS;
        player.horizontalSpeed *= -1;
    }

    // Calculate game vertical speed
    if (player.yCoord <= 70) {
        gameVerticalSpeed = std::min((float)player.score / 20, Player::INITIAL_JUMP_SPEED / 10);
    }
    else {
        float t = (player.yCoord - 70) / 80;  // 70 to 150 normalized to 0 to 1
        gameVerticalSpeed = (Player::INITIAL_JUMP_SPEED / 10) * (1 - t) +
            (Player::INITIAL_JUMP_SPEED /5) * t;
    }

    // Update positions
    for (auto& platform : platforms) {
        platform.beginX += platform.horizontalSpeed * deltaTime;
        if (platform.beginX < 0) {
            platform.beginX = 0;
            platform.horizontalSpeed *= -1;
        }
        else if (platform.beginX > 100 - platform.width) {
            platform.beginX = 100 - platform.width;
            platform.horizontalSpeed *= -1;
        }
        platform.yCoord -= gameVerticalSpeed * deltaTime;
    }

    player.xCoord += player.horizontalSpeed * deltaTime;
    player.yCoord -= gameVerticalSpeed * deltaTime;

    wallPebbles.Update(deltaTime, gameVerticalSpeed);

    // Remove platforms below -10 and create new ones at 160
    while (!platforms.empty() && platforms.front().yCoord < -10) {
        platforms.erase(platforms.begin());
        CreatePlatform(160);
    }

    // Check for game over
    if (player.yCoord < -Player::RADIUS) {
        Sound::StopBackgroundMusic();
        Sound::PlayDeath();
        player.state = PlayerState::DEAD;
    }
}

bool Sound::isMusicPlaying = false;

void Sound::PlayEffect(const wchar_t* soundFile) {
    ::PlaySound(soundFile, NULL, SND_FILENAME | SND_ASYNC);  // Using scope resolution operator
}

void Sound::PlayJump() {
    PlayEffect(L"sounds/jump.wav");
}

void Sound::PlayScore() {
    PlayEffect(L"sounds/score.wav");
}

void Sound::PlayDeath() {
    PlayEffect(L"sounds/death.wav");
}

void Sound::PlayBackgroundMusic() {
    // Close any existing music
    ::mciSendString(L"close music", NULL, 0, NULL);

    // Open and play the music file
    ::mciSendString(L"open \"sounds/background.mp3\" type mpegvideo alias music", NULL, 0, NULL);
    ::mciSendString(L"play music repeat", NULL, 0, NULL);
    isMusicPlaying = true;
}

void Sound::StopBackgroundMusic() {
    ::mciSendString(L"close music", NULL, 0, NULL);
    isMusicPlaying = false;
}

void Sound::PauseBackgroundMusic() {
    if (isMusicPlaying) {
        ::mciSendString(L"pause music", NULL, 0, NULL);
    }
}

void Sound::ResumeBackgroundMusic() {
    if (isMusicPlaying) {
        ::mciSendString(L"resume music", NULL, 0, NULL);
    }
}