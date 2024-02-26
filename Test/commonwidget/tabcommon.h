#ifndef TABCOMMON_H
#define TABCOMMON_H

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

private:

    /**
     * @brief 反射调用API中的方法
     * @param funName 函数名称
     * @param arg 参数
     * @return QVariantMap 返回的数据
     */
    QVariantMap invoke(const QString funName, const QVariantMap arg);

    /**
     * @brief 更新储存的cookie
     * @param ret Api返回的数据
     * @return void
     */
    void updateCookie(const QVariantMap ret);

private:
    Ui::TabCommon *ui;
    QVariantMap cookie;
    QJsonDocument config;
};

#endif // TABCOMMON_H
