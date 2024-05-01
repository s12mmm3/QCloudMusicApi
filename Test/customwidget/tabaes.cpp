#include "tabaes.h"
#include "ui_tabaes.h"
#include "../../QCloudMusicApi/util/crypto.h"

using namespace QCloudMusicApi;
TabAes::TabAes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAes)
{
    ui->setupUi(this);
}

TabAes::~TabAes()
{
    delete ui;
}

void TabAes::on_pushButton_encrypt_clicked()
{
    auto arg = ui->textEdit_arg->toPlainText().toUtf8();
    auto mode = ui->radioButton_ecb->isChecked() ? "ecb" : "cbc";
    auto key = ui->lineEdit_key->text().toUtf8();
    auto iv = ui->lineEdit_iv->text().toUtf8();
    auto ret = Crypto::aesEncrypt(arg,
                                  mode,
                                  key,
                                  iv,
                                  ui->radioButton_base64->isChecked() ? "base64" : "hex");
    ui->textEdit_ret->setText(ret);
}


void TabAes::on_pushButton_decrypt_clicked()
{
    auto arg = ui->radioButton_base64->isChecked()
                   ? QByteArray::fromBase64(ui->textEdit_arg->toPlainText().toUtf8())
                   : QByteArray::fromHex(ui->textEdit_arg->toPlainText().toLower().toUtf8());
    auto mode = ui->radioButton_ecb->isChecked() ? "ecb" : "cbc";
    auto key = ui->lineEdit_key->text().toUtf8();
    auto iv = ui->lineEdit_iv->text().toUtf8();
    auto ret = Crypto::aesDecrypt(arg, mode,
                                  key,
                                  iv);
    ui->textEdit_ret->setText(ret);
}

