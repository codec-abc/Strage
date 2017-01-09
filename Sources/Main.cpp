/** @file Main.cpp
 * Initialize the game engine and run the game.
 * @author Adrien RICCIARDI
 */
#include <cstdlib>
#include <FightingEntityPlayer.hpp>
#include <list>
#include <Log.hpp>
#include <LevelManager.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

// TEST
#include <PickableEntityMedipack.hpp>
#include <MovableEntityBullet.hpp>
#include <FightingEntityEnemy.hpp>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All keyboard keys that matter. */
typedef enum
{
	KEYBOARD_KEY_ID_ARROW_UP,
	KEYBOARD_KEY_ID_ARROW_DOWN,
	KEYBOARD_KEY_ID_ARROW_LEFT,
	KEYBOARD_KEY_ID_ARROW_RIGHT,
	KEYBOARD_KEY_ID_SPACE,
	KEYBOARD_KEY_IDS_COUNT
} KeyboardKeyId;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All pickable entities. */
static std::list<PickableEntity *> pickableEntitiesList;

/** All bullets shot by the player. */
static std::list<MovableEntityBullet *> playerBulletsList;
/** All bullets shot by the enemies. */
static std::list<MovableEntityBullet *> enemiesBulletsList;

/** All enemies. */
static std::list<FightingEntityEnemy *> enemiesList;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
FightingEntityPlayer *pointerPlayer;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Automatically free allocated resources on program shutdown. */
static void exitFreeResources()
{
	LevelManager::uninitialize();
	TextureManager::uninitialize();
	Renderer::uninitialize(); // Must be called at the end because it stops SDL
}

/** Update all game actors. */
static void updateGameLogic()
{
	// Check if pickable objects can be taken by the player
	std::list<PickableEntity *>::iterator pickableListIterator;
	PickableEntity *pointerPickableEntity;
	for (pickableListIterator = pickableEntitiesList.begin(); pickableListIterator != pickableEntitiesList.end(); ++pickableListIterator)
	{
		pointerPickableEntity = *pickableListIterator;
		if (pointerPickableEntity->update() != 0) pickableListIterator = pickableEntitiesList.erase(pickableListIterator);
	}
	
	// Check if player bullets have hit a wall or an enemy
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	MovableEntityBullet *pointerPlayerBullet;
	FightingEntityEnemy *pointerEnemy;
	for (bulletsListIterator = playerBulletsList.begin(); bulletsListIterator != playerBulletsList.end(); ++bulletsListIterator)
	{
		pointerPlayerBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerPlayerBullet->update() != 0)
		{
			bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		// Check with all enemies for a collision
		for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator)
		{
			pointerEnemy = *enemiesListIterator;
			
			// Did the bullet hit this enemy ?
			if (SDL_HasIntersection(pointerPlayerBullet->getPositionRectangle(), pointerEnemy->getPositionRectangle()))
			{
				// Remove the bullet
				bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
				
				// Wound the enemy
				pointerEnemy->modifyLife(-5); // TODO put bullet damage in bullet if more bullet types are to be created
				LOG_DEBUG("Enemy hit.\n");
			}
		}
	}
	
	// Check if enemies bullets have hit the player
	MovableEntityBullet *pointerEnemyBullet;
	for (bulletsListIterator = enemiesBulletsList.begin(); bulletsListIterator != enemiesBulletsList.end(); ++bulletsListIterator)
	{
		pointerEnemyBullet = *bulletsListIterator;
		
		// Remove the bullet if it hit a wall
		if (pointerEnemyBullet->update() != 0)
		{
			bulletsListIterator = playerBulletsList.erase(bulletsListIterator);
			continue;
		}
		
		if (SDL_HasIntersection(pointerPlayer->getPositionRectangle(), pointerEnemyBullet->getPositionRectangle()))
		{
			// Remove the bullet
			bulletsListIterator = enemiesBulletsList.erase(bulletsListIterator);
			
			// Wound the player
			pointerPlayer->modifyLife(-1);
			LOG_DEBUG("Player hit.\n");
			
			// TODO player dead
		}
	}
	
	// Update enemies artificial intelligence
	MovableEntityBullet *pointerBullet;
	for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator)
	{
		pointerEnemy = *enemiesListIterator;
		
		// Remove the enemy if it is dead
		if (pointerEnemy->update(&pointerBullet) != 0)
		{
			enemiesListIterator = enemiesList.erase(enemiesListIterator);
			
			// TODO spawn explosion effect
			
			continue;
		}
		
		// Do not update IA if the enemy did not spotted the player (i.e. the enemy is not visible, as the spotting rectangle is huge)
		
		// Did the enemy fired ?
		if (pointerBullet != NULL) enemiesBulletsList.push_front(pointerBullet);
	}
}

/** Display everything to the screen.
 * @param sceneX The "camera" will render the scene starting from this scene horizontal coordinate.
 * @param sceneY The "camera" will render the scene starting from this scene vertical coordinate.
 */
