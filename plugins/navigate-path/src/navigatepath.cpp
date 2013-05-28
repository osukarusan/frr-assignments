#include "navigatepath.h"
#include "glwidget.h"
#include <QFile>
#include <QTextStream>


NavigatePath::NavigatePath()
{
    widget = new NavigatePathWidget();
    connect(widget, SIGNAL(changedFileName(QString)), this, SLOT(loadActions(QString)));
    connect(widget, SIGNAL(toggledPause(bool)), this, SLOT(pausePath(bool)));
    connect(widget, SIGNAL(clickedReset()), this, SLOT(restartPath()));
}

NavigatePath::~NavigatePath()
{
    for (unsigned int i = 0; i < actions.size(); i++) {
        if (actions[i]) {
            delete actions[i];
        }
    }
}

QWidget* NavigatePath::getPluginWidget()
{
    return widget;
}

void NavigatePath::update(float dt)
{
    if (paused) return;
    if (currentAction < actions.size() && actions[currentAction]) {
        actions[currentAction]->update(dt, pglwidget->scene(), pglwidget->camera());
        if (actions[currentAction]->t_elapsed >= actions[currentAction]->t_length) {
            currentAction++;
            if (currentAction >= actions.size()) {
                pglwidget->stopBenchmarking();
            }
        }
    }
}

void NavigatePath::loadActions(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open actions file: " << filepath.toStdString() << std::endl;
    }

    actions.clear();
    QTextStream instream(&file);
    while (!instream.atEnd()) {
        QString line = instream.readLine();
        QStringList fields = line.split(" ");
        if (fields.at(0) == "move") {
            PathActionMove* act = new PathActionMove();
            act->t_length = fields.at(1).toFloat();
            float mx = fields.at(2).toFloat();
            float my = fields.at(3).toFloat();
            float mz = fields.at(4).toFloat();
            act->movement = Vector(mx, my, mz);
            actions.push_back(act);
        }
        else if (fields[0] == "rotate") {
            PathActionRotate* act = new PathActionRotate();
            act->t_length = fields.at(1).toFloat();
            float rx = fields.at(2).toFloat();
            float ry = fields.at(3).toFloat();
            float rz = fields.at(4).toFloat();
            act->rotations = Vector(rx, ry, rz);
            actions.push_back(act);
        }
    }

    file.close();

    currentAction = 0;
    paused = true;
}

void NavigatePath::pausePath(bool pause)
{
    paused = pause;
}

void NavigatePath::restartPath()
{
    for (unsigned int i = 0; i < actions.size(); i++) {
        if (actions[i]) {
            actions[i]->t_elapsed = 0;
        }
    }
    currentAction = 0;
    pglwidget->resetCamera();
    pglwidget->startBenchmarking();
}


Q_EXPORT_PLUGIN2(navigatepath, NavigatePath)   // plugin name, plugin class
