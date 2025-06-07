#ifndef GAMEROOM_H
#define GAMEROOM_H

#include "game.h"

#include <QObject>
#include <QTcpSocket>

class GameRoom : public QObject
{
    Q_OBJECT
public:
    GameRoom();
    void setPlayer(QTcpSocket *);
    void removePlayer(QTcpSocket *);
public slots:
    void receiveMessage(const QJsonObject&);
signals:
private:
    Game game;
    QList<QTcpSocket*> players;

};

#endif // GAMEROOM_H
