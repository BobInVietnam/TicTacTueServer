#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>

class Player : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL);
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString gameId READ gameId WRITE setGameId NOTIFY gameIdChanged FINAL)
    Q_PROPERTY(int wonTime READ wonTime WRITE setWonTime NOTIFY wonTimeChanged FINAL)
    Q_PROPERTY(int lostTime READ lostTime WRITE setLostTime NOTIFY lostTimeChanged FINAL)
private:
    QString m_id;
    QString m_name;
    QTcpSocket * socket;

    int m_wonTime;

    int m_lostTime;

    QString m_gameId;

public:
    Player(QTcpSocket *, QString);

    QString id() const;
    void setId(const QString &newId);
    QString name() const;
    void setName(const QString &newName);
    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *newSocket);

    int wonTime() const;
    void setWonTime(int newWonTime);

    int lostTime() const;
    void setLostTime(int newLostTime);

    void sendMessage(const QJsonObject&);
    QString gameId() const;
    void setGameId(const QString &newGameId);

signals:
    void idChanged();
    void nameChanged();
    void wonTimeChanged();
    void lostTimeChanged();
    void gameIdChanged();
};

#endif // PLAYER_H
