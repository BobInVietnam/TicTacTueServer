#include "player.h"

#include "tictactueserver.h"


QTcpSocket *Player::getSocket() const
{
    return socket;
}

void Player::setSocket(QTcpSocket *newSocket)
{
    socket = newSocket;
}

Player::Player(QTcpSocket * socket, QString id)
{
    this->socket = socket;
    setId(id);
    setName("Guest");
    setGameId("");
    setWonTime(0);
    setLostTime(0);
}

QString Player::id() const
{
    return m_id;
}

void Player::setId(const QString &newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString Player::name() const
{
    return m_name;
}

void Player::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

int Player::wonTime() const
{
    return m_wonTime;
}

void Player::setWonTime(int newWonTime)
{
    if (m_wonTime == newWonTime)
        return;
    m_wonTime = newWonTime;
    emit wonTimeChanged();
}

int Player::lostTime() const
{
    return m_lostTime;
}

void Player::setLostTime(int newLostTime)
{
    if (m_lostTime == newLostTime)
        return;
    m_lostTime = newLostTime;
    emit lostTimeChanged();
}

void Player::sendMessage(const QJsonObject &json)
{
    TicTacTueServer::sendJsonToClient(socket, json);
}

QString Player::gameId() const
{
    return m_gameId;
}

void Player::setGameId(const QString &newGameId)
{
    if (m_gameId == newGameId)
        return;
    m_gameId = newGameId;
    emit gameIdChanged();
}
