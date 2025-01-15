#include "raylib.h"
#include <unistd.h>
#include "resource_dir.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    /*
    INITIALIZATION
    */
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

    // Init timer variables
    int timeInSeconds = -1;
    float timer = 1.0f; 
    char timeInSecondsStringified[50];

    // Init animation variables
    const int NUM_FRAMES_PER_LINE = 5;
    const int NUM_LINES = 5;
    float frameWidth = (float)(explosion.width/NUM_FRAMES_PER_LINE); 
    float frameHeight = (float)(explosion.height/NUM_LINES); 
    int currentFrame = 0;
    int currentLine = 0;
    int framesCounter = 0;
    Rectangle frameRec = { 0, 0, frameWidth, frameHeight };
    Vector2 explosionPos = {SCREEN_WIDTH/2 - frameRec.width/2, SCREEN_HEIGHT/2 - frameRec.height/2};
    bool explosionActive = true;
    
    // Game loop
    while (!WindowShouldClose()) {
        
        // Time countdown
        if (timeInSeconds > 0) {
            timer -= GetFrameTime();
            if (timer <= 0) {
                timeInSeconds--;
                timer = 1.0f;
            }
        }

        if (explosionActive) {
            framesCounter++;

            if (framesCounter > 2) {
                currentFrame++;

                if (currentFrame >= NUM_FRAMES_PER_LINE) {
                    currentFrame = 0;
                    currentLine++;

                    if (currentLine >= NUM_LINES) {
                        currentLine = 0;
                        explosionActive = false;
                    }
                }

                framesCounter = 0;
            }
        }

        frameRec.x = frameWidth*currentFrame;
        frameRec.y = frameHeight*currentLine;

        // Drawing
        BeginDrawing();

        if (timeInSeconds == -1) {
            if (explosionActive) DrawTextureRec(explosion, frameRec, explosionPos, WHITE);
        } else {
            PlaySound(initSound);
            drawCountdownScreen(timeInSeconds, timeInSecondsStringified, SCREEN_WIDTH, SCREEN_HEIGHT, bombLightOn, bombLightOff);
        }

        EndDrawing();
    }

    UnloadTexture(bombLightOff);
    UnloadTexture(bombLightOn);
    UnloadTexture(explosion);
    UnloadSound(initSound);  
    CloseAudioDevice();      
    CloseWindow();

    return 0;
}


void drawCountdownScreen(int timeInSeconds, char *timeInSecondsStringified, int SCREEN_WIDTH, int SCREEN_HEIGHT, Texture2D bombLightOn, Texture2D bombLightOff) {
    if (timeInSeconds <= 0) {
        ClearBackground(BLACK);
        DrawText("TIME IS UP!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, RED);
    } else {
        ClearBackground(BLACK);
        if (timeInSeconds % 2 == 0) {
            DrawTexture(bombLightOff, SCREEN_WIDTH / 2 - bombLightOff.width / 2, SCREEN_HEIGHT / 2 - bombLightOff.height / 2, WHITE);
        } else {
            DrawTexture(bombLightOn, SCREEN_WIDTH / 2 - bombLightOff.width / 2, SCREEN_HEIGHT / 2 - bombLightOff.height / 2, WHITE);     
        }
        snprintf(timeInSecondsStringified, 50, "%d", timeInSeconds);
        DrawText(timeInSecondsStringified, (SCREEN_WIDTH / 2 - bombLightOff.width / 2) + 70, (SCREEN_HEIGHT / 2 - bombLightOff.height / 2) + 70, 18, BLACK);
    }
}


