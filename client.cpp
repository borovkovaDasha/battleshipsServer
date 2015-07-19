#include "client.h"

Client::Client()
{
    for(int i=0; i < 10; i++){
        for (int j=0; j<10; j++){
            field[i][j]=0;

        }
    }
    for(int i=0; i < 12; i++){
        for (int j=0; j<12; j++){
           if((i==0) || (i==11) || (j==0) || (j==11))
                killfield[i][j]=-1;
           else  killfield[i][j]=0;


        }
    }
}
void Client::resetGame(){
    for(int i=0; i < 10; i++){
        for (int j=0; j<10; j++){
            field[i][j]=0;

        }
    }
    for(int i=0; i < 12; i++){
        for (int j=0; j<12; j++){
           if((i==0) || (i==11) || (j==0) || (j==11))
                killfield[i][j]=-1;
           else  killfield[i][j]=0;


        }
    }
}

void Client::Stringtoarr(QString &str)
{
    str.remove(0,5);
    for(int i = 0; i<10;i++)
    {
        for(int j = 0;j<10;j++)
        {
            QString symbol = str.section(' ', 1, 1);
            this->field[i][j]=symbol.toInt();
            if(this->field[i][j] == -1){
                str.remove(0,3);
                killfield[i+1][j+1]=-1;
             }
            else
                str.remove(0,2);
            printf("%d ",field[i][j]);
        }
        printf("\n");
    }
    status="arrangement_done";
}

int Client::isKill (QString &str)
{
    int x, y, tmp;
    str.remove(0,10);
    QString symbol = str.section(' ', 1, 1);
    x = symbol.toInt();
    str.remove(0,2);
    symbol = str.section(' ', 1, 1);
    y = symbol.toInt();
    //qDebug()<<"x = "<<x<<"y = "<<y;
    tmp = this->field[x][y];
    if (tmp>0){
        this->killfield[x+1][y+1]=1;
        //CheckKilledShips();
        this->field[x][y]=0;
    }
    qDebug()<<tmp;
    return tmp;
}
int Client::checkClient()
{
    for(int i = 0; i<10; i++)
        for(int j = 0; j<10;j++)
        {
            if(this->field[i][j]>0)
            {
                return 0;
            }
        }
    return 1;
}

QString Client::CheckKilledShips(){
    QString answer;
/*4 palub horizontal*/
    for(int i=0; i < 10; i++){
        for (int j=0; j<6; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+1][j+2]==1)&&
                   (this->killfield[i+1][j+3]==1)&&(this->killfield[i+1][j+4]==1)){
               qDebug() << "you killed him!4 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -8";
               this->killfield[i+1][j+1]=0;
               this->killfield[i+1][j+2]=0;
               this->killfield[i+1][j+3]=0;
               this->killfield[i+1][j+4]=0;

           }
        }
    }
/* 3 palubs horizontal*/
    for(int i=0; i < 10; i++){
        for (int j=0; j<7; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+1][j+2]==1)&&
                   (this->killfield[i+1][j+3]==1)&&(this->killfield[i+1][j+4]==-1)
                   &&(this->killfield[i+1][j]==-1)){
               qDebug() << "you killed him! 3 palubs";
               this->killfield[i+1][j+1]=0;
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -7";
               this->killfield[i+1][j+2]=0;
               this->killfield[i+1][j+3]=0;

           }
        }
    }
    /* 2 palubs horizontal*/
    for(int i=0; i < 10; i++){
        for (int j=0; j<8; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+1][j+2]==1)&&
                   (this->killfield[i+1][j+3]==-1)&&(this->killfield[i+1][j]==-1)){
               qDebug() << "you killed him! 2 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -6";
               this->killfield[i+1][j+1]=0;
               this->killfield[i+1][j+2]=0;


           }
        }
    }
    /* 4 palubs vertical*/
    for(int i=0; i < 6; i++){
        for (int j=0; j<10; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+2][j+1]==1)&&
                   (this->killfield[i+3][j+1]==1)&&(this->killfield[i+4][j+1]==1)){
               qDebug() << "you killed him!4 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -4";
               this->killfield[i+1][j+1]=0;
               this->killfield[i+2][j+1]=0;
               this->killfield[i+3][j+1]=0;
               this->killfield[i+4][j+1]=0;

           }
        }
    }
    /* 3 palubs vertical*/
    for(int i=0; i < 7; i++){
        for (int j=0; j<10; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+2][j+1]==1)&&
                   (this->killfield[i+3][j+1]==1)&&(this->killfield[i+4][j+1]==-1)
                   &&(this->killfield[i][j+1]==-1)){
               qDebug() << "you killed him! 3 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -3";
               this->killfield[i+1][j+1]=0;
               this->killfield[i+2][j+1]=0;
               this->killfield[i+3][j+1]=0;

           }
        }
    }
    /* 2 palubs vertical*/
    for(int i=0; i < 8; i++){
        for (int j=0; j<10; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+2][j+1]==1)&&
                   (this->killfield[i+3][j+1]==-1)&&(this->killfield[i][j+1]==-1)){
               qDebug() << "you killed him! 2 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -2";
               this->killfield[i+1][j+1]=0;
               this->killfield[i+2][j+1]=0;

           }
        }
    }
    /* 1 palubs*/
    for(int i=0; i < 10; i++){
        for (int j=0; j<10; j++){
           if((this->killfield[i+1][j+1]==1)&&(this->killfield[i+2][j+1]==-1)&&
                   (this->killfield[i][j+1]==-1)&&(this->killfield[i+1][j]==-1)&&
                   (this->killfield[i+1][j+2]==-1)){
               qDebug() << "you killed him! 1 palubs";
               answer="kill "+ QString::number(i,10)+" " + QString::number(j,10)+" -5";
               this->killfield[i+1][j+1]=0;

           }
        }
    }
    return answer;

}
