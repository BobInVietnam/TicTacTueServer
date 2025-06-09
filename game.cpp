#include "game.h"

CountdownTimer *Game::getXTimer() const
{
    return xTimer;
}

CountdownTimer *Game::getOTimer() const
{
    return oTimer;
}

void Game::xTimerTimesup()
{
    setGs(GameState::OWON);
    emit boardChanged();
}

void Game::oTimerTimesup()
{
    setGs(GameState::XWON);
    emit boardChanged();
}

Game::Game() {
    xTurn = true;
    setGs(GameState::BEGIN);
    xTimer = new CountdownTimer(this);
    oTimer = new CountdownTimer(this);
    xTimer->setInitialTime(30000);
    oTimer->setInitialTime(30000);
    QObject::connect(xTimer, &CountdownTimer::timerExpired, this, &Game::xTimerTimesup);
    QObject::connect(oTimer, &CountdownTimer::timerExpired, this, &Game::oTimerTimesup);
    qDebug() << "Game created";
}

Game::~Game()
{
    delete xTimer;
    delete oTimer;
    qDebug() << "Game destroyed";
}

void Game::switchPlayer() {
    xTurn = !xTurn;
}

void Game::reset()
{
    setGs(GameState::BEGIN);
    board.clear();
    xTurn = true;
    xTimer->reset();
    oTimer->reset();
}

bool Game::move(int x, int y)
{
    if (gs() == GameState::BEGIN) {
        setGs(GameState::STARTED);
    } else if (gs() != GameState::STARTED) {
        return false;
    }
    if (!board.placeMark(x, y, xTurn)) {
        std::cout << "Invalid move! Try again.\n";
        return false;
    }
    if (xTurn) {
        xTimer->start();
        oTimer->pause();
    } else {
        xTimer->pause();
        oTimer->start();
    }
    checkWin();
    emit boardChanged();
    board.display();
    switchPlayer();
    return true;
}

void Game::checkWin()
{
    char winner = board.checkWinner();
    if (winner != ' ') {
        std::cout << "🎉 " << winner << " wins!\n";
        setGs(xTurn ? GameState::XWON : GameState::OWON);
        xTimer->pause();
        oTimer->pause();
        return;
    } else if (board.isFull()) {
        setGs(GameState::DRAW);
        xTimer->pause();
        oTimer->pause();
    }
}

bool Game::getXTurn() const
{
    return xTurn;
}

QString Game::getxTimerString() const
{
    return xTimer->currentTimeText();
}

QString Game::getoTimerString() const
{
    return oTimer->currentTimeText();
}

GameState Game::gs() const
{
    return m_gs;
}

void Game::setGs(GameState newGs)
{
    if (m_gs == newGs)
        return;
    m_gs = newGs;
    emit gsChanged();
}

QString Game::getBoardSeq() const
{
    return QString::fromStdString(board.getSequence());
}

void Game::setBoardSeq(QString seq)
{
    board.setSequence(seq);
    emit boardChanged();
}
