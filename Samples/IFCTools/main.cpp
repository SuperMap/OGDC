#include "ifctools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IFCTools w;
	w.show();
	return a.exec();
}
