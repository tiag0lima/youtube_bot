#include <QApplication>
#include "widget.h"



int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	widget w;
	w.show();

	app.exec();
}
