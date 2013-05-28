#include "drawocclusionwidget.h"
#include "ui_drawocclusionwidget.h"

DrawOcclusionWidget::DrawOcclusionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawOcclusionWidget)
{
    ui->setupUi(this);
}

DrawOcclusionWidget::~DrawOcclusionWidget()
{
    delete ui;
}

bool DrawOcclusionWidget::doFrustumCulling() const {
    return ui->cbFrustumCulling->isChecked();
}

bool DrawOcclusionWidget::doOcclusionQueries() const {
    return ui->cbOcclusionQueries->isChecked();
}

bool DrawOcclusionWidget::useRealGeometry() const {
    return ui->cbRealGeometry->isChecked();
}

bool DrawOcclusionWidget::drawDebugInfo() const {
    return ui->cbDrawInformation->isChecked();
}
