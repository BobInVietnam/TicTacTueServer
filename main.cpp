#include <QCoreApplication>
#include "tictactueserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TicTacTueServer server;
    if (!server.startServer(12345)) { // Use your desired port
        return 1;
    }

    return a.exec();
}
