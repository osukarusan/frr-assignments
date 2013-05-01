#ifndef RENDERSSAOWIDGET_H
#define RENDERSSAOWIDGET_H

#include <QWidget>

namespace Ui {
class RenderSSAOWidget;
}

class RenderSSAOWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RenderSSAOWidget(QWidget *parent = 0);
    ~RenderSSAOWidget();
    
private:
    Ui::RenderSSAOWidget *ui;
};

#endif // RENDERSSAOWIDGET_H
