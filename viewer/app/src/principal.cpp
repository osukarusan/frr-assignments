#include "principal.h"

Principal::Principal(QWidget *parent):QWidget(parent)
{
	ui.setupUi(this);
        connect(ui.glWidget, SIGNAL(addedPluginWidget(QWidget*, const QString&)),
                this, SLOT(addPluginWidget(QWidget*, const QString&)));
        connect(ui.glWidget, SIGNAL(updatedFPS(float)),
                this, SLOT(updateFPS(float)));
        ui.glWidget->initDefaultPlugins();
}

void Principal::addPluginWidget(QWidget *w, const QString &name) {
        ui.toolBox->addItem(w, name);
        ui.toolBox->setCurrentWidget(w);
}

void Principal::updateFPS(float fps) {
    ui.labelFPS->setText(QString::number(int(fps + 0.5)) + QString(" fps"));
}
