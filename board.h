#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <QString>
#include <vector>

class Board {
private:
    std::vector<std::vector<char>> grid;
public:
    Board();
    void clear();
    void display();
    bool placeMark(int row, int col, bool xMove);
    bool isFull();
    char checkWinner();
    std::string getSequence() const;
    void setSequence(QString seq);
};

#endif // BOARD_H
