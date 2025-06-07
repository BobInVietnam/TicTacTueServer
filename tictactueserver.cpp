#include "tictactueserver.h"
#include <QDebug>

TicTacTueServer::TicTacTueServer(QObject *parent)
    : QObject(parent), m_tcpServer(nullptr)
{
    initializeGame();
}

bool TicTacTueServer::startServer(quint16 port)
{
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TicTacTueServer::onNewConnection);

    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qCritical() << "Server could not start:" << m_tcpServer->errorString();
        return false;
    }
    qInfo() << "Server started on port" << port;
    return true;
}

void TicTacTueServer::initializeGame()
{
    game.reset();
}

void TicTacTueServer::processMessage(QTcpSocket* clientSocket, const QJsonObject &json)
{
    QString type = json.value("type").toString();
    if (type == "START") {

    } else if (type == "MOVE") {
        int pos = json.value("pos").toInt();
        handleMove(clientSocket, pos);
    }
}

void TicTacTueServer::onNewConnection()
{
    QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
    m_clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TicTacTueServer::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &TicTacTueServer::onReadyRead);

    QJsonObject json;
    json["game"] = false;
    json["type"] = "assid";
    json["id"] = "123456";
    sendToClient(clientSocket, QJsonDocument(json).toJson(QJsonDocument::Compact));

    qInfo() << "Client connected:" << clientSocket->peerAddress().toString();
}

void TicTacTueServer::assignSymbolsAndStart()
{
    if (m_clients.size() == 2) {
        initializeGame(); // Reset board for new game
        qInfo() << "Two players connected. Game starting. X goes first.";
        // sendToClient(m_clients[0], "ASSIGN:X"); // Already sent on connection
        // sendToClient(m_clients[1], "ASSIGN:O");
        QJsonObject json;
        sendToClient(m_clients[0], QJsonDocument().toJson());
        sendToClient(m_clients[1], QJsonDocument().toJson());

        QString boardStateStr = game.getBoardSeq();
        json["type"] = "BOARD";
        json["seq"] = boardStateStr;
        broadcastMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
        game.setGs(GameState::STARTED);
    }
}


void TicTacTueServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qInfo() << "Client disconnected:" << clientSocket->peerAddress().toString();
    m_clients.removeAll(clientSocket);
    m_playerinfos.erase(clientSocket);
    clientSocket->deleteLater();

    if (game.gs() == GameState::STARTED) { // If game was in progress
        initializeGame(); // Reset game
        QTcpSocket* remainingPlayer = getOpponent(nullptr); // Gets the only player left, if any
        if(remainingPlayer){
            sendToClient(remainingPlayer, QJsonDocument().toJson());
            sendToClient(remainingPlayer, QJsonDocument().toJson()); // Put remaining player back into waiting state if they want to play again
        }
        qInfo() << "Opponent left. Game reset.";
    } else if (m_clients.size() == 1) {
        // If one was waiting and the other disconnects before game start, or one disconnects and one remains.
        sendToClient(m_clients[0], QJsonDocument().toJson());
    }
}

