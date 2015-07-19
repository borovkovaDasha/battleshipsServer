#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>

class Client
{
public:
//    enum Status
//    {
//        connected,
//        arrangenment,
//        arrangement_done,
//        waiting,
//        play
//    };
    QString status;
    Client();
    int field[10][10];
    int killfield[12][12];
    QTcpSocket* pClientSocket;
    void Stringtoarr(QString &str);
    int isKill (QString &str);
    int checkClient();
    QString CheckKilledShips();
    void resetGame();
};

#endif // CLIENT_H
