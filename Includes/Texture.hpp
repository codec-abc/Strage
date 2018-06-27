/** @file Texture.hpp
 * @author Adrien RICCIARDI
 */
#ifndef HPP_TEXTURE_HPP
#define HPP_TEXTURE_HPP

#include <Log.hpp>
#include <Renderer.hpp>
#include <SDL2/SDL.h>

/** @class Texture
 * Allow to load and display a static picture.
 */
class Texture
{
	protected:
		/** The texture. */
		SDL_Texture *_pointerSDLTexture;
		
		/** Use this rectangle to render the texture (some values are cached this way). */
		SDL_Rect _positionRectangle;
	
	public:
		/** Create a still texture.
		 * @param pointerSDLTexture The SDL texture to display.
		 */
		Texture(SDL_Texture *pointerSDLTexture)
		{
			unsigned int pixelFormat;
			int access;
			
			// Cache width and height parameters
			_pointerSDLTexture = pointerSDLTexture;
			if (SDL_QueryTexture(_pointerSDLTexture, &pixelFormat, &access, &_positionRectangle.w, &_positionRectangle.h) != 0)
			{
				LOG_ERROR("Failed to query texture information (%s).", SDL_GetError());
				exit(-1);
			}
		}
		
		/** Free allocated resources. */
		virtual ~Texture() {}
		
		/** Render the texture using the main renderer. The provided coordinates indicate texture's top left angle.
		 * @param x X coordinate where to draw the texture on the display.
		 * @param y Y coordinate where to draw the texture on the display.
		 * @return Always 0.
		 */
		virtual int render(int x, int y)
		{
			_positionRectangle.x = x;
			_positionRectangle.y = y;
			
			SDL_RenderCopy(Renderer::pointerRenderer, _pointerSDLTexture, NULL, &_positionRectangle);
			
			return 0;
		}
		
		/** Get the texture width in pixels.
		 * @return The texture width.
		 */
		inline int getWidth()
		{
			return _positionRectangle.w;
		}
		
		/** Get the texture height in pixels.
		 * @return The texture height.
		 */
		inline int getHeight()
		{
			return _positionRectangle.h;
		}
		
		/** Get the SDL texture which can be rendered using SDL API.
		 * @return The SDL texture.
		 */
		inline SDL_Texture *getTexture() // TODO rename in getSDLTexture ?
		{
			return _pointerSDLTexture;
		}
};

#endif
