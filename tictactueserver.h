#ifndef TICTACTUESERVER_H
#define TICTACTUESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <random>

class Player;
class GameRoom;

class TicTacTueServer : public QObject
{
    Q_OBJECT
public:
    explicit TicTacTueServer(QObject *parent = nullptr);
    ~TicTacTueServer();
    bool startServer(quint16 port);
    static void sendJsonToClient(QTcpSocket* client, const QJsonObject& json);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();
    void onRoomEmptied(const QString& roomId);

private:
    void processMessage(QTcpSocket* clientSocket, const QJsonObject& json);

    // Command Handlers
    void playerCreateRoom(QTcpSocket* clientSocket, const QJsonObject& json);
    void playerJoinRoom(QTcpSocket* clientSocket, const QJsonObject& json);
    void playerLeaveRoom(QTcpSocket* clientSocket, const QJsonObject& json);
    void playerAssignUsername(QTcpSocket *, const QJsonObject &);

    QString generateUniqueId(int length);

    QTcpServer* m_tcpServer;
    // Maps to track server state
    QMap<QTcpSocket*, Player*> m_clients;
    QMap<QString, GameRoom*> m_gameRooms;

    // Buffer for handling incoming TCP data streams
    QMap<QTcpSocket*, QByteArray> m_readBuffers;

    // For random ID generation
    std::mt19937 m_rng;
};

#endif // TICTACTUESERVER_H
