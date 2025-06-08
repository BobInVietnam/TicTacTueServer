#include "gameroom.h"
#include <QDebug>
#include <QJsonDocument>

GameRoom::GameRoom(const QString& roomId, QObject *parent)
    : QObject(parent), m_roomId(roomId)
{
    // Connect signals from the internal Game object to this room's slots.
    // This allows the GameRoom to react to game events and notify m_players.
    connect(&m_game, &Game::gsChanged, this, &GameRoom::onGameStateChanged);
    connect(&m_game, &Game::boardChanged, this, &GameRoom::onBoardChanged);
}

bool GameRoom::addPlayer(Player* player)
{
    if (m_players.length() >= 2) {
        return false; // Room is full
    }
    m_players.append(player);
    player->setGameId(m_roomId);
    m_rematchVotes[player] = false;

    qInfo() << "Player" << player->id() << "joined room" << m_roomId;

    if (m_players.length() == 2) {
        assignSymbolsAndStart();
    }
    return true;
}

void GameRoom::removePlayer(Player* player)
{
    m_players.removeAll(player);
    m_rematchVotes.remove(player);
    player->setGameId(""); // Clear the player's gameId
    qInfo() << "Player" << player->id() << "left room" << m_roomId;

    // If the game was in progress, notify the remaining player.
    if (!m_players.isEmpty()) {
        QJsonObject opponentLeftMsg;
        opponentLeftMsg["CID"] = m_players.first()->id();
        opponentLeftMsg["CMD"] = "OPP_LEFT";
        opponentLeftMsg["INGAME"] = true;
        opponentLeftMsg["RID"] = m_roomId;
        m_players.first()->sendMessage(opponentLeftMsg);
        m_game.reset();
    }

    if (m_players.empty()) {
        // Notify the server to delete this room after a short delay
        QTimer::singleShot(0, [this]() { emit roomEmpty(m_roomId); });
    }
}

bool GameRoom::isFull() const
{
    return m_players.length() >= 2;
}

void GameRoom::assignSymbolsAndStart()
{
    if (m_players.length() != 2) return;

    qInfo() << "Two m_players in room" << m_roomId << ". Assigning symbols and starting game.";
    m_game.reset();

    // Assign X to the first player, O to the second
    for (int i = 0; i < m_players.length(); ++i) {
        Player* player = m_players.at(i);
        char symbol = (i == 0) ? 'X' : 'O';

        QJsonObject json;
        json["CID"] = player->id();
        json["CMD"] = "ASN";
        json["SYM"] = QString(symbol);
        json["INGAME"] = true;
        json["RID"] = m_roomId;
        player->sendMessage(json);
    }

    // Broadcast initial board state
    onBoardChanged();
}

void GameRoom::receiveMessage(const QJsonObject& json)
{
    QString command = json.value("CMD").toString();
    if (command == "MOVE") {
        processMove(json);
    } else if (command == "REMATCH") {
        processRematch(json);
    } else if (command == "CHAT") {
        processChat(json);
    } else {
        qWarning() << "GameRoom" << m_roomId << "received unknown command:" << command;
    }
}

void GameRoom::processMove(const QJsonObject& json)
{
    QString playerId = json.value("CID").toString();
    Player* currentPlayer = nullptr;
    for (Player* p : m_players) {
        if (p->id() == playerId) {
            currentPlayer = p;
            break;
        }
    }

    if (!currentPlayer) {
        qWarning() << "Move request from unknown player" << playerId;
        return;
    }

    bool isXTurn = m_game.getXTurn();
    char playerSymbol = getPlayerSymbol(currentPlayer);

    if ((isXTurn && playerSymbol == 'X') || (!isXTurn && playerSymbol == 'O')) {
        int index = json.value("AT").toInt(-1);
        if (index >= 0 && index <= 8) {
            m_game.move(index / 3, index % 3);
            // The onBoardChanged and onGameStateChanged signals will handle broadcasting
        } else {
            qWarning() << "Invalid move index:" << index;
        }
    } else {
        // It's not this player's turn, send an error or ignore
        qWarning() << "Player" << playerId << "tried to move out of turn.";
        // Optionally send a "NOT_YOUR_TURN" message back
    }
}

void GameRoom::processChat(const QJsonObject& json)
{
    QString playerId = json.value("CID").toString();
    Player* currentPlayer = nullptr;
    for (Player* p : m_players) {
        if (p->id() == playerId) {
            currentPlayer = p;
            break;
        }
    }

    QJsonObject jsonData;
    jsonData["RID"] = m_roomId;
    jsonData["CMD"] = json.value("CMD");
    jsonData["MSG"] = currentPlayer->name() + ": " + json.value("MSG").toString();
    broadcastJson(jsonData);
}

void GameRoom::processRematch(const QJsonObject& json)
{
    QString playerId = json.value("CID").toString();
    Player* votingPlayer = nullptr;
    for (Player* p : m_players) {
        if (p->id() == playerId) {
            votingPlayer = p;
            break;
        }
    }

    if (!votingPlayer) return;

    m_rematchVotes[votingPlayer] = true;
    qInfo() << "Player" << playerId << "voted for a rematch in room" << m_roomId;

    // Check if all m_players have voted for a rematch
    bool allVoted = true;
    if (m_players.length() < 2) {
        allVoted = false; // Can't rematch alone
    } else {
        for (bool voted : m_rematchVotes.values()) {
            if (!voted) {
                allVoted = false;
                break;
            }
        }
    }

    if (allVoted) {
        qInfo() << "All m_players agreed to a rematch in room" << m_roomId << ". Resetting game.";
        // Reset votes for next round
        for (Player* p : m_players) {
            m_rematchVotes[p] = false;
        }
        // Restart the game. It will reset the board and broadcast the new state.
        assignSymbolsAndStart();
    }
}

void GameRoom::onBoardChanged()
{
    // This slot is connected to the game's boardChanged signal.
    // We can use it to broadcast the state whenever a move is made.
    onGameStateChanged(); // Consolidate broadcasting logic here
}

void GameRoom::onGameStateChanged()
{
    QJsonObject updateJson;
    updateJson["INGAME"] = true;
    updateJson["RID"] = m_roomId;
    updateJson["CMD"] = "UPD";
    updateJson["SEQ"] = m_game.getBoardSeq();
    updateJson["X_T"] = m_game.getXTimer()->currentTime(); // Assuming CountdownTimer has this method
    updateJson["O_T"] = m_game.getOTimer()->currentTime(); // Assuming CountdownTimer has this method

    QString gameStateChar = "N"; // N for "Neither" (ongoing)
    switch (m_game.gs()) {
    case GameState::XWON: gameStateChar = "X"; break;
    case GameState::OWON: gameStateChar = "O"; break;
    case GameState::DRAW: gameStateChar = "D"; break;
    default: break;
    }
    updateJson["GS"] = gameStateChar;

    broadcastJson(updateJson);
}

void GameRoom::broadcastJson(QJsonObject json)
{
    for (Player* player : m_players) {
        json["CID"] = player->id();
        player->sendMessage(json);
    }
}

char GameRoom::getPlayerSymbol(const Player* player) const
{
    int playerIndex = m_players.indexOf(const_cast<Player*>(player));
    if (playerIndex == 0) return 'X';
    if (playerIndex == 1) return 'O';
    return ' '; // Should not happen
}
