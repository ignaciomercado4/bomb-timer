#include "raylib.h"
#include <unistd.h>
#include "resource_dir.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 800;
    int timeInSeconds = -1;
    float timer = 1.0f; 
    char timeInSecondsStringified[50];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomb Timer");
    InitAudioDevice();

    SearchAndSetResourceDir("resources");

    Texture2D bombLightOff = LoadTexture("sprites/bomb_light_off.png");
    Texture2D bombLightOn = LoadTexture("sprites/bomb_light_on.png");    
    Sound initSound = LoadSound("audio/ok_lets_go.wav");    
    
    PlaySound(initSound);

    while (!WindowShouldClose()) {
        if (timeInSeconds > 0) {
            timer -= GetFrameTime();
            if (timer <= 0) {
                timeInSeconds--;
                timer = 1.0f;
            }
        }

        BeginDrawing();

        if (timeInSeconds == -1) {
            ClearBackground(BLACK);
        } else {
            drawCountdownScreen(timeInSeconds, timeInSecondsStringified, SCREEN_WIDTH, SCREEN_HEIGHT, bombLightOn, bombLightOff);
        }

        EndDrawing();
    }

    UnloadTexture(bombLightOff);
    UnloadTexture(bombLightOn);
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


