#include "board.h"

Board::Board() : grid(3, std::vector<char>(3, ' ')) {}

void Board::clear()
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            grid[i][j] = ' ';
    }
    std::cout << "Board cleared";
}

void Board::display()
{
    std::cout << "\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << " ";
        for (int j = 0; j < 3; ++j) {
            std::cout << grid[i][j];
            if (j < 2) std::cout << " | ";
        }
        std::cout << "\n";
        if (i < 2) std::cout << "---+---+---\n";
    }
    std::cout << "\n";
}

bool Board::placeMark(int row, int col, bool xMove)
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3 || grid[row][col] != ' ') {
        return false;
    }
    grid[row][col] = xMove? 'x' : 'o';
    return true;
}

bool Board::isFull()
{
    for (auto& row : grid)
        for (char cell : row)
            if (cell == ' ') return false;
    return true;
}

char Board::checkWinner()
{
    // Rows & columns
    for (int i = 0; i < 3; ++i) {
        if (grid[i][0] != ' ' && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
            return grid[i][0];
        if (grid[0][i] != ' ' && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
            return grid[0][i];
    }
    // Diagonals
    if (grid[0][0] != ' ' && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
        return grid[0][0];
    if (grid[0][2] != ' ' && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
        return grid[0][2];

    return ' '; // No winner
}

std::string Board::getSequence() const
{
    std::string s = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            s.push_back(grid[i][j]);
        }
    }
    return s;
}

void Board::setSequence(QString seq)
{
    if (seq.length() != 9) {
        throw new std::out_of_range("Board sequence not in correct length");
    }
    for (auto ch : seq) {
        char c = ch.toLatin1();
        if (c != 'x' && c != 'o' && c != ' ') {
            throw new std::logic_error("Invalid character in board sequence");
        }
    }
    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            grid[i][j] = seq.at(index).toLatin1();
            index++;
        }
    }
}
