#include "../include/button.hpp"
#include "../include/game.hpp"
#include "sdl2/SDL_image.h"
#include <algorithm>
#include <iterator>

SDL_Window *window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

SDL_Texture *cross = loadTexture(renderer, "res/cross.png");
SDL_Texture *circle = loadTexture(renderer, "res/circle.png");
SDL_Texture *cell = loadTexture(renderer, "res/cell.png");

// Each cell is a button and has a state
class Cell : public Button
{
public:
    State state;
    Cell() : state(State::NONE) {}
};

static Cell grid[totalCells];

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedSurface = nullptr;

    texture = IMG_LoadTexture(renderer, filePath);

    SDL_FreeSurface(loadedSurface);
    return texture;
}

void spawnCells()
{
    // Define borders and thus dimensions of cells
    const int borderThickness = 10;
    const int buttonWidth = (windowWidth - 4 * borderThickness) / cols;
    const int buttonHeight = buttonWidth;

    // Set all cell starting positions, dimensions and state
    int cellStartY = borderThickness;
    for (int row = 0; row < rows; row++)
    {
        int cellStartX = borderThickness;
        for (int col = 0; col < cols; col++)
        {
            // Get index
            int index = getIndex(cols, row, col);

            // Set starting position and dimensions
            SDL_Rect rect = {cellStartX, cellStartY, buttonWidth, buttonHeight};
            grid[index].setButtonRect(rect);

            // Set to empty texture and center each texture onto button
            grid[index].setTexture(cell);
            grid[index].centerTextureRect();

            cellStartX += buttonWidth + borderThickness;
        }
        cellStartY += buttonHeight + borderThickness;
    }
}

void endGame(SDL_Event event)
{
    player = initialPlayer;

    // Wait until another mouse press to reset grid
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        // Clear cells
        for (int i = 0; i < totalCells; i++)
        {
            grid[i].state = State::NONE;
            grid[i].setTexture(cell);
            grid[i].setBackground({255, 255, 255, 255});
        }

        winner = State::NONE;
        cellsFilled = 0;
    }
}

void checkWin()
{
    // Choose background colour to fill for winning condition based on player
    SDL_Color background = player == State::CIRCLE ? red : lightBlue;

    // Check each row
    if (grid[0].state == player && grid[1].state == player && grid[2].state == player)
    {
        grid[0].setBackground(background);
        grid[1].setBackground(background);
        grid[2].setBackground(background);
        winner = player;
    }
    else if (grid[3].state == player && grid[4].state == player && grid[5].state == player)
    {
        grid[3].setBackground(background);
        grid[4].setBackground(background);
        grid[5].setBackground(background);
        winner = player;
    }
    else if (grid[6].state == player && grid[7].state == player && grid[8].state == player)
    {
        grid[6].setBackground(background);
        grid[7].setBackground(background);
        grid[8].setBackground(background);
        winner = player;
    }

    // Check each col
    else if (grid[0].state == player && grid[3].state == player && grid[6].state == player)
    {
        grid[0].setBackground(background);
        grid[3].setBackground(background);
        grid[6].setBackground(background);
        winner = player;
    }
    else if (grid[1].state == player && grid[4].state == player && grid[7].state == player)
    {
        grid[1].setBackground(background);
        grid[4].setBackground(background);
        grid[7].setBackground(background);
        winner = player;
    }
    else if (grid[2].state == player && grid[5].state == player && grid[8].state == player)
    {
        grid[2].setBackground(background);
        grid[5].setBackground(background);
        grid[8].setBackground(background);
        winner = player;
    }

    // Check diagonals
    else if (grid[0].state == player && grid[4].state == player && grid[8].state == player)
    {
        grid[0].setBackground(background);
        grid[4].setBackground(background);
        grid[8].setBackground(background);
        winner = player;
    }
    else if (grid[2].state == player && grid[4].state == player && grid[6].state == player)
    {
        grid[2].setBackground(background);
        grid[4].setBackground(background);
        grid[6].setBackground(background);
        winner = player;
    }
}

void fillCell(int i)
{
    // Set the state to the player whose turn it is and increment it as a filled cell
    grid[i].state = player;
    cellsFilled++;

    checkWin();

    // Switch players and set textures
    if (player == State::CIRCLE)
    {
        // Set circle texture and switch player to cross
        grid[i].setTexture(circle);
        player = winner == State::NONE ? State::CROSS : player;
    }
    else
    {
        // Set cross texture and switch player to circle
        grid[i].setTexture(cross);
        player = winner == State::NONE ? State::CIRCLE : player;
    }
}

State miniMax(Cell g[totalCells], int depth, State side)
{
    checkWin();
    if (winner != State::NONE || (winner == State::NONE && cellsFilled == totalCells))
    {
        State score = winner;
        return score;
    }

    if (side == State::CROSS)
    {
        State bestScore = State::NINF;
        for (int i = 0; i < totalCells; i++)
        {
            if (g[i].state == State::NONE)
            {
                g[i].state = State::CROSS;
                State score = miniMax(g, depth + 1, State::CIRCLE);
                g[i].state = State::NONE;
                checkWin();
                bestScore = (int)score > (int)bestScore ? score : bestScore;
            }
        }
        return bestScore;
    }
    else
    {
        State bestScore = State::INF;
        for (int i = 0; i < totalCells; i++)
        {
            if (g[i].state == State::NONE)
            {
                g[i].state = State::CIRCLE;
                State score = miniMax(g, depth + 1, State::CROSS);
                g[i].state = State::NONE;
                checkWin();
                bestScore = (int)score < (int)bestScore ? score : bestScore;
            }
        }
        return bestScore;
    }
}

void handleBotTurn()
{
    State bestScore = State::NINF;
    int bestMove;
    
    for (int i = 0; i < totalCells; i++)
    {
        if (grid[i].state == State::NONE)
        {
            grid[i].state = State::CROSS;
            State score = miniMax(grid, 0, State::CIRCLE);
            grid[i].state = State::NONE;
            checkWin();
            if ((int)score > (int)bestScore)
            {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    fillCell(bestMove);
}

void handleTurn(SDL_Event event)
{
    // Get mouse position and handle mouse events
    int x, y;
    SDL_GetMouseState(&x, &y);
    for (int i = 0; i < totalCells; i++)
    {
        // If cell is empty
        if (grid[i].state == State::NONE)
        {
            // If mouse pressed while also being over the cell
            if (event.type == SDL_MOUSEBUTTONDOWN && grid[i].checkMouseOver(x, y))
            {
                fillCell(i);
            }
        }
    }
}

void startTurn(SDL_Event event)
{
    // If there is a winner or all cells are filled
    if (winner != State::NONE || (winner == State::NONE && cellsFilled == totalCells))
    {
        // End the game
        endGame(event);
    }
    else
    {
        // If it is the bot's turn to play
        if (player == botTurn)
        {
            // Play the bot's move
            handleBotTurn();
        }
        else
        {
            // Otherwise, continue procressing turn for player
            handleTurn(event);
        }
    }
}

void renderAll()
{
    // Render background and texture of every button
    for (int i = 0; i < totalCells; i++)
    {
        grid[i].renderBackground(renderer);
        grid[i].renderTexture(renderer);
    }
}