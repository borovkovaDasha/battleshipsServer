#include "myserver.h"
#include <QTcpServer>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include "client.h"

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , m_nNextBlockSize(0)
{
    flag = 0;
    countClients=0;

    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );


    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}
void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
   countClients++;
   qDebug() << "started new connection!!!" << countClients;

    if(countClients == 1)
    {
        client1 = new Client();
        client1->pClientSocket = pClientSocket;
        qDebug() << "1 "<<client1->pClientSocket;
        sendToClient(client1->pClientSocket, "connected");
        qDebug() << "i sent it!!";
        //client1->pClientSocket->setSocketOption(QAbstractSocket:: KeepAliveOption, 1);
        connect(client1->pClientSocket, &QTcpSocket::disconnected, this, &MyServer::Client1Disconnected);

    }
    else if(countClients == 2)
    {
        client2 = new Client();
        client2->pClientSocket = pClientSocket;
        qDebug() <<"2" <<  client2->pClientSocket;
        sendToClient(client2->pClientSocket, ("connected"));
        sendToClient(client1->pClientSocket, ("arrangement"));
        sendToClient(client2->pClientSocket, ("arrangement"));
        //client2->pClientSocket->setSocketOption(QAbstractSocket:: KeepAliveOption, 1);
        connect(client2->pClientSocket, &QTcpSocket::disconnected, this, &MyServer::Client2Disconnected);
        m_ptxt->append("arrangement");
       // flag=2;

    }
   else if (countClients>2){
        sendToClient(pClientSocket,"refused");
        pClientSocket->close();
        countClients--;
    }
}
void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;

            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        //in >> str;
        in >> time >> str;
        qDebug()<< "new message from clients:" << str;
        m_nNextBlockSize = 0;
        if(pClientSocket == client1->pClientSocket)
        {
            if(str.contains("message", Qt::CaseInsensitive))
            {
                //str.remove(0,7);
                sendToClient(client2->pClientSocket,str);
                m_nNextBlockSize = 0;
            }
            else if(str.contains("field", Qt::CaseInsensitive))
            {
                client1->Stringtoarr(str);
                if(client2->status=="arrangement_done")
                {
                    sendToClient(client1->pClientSocket,"play");
                    sendToClient(client2->pClientSocket,"waiting");
                }
                m_nNextBlockSize = 0;
            }
            else if(str.contains("coordinats", Qt::CaseInsensitive))
            {
                QString str1 = str;
                int answer = client2->isKill(str);
                QString msg = str1.remove(0,10);
                if(answer >0)
                {
                  QString msg1 = "enemy" + msg + " 1 ";
                    sendToClient(client2->pClientSocket,msg1);
                    QString msg2 = "results" + msg + " 1 ";
                    sendToClient(client1->pClientSocket,msg2);
                    int flag = client2->checkClient();
                    QString ans=client2->CheckKilledShips();
                    if (ans!=""){
                        QString ans1=ans;
                        QString ans2=ans;
                        ans1.insert(4,"1");
                        ans2.insert(4,"2");
                        sendToClient(client1->pClientSocket,ans1);
                        sendToClient(client2->pClientSocket,ans2);
                    }
                    if(flag == 1)
                    {
                        sendToClient(client1->pClientSocket,"win");
                        sendToClient(client2->pClientSocket, "lose");

                       client1->pClientSocket->close();
                       client2->pClientSocket->close();
                       bool asds= disconnect(client1->pClientSocket);
                       qDebug() << asds;


                    }
                    else
                        sendToClient(client1->pClientSocket,"play");
                }
                else
                {
                    QString msg1 = "enemy" + msg + " 0 ";
                      sendToClient(client2->pClientSocket,msg1);
                      QString msg2 = "results" + msg + " 0 ";
                      sendToClient(client1->pClientSocket,msg2);
                    sendToClient(client1->pClientSocket, "waiting");
                    sendToClient(client2->pClientSocket, "play");
                }

                m_nNextBlockSize = 0;
            }
        }
        if(pClientSocket == client2->pClientSocket)
        {
            if(str.contains("message", Qt::CaseInsensitive))
            {
                //str.remove(0,7);
                sendToClient(client1->pClientSocket,str);
                m_nNextBlockSize = 0;
            }

            else if(str.contains("field", Qt::CaseInsensitive))
            {
                client2->Stringtoarr(str);
                if(client1->status=="arrangement_done")
                {
                    sendToClient(client1->pClientSocket,"play");
                    sendToClient(client2->pClientSocket,"waiting");
                }
                m_nNextBlockSize = 0;
            }
            else if(str.contains("coordinats", Qt::CaseInsensitive))
            {
                QString str1 = str;
                int answer = client1->isKill(str);
                QString msg = str1.remove(0,10);
                if(answer >0)
                {
                  QString msg1 = "enemy" + msg + " 1 ";
                    sendToClient(client1->pClientSocket,msg1);
                    QString msg2 = "results" + msg + " 1 ";
                    sendToClient(client2->pClientSocket,msg2);
                    int flag = client1->checkClient();
                    QString ans=client1->CheckKilledShips();
                    if (ans!=""){
                        QString ans1=ans;
                        QString ans2=ans;
                        ans1.insert(4,"1");
                        ans2.insert(4,"2");
                        sendToClient(client2->pClientSocket,ans1);
                        sendToClient(client1->pClientSocket,ans2);
                    }
                    if(flag == 1)
                    {

                        sendToClient(client2->pClientSocket,"win");
                        sendToClient(client1->pClientSocket, "lose");
                        client1->pClientSocket->close();
                        client2->pClientSocket->close();

                    }
                    else
                        sendToClient(client2->pClientSocket,"play");
                }
                else
                {
                    QString msg1 = "enemy" + msg + " 0 ";
                      sendToClient(client1->pClientSocket,msg1);
                      QString msg2 = "results" + msg + " 0 ";
                      sendToClient(client2->pClientSocket,msg2);
                    sendToClient(client2->pClientSocket, "waiting");
                    sendToClient(client1->pClientSocket, "play");
                }

                m_nNextBlockSize = 0;
            }
        }
        //QString strMessage =
        //    time.toString() + "Client has sended - " + str;
        //m_ptxt->append(strMessage);
        //if(pClientSocket == client1->pClientSocket)
        //    sendToClient(client2->pClientSocket, str);
        //else
        //{
        //    sendToClient(client1->pClientSocket, str);
        //}

        //m_nNextBlockSize = 0;
        //sendToClient(pClientSocket,
        //             "Server Response: Received \"" + str + "\""
        //            );
    }
}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
void MyServer::Client1Disconnected()
{   client1->resetGame();
   client1->pClientSocket=0;
   if(countClients==2){
   if( client2->pClientSocket!=0){
   sendToClient(client2->pClientSocket, "disconnect");
   //qDebug() << "socket is:" << client1->pClientSocket;
   client2->resetGame();
   client2->pClientSocket->close();
   countClients=0;
   }}
   countClients=0;
    // client has disconnected, so remove from list
    qDebug() << "1 client disconnected !! ALAL";
}

void MyServer::Client2Disconnected()
{
    client2->resetGame();
    client2->pClientSocket=0;
    if (countClients==2){
        if( client1->pClientSocket!=0){
            sendToClient(client1->pClientSocket, "disconnect");
            client1->resetGame();
    //qDebug() << "socket is:" << client1->pClientSocket;
            client1->pClientSocket->close();

            countClients=0;
         }
    }
    countClients=0;

    qDebug() << "2 client disconnected !! ALAL";
}
