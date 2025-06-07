#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QObject>
#include <QTimer> // Required for QTimer
#include <QString> // For time formatting in QML
#include <QTime>   // For QTime::fromMSecsSinceStartOfDay for formatting

class CountdownTimer : public QObject
{
    Q_OBJECT

    // Q_PROPERTY for initial total time, in milliseconds (qint64 for larger values)
    Q_PROPERTY(qint64 initialTime READ initialTime WRITE setInitialTime NOTIFY initialTimeChanged FINAL)
    // Q_PROPERTY for current remaining time, in milliseconds
    Q_PROPERTY(qint64 currentTime READ currentTime NOTIFY currentTimeChanged FINAL)
    // Q_PROPERTY for formatted time string (for direct QML display)
    Q_PROPERTY(QString currentTimeText READ currentTimeText NOTIFY currentTimeChanged)
    // Q_PROPERTY to indicate if the timer is currently running
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged FINAL)
    // Q_PROPERTY to indicate if the timer has run out
    Q_PROPERTY(bool isTimesUp READ isTimesUp NOTIFY isTimesUpChanged FINAL)

public:
    explicit CountdownTimer(QObject *parent = nullptr);
    ~CountdownTimer();

    // Q_INVOKABLE functions to control the timer from C++ or QML
    void pause();
    void resume();
    void reset(); // Resets to initialTime and stops
    void start(); // Convenience to start (resumes if paused, resets if stopped)

    // Getters for Q_PROPERTYs
    qint64 initialTime() const;
    qint64 currentTime() const;
    QString currentTimeText() const; // Formatted string for QML display
    bool isRunning() const;
    bool isTimesUp() const;

    // Setter for initialTime (also resets currentTime)
    void setInitialTime(qint64 milliseconds);

private slots:
    void onTimeout(); // Slot connected to QTimer::timeout()

signals:
    // NOTIFY signals for Q_PROPERTYs
    void initialTimeChanged();
    void currentTimeChanged(); // Emitted when currentTimeMs changes
    void isRunningChanged();
    void isTimesUpChanged();

    // Custom signal emitted when the timer runs out
    void timerExpired();

private:
    // Private setters to manage internal state and emit NOTIFY signals
    void setCurrentTime(qint64 milliseconds);
    void setIsRunning(bool running);
    void setIsTimesUp(bool timesUp);

    QTimer m_timer;              // Internal timer for countdown
    qint64 m_initialTimeMs;      // Stores the total time set for the countdown (in ms)
    qint64 m_currentTimeMs;      // Stores the current remaining time (in ms)
    bool m_isRunning;            // Tracks if the timer is active
    bool m_isTimesUp;            // Tracks if the timer has reached zero

    const int m_updateInterval = 100; // Update interval in milliseconds
};

#endif // COUNTDOWNTIMER_H
