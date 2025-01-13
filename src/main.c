#include "raylib.h"

#include "resource_dir.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	const int SCREEN_HEIGHT = 800;
	const int SCREEN_WIDTH = 600;
	int timeInSeconds = 100;
	char timeInSecondsStringified[50];


	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomb timer");

	SearchAndSetResourceDir("resources");

	Texture2D bomb = LoadTexture("bomb.png");
	
	while (!WindowShouldClose()) {

		BeginDrawing();

		if (timeInSeconds == -1) {
			// drawing time setup screen			
			ClearBackground(BLACK);

			DrawText("Bomb Timer", 0, 0, 20, WHITE);

			DrawTexture(bomb, SCREEN_HEIGHT/2 - bomb.height/2 , SCREEN_WIDTH/2 - bomb.width/2, WHITE);
		} else {
			// drawing time countdown screen
			ClearBackground(BLACK);

			snprintf(timeInSecondsStringified, sizeof(timeInSecondsStringified), "TIME LEFT IN SEC: %d", timeInSeconds);

			DrawText(timeInSecondsStringified, 0, 0, 20, WHITE);

			DrawTexture(bomb, SCREEN_HEIGHT/2 - bomb.height/2 , SCREEN_WIDTH/2 - bomb.width/2, WHITE);
		}
		
		EndDrawing();
	}

	// cleanup
	UnloadTexture(bomb);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
