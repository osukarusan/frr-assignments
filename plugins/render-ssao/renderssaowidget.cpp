#include "renderssaowidget.h"
#include "ui_renderssaowidget.h"
#include "renderssao.h"

RenderSSAOWidget::RenderSSAOWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderSSAOWidget)
{
    ui->setupUi(this);
}

RenderSSAOWidget::~RenderSSAOWidget()
{
    delete ui;
}

void RenderSSAOWidget::on_btnReload_clicked() {
    renderer->reloadShaders();
}

void RenderSSAOWidget::on_rbSampleWorld_toggled(bool b) {
    if (b) {
        renderer->setSampleMode(RenderSSAO::WORLD_SPACE);
        renderer->reloadShaders();
    }
}

void RenderSSAOWidget::on_rbSampleScreen_toggled(bool b) {
    if (b) {
        renderer->setSampleMode(RenderSSAO::SCREEN_SPACE);
        renderer->reloadShaders();
    }
}

void RenderSSAOWidget::on_rbFilterNone_toggled(bool b) {
    if (b) {
        renderer->setFilterMode(RenderSSAO::NO_FILTER);
        renderer->reloadShaders();
    }
}

void RenderSSAOWidget::on_rbFilterGauss_toggled(bool b) {
    if (b) {
        renderer->setFilterMode(RenderSSAO::GAUSSIAN_BLUR);
        renderer->reloadShaders();
    }
}

void RenderSSAOWidget::on_rbFilterBilateral_toggled(bool b) {
    if (b) {
        renderer->setFilterMode(RenderSSAO::BILATERAL_BLUR);
        renderer->reloadShaders();
    }
}

void RenderSSAOWidget::on_slFilterRadius_valueChanged(int i) {
    renderer->setFilterRadiusSize(i/100.0f);
}

void RenderSSAOWidget::on_slExtinctionCoeff_valueChanged(int i) {
    renderer->setExtinctionCoefficient(i/100.0f);
}

void RenderSSAOWidget::on_slSampleRadius_valueChanged(int i) {
    renderer->setSampleRadiusSize(i/100.0f);
}
