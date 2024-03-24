#ifndef TABCOMMON_H
#define TABCOMMON_H

#include "../../QCloudMusicApi/apihelper.h"

#include <QWidget>
#include <QJsonDocument>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabCommon;
}
QT_END_NAMESPACE

class TabCommon : public QWidget
{
    Q_OBJECT

public:
    explicit TabCommon(QWidget *parent = nullptr);
    ~TabCommon();

private slots:
    void on_pushButton_send_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void update(QVariantMap ret);

signals:
    void invoked(QVariantMap ret);

private:
    Ui::TabCommon *ui;
    ApiHelper helper;
};

#endif // TABCOMMON_H
