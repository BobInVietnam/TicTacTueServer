#include "countdowntimer.h"
#include <QDebug>
#include <QDateTime> // For QDateTime::currentMSecsSinceEpoch() if needed for more complex time tracking

CountdownTimer::CountdownTimer(QObject *parent)
    : QObject(parent),
    m_initialTimeMs(10000),
    m_currentTimeMs(10000),
    m_isRunning(false),
    m_isTimesUp(false)
{
    // Configure the internal QTimer
    m_timer.setInterval(m_updateInterval);
    // Connect the timer's timeout signal to our private slot
    connect(&m_timer, &QTimer::timeout, this, &CountdownTimer::onTimeout);

    qDebug() << "CountdownTimer created. Update interval:" << m_updateInterval << "ms";
}

CountdownTimer::~CountdownTimer()
{
    // Ensure the timer is stopped when the object is destroyed
    m_timer.stop(); // Calling stop() is safe even if not running
    qDebug() << "CountdownTimer destroyed.";
}

// --- Q_PROPERTY Getters ---
qint64 CountdownTimer::initialTime() const
{
    return m_initialTimeMs;
}

qint64 CountdownTimer::currentTime() const
{
    return m_currentTimeMs;
}

QString CountdownTimer::currentTimeText() const
{
    // Format milliseconds into HH:MM:SS, MM:SS, or S.ms
    QTime time(0, 0, 0);
    time = time.addMSecs(m_currentTimeMs);

    if (m_currentTimeMs >= 60000) { // If >= 1 second
        return time.toString("mm:ss");
    } else { // Less than 1 second, show milliseconds
        return QString("%1.%2").arg(time.second()).arg(time.msec() / 100, 1, 10, QChar('0'));
    }
}

bool CountdownTimer::isRunning() const
{
    return m_isRunning;
}

bool CountdownTimer::isTimesUp() const
{
    return m_isTimesUp;
}

// --- Q_PROPERTY Setters (private for internal state management) ---
void CountdownTimer::setCurrentTime(qint64 milliseconds)
{
    if (m_currentTimeMs == milliseconds)
        return; // Only update if value actually changed

    m_currentTimeMs = milliseconds;
    emit currentTimeChanged(); // Notify QML
}

void CountdownTimer::setIsRunning(bool running)
{
    if (m_isRunning == running)
        return;
    m_isRunning = running;
    emit isRunningChanged(); // Notify QML
}

void CountdownTimer::setIsTimesUp(bool timesUp)
{
    if (m_isTimesUp == timesUp)
        return;
    m_isTimesUp = timesUp;
    emit isTimesUpChanged(); // Notify QML
}

// --- Q_INVOKABLE / Public Control Functions ---
void CountdownTimer::setInitialTime(qint64 milliseconds)
{
    if (milliseconds < 0) milliseconds = 0; // Ensure non-negative
    if (m_initialTimeMs == milliseconds)
        return; // Only update if value actually changed

    m_initialTimeMs = milliseconds;
    emit initialTimeChanged(); // Notify QML

    // When initial time is set, reset the current time and state
    reset();
    qDebug() << "Initial time set to:" << m_initialTimeMs << "ms";
}

void CountdownTimer::pause()
{
    if (m_isRunning) {
        m_timer.stop();
        setIsRunning(false);
        qDebug() << "Timer paused. Remaining:" << m_currentTimeMs << "ms";
    }
}

void CountdownTimer::resume()
{
    if (!m_isRunning && !m_isTimesUp && m_currentTimeMs > 0) {
        m_timer.start();
        setIsRunning(true);
        qDebug() << "Timer resumed. Remaining:" << m_currentTimeMs << "ms";
    } else if (m_isTimesUp) {
        qDebug() << "Cannot resume, timer has expired. Reset first.";
    } else if (m_currentTimeMs <= 0) {
        qDebug() << "Cannot resume, current time is zero. Reset first.";
    }
}

void CountdownTimer::reset()
{
    m_timer.stop(); // Ensure timer is stopped
    setCurrentTime(m_initialTimeMs); // Set current time back to initial
    setIsTimesUp(false); // Clear times-up state
    qDebug() << "Timer reset to initial time.";
}

void CountdownTimer::start()
{
    // This function acts as a convenience to start/resume the timer.
    // If it's already running, do nothing.
    // If it's paused or stopped but has time left, resume.
    // If it's times up or has no time, reset and then start (if initial time > 0).
    if (m_isRunning) {
        qDebug() << "Timer already running.";
        return;
    }

    if (m_isTimesUp || m_currentTimeMs <= 0) {
        reset(); // Reset if times up or no time left
    }

    // Only start if there's time to count down
    if (m_currentTimeMs > 0) {
        resume(); // Attempt to resume (which will start if not running)
    } else {
        qDebug() << "Cannot start timer: initial time is 0.";
    }
}

void CountdownTimer::onTimeout()
{
    // Decrement current time by the interval
    m_currentTimeMs -= m_updateInterval;

    if (m_currentTimeMs <= 0) {
        // Time has run out
        setCurrentTime(0); // Ensure it's exactly 0
        m_timer.stop();; // Stop the timer
        setIsTimesUp(true); // Mark as times up
        emit timerExpired(); // Emit the signal
        qDebug() << "Timer expired!";
    } else {
        // Update QML about the time change
        emit currentTimeChanged();
    }
}
