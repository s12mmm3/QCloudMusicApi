#ifndef TABAES_H
#define TABAES_H

#include <QJsonDocument>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabAes;
}
QT_END_NAMESPACE

class TabAes : public QWidget
{
    Q_OBJECT

public:
    explicit TabAes(QWidget *parent = nullptr);
    ~TabAes();

private slots:

    void on_pushButton_encrypt_clicked();

    void on_pushButton_decrypt_clicked();

private:
    Ui::TabAes *ui;
};

#endif // TABAES_H
