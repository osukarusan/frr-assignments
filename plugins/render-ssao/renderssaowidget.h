#ifndef RENDERSSAOWIDGET_H
#define RENDERSSAOWIDGET_H

#include <QWidget>

class RenderSSAO;

namespace Ui {
class RenderSSAOWidget;
}

class RenderSSAOWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RenderSSAOWidget(QWidget *parent = 0);
    ~RenderSSAOWidget();

    void setRendererPointer(RenderSSAO* p) { renderer = p; }
    
private slots:

    void on_btnReload_clicked();

    void on_rbSampleWorld_toggled(bool);
    void on_rbSampleScreen_toggled(bool);

    void on_rbFilterNone_toggled(bool);
    void on_rbFilterGauss_toggled(bool);
    void on_rbFilterBilateral_toggled(bool);

    void on_slFilterRadius_valueChanged(int);
    void on_slExtinctionCoeff_valueChanged(int);
    void on_slSampleRadius_valueChanged(int);


private:
    Ui::RenderSSAOWidget *ui;

    RenderSSAO* renderer;
};

#endif // RENDERSSAOWIDGET_H
