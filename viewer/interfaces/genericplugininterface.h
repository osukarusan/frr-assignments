#ifndef _GENERIC_PLUGIN_INTERFACE_H
#define _GENERIC_PLUGIN_INTERFACE_H

#include <QtPlugin>
#include <QObject>
#include <QString>

class GLWidget;

class GenericPluginInterface
{
public:
    virtual ~GenericPluginInterface() {}
    virtual void setWidget(GLWidget* glwidget) {pglwidget = glwidget; }
    virtual void onPluginLoad() {}
    virtual void onObjectAdd() {}

    virtual QWidget* getPluginWidget() { return 0; } 
    virtual QString  getPluginName() { return ""; }
    
 protected:
    GLWidget* pglwidget;
 };


 #endif
 
