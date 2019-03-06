#include "Ftp.h"
#define CHAR_CR QString("\r\n")


Ftp::Ftp(QString address,QObject *parent) : QObject(parent),mIsLogin(false)
{
    initConnect();
    mCmdSocket->connectToHost(address,21);
}

void Ftp::initConnect()
{
    mCmdSocket = new QTcpSocket(this);
    mDataSocket = new QTcpSocket(this);
    connect(mCmdSocket,SIGNAL(readyRead()),SLOT(onCmdData()),Qt::UniqueConnection);
    connect(mCmdSocket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(connectError(QAbstractSocket::SocketError)));
    connect(mDataSocket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(connectError(QAbstractSocket::SocketError)));
    connect(mDataSocket,SIGNAL(bytesWritten(qint64)),SLOT(uploadFileData()));
    connect(mDataSocket,SIGNAL(readyRead()),SLOT(downloadFileData()));

}


void Ftp::login(QString username, QString pasword)
{
    if(!mIsLogin) return;
    QString cmd="USER "+username+CHAR_CR+"PASS "+pasword+CHAR_CR;
    sendCmd(cmd);

}


void Ftp::upload(QString local_filename, QString remote_filename, qint64 ofset)
{

}

void Ftp::download(QString local_filename, QString remote_filename, qint64 ofset)
{

}

void Ftp::connectError(QAbstractSocket::SocketError code)
{
    mIsLogin = false;

    switch(code){
      case QTcpSocket::ConnectionRefusedError:
          emit error(0,"connect resfuse error!");
          break;
      case QTcpSocket::RemoteHostClosedError:
         emit error(1,"remote host closed!");
          break;
      case QTcpSocket::HostNotFoundError:
          emit error(2,"host not found!");
          break;
      case QTcpSocket::SocketTimeoutError:
         emit error(3,"connect timeout!");
          break;
      case QTcpSocket::NetworkError:
         emit error(4,"network error!");
          break;
      default:
         emit error(code,"unkown error,please check tcp socket!");
      }

}

void Ftp::uploadFileData()
{

}

void Ftp::downloadFileData()
{

}

void Ftp::onCmdData()
{
    QByteArray data;
     while((data=m_CmdSocket->readLine()).length()>0){
         QString result= QString::fromLatin1(data);
         QRegExp regexp("^\\d{3}\.+");
         if(!regexp.exactMatch(result))   continue;

         QStringList strlist=result.split(' ');
         bool toInt=false;
         int code=strlist.first().toInt(&toInt);
         if(!toInt) continue;

         switch (code) {
         case 150:
             if(m_cmdType == XzpFtp::CMD_PUT) uploadFileData();
             else if(m_cmdType == XzpFtp::CMD_GET) readFileData();
             break;
         case 200:{
             if(m_waitCmd.size() > 0){
                 sendListCmd();
             }
         }   break;
         case 213:{
             bool toInt=false;
             qint64 size=strlist.last().toLongLong(&toInt);
             xPrint<<size;
             if(toInt && size>0)   n_remoteFileSize=size;
             if(m_cmdType == XzpFtp::CMD_PUT) startPutFile();
             else if(m_cmdType == XzpFtp::CMD_GET) startGetFile();
             }
             break;

         case 226:
             if(m_cmdType == XzpFtp::CMD_PUT){
                  m_cmdType = XzpFtp:: CMD_OTHER;
                  qDebug()<<"upload file success";
                  initDataSocketConect(false);
                  m_DataSocket->close();
                  emit uploadFinished();
             }else if(m_cmdType == XzpFtp::CMD_GET){
                 readFileData();
                 stopDownLoad();
                msleep(200);
                 emit downloadFinished();
             }
             break;
         case 227:{
             m_Mode = XzpFtp::PASV;
             QRegExp regexp("\(?:(\\d{1,3}),(\\d{1,3}),(\\d{1,3}),(\\d{1,3}),(\\d{1,3}),(\\d{1,3})\)");
             QString ip;quint16 port;
             if(regexp.indexIn(result)!=-1){
                 ip=regexp.cap(1)+"."+regexp.cap(2)+"."+regexp.cap(3)+"."+regexp.cap(4);
                 port=(regexp.cap(5).toUInt()<<8)+regexp.cap(6).toUInt();
             }
             if(!m_DataSocket->isValid()){
                 m_DataSocket->connectToHost(ip,port);
                 if(!m_DataSocket->waitForConnected()){
                       emit error(20,"data scoket connect failed");
                       return;
                 }
                 initDataSocketConect(true);
                 sendListCmd();
             }
         }
             break;
         case 230:{
             if(!b_isLogined){
                 qDebug()<<"login success";
                 b_isLogined = true;
                 setTransferProperty();
                 emit loginSuccess();
             }


         }
          break;
         case 350:{
               sendListCmd();
         }
             break;
         case 421://FTP timeout
             xPrint<<"FTP timeout";
             connectToHost();
             break;
         case 530://ftp password error
             error(530,"ftp password error");
             break;
         case 550:{
 //            qDebug()<<"file is not exist";
             if(m_cmdType == XzpFtp::CMD_PUT) startPutFile();
             else if(m_cmdType == XzpFtp:: CMD_GET) {
                 stopDownLoad();
                 xPrint<<"file is not exist:"<<m_remoteFile;
                 error(550,"file is not exist");
             }
         }break;
         default:
             break;
         }
    }
}

qint64 Ftp::remoteFileSize(QString remote_filename)
{

}

void Ftp::sendCmd(QString cmd)
{
    mCmdSocket->write(cmd.toStdString().c_str());
}
