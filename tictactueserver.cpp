#include "tictactueserver.h"
#include "player.h"
#include "gameroom.h"
#include <QDebug>
#include <QJsonDocument>

TicTacTueServer::TicTacTueServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_rng(std::random_device{}()) // Seed the random number generator
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TicTacTueServer::onNewConnection);
}

TicTacTueServer::~TicTacTueServer()
{
    qDeleteAll(m_clients);
    qDeleteAll(m_gameRooms);
}

bool TicTacTueServer::startServer(quint16 port)
{
    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qCritical() << "Server could not start:" << m_tcpServer->errorString();
        return false;
    }
    qInfo() << "Server started on port" << port;
    return true;
}

void TicTacTueServer::onNewConnection()
{
    QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
    if (!clientSocket) return;

    connect(clientSocket, &QTcpSocket::disconnected, this, &TicTacTueServer::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &TicTacTueServer::onReadyRead);

    QString playerId = generateUniqueId(6);
    Player* newPlayer = new Player(clientSocket, playerId);
    m_clients.insert(clientSocket, newPlayer);

    QJsonObject json;
    json["CID"] = playerId;
    json["CMD"] = "A_ID";
    sendJsonToClient(clientSocket, json);

    qInfo() << "Client connected:" << clientSocket->peerAddress().toString() << "assigned ID" << playerId;
}

void TicTacTueServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket || !m_clients.contains(clientSocket)) return;

    Player* player = m_clients.value(clientSocket);
    qInfo() << "Client" << player->id() << "disconnected.";

    // If player was in a room, ensure they are removed
    if (!player->gameId().isEmpty() && m_gameRooms.contains(player->gameId())) {
        m_gameRooms[player->gameId()]->removePlayer(player);
    }

    // Clean up all data associated with this socket
    m_clients.remove(clientSocket);
    m_readBuffers.remove(clientSocket);
    delete player;
    clientSocket->deleteLater();
}

void TicTacTueServer::onReadyRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // TCP is a stream, so we must handle partial messages.
    // A common way is to prefix each message with its size.
    QByteArray& buffer = m_readBuffers[clientSocket];
    buffer.append(clientSocket->readAll());

    while (buffer.size() >= sizeof(quint32)) {
        quint32 messageSize;
        QDataStream sizeStream(buffer.left(sizeof(quint32)));
        sizeStream >> messageSize;

        if (buffer.size() < (qint32)(sizeof(quint32) + messageSize)) {
            // Full message has not arrived yet
            break;
        }

        // Extract the full JSON message
        buffer.remove(0, sizeof(quint32)); // Remove the size prefix
        QByteArray jsonData = buffer.left(messageSize);
        buffer.remove(0, messageSize); // Remove the message data

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
        if (doc.isObject()) {
            qDebug() << "Server RCV from" << m_clients.value(clientSocket)->id() << ":" << doc.object();
            processMessage(clientSocket, doc.object());
        } else {
            qWarning() << "Invalid JSON from" << clientSocket->peerAddress() << ":" << parseError.errorString();
        }
    }
}

void TicTacTueServer::processMessage(QTcpSocket* clientSocket, const QJsonObject& json)
{
    if (json.value("INGAME").toBool()) {
        QString roomId = json.value("RID").toString();
        if (m_gameRooms.contains(roomId)) {
            m_gameRooms[roomId]->receiveMessage(json);
        } else {
            qWarning() << "Server: Received INGAME message for non-existent room" << roomId;
        }
    } else {
        QString command = json.value("CMD").toString();
        if (command == "CR") {
            playerCreateRoom(clientSocket, json);
        } else if (command == "JR") {
            playerJoinRoom(clientSocket, json);
        } else if (command == "LR") {
            playerLeaveRoom(clientSocket, json);
        } else {
            // Other out-of-game commands like set username can be added here
        }
    }
}

void TicTacTueServer::playerCreateRoom(QTcpSocket* clientSocket, const QJsonObject& json)
{
    Player* player = m_clients.value(clientSocket);
    if (!player) return;

    QString roomId = json.value("RID").toString();
    QJsonObject response;
    response["CID"] = player->id();
    response["INGAME"] = false;

    if (m_gameRooms.contains(roomId)) {
        response["CMD"] = "ERR";
        response["MSG"] = "Room already exists.";
        qWarning() << "Player" << player->id() << "failed to create room" << roomId << "- already exists.";
    } else {
        GameRoom* newRoom = new GameRoom(roomId, this);
        connect(newRoom, &GameRoom::roomEmpty, this, &TicTacTueServer::onRoomEmptied);
        m_gameRooms.insert(roomId, newRoom);
        newRoom->addPlayer(player); // Player automatically joins the room they create

        response["CMD"] = "CR_OK";
        response["RID"] = roomId;
        qInfo() << "Player" << player->id() << "created room" << roomId;
    }
    sendJsonToClient(clientSocket, response);
}

void TicTacTueServer::playerJoinRoom(QTcpSocket* clientSocket, const QJsonObject& json)
{
    Player* player = m_clients.value(clientSocket);
    if (!player) return;

    QString roomId = json.value("RID").toString();
    QJsonObject response;
    response["CID"] = player->id();
    response["INGAME"] = false;

    if (m_gameRooms.contains(roomId) && !m_gameRooms[roomId]->isFull()) {
        response["CMD"] = "JR_OK";
        response["RID"] = roomId;
        qInfo() << "Player" << player->id() << "joined room" << roomId;
        sendJsonToClient(clientSocket, response);

        m_gameRooms[roomId]->addPlayer(player);
    } else {
        response["CMD"] = "ERR";
        response["MSG"] = "Can't join room. It might be full or non-existent.";
        qWarning() << "Player" << player->id() << "failed to join room" << roomId;
        sendJsonToClient(clientSocket, response);
    }
}

void TicTacTueServer::playerLeaveRoom(QTcpSocket* clientSocket, const QJsonObject& json)
{
    Player* player = m_clients.value(clientSocket);
    if (!player) return;

    QString roomId = json.value("RID").toString();
    if (player->gameId() == roomId && m_gameRooms.contains(roomId)) {
        m_gameRooms[roomId]->removePlayer(player);
    } else {
        qWarning() << "Player" << player->id() << "tried to leave a room they are not in:" << roomId;
    }
}

void TicTacTueServer::onRoomEmptied(const QString& roomId)
{
    if (m_gameRooms.contains(roomId)) {
        qInfo() << "Room" << roomId << "is now empty and being deleted.";
        GameRoom* room = m_gameRooms.take(roomId);
        delete room; // Or room->deleteLater(); for safety
    }
}

QString TicTacTueServer::generateUniqueId(int length)
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> dist(0, chars.length() - 1);
    QString id;
    id.reserve(length);
    do {
        id.clear();
        for (int i = 0; i < length; ++i) {
            id.append(chars.at(dist(m_rng)));
        }
    } while (m_gameRooms.contains(id)); // Check for room ID collisions
    return id;
}

void TicTacTueServer::sendJsonToClient(QTcpSocket* client, const QJsonObject& json)
{
    if (!client || client->state() != QAbstractSocket::ConnectedState) {
        return;
    }
    QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)jsonData.size(); // Prefix with size
    block.append(jsonData);
    client->write(block);
    qDebug() << "Server SENT to" << client << ":" << json;
}
