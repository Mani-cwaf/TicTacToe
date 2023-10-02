#pragma once
#include "sdl2/SDL.h"

class Button
{
public:
	// Constructor
	Button();

	// Apply the texture to the button
	void setTexture(SDL_Texture *texture);

	// Set the rect of the button
	void setButtonRect(const SDL_Rect &rect);
	// Center the texture over the button's rect
	void centerTextureRect();

	// Returns true if the mouse is hovering over the button
	bool checkMouseOver(const int x, const int y);

	// Set the color of the button under the texture
	void setBackground(SDL_Color colour);

	// Render the background color of the button to the screen
	void renderBackground(SDL_Renderer *renderer);
	// Render the texture to the screen
	void renderTexture(SDL_Renderer *renderer);

private:
	// Define texture to display
	SDL_Texture *mTexture;

	// Define background colour
	SDL_Color mBackground;

	// Define the position of the top left and the dimensions of the button
	SDL_Rect mButtonRect;

	// Define the position of the top left and the dimensions of the texture
	SDL_Rect mTextureRect;
};