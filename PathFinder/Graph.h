#pragma once
#include <QtCore/QCoreApplication>
#include <QList>
class Graph
{
public: 
	Graph(QStringList lines);

	void calculateLabelsFromPoint(QString pointName);
	int getDistanceTo(QString pointName); // -1 - not connected
	QStringList getMinPathTo(QString pointName); // length == 0 - not connected

private:
	QStringList pointNames;
	QList<QList<int>> distancesMatrix;
	int originPointIndex;
	QList<int> pointLabels;

	int getPointIndex(QString pointName); // -1 if not exists
	int getIndexOfMin(QList<int> list);
	QList<int> getConnectedPoints(int toPoint);
};

