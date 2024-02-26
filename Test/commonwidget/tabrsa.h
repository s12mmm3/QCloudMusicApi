#ifndef TABRSA_H
#define TABRSA_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabRsa;
}
QT_END_NAMESPACE

class TabRsa : public QWidget
{
    Q_OBJECT

public:
    explicit TabRsa(QWidget *parent = nullptr);
    ~TabRsa();

private slots:
    void on_pushButton_encrypt_clicked();

private:
    Ui::TabRsa *ui;
};

#endif // TABRSA_H
