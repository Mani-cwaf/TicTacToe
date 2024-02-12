// get elements from DOM
const CELLS = [...document.querySelectorAll(".cell")]  as HTMLElement[];
const resultEl: Element = document.querySelector('.result') as HTMLElement;
const gameTypeEl = document.querySelector('.dropdown')  as HTMLSelectElement;

// declare constants

class Cell {
    status: string | null;
    constructor() {
        this.status = null;
    }
}

class Board {
    cells: Cell[];
    constructor() {
        this.cells = [];
        // adds 9 instances of the Cell class to an array.
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                this.cells.push(new Cell());
            }
        }
    }
}

// sets an enum for none, circle, and cross for easy refactoring
const PLAYERS = {
    none: "n",
    cross: "x",
    circle: "o"
}

// declare variables
let board: Board;
let playerMove = PLAYERS['circle'];

let game = true;
let gameStarted = false;

let gameType = gameTypeEl.value;
gameTypeEl.addEventListener('change', () => {
    if (gameStarted) {
        endGame(PLAYERS['none']);
        board = new Board();
        game = true;
        gameStarted = false;
        // reload textures on screen.
        resultEl.innerHTML = '';
        reloadAssets();
    } 
    gameType = gameTypeEl.value;
})


// GETTERS

// finds the winner or draw, else returns false
function winCheck(state: number[][] | any[][]) {
    for (let play = 0; play < 2; play++) {
        let player = play == 0 ? PLAYERS['cross'] : PLAYERS['circle'];
        //check all rows.
        for (let row = 0; row < state.length; row++) {
            // Assume winner.
            let win = true;
            // Check if no winner.
            for (let col = 0; col < state.length; col++) // Check cells in all three columns in a row.
            {
                if (state[row][col] != player) {
                    win = false;
                }
            }
            if (win) {
                return player;
            }
        }
        //check all columns.
        for (let col = 0; col < state.length; col++) {
            // Assume winner.
            let win = true;
            // Check if no winner.
            for (let row = 0; row < state.length; row++) // Check cells in all three columns in a row.
            {
                if (state[row][col] != player) {
                    win = false;
                }
            }
            if (win) {
                return player;
            }
        }

        // Check Diagonals.
        for (let diags = 0; diags < 2; diags++) {
            // Assume winner.
            let win = true;
            // Check if no winner.
            let cell = diags == 0 ? [0, 0] : [0, 2];
            for (let i = 0; i < 3; i++) // Check cells in all three cells in a diagonal.
            {
                if (state[cell[0]][cell[1]] != player) {
                    win = false;
                }
                cell[0] += 1;
                cell[1] += diags == 0 ? 1 : -1;
            }
            if (win) {
                return player;
            }
        }
    }

    // if no more moves left but no winner, return none
    if (movesLeft(state)) {
        return PLAYERS['none'];
    }

    return false;
}
function movesLeft(state: number[][] | any[][]) {
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            if (state[i][j] == null)
                return false;
        }
    }
    return true;
}

// returns a 2d array with the current status of all the cells on the board.
function getState(): number[][] | any[][] {
    let state: number[][] = [];
    let index = 0;
    // loops through the rows in the board
    for (let i = 0; i < 3; i++) {
        // creates a sub-array for the cells in the different colums in a row
        let temp: any[] = [];
        for (let j = 0; j < 3; j++) {
            // adds the status of a cell to the row
            temp.push(board.cells[index].status);
            index++;
        }
        state.push(temp); // adds all the arrays for the rows to create a 2d array.
    }
    return state;
}

// SETTERS

// reloads textures of cells.
function reloadAssets() {
    let l = 0;
    // loop through all cells and assign the image associated with their current status.
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            let imgEl: HTMLImageElement = [...CELLS[l].children][0] as HTMLImageElement
            if (board.cells[l].status == PLAYERS['cross']) {
                imgEl.src = 'cross.svg';
            } else if (board.cells[l].status == PLAYERS['circle']) {
                imgEl.src = 'circle.svg';
            } else {
                imgEl.src = 'blank.svg';
            }
            l++;
        }
    }
}

// displays text on screen and ends game.
function endGame(winner: string | boolean): boolean {
    let winnerResult: string = '';
    if (winner === PLAYERS['cross']) {
        winnerResult = "Cross Won!";
    } else if (winner === PLAYERS['circle']) {
        winnerResult = "Circle Won!";
    } else if (winner === PLAYERS['none']) {
        winnerResult = 'Draw!';
    }
    resultEl.innerHTML = winnerResult;
    game = false;
    return true;
}

