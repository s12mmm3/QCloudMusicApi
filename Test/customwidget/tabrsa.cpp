#include "tabrsa.h"
#include "ui_tabrsa.h"
#include "../../QCloudMusicApi/util/crypto.h"

using namespace QCloudMusicApiNS;
TabRsa::TabRsa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabRsa)
{
    ui->setupUi(this);
}

TabRsa::~TabRsa()
{
    delete ui;
}

void TabRsa::on_pushButton_encrypt_clicked()
{
    auto arg = ui->textEdit_arg->toPlainText();
    auto pubKey = ui->plainTextEdit_pubKey->toPlainText();
    auto ret = Crypto::rsaEncrypt(arg,
                                  pubKey);
    ui->textEdit_ret->setText(ui->checkBox_hex->isChecked() ? ret.toHex() : ret);
}

