#include "Graph.h"

Graph::Graph(QStringList lines) {
	originPointIndex = -1;
	pointNames = lines[0].split(QChar(';'));

	if (pointNames.length() < 2)
		throw QString::fromUtf8(u8"“очек в схеме должно быть минимум 2-е или более");

	if (pointNames.length() != lines.length() - 1)
			throw QString::fromUtf8(u8" ол-во точек должно быть столько же, сколько строк в матрице смежности.  ол-во точек: С%1Т.  ол-во строк С$2Т.")
				.arg(QString(pointNames.length()), QString(lines.length() - 1));

	for (int i = 1; i < lines.length(); i++) {
		QStringList lineElements = lines[i].split(QChar(';'));
		QList<int> matrixLine;
		
		if (pointNames.length() != lines.length() - 1)
			throw QString::fromUtf8(u8" ол-во точек должно быть столько же, сколько элементов в каждой строке матрицы смежности.  ол-во точек: '%1'.  ол-во элементов: С%2Т в строке С%2Т.")
				.arg(QString(pointNames.length()), QString(lineElements.length()), QString(i));

		for (int j = 0; j < lineElements.length(); j++) {
			bool ok;
			int number = lineElements[j].toInt(&ok);
			if (!ok || number < 0)
				throw QString::fromUtf8(u8"Ёлемент матрицы в строке С%1Т под номером С%2Т имеет вид С%3Т. ƒопустимые значени€ элементов Ц только положительные числа.")
					.arg(QString(i), QString(j+1), lineElements[j]);

			if (i - 1 == j && number != 0)
				throw QString::fromUtf8(u8"Ќа главной диагонали матрицы рассто€ний может быть значение только У0Ф. “очки не могут быть соединены сами с собой.");

			matrixLine.append(number);
		}

		distancesMatrix.append(matrixLine);
	}
}

void Graph::calculateLabelsFromPoint(QString pointName) {
	originPointName = pointName;
	originPointIndex = getPointIndex(pointName);
	
	pointLabels.clear();
	for (int i = 0; i < pointNames.length(); i++)
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

int Graph::getDistanceTo(QString fromPointName, QString toPointName) { // -1 - not connected
	if (originPointName != fromPointName)
		calculateLabelsFromPoint(fromPointName);

	int distance = pointLabels[getPointIndex(toPointName)];
	return distance == INT_MAX ? -1 : distance;
}

QStringList Graph::getMinPathTo(QString fromPointName, QString toPointName) { // length == 0 - not connected
	if (originPointName != fromPointName)
		calculateLabelsFromPoint(fromPointName);
	
	QStringList result;
	int currentIndex = getPointIndex(toPointName);
	while (currentIndex != originPointIndex) {
		result.append(pointNames[currentIndex]);
		QList<int> distancesToPoints = getConnectedPoints(currentIndex);

		for (int i = 0; i < distancesToPoints.length(); i++)
			if (distancesToPoints[i] != 0 && pointLabels[i] == pointLabels[currentIndex] - distancesToPoints[i]) {
				currentIndex = i;
				break;
			}
	}

	if (result.length() != 0) {
		result.append(pointNames[originPointIndex]);
		for (int k = 0, s = result.size(), max = (s / 2); k < max; k++)
			result.swapItemsAt(k, s - (1 + k));
	}

	return result;
}

int Graph::getPointIndex(QString pointName) { // -1 if not exists
	for (int i = 0; i < pointNames.length(); i++)
		if (pointNames[i] == pointName)
			return i;

	throw QString::fromUtf8(u8" онечна€ или начальна€ точка не была найдена в списке всех точек.");
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

QStringList Graph::getPointNames() {
	return pointNames;
}

QList<QList<int>> Graph::getDistanceMatrix() {
	return distancesMatrix;
}