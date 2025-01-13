#include "raylib.h"
#include <unistd.h>
#include "resource_dir.h"


int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 800;
    int timeInSeconds = 60;
    float timer = 1.0f; 
    char timeInSecondsStringified[50];
	

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomb Timer");
    SearchAndSetResourceDir("resources");


    Font font = LoadFont("resources/fonts/pixelplay.png");
    Texture2D bombLightOff = LoadTexture("bomb_light_off.png");
    Texture2D bombLightOn = LoadTexture("bomb_light_on.png");	
    
	while (!WindowShouldClose()) {
        if (timeInSeconds > 0) {
            timer -= GetFrameTime();
            if (timer <= 0) {
                timeInSeconds--;
                timer = 1.0f;
            }
        }

        BeginDrawing();

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

        EndDrawing();
    }

    UnloadTexture(bombLightOff);
    CloseWindow();

    return 0;
}
