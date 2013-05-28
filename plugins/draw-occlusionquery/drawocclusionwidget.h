#ifndef DRAWOCCLUSIONWIDGET_H
#define DRAWOCCLUSIONWIDGET_H

#include <QWidget>

namespace Ui {
    class DrawOcclusionWidget;
}

class DrawOcclusionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawOcclusionWidget(QWidget *parent = 0);
    ~DrawOcclusionWidget();

    bool doFrustumCulling() const;
    bool doOcclusionQueries() const;
    bool useRealGeometry() const;
    bool drawDebugInfo() const;

private:
    Ui::DrawOcclusionWidget *ui;
};





#endif // DRAWOCCLUSIONWIDGET_H
