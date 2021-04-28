#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "enemy.h"
#include "coin.h"

#include <string>
#include <vector>
using namespace std;

// boolean to maintain program loop
bool quit = false;

// deltaTime init() - for frame rate ind
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

// NEW PATROLLING 1 ***********************************
vector<Enemy> enemyList;

// declare max number of enemies
int numberOfEnemies;

// NEW PATROLLING 1 END *******************************

// new inventory

//coin list variable
vector<Coin> coinList;

// declare max number of coins
int numberOfCoins;

// declare sounds
Mix_Chunk* pickup;

int main(int argc, char* argv[]) {

	SDL_Window* window;			// declare a pointer

	// create a renderer variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);		// Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"Dungeon Crawler",			// window title
		SDL_WINDOWPOS_UNDEFINED,	// initial x position
		SDL_WINDOWPOS_UNDEFINED,	// initial y position
		642,						// width, in pixels
		358,						// height, in pixels
		SDL_WINDOW_OPENGL			// flags - see below
	);

	// check that the window was succesfully created
	if (window == NULL) {
		// in the case that the window could not be made
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// background image -- CREATE

	// create a SDL surface
	SDL_Surface* surface = IMG_Load("./Assets/level.png");

	// create bkgd texture
	SDL_Texture* bkgd;

	// place surface into the texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	// create rectangles for the menu graphics
	SDL_Rect bkgdPos;

	// set bkgd pos x, y, width, and height
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 642;
	bkgdPos.h = 358;

	// background image -- CREATE END

	// player image -- CREATE
	surface = IMG_Load("./Assets/player.png");

	// create player texture
	SDL_Texture* player;

	// place surface into the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	// player movement - controlled by keypress, not deltatime
	int playerMovement = 71;

	// create rectangles for the player
	SDL_Rect playerPos;

	// set playerPos x, y, width, and height
	playerPos.x = 291;
	playerPos.y = 291;
	playerPos.w = 59;
	playerPos.h = 59;

	// player image -- CREATE END

	// SDL event to handle input
	SDL_Event event;

	// maze settings - width and height
	const int MAZE_WIDTH = 9;
	const int MAZE_HEIGHT = 5;

	// create the array for the maze - "O" = open space, "W" = wall, "P" = player
	string maze[MAZE_HEIGHT][MAZE_WIDTH] = {
		{"O","O","O","O","O","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","W","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","P","O","O","O","O"}
	};

	// player starting position in the maze Row 5 Column 5 - maze[4][4]
	int playerHorizontal = 4;
	int playerVertical = 4;

	// NEW PATROLLING 2 ***********************************
	// clear enemy list
	enemyList.clear();

	// fill enemy list

	// init max number of enemies
	numberOfEnemies = 4;

	// enemy for the upper right
	Enemy tempEnemy(renderer, 71, 2, 2, "left", "CCW", 575, 7);

	// add to enemy list
	enemyList.push_back(tempEnemy);

	// enemy for the upper left
	Enemy tempEnemy2(renderer, 71, 2, 2, "right", "CW", 7, 7);

	// add to the enemy list
	enemyList.push_back(tempEnemy2);

	// enemy for the lower right
	Enemy tempEnemy3(renderer, 71, 2, 2, "right", "CW", 433, 149);

	// add to the enemy list
	enemyList.push_back(tempEnemy3);

	// enemy for the lower left area
	Enemy tempEnemy4(renderer, 71, 2, 2, "up", "CCW", 149, 291);

	// add to the enemy list
	enemyList.push_back(tempEnemy4);

	// NEW PATROLLING 2 END *******************************

	// new inventory 2
	coinList.clear();

	// fill coin list

	// init max number of coins
	numberOfCoins = 4;

	// coins picked up
	int totalCoins = 0;

	// coin for the upper left area
	Coin tempCoin(renderer, 18, 18);

	// add to coinlist
	coinList.push_back(tempCoin);

	// coin for the lower left area
	Coin tempCoin2(renderer, 18, 302);

	// add to coinlist
	coinList.push_back(tempCoin2);

	// coin for the upper right area
	Coin tempCoin3(renderer, 586, 18);

	// add to coinlist
	coinList.push_back(tempCoin3);

	// coin for the lower right area
	Coin tempCoin4(renderer, 586, 302);

	// add to coinlist
	coinList.push_back(tempCoin4);



	// init audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// load pickup sound
	pickup = Mix_LoadWAV("./Assets/pickup.wav");

	// basic program loop
	while (!quit) {

		// create deltaTime
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		// check for input for closing window and firing
		if (SDL_PollEvent(&event)) {
			// close window by the window's x button
			if (event.type == SDL_QUIT) {
				quit = true;
			}

			switch (event.type)
			{
				/* Look for a keypress and release*/
			case SDL_KEYUP:

				/* Check the SDLKey values */
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT: // move right

					// check to see if the player's potential horizaontal position is within the maze's right side limit
					if ((playerHorizontal + 1) < MAZE_WIDTH) {
						if (maze[playerVertical][playerHorizontal + 1] == "O") {
							// move the player in the maze array - set the future spot to "P"
							maze[playerVertical][playerHorizontal + 1] = "P";
							
							//set the player's old spot to "O"
							maze[playerVertical][playerHorizontal] = "O";

							playerHorizontal++;
							playerPos.x += playerMovement;
						}
						
					}
					break;

				case SDLK_LEFT: // move left

					// check to see if the player's potential horizaontal position is within the maze's left side limit
					if ((playerHorizontal - 1) >= 0) {
						if (maze[playerVertical][playerHorizontal - 1] == "O") {
							// move the player in the maze array - set the future spot to "P"
							maze[playerVertical][playerHorizontal - 1] = "P";

							//set the player's old spot to "O"
							maze[playerVertical][playerHorizontal] = "O";

							playerHorizontal--;
							playerPos.x -= playerMovement;
						}
						
					}
					break;

				case SDLK_UP: // move up

					// check to see if the player's vertical position in within the mze's top side limit
					if ((playerVertical - 1) >= 0) {
						if (maze[playerVertical - 1][playerHorizontal] == "O") {
							// move the player in the maze array - set the future spot to "P"
							maze[playerVertical - 1][playerHorizontal] = "P";

							//set the player's old spot to "O"
							maze[playerVertical][playerHorizontal] = "O";

							// decrease the player's Y position by 71 - move up
							playerVertical--;
							playerPos.y -= playerMovement;
						}
						
					}
					break;

				case SDLK_DOWN: // move down

					// check to see if the player's vertical position is within the maze's bottome side limit
					if ((playerVertical + 1) < MAZE_HEIGHT) {
						if (maze[playerVertical + 1][playerHorizontal] == "O") {
							// move the player in the maze array - set the future spot to "P"
							maze[playerVertical + 1][playerHorizontal] = "P";

							//set the player's old spot to "O"
							maze[playerVertical][playerHorizontal] = "O";

							// increase the player's Y position by 71 - move down
							playerVertical++;
							playerPos.y += playerMovement;
						}
						
					}
					break;
				}
			}

		}
		// START UPDATE ***********************************

		// Update enemies
		for (int i = 0; i < numberOfEnemies; i++) {
			enemyList[i].Update(deltaTime);
		}

		// check for collision with the player and all enemies
		for (int i = 0; i < enemyList.size(); i++) {
			if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect)) {
				cout << "Player hit by an Enemy!!" << endl;
			}
		}

		// check for collision with the player and all coins
		for (int i = 0; i < coinList.size(); i++) {
			// see if there is a collision between the player and this coin
			if (SDL_HasIntersection(&playerPos, &coinList[i].posRect)) {
				// remove coin
				coinList[i].RemoveFromScreen();

				// play pickup sound
				Mix_PlayChannel(-1, pickup, 0);
				
				// add one coin to total
				totalCoins++;

				// output some temporary feedback
				cout << "Total coins found: " << totalCoins << endl;
			}
		}

		// END UPDATE *************************************
		
		// START DRAW *************************************

		// Draw Section
		// clear the old buffer
		SDL_RenderClear(renderer);

		// draw bkgd
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

		// draw player
		SDL_RenderCopy(renderer, player, NULL, &playerPos);

		// draw coins
		for (int i = 0; i < coinList.size(); i++) {
			coinList[i].Draw(renderer);
		}

		// draw enemies
		for (int i = 0; i < enemyList.size(); i++) {
			enemyList[i].Draw(renderer);
		}

		// draw new info to the screen
		SDL_RenderPresent(renderer);



		// END DRAW *********************************************
	}// ends game loop

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();

	return 0;
	
}