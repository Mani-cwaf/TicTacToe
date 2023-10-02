#include "../include/button.hpp"

// Define default values
Button::Button() : mTexture(nullptr),
				   mBackground({255, 255, 255, 255}),
				   mButtonRect({0, 0, 0, 0}),
				   mTextureRect({0, 0, 0, 0})
{
}

void Button::setTexture(SDL_Texture *texture)
{
	mTexture = texture;
}

void Button::setButtonRect(const SDL_Rect &rect)
{
	mButtonRect = rect;
}

void Button::setBackground(SDL_Color colour)
{
	mBackground = colour;
}

void Button::centerTextureRect()
{
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(mTexture, NULL, NULL, &textureWidth, &textureHeight);

	// Create margins for the texture based the difference of it's rect and the button's rect
	const int textureStartRow = mButtonRect.y + ((mButtonRect.h - textureHeight) / 2);
	const int textureStartCol = mButtonRect.x + ((mButtonRect.w - textureWidth) / 2);

	mTextureRect = {textureStartCol, textureStartRow, textureWidth, textureHeight};
}

bool Button::checkMouseOver(const int x, const int y)
{
	// Assume mouse is not over button
	bool inside = false;

	// Check if the mouse location is above the start of the x and y of the button, and below the end of the x and y of the button
	if (x > mButtonRect.x &&
		x < mButtonRect.x + mButtonRect.w && // The starting x plus the width is the end of the button x
		y > mButtonRect.y &&
		y < mButtonRect.y + mButtonRect.h) // The starting y plus the height is the end of the button y
	{
		inside = true;
	}

	return inside;
}

void Button::renderBackground(SDL_Renderer *renderer)
{
	// Set rendering colour for background
	SDL_SetRenderDrawColor(renderer, mBackground.r, mBackground.g, mBackground.b, mBackground.a);
	SDL_RenderFillRect(renderer, &mButtonRect);
}

void Button::renderTexture(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, mTexture, nullptr, &mTextureRect);
}
