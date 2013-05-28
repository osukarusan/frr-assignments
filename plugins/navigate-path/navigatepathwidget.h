#ifndef NAVIGATEPATHWIDGET_H
#define NAVIGATEPATHWIDGET_H

#include <QWidget>

namespace Ui {
class NavigatePathWidget;
}

class NavigatePathWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit NavigatePathWidget(QWidget *parent = 0);
    ~NavigatePathWidget();

private slots:
    void on_btnLoad_clicked();
    void on_btnReset_clicked();
    void on_btnPause_toggled(bool);

signals:
    void changedFileName(QString);
    void clickedReset();
    void toggledPause(bool);
    
private:
    Ui::NavigatePathWidget *ui;
};

#endif // NAVIGATEPATHWIDGET_H
