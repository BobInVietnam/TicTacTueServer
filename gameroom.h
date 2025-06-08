#ifndef GAMEROOM_H
#define GAMEROOM_H

#include "game.h"
#include "player.h"

#include <QObject>
#include <QJsonObject>
#include <QList>
#include <QTimer>

class GameRoom : public QObject
{
    Q_OBJECT
public:
    explicit GameRoom(const QString& roomId, QObject *parent = nullptr);
    bool addPlayer(Player* player);
    void removePlayer(Player* player);
    bool isFull() const;

    void receiveMessage(const QJsonObject& json);

signals:
    // Signal to notify the server that the room is empty and can be deleted.
    void roomEmpty(const QString& roomId);

private slots:
    // Slot to handle changes in the game state (win, lose, draw).
    void onGameStateChanged();
    // Slot to handle board updates to notify players.
    void onBoardChanged();

private:
    void broadcastJson(QJsonObject json);
    void assignSymbolsAndStart();
    void processMove(const QJsonObject& json);
    void processChat(const QJsonObject& json);
    void processRematch(const QJsonObject& json);
    char getPlayerSymbol(const Player* player) const;

    QString m_roomId;
    Game m_game;
    QList<Player*> m_players;

    // Track rematch requests
    QMap<Player*, bool> m_rematchVotes;
};

#endif // GAMEROOM_H
