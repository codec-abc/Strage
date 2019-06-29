/** @file TextureManager.hpp
 * Load and provide to other subsystems all game textures.
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_MANAGER_HPP
#define HPP_TEXTURE_MANAGER_HPP

#include <AnimatedTexture.hpp>
#include <Texture.hpp>

namespace TextureManager
{
	/** All available textures. */
	typedef enum
	{
		// Scene floors
		TEXTURE_ID_SCENE_FLOOR_0,
		TEXTURE_ID_SCENE_FLOOR_1,
		TEXTURE_ID_SCENE_FLOOR_2,
		TEXTURE_ID_SCENE_FLOOR_3,
		TEXTURE_ID_SCENE_FLOOR_4,
		TEXTURE_ID_SCENE_FLOOR_5,
		TEXTURE_ID_SCENE_FLOOR_6,
		TEXTURE_ID_SCENE_FLOOR_7,
		TEXTURE_ID_SCENE_FLOOR_8,
		TEXTURE_ID_SCENE_FLOOR_9,
		TEXTURE_ID_SCENE_FLOOR_10,
		TEXTURE_ID_SCENE_FLOOR_11,
		TEXTURE_ID_SCENE_FLOOR_12,
		TEXTURE_ID_SCENE_FLOOR_13,
		TEXTURE_ID_SCENE_FLOOR_14,
		TEXTURE_ID_SCENE_FLOOR_15,
		TEXTURE_ID_SCENE_FLOOR_16,
		
		// Scene walls
		TEXTURE_ID_SCENE_WALL_0,
		TEXTURE_ID_SCENE_WALL_1,
		TEXTURE_ID_SCENE_WALL_2,
		TEXTURE_ID_SCENE_WALL_3,
		TEXTURE_ID_SCENE_WALL_4,
		TEXTURE_ID_SCENE_WALL_5,
		TEXTURE_ID_SCENE_WALL_6,
		
		// Static objects
		TEXTURE_ID_MEDIPACK,
		TEXTURE_ID_GOLDEN_MEDIPACK,
		TEXTURE_ID_AMMUNITION,
		TEXTURE_ID_MACHINE_GUN,
		TEXTURE_ID_BULLET_PROOF_VEST,
		TEXTURE_ID_ENEMY_SPAWNER,
		TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_1,
		TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_2,
		TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_3,
		TEXTURE_ID_ENEMY_SPAWNER_DAMAGED_4,
		
		// Moving objects
		TEXTURE_ID_PLAYER_FACING_UP,
		TEXTURE_ID_PLAYER_FACING_DOWN,
		TEXTURE_ID_PLAYER_FACING_LEFT,
		TEXTURE_ID_PLAYER_FACING_RIGHT,
		TEXTURE_ID_SMALL_ENEMY_FACING_UP,
		TEXTURE_ID_SMALL_ENEMY_FACING_DOWN,
		TEXTURE_ID_SMALL_ENEMY_FACING_LEFT,
		TEXTURE_ID_SMALL_ENEMY_FACING_RIGHT,
		TEXTURE_ID_MEDIUM_ENEMY_FACING_UP,
		TEXTURE_ID_MEDIUM_ENEMY_FACING_DOWN,
		TEXTURE_ID_MEDIUM_ENEMY_FACING_LEFT,
		TEXTURE_ID_MEDIUM_ENEMY_FACING_RIGHT,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_UP,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_DOWN,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_LEFT,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_1_FACING_RIGHT,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_UP,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_DOWN,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_LEFT,
		TEXTURE_ID_MEDIUM_ENEMY_DAMAGED_2_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_1_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_2_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_3_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_DAMAGED_4_FACING_RIGHT,
		TEXTURE_ID_PLAYER_BULLET_FACING_UP,
		TEXTURE_ID_PLAYER_BULLET_FACING_DOWN,
		TEXTURE_ID_PLAYER_BULLET_FACING_LEFT,
		TEXTURE_ID_PLAYER_BULLET_FACING_RIGHT,
		TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_UP,
		TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_DOWN,
		TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_LEFT,
		TEXTURE_ID_PLAYER_BULLET_MORTAR_SHELL_FACING_RIGHT,
		TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_UP,
		TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_DOWN,
		TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_LEFT,
		TEXTURE_ID_SMALL_ENEMY_BULLET_FACING_RIGHT,
		TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_UP,
		TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_DOWN,
		TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_LEFT,
		TEXTURE_ID_MEDIUM_ENEMY_BULLET_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_BULLET_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_BULLET_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_BULLET_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_BULLET_FACING_RIGHT,
		
		// Overlays
		TEXTURE_ID_PLAYER_HIT_OVERLAY,
		
		// Graphic user interface
		TEXTURE_ID_HEAD_UP_DISPLAY_BACKGROUND,
		TEXTURE_ID_MENU_BACKGROUND,
		
		// Animated textures
		TEXTURE_ID_ENEMY_SPAWNER_EXPLOSION,
		TEXTURE_ID_SMALL_ENEMY_EXPLOSION,
		TEXTURE_ID_MEDIUM_ENEMY_EXPLOSION,
		TEXTURE_ID_BIG_ENEMY_EXPLOSION,
		TEXTURE_ID_BULLET_EXPLOSION,
		TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_UP,
		TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_DOWN,
		TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_LEFT,
		TEXTURE_ID_PLAYER_MUZZLE_FLASH_FACING_RIGHT,
		TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_UP,
		TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TEXTURE_ID_SMALL_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_UP,
		TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TEXTURE_ID_MEDIUM_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_UP,
		TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_DOWN,
		TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_LEFT,
		TEXTURE_ID_BIG_ENEMY_MUZZLE_FLASH_FACING_RIGHT,
		TEXTURE_ID_PLAYER_MUZZLE_FLASH_MORTAR_SHELL,
		TEXTURE_ID_MEDIPACK_TAKEN,
		TEXTURE_ID_GOLDEN_MEDIPACK_TAKEN,
		TEXTURE_ID_AMMUNITION_TAKEN,
		TEXTURE_ID_ENEMY_SPAWNER_TELEPORTATION_EFFECT,
		TEXTURE_ID_MACHINE_GUN_TAKEN,
		
		TEXTURE_IDS_COUNT
	} TextureId;

	/** Load all game textures.
	* @return 0 if all textures were successfully initialized,
	* @return -1 if an error occurred.
	*/
	int initialize();

	/** Free all allocated resources. */
	void uninitialize();

	/** Retrieve a texture.
	* @param id The texture identifier.
	* @return The texture.
	* @note The function will crash if an invalid ID is provided.
	*/
	Texture *getTextureFromId(TextureId id);

	/** Create an animated texture that can be used like a Texture object.
	* @param id The texture ID to use as template.
	* @param isAnimationLooping Set to true to make the animation loop forever, set to false to display the animation only once.
	* @return The animated texture.
	* @note It's the caller responsibility to free the created animated texture.
	* @note The function will crash if an invalid ID is provided.
	*/
	AnimatedTexture *createAnimatedTextureFromId(TextureId id, bool isAnimationLooping);
}

#endif
