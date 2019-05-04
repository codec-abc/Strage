/** @file FightingEntity.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_FIGHTING_ENTITY_HPP
#define HPP_FIGHTING_ENTITY_HPP

#include <EffectManager.hpp>
#include <LevelManager.hpp>
#include <MovingEntity.hpp>
#include <BulletMovingEntity.hpp>
#include <SDL2/SDL.h>
#include <StaticEntityAnimatedTexture.hpp>
#include <TextureManager.hpp>

/** @class FightingEntity
 * A moving entity that has life points and can shoot bullets.
 */
class FightingEntity: public MovingEntity
{
	protected:
		/** How many life points the entity owns. */
		int _lifePointsAmount;
		/** The maximum entity life value. */
		int _maximumLifePointsAmount;
		
		/** Offset to add to entity coordinates to fire the bullet in the entity facing direction. */
		SDL_Point _bulletStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
		
		/** Offset to add to entity coordinates to put the shoot firing effect in front of the entity cannon. */
		SDL_Point _firingEffectStartingPositionOffsets[DIRECTIONS_COUNT]; // Offsets are in the same order than Direction enum
		
		/** When was the last shot fired (in milliseconds). This is part of the fire rate mechanism. */
		unsigned int _lastShotTime;
		/** How many milliseconds to wait between two shots. */
		unsigned int _timeBetweenShots;
		
		/** The effect to play when the entity shoots. */
		EffectManager::EffectId _firingEffectId;
		
		/** Generate a bullet from the type the enemy fires.
		 * @param x Bullet X coordinate.
		 * @param y Bullet Y coordinate.
		 * @return An enemy-specific bullet.
		 */
		virtual BulletMovingEntity *_fireBullet(int x, int y) = 0;
		
		/** Compute the offset to entity coordinates to spawn a bullet at the cannon location for each entity direction.
		 * @param pointerEntityTexture The entity texture (all textures must have the same dimensions in all entity directions).
		 * @param pointerFacingUpBulletTexture The bullet texture in the "facing up" direction.
		 * @param pointerPositionOffsets On output, contain the computed offsets.
		 */
		void _computeBulletStartingPositionOffsets(Texture *pointerEntityTexture, Texture *pointerFacingUpBulletTexture, SDL_Point *pointerPositionOffsets)
		{
			int entityWidth, bulletWidth, bulletHeight;
			
			// Cache dimensions
			entityWidth = pointerEntityTexture->getWidth(); // Only entity width is required because the entity is always facing the direction it shoots to
			bulletWidth = pointerFacingUpBulletTexture->getWidth();
			bulletHeight = pointerFacingUpBulletTexture->getHeight();
			
			// Assume that entity is faced to the direction the bullet is fired
			pointerPositionOffsets[DIRECTION_UP].x = (entityWidth - bulletWidth) / 2;
			pointerPositionOffsets[DIRECTION_UP].y = 2; // Make the bullet start into the entity, with two more pixels to be sure to hit an underneath entity
			pointerPositionOffsets[DIRECTION_DOWN].x = pointerPositionOffsets[DIRECTION_UP].x;
			pointerPositionOffsets[DIRECTION_DOWN].y = entityWidth - bulletHeight - 2; // -1 should be enough due entityHeight usage (which results in coordinate + 1), but -2 is needed to make the underneath entity killable
			pointerPositionOffsets[DIRECTION_LEFT].x = 2; // Manually adjusted value to allow an underneath entity to be hit when this entity is facing left (TODO dependent of bullet size and speed)
			pointerPositionOffsets[DIRECTION_LEFT].y = pointerPositionOffsets[DIRECTION_UP].x;
			pointerPositionOffsets[DIRECTION_RIGHT].x = pointerPositionOffsets[DIRECTION_DOWN].y; // Entity is facing right, so its horizontal width is its height
			pointerPositionOffsets[DIRECTION_RIGHT].y = pointerPositionOffsets[DIRECTION_UP].x;
		}
		
