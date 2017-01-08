#ifndef HPP_MOVABLE_ENTITY_BULLET_HPP
#define HPP_MOVABLE_ENTITY_BULLET_HPP

#include <MovableEntity.hpp>
#include <TextureManager.hpp>

/** @class MovableEntityBullet
 * A bullet fired by the player or the enemies.
 * @author Adrien RICCIARDI
 */
class MovableEntityBullet: public MovableEntity
{
	public:
		/** Spawn a bullet.
		 * @param x Spawning X coordinate.
		 * @param y Spawning Y coordinate.
		 * @param facingDirection In which direction the bullet will move.
		 */
		MovableEntityBullet(int x, int y, FacingDirection facingDirection): MovableEntity(TextureManager::TEXTURE_ID_BULLET, x, y, 6)
		{
			_facingDirection = facingDirection;
		}
		
		/** Check if a scene wall has been hit.
		 * @return 0 if nothing was hit,
		 * @return 1 if scene wall was hit and the bullet must be destroyed.
		 * @warning This function does not check against enemy or player collision.
		 */
		virtual int update()
		{
			int previousX, previousY;
		
			// Keep coordinates before move to easily determine if the bullet moved
			previousX = _renderingDestinationRectangle.x;
			previousY = _renderingDestinationRectangle.y;
			
			// Update position
			switch (_facingDirection)
			{
				case FACING_DIRECTION_UP:
					moveToUp();
					break;
					
				case FACING_DIRECTION_DOWN:
					moveToDown();
					break;
					
				case FACING_DIRECTION_LEFT:
					moveToLeft();
					break;
					
				case FACING_DIRECTION_RIGHT:
					moveToRight();
					break;
			}
			
			// Did the bullet really moved ?
			if ((previousX == _renderingDestinationRectangle.x) && (previousY == _renderingDestinationRectangle.y)) return 1; // It did not move, so it has hit a wall
			return 0;
		}
};

#endif