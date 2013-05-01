#ifndef _NAVIGATEPATH_H
#define _NAVIGATEPATH_H

#include "actioninterface.h"
#include "pathaction.h"
#include "navigatepathwidget.h"
#include <vector>

class NavigatePath : public QObject, public ActionInterface
 {
    Q_OBJECT
    Q_INTERFACES(ActionInterface)

 public:
    NavigatePath();
    virtual ~NavigatePath();
 
    void	keyPressEvent ( QKeyEvent *  ) {}
    void	keyReleaseEvent ( QKeyEvent *  ) {}

    void    update(float);
    
    void	mouseMoveEvent ( QMouseEvent * ) {}
    void	mousePressEvent ( QMouseEvent * ) {}
    void	mouseReleaseEvent ( QMouseEvent * ) {}
    void	wheelEvent ( QWheelEvent *  ) {}
 
    virtual QWidget* getPluginWidget();
    virtual QString  getPluginName() { return "Navigation Path"; }

public slots:
    void    loadActions(const QString& filepath);
    void    pausePath(bool);
    void    restartPath();

 private:

    std::vector<PathAction*>    actions;
    unsigned int                currentAction;
    bool                        paused;

    NavigatePathWidget* widget;

 };
 

 #endif
 

