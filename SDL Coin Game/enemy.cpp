#include "enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, int speed, int maxH, int maxV, string dir, string type, int startX, int startY){
	enemyMovement = speed; // set up in pixels to move left, right, up, or down

	maxHorizontalMovement = maxH; // maximum numner of moves the enemy can make to the left or the right
	maxVerticalMovement = maxV; // maximum number of moves the nemy can make up or down

	currentDirection = dir; // the direction the enemy is currently moving

	enemyType = type; // clockwise or counter-clockwise

	SDL_Surface* surface = IMG_Load("./Assets/enemy.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	posRect.x = startX; // the starting X position of the enemy
	posRect.y = startY; // the starting Y position of the enemy

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	lastTime = 0; // int used to hold the latest value of currentTime
}

void Enemy::Update(float deltaTime){
	// move once per 1/2 second - 1000 = 1 second
	// update currentTime to the latest time elapsed
	currentTime = SDL_GetTicks();

	// check to see if enough time has lapsed and move the enemy if so
	if (currentTime > lastTime + 500) {
		if ((currentDirection == "left") && (horizontalCounter < maxHorizontalMovement)) { // if the enemy wants to move left and still has horizontal moves available
			posRect.x -= enemyMovement; // move neemy left one increment
			horizontalCounter++; // add to the counter tracking times the enemy has moved horizontally

			if (horizontalCounter >= maxHorizontalMovement) // if the enemy has moved it's alloted spaces
			{
				horizontalCounter = 0; // reset the enemy's horizontal movement counter to 0

				if (enemyType == "CW") { // if the enemy is meant to be moving clockwise
					currentDirection = "up"; // set current direction to up
				}
				else
				{
					currentDirection = "down";
				}
			}
		}
		else if ((currentDirection == "down") && (verticalCounter < maxVerticalMovement)) { // if the enemy wants to move down and still has vertical moves available
			posRect.y += enemyMovement; // move enemy down one increment
			verticalCounter++; // add to the counter tracking times the enemy has moved vertically

			if (verticalCounter >= maxVerticalMovement) // if the enemy has moved it's alloted spaces
			{
				verticalCounter = 0; // reset the enemy's vertical movement counter to 0

				if (enemyType == "CW") { // if the enemy is meant to be moving clockwise
					currentDirection = "left"; // set current direction to up
				}
				else
				{
					currentDirection = "right";
				}
			}
		}
		else if ((currentDirection == "right") && (horizontalCounter < maxHorizontalMovement)) { // if the enemy wants to move right and still has horizontal moves available
			posRect.x += enemyMovement; // move neemy right one increment
			horizontalCounter++; // add to the counter tracking times the enemy has moved horizontally

			if (horizontalCounter >= maxHorizontalMovement) // if the enemy has moved it's alloted spaces
			{
				horizontalCounter = 0; // reset the enemy's horizontal movement counter to 0

				if (enemyType == "CW") { // if the enemy is meant to be moving clockwise
					currentDirection = "down"; // set current direction to up
				}
				else
				{
					currentDirection = "up";
				}
			}
		}
		else if ((currentDirection == "up") && (verticalCounter < maxVerticalMovement)) { // if the enemy wants to move up and still has vertical moves available
			posRect.y -= enemyMovement; // move enemy up one increment
			verticalCounter++; // add to the counter tracking times the enemy has moved vertically

			if (verticalCounter >= maxVerticalMovement) // if the enemy has moved it's alloted spaces
			{
				verticalCounter = 0; // reset the enemy's vertical movement counter to 0

				if (enemyType == "CW") { // if the enemy is meant to be moving clockwise
					currentDirection = "right"; // set current direction to up
				}
				else
				{
					currentDirection = "left";
				}
			}
		}

		lastTime = currentTime; // update clock
	}
}

void Enemy::Draw(SDL_Renderer* renderer){
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Enemy::Reset(){

}

Enemy::~Enemy(){
	//SDL__DestroyTexture(texture);
}