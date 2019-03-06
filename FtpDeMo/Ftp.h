#ifndef FTP_H
#define FTP_H

#include <QObject>
#include<QTcpSocket>

class Ftp : public QObject
{
    Q_OBJECT
public:
    enum Mode{PASV,PORT};
    enum Type{BINARY='I',ASSCII='A',EBCDIC='E',LOCAL='L'};
    enum CMD{CMD_PUT,CMD_GET,CMD_LIST,CMD_OTHER};

    explicit Ftp(QString address,QObject *parent = 0);

    void upload(QString local_filename,QString remote_filename,qint64 ofset=0);
    void download(QString local_filename,QString remote_filename,qint64 ofset=0);
    void login(QString username,QString pasword);


signals:
    void error(int code,QString errStr);
    void uploadProgess(int progess);
    void downloadProgess(int progess);
    void loginSucess();

private slots:
    void connectError(QAbstractSocket::SocketError code);
    void uploadFileData();
    void downloadFileData();
    void onCmdData();

private:
    void initConnect();
    qint64 remoteFileSize(QString remote_filename);
    void sendCmd(QString cmd);

private:
    QTcpSocket*     mCmdSocket;
    QTcpSocket*     mDataSocket;
    bool            mIsLogin;


};

#endif // FTP_H
