#ifndef _NAVIGATEWASD_H
#define _NAVIGATEWASD_H

#include "actioninterface.h"
#include "vector.h"

class NavigateWASD : public QObject, public ActionInterface
 {
     Q_OBJECT
     Q_INTERFACES(ActionInterface)

 public:

    NavigateWASD();
 
    void    keyPressEvent ( QKeyEvent *  );
    void    keyReleaseEvent ( QKeyEvent *  );

    void    update(float dt);
    
    void    mouseMoveEvent ( QMouseEvent * ) {}
    void    mousePressEvent ( QMouseEvent * ) {}
    void    mouseReleaseEvent ( QMouseEvent * ) {}
    void    wheelEvent ( QWheelEvent *  ) {}
 
 private:

    bool    pressedW, pressedA, pressedS, pressedD;

 };
 

 #endif
 

