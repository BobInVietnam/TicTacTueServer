#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player {
private:
    QString name;
    char symbol;
public:
    Player(QString n, char s);

    QString getName();
    char getSymbol();
};

#endif // PLAYER_H
