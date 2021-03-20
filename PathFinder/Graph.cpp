#include "Graph.h"

Graph::Graph(QStringList lines) {
	originPointIndex = -1;
	pointNames = lines[0].split(QChar(';'));

	for (int i = 1; i < lines.length(); i++) {
		QStringList lineElements = lines[i].split(QChar(';'));
		QList<int> matrixLine;

		for (int j = 0; j < lineElements.length(); j++)
			matrixLine.append(lineElements[j].toInt());

		distancesMatrix.append(matrixLine);
	}
}

void Graph::calculateLabelsFromPoint(QString pointName) {
	originPointIndex = getPointIndex(pointName);
	
	pointLabels.clear();
	for (int i = 0; i < pointLabels.length(); i++)
		pointLabels.append(i == originPointIndex ? 0 : INT_MAX);

	QList<int> visitedPointsIndexes;
	while (visitedPointsIndexes.length() < pointLabels.length()) {
		QList<int> pointsIndexes;
		QList<int> pointsValues;

		for (int i = 0; i < pointLabels.length(); i++)
			if (!visitedPointsIndexes.contains(i)) {
				pointsIndexes.append(i);
				pointsValues.append(pointLabels[i]);
			}

		int minIndex = pointsIndexes[getIndexOfMin(pointsValues)];
		QList<int> distancesToPoints = distancesMatrix[minIndex];

		for (int i = 0; i < distancesToPoints.length(); i++)
			if (distancesToPoints[i] != 0 && pointLabels[minIndex] != INT_MAX)
				if (pointLabels[i] > pointLabels[minIndex] + distancesToPoints[i])
					pointLabels[i] = pointLabels[minIndex] + distancesToPoints[i];

		visitedPointsIndexes.append(minIndex);
	}
}

int Graph::getDistanceTo(QString pointName) { // -1 - not connected
	int distance = pointLabels[getPointIndex(pointName)];
	return distance == INT_MAX ? -1 : distance;
}

QStringList Graph::getMinPathTo(QString pointName) { // length == 0 - not connected
	QStringList result;
	
	int currentIndex = getPointIndex(pointName);
	while (currentIndex != originPointIndex) {
		result.append(pointNames[originPointIndex]);
		QList<int> distancesToPoints = getConnectedPoints(currentIndex);

		for (int i = 0; i < distancesToPoints.length(); i++)
			if (pointLabels[i] == pointLabels[currentIndex] - distancesToPoints[i]) {
				currentIndex = i;
				continue;
			}
	}

	if (result.length() != 0)
		result.append(pointNames[originPointIndex]);

	return result;
}

int Graph::getPointIndex(QString pointName) { // -1 if not exists
	for (int i = 0; i < pointNames.length(); i++)
		if (pointNames[i] == pointName)
			return i;

	return -1;
}

int Graph::getIndexOfMin(QList<int> list) {
	int minIndex = 0;
	int minValue = list[0];

	for (int i = 0; i < list.length(); i++)
		if (minValue > list[i]) {
			minIndex = i;
			minValue = list[i];
		}

	return minIndex;
}

QList<int> Graph::getConnectedPoints(int toPoint) {
	QList<int> result;

	for (int i = 0; i < distancesMatrix.length(); i++)
		result.append(distancesMatrix[i][toPoint]);

	return result;
}