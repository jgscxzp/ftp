#include "FtpDemo.h"
#include "ui_FtpDemo.h"

FtpDemo::FtpDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FtpDemo)
{
    ui->setupUi(this);
}

FtpDemo::~FtpDemo()
{
    delete ui;
}

/*
@brief       login
*/
void FtpDemo::on_btn_ftp_login_clicked()
{

}

/*
@brief       select file
*/
void FtpDemo::on_btn_select_file_clicked()
{

}

/*
@brief       upload file
*/
void FtpDemo::on_btn_upload_clicked()
{

}

/*
@brief       download file
*/
void FtpDemo::on_btn_download_clicked()
{

}