		/** Compute the offset to entity coordinates to spawn a muzzle flash effect at the cannon location for each entity direction.
		 * @param pointerEntityTexture The entity texture (all textures must have the same dimensions in all entity directions).
		 * @param firingEffectId The firing effect.
		 * @param pointerPositionOffsets On output, contain the computed offsets.
		 */
		void _computeFiringEffectStartingPositionOffsets(Texture *pointerEntityTexture, EffectManager::EffectId firingEffectId, SDL_Point *pointerPositionOffsets)
		{
			int entityWidth, entityHeight, firingEffectWidth, firingEffectHeight;
			
			// Cache dimensions
			entityWidth = pointerEntityTexture->getWidth();
			entityHeight = pointerEntityTexture->getHeight();
			firingEffectWidth = EffectManager::getEffectTextureWidth(firingEffectId);
			firingEffectHeight = EffectManager::getEffectTextureHeight(firingEffectId);
			
			// Assume that entity is faced to the direction the fire effect is spawned
			pointerPositionOffsets[DIRECTION_UP].x = (entityWidth - firingEffectWidth) / 2;
			pointerPositionOffsets[DIRECTION_UP].y = -firingEffectHeight;
			pointerPositionOffsets[DIRECTION_DOWN].x = pointerPositionOffsets[DIRECTION_UP].x;
			pointerPositionOffsets[DIRECTION_DOWN].y = entityHeight;
			pointerPositionOffsets[DIRECTION_LEFT].x = -firingEffectHeight;
			pointerPositionOffsets[DIRECTION_LEFT].y = pointerPositionOffsets[DIRECTION_UP].x;
			pointerPositionOffsets[DIRECTION_RIGHT].x = pointerPositionOffsets[DIRECTION_DOWN].y;
			pointerPositionOffsets[DIRECTION_RIGHT].y = pointerPositionOffsets[DIRECTION_UP].x;
		}
		
	public:
		/** Initialize life points in addition to parent classes fields.
		 * @param x The X coordinate where to spawn the entity.
		 * @param y The Y coordinate where to spawn the entity.
		 * @param textureId The texture to use on rendering.
		 * @param movingPixelsAmount Entity moving speed.
		 * @param maximumLifePointsAmount Entity maximum life points count.
		 * @param timeBetweenShots How many milliseconds to wait between two shots.
		 * @param facingUpBulletTextureId The "facing up" texture of the bullet fired by the entity.
		 * @param firingEffectId The effect to play when the entity shoots.
		 */
		FightingEntity(int x, int y, TextureManager::TextureId textureId, int movingPixelsAmount, int maximumLifePointsAmount, int timeBetweenShots, TextureManager::TextureId facingUpBulletTextureId, EffectManager::EffectId firingEffectId): MovingEntity(x, y, textureId, movingPixelsAmount)
		{
			_lifePointsAmount = maximumLifePointsAmount;
			_maximumLifePointsAmount = maximumLifePointsAmount;
			_firingEffectId = firingEffectId;
			
			// Player and enemies collide with walls and enemy spawners
			_collisionBlockContent = LevelManager::BLOCK_CONTENT_WALL | LevelManager::BLOCK_CONTENT_ENEMY_SPAWNER;
			
			// Cache the offset to add to entity coordinates to make fired bullets start from where the cannon is (bullets are spawned a little nearer from the entity center than the cannon muzzle, so an underneath entity can be hit)
			// Warning : for the underneath entity to be killed, bullet speed must be less than the bullet texture's larger dimension
			Texture *pointerBulletTexture = TextureManager::getTextureFromId(facingUpBulletTextureId); // Use textures to avoid instantiate a bullet to get its dimensions
			_computeBulletStartingPositionOffsets(_pointerTextures[DIRECTION_UP], pointerBulletTexture, _bulletStartingPositionOffsets);
			
			// Cache the offset to add to entity coordinates to put the muzzle flash in front of the cannon
			_computeFiringEffectStartingPositionOffsets(_pointerTextures[DIRECTION_UP], firingEffectId, _firingEffectStartingPositionOffsets);
			
			_timeBetweenShots = timeBetweenShots;
			_lastShotTime = 0; // Allow to shoot immediately
		}
		