void TicTacTueServer::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    QByteArray& buffer = m_socketReadBuffers[clientSocket];
    quint32& currentBlockSize = m_socketBlockSizes[clientSocket];

    while (clientSocket->bytesAvailable() > 0) {
        buffer.append(clientSocket->readAll());

        while (buffer.size() > 0) {
            // Stage 1: Read block size
            if (currentBlockSize == 0 && buffer.size() >= (qint32)sizeof(quint32)) {
                QDataStream in(&buffer, QIODevice::ReadOnly);
                in.setVersion(QDataStream::Qt_6_8); // MUST match client's QDataStream version

                in >> currentBlockSize;
                buffer = buffer.mid(sizeof(quint32)); // Remove size prefix from buffer
            }

            // Stage 2: Read actual block data
            if (currentBlockSize > 0 && buffer.size() >= (qint32)currentBlockSize) {
                QByteArray jsonData = buffer.mid(0, currentBlockSize);
                buffer = buffer.mid(currentBlockSize); // Remove processed data

                QJsonParseError parseError;
                QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

                if (doc.isNull() || !doc.isObject()) {
                    // sendFramedJsonMessage(clientSocket, QJsonDocument(QJsonObject{{"type", "error"}, {"message", "Invalid JSON format: " + parseError.errorString()}}).toJson(QJsonDocument::Compact));
                    qWarning() << "Received invalid JSON from" << clientSocket->peerAddress().toString() << ":" << parseError.errorString();
                } else {
                    qDebug() << "GM: Received JSON from " << clientSocket->peerAddress().toString() << ": " << doc;
                    processMessage(clientSocket, doc.object());
                }
                currentBlockSize = 0; // Reset for next message
            } else {
                // Not enough data for the full message yet, break and wait for more
                break;
            }
        }
    }
    // if (!clientSocket) return;

    // QByteArray data = clientSocket->readAll();
    // QString message = QString::fromUtf8(data).trimmed();
    // qInfo() << "Received from" << getPlayerSymbol(clientSocket) << ":" << message;

    // if (game.gs() != GameState::STARTED && m_clients.size() < 2) {
    //     sendToClient(clientSocket, "WAITING"); // In case they send messages too early
    //     return;
    // }
    // if (game.gs() != GameState::STARTED && m_clients.size() == 2 && message.startsWith("MOVE:")){
    //     // This case handles if a move is sent just before the game officially starts for a player
    //     // Can choose to ignore or queue, for simplicity, ensure gameActive is true before processing moves.
    //     // assignSymbolsAndStart() should make m_gameActive true.
    //     // Let's resend current state if game isn't active yet for some reason.
    //     QString boardStateStr = game.getBoardSeq();
    //     sendToClient(clientSocket, QJsonDocument().toJson());
    //     return;
    // }


    // if (message.startsWith("MOVE:")) {
    //     if (game.gs() != GameState::STARTED) {
    //         sendToClient(clientSocket, "INVALID_MOVE"); // Game not active
    //         return;
    //     }
    //     bool ok;
    //     int index = message.mid(5).toInt(&ok);
    //     if (ok) {
    //         handleMove(clientSocket, index);
    //     } else {
    //         sendToClient(clientSocket, "INVALID_MOVE"); // Malformed command
    //     }
    // }
}

void TicTacTueServer::checkGameState()
{
    QJsonObject json;
    json["type"] = "WINNER";
    switch (game.gs()) {
    case XWON:
        json["symbol"] = 'X';
        broadcastMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
        qInfo() << "Player " << m_playerinfos.at(m_clients[0]).getName() << "wins!";
        break;
    case OWON:
        json["symbol"] = 'O';
        broadcastMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
        qInfo() << "Player " << m_playerinfos.at(m_clients[1]).getName() << "wins!";
        break;
    case DRAW:
        json["symbol"] = 'N';
        broadcastMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
        qInfo() << "Game is a draw!";
        break;
    default:
        break;
    }
}

char TicTacTueServer::getPlayerSymbol(QTcpSocket *client) {
    if (m_clients.indexOf(client) == 0) return 'X';
    if (m_clients.indexOf(client) == 1) return 'O';
    return ' '; // Spectator or error
}

QTcpSocket* TicTacTueServer::getOpponent(QTcpSocket* client) {
    if (m_clients.size() != 2) return nullptr;
    if (client == m_clients[0]) return m_clients[1];
    if (client == m_clients[1]) return m_clients[0];
    if (client == nullptr && m_clients.size() == 1) return m_clients[0]; // Get remaining if one disconnected
    return nullptr;
}


void TicTacTueServer::handleMove(QTcpSocket *sender, int index)
{
    if (!game.move(index / 3, index % 3)) {
        sendToClient(sender, "INVALID_MOVE");
        return;
    }
    QString boardStateStr = game.getBoardSeq();
    QJsonObject json;
    json["type"] = "BOARD";
    json["seq"] = boardStateStr;
    broadcastMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));
    checkGameState();
}



void TicTacTueServer::broadcastMessage(const QByteArray &message, QTcpSocket *excludeClient)
{
    for (QTcpSocket *client : m_clients) {
        if (client != excludeClient) {
            sendToClient(client, message);
        }
    }
}

void TicTacTueServer::sendToClient(QTcpSocket *client, const QByteArray &jsonData)
{
    if (!client || client->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Attempted to send to disconnected or invalid socket.";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8); // Must match client

    // out << (quint32)0; // Reserve space for message size (4 bytes)
    out << jsonData; // Write the actual JSON data
    // out.device()->seek(0); // Go back to the beginning of the block
    // out << (quint32)(block.size() - sizeof(quint32)); // Write the actual size

    client->write(block);
    client->flush(); // Ensure data is sent immediately
}
