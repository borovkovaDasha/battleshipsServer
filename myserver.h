#ifndef MYSERVER_H
#define MYSERVER_H
#include <QString>

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include "client.h"


class MyServer : public QWidget {
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;//служит для хранения длины следующего, полученного от сокета блока.
   Client *client1, *client2;
    Client *arr[10];
    int countClients;

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
    MyServer(int nPort, QWidget* pwgt = 0);
    int flag;

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
            void Client1Disconnected();
            void Client2Disconnected();
};



#endif // MYSERVER_H
