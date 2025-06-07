#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "countdowntimer.h"
#include <QObject>
#include <QTimer>

enum GameState {
    BEGIN,
    STARTED,
    XWON,
    OWON,
    DRAW,
};

class Game : public QObject {
    Q_OBJECT
    Q_PROPERTY(GameState gs READ gs WRITE setGs NOTIFY gsChanged FINAL)
public:
    Game();
    ~Game();
    void reset();
    bool move(int x, int y);

    bool getXTurn() const;
    QString getxTimerString() const;
    QString getoTimerString() const;

    GameState gs() const;
    void setGs(GameState newGs);

    QString getBoardSeq() const;
    void setBoardSeq(QString seq);

    CountdownTimer *getXTimer() const;
    CountdownTimer *getOTimer() const;
private slots:
    void xTimerTimesup();
    void oTimerTimesup();
signals:
    void gsChanged();
    void boardChanged();
protected:
    Board board;
    bool xTurn;
    GameState m_gs;
    CountdownTimer * xTimer;
    CountdownTimer * oTimer;

    void switchPlayer();
    void checkWin();
};

#endif // GAME_H
