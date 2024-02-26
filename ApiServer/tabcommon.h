#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <QWidget>

namespace Ui {
class TabCommon;
}

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