// reload textures and check if there is a winner.
function endTurn(): boolean {
    reloadAssets();

    let state = getState();
    let win = winCheck(state);
    if (win) {
        return endGame(win); // runs endGame when win is not false
    }
    return false;
}


function setup() {
    // adds event listeners to start the round when a cell is clicked.
    let index = 0;
    CELLS.forEach((cell) => {
        let currentIndex = index;
        cell.addEventListener('click', () => {
            play(currentIndex);
        });
        index++;
    })
    // sets the current board to a new instance of the Board class.
    board = new Board();
}
setup();

// runs when player clicks, executes player and computer turns.
function play(cellIndex: number) {
    // if game ends, reset board.
    if (!game) {
        board = new Board();
        game = true;
        gameStarted = false;
        // reload textures on screen.
        resultEl.innerHTML = '';
        reloadAssets();
        // return the function so the player has to click again.
        return;
    }

    gameStarted = true

    if (gameType == 'ai') {
        playerMove = PLAYERS['circle'];
        // make player move.
        if (board.cells[cellIndex].status == null) {
            // sets cell to player's chosen side if the cell is empty.
            board.cells[cellIndex].status = playerMove;
        } else {
            return; // if the cell is occupied, stop the function and let the player try again.
        }
        endTurn();

        // make computer move.
        let newState = MiniMax(getState(), true, 0, -99999, 99999)[1]; // take the optimal state for this turn from the minimax algorithm.
        let index = 0; // loop through all cells and match them with optimal state.
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                board.cells[index].status = newState[i][j];
                index++;
            }
        }
        endTurn();
    } else {
        // make player move.
        if (board.cells[cellIndex].status == null) {
            // sets cell to player's chosen side if the cell is empty.
            board.cells[cellIndex].status = playerMove;
            playerMove = playerMove == PLAYERS['circle'] ? PLAYERS['cross'] : PLAYERS['circle']; // get opposite of playerMove
        } else {
            return; // if the cell is occupied, stop the function and let the player try again.
        }
        endTurn();
    }
}

// I got the minimax algorithm from https://github.com/rinovethamoses97

function MiniMax(state: number[][] | any[][], max: boolean, depth: number, alpha: number, beta: number) {
    // the player will try to get a lower score.
    let minimiser = playerMove;

    // the computer will try to get a higher score.
    let maximiser = playerMove == PLAYERS['circle'] ? PLAYERS['cross'] : PLAYERS['circle']; // get opposite of playerMove

    // once the game ends, return who won, give the state that allowed the game to end in that way, and the depth at which it ended.
    let win = winCheck(state);
    if (win) {
        if (win === minimiser) {
            return [-10, state, depth];
        }
        else if (win === maximiser) {
            return [10, state, depth];
        }
        else if (win === PLAYERS['none']) {
            return [0, state, depth];
        }
    }

    // maximiser.
    if (max) {
        let maxState;
        let maxScore = -9999;
        // try all the available cells in the current state.
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                if (state[i][j] == null) {
                    // prune sub-optimal paths.
                    if (beta <= alpha) {
                        continue; // end attempt for current cell calculation
                    }
                    // test the cell with the maximiser's side.
                    state[i][j] = maximiser;

                    let temp = MiniMax(state, !max, depth + 1, alpha, beta); // test all possible moves that can be done on the minimiser's side, then loop back and forth until the game is over.

                    // if the game ends better than the previous best (evaluated winner score - depth), this will be the new optimal move.
                    if ((temp[0] - temp[2]) > maxScore) {
                        maxScore = temp[0] - temp[2];
                        alpha = maxScore;
                        maxState = JSON.parse(JSON.stringify(state));
                    }
                    // reset the cell
                    state[i][j] = null;
                }
            }
        }
        return [maxScore, maxState, depth];
    }
    else {
        let minState;
        let minScore = 9999;
        for (let i = 0; i < 3; i++) {
            for (let j = 0; j < 3; j++) {
                if (state[i][j] == null) {
                    // prune best paths.
                    if (beta <= alpha) {
                        continue; // end attempt for current cell calculation
                    }
                    // test the cell with the minimiser's side.
                    state[i][j] = minimiser;

                    let temp = MiniMax(state, !max, depth + 1, alpha, beta); // test all possible moves that can be done on the maximiser's side, then loop back and forth until the game is over.

                    // if the game ends worse than the previous worst (evaluated winner score + depth), this will be the new optimal move to reduce score.
                    if ((temp[0] + temp[2]) < minScore) {
                        minScore = temp[0] + temp[2];
                        beta = minScore;
                        minState = JSON.parse(JSON.stringify(state));
                    }
                    // reset the cell.
                    state[i][j] = null;
                }
            }
        }
        return [minScore, minState, depth];
    }
}