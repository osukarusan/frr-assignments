#include "ui_principal.h"

class Principal:public QWidget
{
	Q_OBJECT
	public:
		Principal(QWidget *parent=0);
	
	//private:
		Ui::Principal ui;

        public slots:
                void addPluginWidget(QWidget* w, const QString& name);
                void updateFPS(float);

};
