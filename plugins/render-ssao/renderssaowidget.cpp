#include "renderssaowidget.h"
#include "ui_renderssaowidget.h"

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
