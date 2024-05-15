#ifndef TABCOMMONUNIT_H
#define TABCOMMONUNIT_H

#include <QComboBox>
#include <QTextEdit>
#include <QWidget>
#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabCommonUnit;
}
QT_END_NAMESPACE

// 抽取了共同的，读取配置并触发API方法的界面逻辑
class TabCommonUnit : public QWidget
{
    Q_OBJECT

public:
    explicit TabCommonUnit(QWidget *parent = nullptr);
    ~TabCommonUnit();

public:
    // 点击send按钮触发的动作
    std::function<QVariantMap(QString member, QString arg)> callback = Q_NULLPTR;

    // 向外暴露的控件们
    QComboBox *comboBox_function = Q_NULLPTR;
    QTextEdit *textEdit_arg = Q_NULLPTR;
    QTextEdit *textEdit_ret = Q_NULLPTR;

private slots:
    void on_pushButton_send_clicked();

    void on_comboBox_function_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void update(QVariantMap ret);

private:
    Ui::TabCommonUnit *ui;
};

#endif // TABCOMMONUNIT_H
