# Icy Tower Clone - OpenGL

A simple Icy Tower clone implemented in C++ using legacy OpenGL, featuring a bouncing character navigating through platforms with various visual elements and physics.

## Features

- Smooth character movement with physics
- Randomly generated platforms with pebble decorations
- Scrolling background with gradient effect
- Sound effects and background music

## Controls

- **Any Arrow Key**: Start game
- **Left/Right Arrow Keys**: Move character
- **Up Arrow Key**: Jump (can be held pressed)

## Fun Things to Try to Code (Add your touch)

### Visual Tweaks
1. **Character Appearance**
   - Modify `Player::RADIUS` to change character size
   - Adjust eye positions and sizes in `Player::Draw()`

2. **Platform Aesthetics**
   - Change `Platform::HEIGHT` for different platform thickness
   - Modify pebble generation parameters for different textures

3. **Background Effects**
   - Add floating particles in the background
   - Create a day/night cycle by gradually changing colors

### Physics Modifications
1. **Jump Mechanics**
   - Adjust `Player::INITIAL_JUMP_SPEED` for higher/lower jumps
   - Modify `Player::GRAVITY` for different falling speeds
   - Add double jump capability
   - Implement variable jump height based on hold duration

2. **Movement Dynamics**
   - Change `Player::MAX_HORIZONTAL_SPEED` for faster/slower movement
   - Modify `HORIZONTAL_SPEED_DECAY` for different momentum feels
   - Implement wall sliding

3. **Platform Behavior**
   - Add bouncy / slippery platforms (multiply jump speed, speed decay)
   - Create breaking platforms
   - Add moving platforms with different patterns

### Game Mechanics
1. **Scoring System**
   - Change score increments (currently 10 per platform)
   - Add bonus points for specific heights
   - Implement combo system for consecutive platforms
   - Add collectible items

2. **Difficulty Progression**
   - Modify platform spacing as score increases
   - Change platform width/speed based on height
   - Add hazards or enemies
   - Implement different game modes

### Sound Effects
1. **Audio Feedback**
   - Add different jump sounds based on jump height
   - Create dynamic music based on height/score

### Multiplayer
1. **Multiplayer Mode**
   - Allow multiple players to share the same window
   - Allow multiple players in multiple adjacent windows
2. **Highest Score Tracking**
   - Add a scoreboard

## Tips for Modifications
- Most constants are defined at the top of each class
- Physics values can be found in the Player class
- Visual elements are primarily in Draw() methods
- Game logic is centralized in Game::Update()

Would you like me to expand on any of these sections or add more specific modification suggestions?
