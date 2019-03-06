#ifndef FTPDEMO_H
#define FTPDEMO_H

#include <QWidget>

namespace Ui {
class FtpDemo;
}

class FtpDemo : public QWidget
{
    Q_OBJECT

public:
    explicit FtpDemo(QWidget *parent = 0);
    ~FtpDemo();

private:
    Ui::FtpDemo *ui;
};

#endif // FTPDEMO_H
