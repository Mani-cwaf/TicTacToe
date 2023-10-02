#pragma once
#include "sdl2/SDL.h"

enum class State
{
    NONE = 0,
    CROSS = 1,
    CIRCLE = -1,
    INF = 9999,
    NINF = -9999,
};

class Cell;

// Define window dimensions
const int windowWidth = 500;
const int windowHeight = windowWidth;

// Create window
extern SDL_Window *window;

// Create renderer
extern SDL_Renderer *renderer;

static const State initialPlayer = State::CIRCLE;
static State botTurn = initialPlayer == State::CIRCLE ? State::CROSS : State::CIRCLE;
static State player = initialPlayer;
static State winner = State::NONE;
static int cellsFilled = 0;

// Create 3 x 3 grid of Cells
static const int rows = 3;
static const int cols = rows;
static const int totalCells = rows * cols;

const SDL_Color lightBlue = {0, 130, 255, 255};
const SDL_Color red = {255, 0, 0, 255};

// Load texture from an image file
SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filePath);

// Load textures for (X), (O), and an empty cell
extern SDL_Texture *cross;
extern SDL_Texture *circle;
extern SDL_Texture *cell;

// Get index of a grid
inline int getIndex(const int cols, const int row, const int col)
{
    return cols * row + col;
}

/* Division of game logic across multiple functions */

// Creates the initial empty cells
void spawnCells();

// End the game
void endGame(SDL_Event event);

// Check all cell combinations for a win
void checkWin();

// Runs code to change cell to player and change player
void fillCell(int i);

// Use the minimax recursive algorithm to find the best move for a specific cell
State miniMax(Cell g[totalCells], int depth, State side);

// Handle the turn based on bot's decision
void handleBotTurn();

// Handle the turn based on mouse inputs of the player
void handleTurn(SDL_Event event);

// Decide which turn code to run
void startTurn(SDL_Event event);

// Adds all textures to rendering backbuffer
void renderAll();