static void renderGame(int sceneX, int sceneY)
{
	// Start rendering
	Renderer::beginRendering(sceneX, sceneY);
	
	// Render the level walls
	LevelManager::renderScene(sceneX, sceneY);
	
	// Display pickable entities (as they are laying on the floor, any other entity is walking on top of them)
	std::list<PickableEntity *>::iterator pickableListIterator;
	for (pickableListIterator = pickableEntitiesList.begin(); pickableListIterator != pickableEntitiesList.end(); ++pickableListIterator) (*pickableListIterator)->render();
	
	// Display enemies
	std::list<FightingEntityEnemy *>::iterator enemiesListIterator;
	for (enemiesListIterator = enemiesList.begin(); enemiesListIterator != enemiesList.end(); ++enemiesListIterator) (*enemiesListIterator)->render();
	
	// Display bullets after enemies, so when multiple enemies fire on themselves bullets are visible on top of enemies
	std::list<MovableEntityBullet *>::iterator bulletsListIterator;
	for (bulletsListIterator = playerBulletsList.begin(); bulletsListIterator != playerBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	for (bulletsListIterator = enemiesBulletsList.begin(); bulletsListIterator != enemiesBulletsList.end(); ++bulletsListIterator) (*bulletsListIterator)->render();
	
	// Display the player at the end, so it is always rendered on top on everything else and can always be visible
	pointerPlayer->render();
	
	// Display HUD
	char string[64];
	// Life points
	sprintf(string, "Life : %d%%", pointerPlayer->getLifePointsAmount());
	Renderer::renderText(CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X, CONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y, string);
	// Ammunitions count
	// TODO
	
	// Display the rendered picture
	SDL_RenderPresent(Renderer::pointerMainRenderer);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	SDL_Event event;
	unsigned int Starting_Time, Elapsed_Time;
	int isKeyPressed[KEYBOARD_KEY_IDS_COUNT] = {0}, isLastDirectionVertical;
	
	// Engine initialization
	if (Renderer::initialize() != 0) return -1;
	if (TextureManager::initialize() != 0) return -1;
	if (LevelManager::initialize() != 0) return -1;
	
	// Automatically dispose of allocated resources on program exit (allowing to use exit() elsewhere in the program)
	atexit(exitFreeResources);
	LOG_INFORMATION("Game engine successfully initialized.\n");
	
	// TEST
	int camX = 0, camY = 0;
	LevelManager::loadLevel("Levels/Test_Scene.csv", "Levels/Test_Objects.csv");
	PickableEntityMedipack m1(81 + 64, 93);
	PickableEntityMedipack m2(64 * 10 + 25, 64 * 5 + 47);
	PickableEntityMedipack m3(64 * 6 + 25, 64 * 12 + 47);
	pickableEntitiesList.push_front(&m1);
	pickableEntitiesList.push_front(&m2);
	pickableEntitiesList.push_front(&m3);
	FightingEntityPlayer player(5 * 64 + 20, 80);
	pointerPlayer = &player;
	FightingEntityEnemy e1(64*8 + 13, 64 * 3 + 35);
	enemiesList.push_front(&e1);
	
	while (1)
	{
		// Store the time when the loop started
		Starting_Time = SDL_GetTicks();
		
		// Handle all relevant events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					goto Exit;
				
				// Remember which key are pressed
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 1;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 0;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 1;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 1;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 0;
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 1;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_SPACE:
							isKeyPressed[KEYBOARD_KEY_ID_SPACE] = 1;
							break;
							
						case SDL_SCANCODE_Q:
							player.modifyLife(-10);
							printf("player life : %d\n", player.getLifePointsAmount());
							break;
							
						default:
							break;
					}
					break;
				}
				
				// Some key has been released
				case SDL_KEYUP:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_DOWN:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN] = 0;
							isLastDirectionVertical = 0;
							break;
							
						case SDL_SCANCODE_LEFT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_RIGHT:
							isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT] = 0;
							isLastDirectionVertical = 1;
							break;
							
						case SDL_SCANCODE_SPACE:
							isKeyPressed[KEYBOARD_KEY_ID_SPACE] = 0;
							break;
							
						default:
							break;
					}
					break;
				}
			}
		}
		
		// React to player key press without depending of keyboard key repetition rate
		// Handle both vertical and horizontal direction movement
		if ((isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP] || isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN]) && (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT] || isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT]))
		{
			// Keep trace of the last direction the player took to favor it, when this key will be released the previous direction will be favored
			if (isLastDirectionVertical)
			{
				if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP]) player.moveToUp();
				else player.moveToDown();
			}
			else
			{
				if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT]) player.moveToLeft();
				else player.moveToRight();
			}
		}
		// Handle a single key press
		else
		{
			if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_UP]) player.moveToUp();
			else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_DOWN]) player.moveToDown();
			else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_LEFT]) player.moveToLeft();
			else if (isKeyPressed[KEYBOARD_KEY_ID_ARROW_RIGHT]) player.moveToRight();
		}
		
		// Fire a bullet
		if (isKeyPressed[KEYBOARD_KEY_ID_SPACE])
		{
			MovableEntityBullet *pointerBullet = player.shoot();
			
			// Is the player allowed to shoot ?
			if (pointerBullet != NULL) playerBulletsList.push_front(pointerBullet);
		}
		
		// TEST
		camX = player.getX() - (CONFIGURATION_DISPLAY_WIDTH / 2) + 16;
		camY = player.getY() - (CONFIGURATION_DISPLAY_HEIGHT / 2) + 16;
		
		updateGameLogic();
		
		renderGame(camX, camY);
		
		// Wait enough time to achieve a 60Hz refresh rate
		Elapsed_Time = SDL_GetTicks() - Starting_Time;
		if (Elapsed_Time < CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS) SDL_Delay(CONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS - Elapsed_Time);
	}
	
Exit:
	return 0;
}