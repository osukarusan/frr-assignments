#include "navigatepathwidget.h"
#include "ui_navigatepathwidget.h"
#include <QFileDialog>
#include <QDir>
#include <iostream>

NavigatePathWidget::NavigatePathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigatePathWidget)
{
    ui->setupUi(this);
}

NavigatePathWidget::~NavigatePathWidget()
{
    delete ui;
}

void NavigatePathWidget::on_btnLoad_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a path file to load");
    if (file.length()) {
        QStringList route = file.split(QDir::separator());
        ui->fileLine->setText(route.last());
        ui->btnPause->setChecked(true);
        emit changedFileName(file);
        emit toggledPause(true);
    }
}

void NavigatePathWidget::on_btnPause_toggled(bool b)
{
    emit toggledPause(b);
}

void NavigatePathWidget::on_btnReset_clicked()
{
    emit clickedReset();
}
