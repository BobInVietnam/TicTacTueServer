#ifndef TicTacTueServer_H
#define TicTacTueServer_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include "game.h"
#include "player.h"

class TicTacTueServer : public QObject
{
    Q_OBJECT
public:
    explicit TicTacTueServer(QObject *parent = nullptr);
    bool startServer(quint16 port);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();
    void checkGameState();
signals:

private:
    QTcpServer *m_tcpServer;
    QVector<QTcpSocket*> m_clients;
    Game game;
    std::unordered_map<QTcpSocket*, Player> m_playerinfos;
    QMap<QTcpSocket*, QByteArray> m_socketReadBuffers;
    QMap<QTcpSocket*, quint32> m_socketBlockSizes;

    void generateRandomId();
    void initializeGame();
    void processMessage(QTcpSocket*, const QJsonObject&);
    void broadcastMessage(const QByteArray &message, QTcpSocket *excludeClient = nullptr);
    void sendToClient(QTcpSocket *, const QByteArray&);
    void handleMove(QTcpSocket *sender, int index);
    bool checkWin(char player);
    bool checkDraw();
    void assignSymbolsAndStart();
    char getPlayerSymbol(QTcpSocket *client);
    QTcpSocket* getOpponent(QTcpSocket* client);
};

#endif // TicTacTueServer_H
