#include "navigatewasd.h"
#include "glwidget.h"

NavigateWASD::NavigateWASD()
{
    pressedW = pressedA = pressedS = pressedD = false;
}

void NavigateWASD::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
        case Qt::Key_W:
            pressedW = true;
            break;
        case Qt::Key_A:
            pressedA = true;
            break;
        case Qt::Key_S:
            pressedS = true;
            break;
        case Qt::Key_D:
            pressedD = true;
            break;
        default:
            break;
    }
}

void NavigateWASD::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key()) {
        case Qt::Key_W:
            pressedW = false;
            break;
        case Qt::Key_A:
            pressedA = false;
            break;
        case Qt::Key_S:
            pressedS = false;
            break;
        case Qt::Key_D:
            pressedD = false;
            break;
        default:
            break;
    }
}

void NavigateWASD::update(float )
{
    Vector dir(0, 0, 0);
    if (pressedW) dir += Vector( 0.0f, 0, -1.0f);
    if (pressedA) dir += Vector(-1.0f, 0,  0.0f);
    if (pressedS) dir += Vector( 0.0f, 0,  1.0f);
    if (pressedD) dir += Vector( 1.0f, 0,  0.0f);
    dir.normalize();

	dir *= 0.01f*pglwidget->scene()->boundingBox().radius();

    pglwidget->camera()->moveLocal(dir);
    pglwidget->camera()->updateClippingPlanes(pglwidget->scene()->boundingBox());
}

Q_EXPORT_PLUGIN2(navigatewasd, NavigateWASD)   // plugin name, plugin class
