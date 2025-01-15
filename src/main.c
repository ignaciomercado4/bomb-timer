#include "raylib.h"
#include <unistd.h>
#include "resource_dir.h"
#include <string.h>

#define MAX_INPUT_CHARS 3

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomb Timer");
    InitAudioDevice();
    SearchAndSetResourceDir("resources");

    // Load textures
    Texture2D bombLightOff = LoadTexture("sprites/bomb_light_off.png");
    Texture2D bombLightOn = LoadTexture("sprites/bomb_light_on.png");    
    Texture2D explosion = LoadTexture("sprites/explosion.png");    
    
    // Load sounds
    Sound initSound = LoadSound("audio/ok_lets_go.wav"); 
    Sound keyPressSound = LoadSound("audio/key_press.wav");
    Sound explosionSound = LoadSound("audio/explosion.wav");

    // Input screen variables
    char minutes[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    Rectangle textBox = { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 25, 200, 50 };
    bool mouseOnText = false;
    bool inputScreenActive = true;
    int framesCounter = 0;
    bool showError = false; 
    int errorTimer = 0;     

    // Init timer variables
    int timeInSeconds = 0;
    float timer = 1.0f; 
    char timeFormatted[50];
    bool timerStarted = false;

    // Init animation variables
    const int NUM_FRAMES_PER_LINE = 5;
    const int NUM_LINES = 5;
    float frameWidth = (float)(explosion.width/NUM_FRAMES_PER_LINE); 
    float frameHeight = (float)(explosion.height/NUM_LINES); 
    int currentFrame = 0;
    int currentLine = 0;
    int explosionFramesCounter = 0;
    Rectangle frameRec = { 0, 0, frameWidth, frameHeight };
    Vector2 explosionPos = {SCREEN_WIDTH/2 - frameRec.width/2, SCREEN_HEIGHT/2 - frameRec.height/2};
    bool explosionActive = false;
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (inputScreenActive) {
            mouseOnText = CheckCollisionPointRec(GetMousePosition(), textBox);

            if (mouseOnText) {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                int key = GetCharPressed();

                while (key > 0) {
                    if ((key >= 48) && (key <= 57) && (letterCount < MAX_INPUT_CHARS)) {
                        PlaySound(keyPressSound);
                        minutes[letterCount] = (char)key;
                        minutes[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    PlaySound(keyPressSound);
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    minutes[letterCount] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                    PlaySound(keyPressSound);
                }
            } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

            if (mouseOnText) framesCounter++;
            else framesCounter = 0;

            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                int inputMinutes = atoi(minutes);
                if (inputMinutes > 0) {
                    timeInSeconds = inputMinutes * 60;
                    inputScreenActive = false;
                    timerStarted = true;
                    showError = false;
                } else {
                    showError = true;
                    errorTimer = 120; 
                }
            }

            if (errorTimer > 0) errorTimer--;
            if (errorTimer == 0) showError = false;

        } else {
            // Timer logic
            if (timeInSeconds > 0) {
                timer -= GetFrameTime();
                if (timer <= 0) {
                    timeInSeconds--;
                    timer = 1.0f;
                }
            }

            // Bom animation logic
            if (explosionActive) {
                explosionFramesCounter++;

                if (explosionFramesCounter > 2) {
                    currentFrame++;

                    if (currentFrame >= NUM_FRAMES_PER_LINE) {
                        currentFrame = 0;
                        currentLine++;

                        if (currentLine >= NUM_LINES) {
                            currentLine = 0;
                            explosionActive = false;
                        }
                    }

                    explosionFramesCounter = 0;
                }
            }

            frameRec.x = frameWidth*currentFrame;
            frameRec.y = frameHeight*currentLine;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (inputScreenActive) {
                DrawText("Enter minutes (1-999):", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 60, 20, WHITE);
                DrawRectangleRec(textBox, LIGHTGRAY);
                DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, mouseOnText ? RED : DARKGRAY);
                DrawText(minutes, textBox.x + 5, textBox.y + 8, 40, MAROON);

                if (mouseOnText && (letterCount < MAX_INPUT_CHARS) && ((framesCounter/20)%2 == 0)) {
                    DrawText("_", textBox.x + 8 + MeasureText(minutes, 40), textBox.y + 12, 40, MAROON);
                }

                DrawText("Press ENTER to start", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 50, 20, WHITE);

                if (showError) {
                    DrawText("Please enter a number greater than 0", 
                            SCREEN_WIDTH/2 - 150, 
                            SCREEN_HEIGHT/2 + 80, 
                            20, RED);
                }

            } else {
                if (explosionActive) {
                    DrawTextureRec(explosion, frameRec, explosionPos, WHITE);
                    DrawText("TIME IS UP!", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 20, RED);
                }

                if (timeInSeconds <= 0) {
                    if (!explosionActive) {
                        explosionActive = true;
                        PlaySound(explosionSound);
                    }
                } else {
                    if (timerStarted) PlaySound(initSound);
                    timerStarted = false;
                    
                    if (timeInSeconds % 2 == 0) {
                        DrawTexture(bombLightOff, SCREEN_WIDTH/2 - bombLightOff.width/2, SCREEN_HEIGHT/2 - bombLightOff.height/2, WHITE);
                    } else {
                        DrawTexture(bombLightOn, SCREEN_WIDTH/2 - bombLightOff.width/2, SCREEN_HEIGHT/2 - bombLightOff.height/2, WHITE);     
                    }
                    
                    int minutes = timeInSeconds / 60;
                    int seconds = timeInSeconds % 60;
                    snprintf(timeFormatted, 50, "%02d:%02d", minutes, seconds);
                    
                    int textWidth = MeasureText(timeFormatted, 18);
                    DrawText(timeFormatted, 
                           (SCREEN_WIDTH/2 - bombLightOff.width/2) + 100 - (textWidth/2), 
                           (SCREEN_HEIGHT/2 - bombLightOff.height/2) + 70, 
                           18, BLACK);
                }
            }
        EndDrawing();
    }

    UnloadTexture(bombLightOff);
    UnloadTexture(bombLightOn);
    UnloadTexture(explosion);
    UnloadSound(initSound);
    UnloadSound(keyPressSound);
    UnloadSound(explosionSound);
    CloseAudioDevice();      
    CloseWindow();

    return 0;
}