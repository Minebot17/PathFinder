#pragma once
#include <QtCore/QCoreApplication>
#include <QList>
class Graph
{
public: 
	Graph(QList<QString> linesData);

	void calculateLabelsFromPoint(QString pointName);
	int getDistanceTo(QString pointName); // -1 - not connected
	QList<QString> getMinPathTo(QString pointName); // NULL - not connected

private:
	
};

