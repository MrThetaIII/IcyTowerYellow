#ifndef GAME_CLASSES_H
#define GAME_CLASSES_H

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Forward declarations
class Platform;
class Player;
class Game;
class Controls;

// Platform class
struct PebblePoint {
    float r;
    float theta;
};

struct Pebble {
    float xPos;
    float yPos;
    std::vector<PebblePoint> points;
};

class Platform {
public:
    static const float HEIGHT;
    static const float PEBBLE_SPACING_TOLERANCE;
    float yCoord;
    float beginX;
    float width;
    float horizontalSpeed;
    bool visited;

    int numPebbles;
    std::vector<Pebble> pebbles;

    Platform(float x, float y, float w, float speed);
    void Draw() const;
private:
    void GeneratePebbles();
    void DrawPebble(const Pebble& pebble) const;
    float RandomFloat(float min, float max) const;
    int RandomInt(int min, int max) const;
};

// Player class
enum class PlayerState {
    JUMPING,
    ON_PLATFORM,
    DEAD
};

class Player {
public:
    static const float GRAVITY;
    static const float HORIZONTAL_SPEED_DECAY;
    static const float MAX_HORIZONTAL_SPEED;
    static const float INITIAL_JUMP_SPEED;
    static const float RADIUS;

    float horizontalSpeed;
    float horizontalTargetSpeed;
    float playerVerticalSpeed;
    float xCoord;
    float yCoord;
    bool underControl;
    bool readyToJump;
    PlayerState state;
    Platform* currentPlatform;
    int score;

    static const float SPEED_INTERPOLATION_FACTOR;  // For weighted average
    static const float PLATFORM_COLLISION_TOLERANCE_SPEED_FACTOR;

    Player(float x, float y);
    void Draw();
};

struct WallPebble {
    float yPos;
    std::vector<PebblePoint> points;
};

class WallPebbles {
public:
    static const float WALL_WIDTH;
    static const float PEBBLE_SPACING_TOLERANCE;

    std::vector<WallPebble> leftWallPebbles;
    std::vector<WallPebble> rightWallPebbles;

    WallPebbles();
    void Update(float deltaTime, float gameSpeed);
    void Draw() const;

private:
    WallPebble GenerateNewPebble(float yPos) const;
    float RandomFloat(float min, float max) const;
    int RandomInt(int min, int max) const;
};

class Game {
public:
    static const int PLATFORMS_NUMBER = 5;
    static const float MAX_PLATFORM_WIDTH;
    static const float MIN_PLATFORM_WIDTH;
    static const float MAX_PLATFORM_SPEED;

    static Game* instance;  // Change to pointer

    WallPebbles wallPebbles;
    Player player;
    std::vector<Platform> platforms;
    float gameVerticalSpeed;

    Game();
    void CreatePlatform(float yCoord);
    void Draw();
    void Update(float deltaTime);

private:
    float RandomFloat(float min, float max);
};

class Sound {
public:
    static void PlayJump();
    static void PlayScore();
    static void PlayDeath();

    static void PlayBackgroundMusic();
    static void StopBackgroundMusic();
    static void PauseBackgroundMusic();
    static void ResumeBackgroundMusic();

private:
    static void PlayEffect(const wchar_t* soundFile);
    static bool isMusicPlaying;
};

#endif