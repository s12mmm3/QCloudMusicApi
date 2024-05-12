#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabCommon;
}
QT_END_NAMESPACE

// Api类接口测试
class TabCommon : public QWidget
{
    Q_OBJECT

public:
    explicit TabCommon(QWidget *parent = nullptr);
    ~TabCommon();

private:
    Ui::TabCommon *ui;
};

#endif // TABCOMMON_H
