#include "player.h"

Player::Player(QString n, char s) : name(n), symbol(s) {}

char Player::getSymbol() { return symbol; }

QString Player::getName() { return name; }