		/** Free allocated resources. */
		virtual ~FightingEntity() {}
		
		/** Get the entity life points.
		 * @return Entity life points amount.
		 */
		inline int getLifePointsAmount()
		{
			return _lifePointsAmount;
		}
		
		/** Set the entity life points.
		 * @param lifePointsAmount Entity life points amount.
		 */
		inline void setLifePointsAmount(int lifePointsAmount)
		{
			_lifePointsAmount = lifePointsAmount;
		}
		
		/** Get the entity maximum life points.
		 * @return Entity maximum life points amount.
		 */
		inline int getMaximumLifePointsAmount()
		{
			return _maximumLifePointsAmount;
		}
		
		/** Set the entity maximum life points.
		 * @param lifePointsAmount Entity maximum life points amount.
		 */
		inline void setMaximumLifePointsAmount(int lifePointsAmount)
		{
			_maximumLifePointsAmount = lifePointsAmount;
		}
		
		/** Change entity life value by adding or removing some life points.
		 * @param lifePointsAmount How many life points to add or subtract.
		 */
		inline void modifyLife(int lifePointsAmount)
		{
			_lifePointsAmount += lifePointsAmount;
			
			// Clamp life points to [0; _maximumLifePointsAmount]
			if (_lifePointsAmount < 0) _lifePointsAmount = 0;
			else if (_lifePointsAmount > _maximumLifePointsAmount) _lifePointsAmount = _maximumLifePointsAmount;
		}
		
		/** Tell if the entity is at maximum life or not.
		 * @return 1 if the entity can be healed,
		 * @return 0 if the entity is healthy.
		 */
		inline int isWounded()
		{
			if (_lifePointsAmount < _maximumLifePointsAmount) return 1;
			return 0;
		}
		
		/** Tell if the entity is dead or alive.
		 * @return 1 if the entity has no more life point,
		 * @return 0 if the entity is still alive.
		 */
		inline int isDead()
		{
			if (_lifePointsAmount == 0) return 1;
			return 0;
		}
		
		/** Generate a bullet facing the entity direction and play the associated effect.
		 * @return A valid pointer if the entity was allowed to shot,
		 * @return NULL if the entity could not shoot (no more ammunition, slower fire rate...).
		 */
		virtual BulletMovingEntity *shoot()
		{
			int bulletStartingPositionOffsetX, bulletStartingPositionOffsetY, firingEffectStartingPositionOffsetX, firingEffectStartingPositionOffsetY, entityX, entityY;
			EffectManager::EffectId muzzleFlashEffectId;
			BulletMovingEntity *pointerBullet;
			
			// Allow to shoot only if enough time elapsed since last shot
			if (SDL_GetTicks() - _lastShotTime >= _timeBetweenShots)
			{
				// Cache entity coordinates
				entityX = _positionRectangles[_facingDirection].x;
				entityY = _positionRectangles[_facingDirection].y;
				
				// Select the right offsets according to entity direction
				bulletStartingPositionOffsetX = _bulletStartingPositionOffsets[_facingDirection].x;
				bulletStartingPositionOffsetY = _bulletStartingPositionOffsets[_facingDirection].y;
				firingEffectStartingPositionOffsetX = _firingEffectStartingPositionOffsets[_facingDirection].x;
				firingEffectStartingPositionOffsetY = _firingEffectStartingPositionOffsets[_facingDirection].y;
				
				// Create the bullet
				pointerBullet = _fireBullet(entityX + bulletStartingPositionOffsetX, entityY + bulletStartingPositionOffsetY);
				
				// Play the shoot effect
				muzzleFlashEffectId = (EffectManager::EffectId) ((int) _firingEffectId + (int) _facingDirection); // Select the right effect according to entity direction
				EffectManager::addEffect(entityX + firingEffectStartingPositionOffsetX, entityY + firingEffectStartingPositionOffsetY, muzzleFlashEffectId);
				
				// Get time after having generated the bullet, in case this takes more than 1 millisecond
				_lastShotTime = SDL_GetTicks();
				
				return pointerBullet;
			}
			return NULL; // No shot allowed
		}
};

#endif
