#pragma once
#include <QtCore/QCoreApplication>
#include <QList>
class Graph
{
public: 

	/// <summary>
	/// Инициализирует граф
	/// </summary>
	/// <param name="lines">На первой строке список точек из файла, на других матрица расстояний</param>
	Graph(QStringList lines);

	/// <summary>
	/// Возвращает дистанцию от точки до точки в графе
	/// </summary>
	/// <param name="pointName">Название точки</param>
	/// <returns>Расстояние от указанной точки до расчётной. Если точки не соедененны, возвращает -1</returns>
	int getDistanceTo(QString fromPointName, QString toPointName);

	/// <summary>
	/// Возвращает полный кратчайший путь от точки до точки в графе
	/// </summary>
	/// <param name="pointName"></param>
	/// <returns>Список имён точек, которые представляют кратчайший путь. Если точки не соедененны, возвращает пустой список</returns>
	QStringList getMinPathTo(QString fromPointName, QString toPointName);

private:
	/// <summary>
	/// Последовательный список имён всех точек графа
	/// </summary>
	QStringList pointNames;

	/// <summary>
	/// Взвешенная матрица смежности
	/// </summary>
	QList<QList<int>> distancesMatrix;

	/// <summary>
	/// Индекс точки, откуда производился расчёт по алгоритму Дейкстры
	/// </summary>
	int originPointIndex;

	/// <summary>
	/// Имя точки, откуда производился расчёт по алгоритму Дейкстры
	/// </summary>
	QString originPointName;

	/// <summary>
	/// Список меток, рассчитанных алгоритмом Дейкстры для конкретной точки
	/// </summary>
	QList<int> pointLabels;

	/// <summary>
	/// Расчитывает расстояние до всех точек графа из определенной точки
	/// </summary>
	/// <param name="pointName">Точка, до куда расчитывать расстояние</param>
	void calculateLabelsFromPoint(QString pointName);

	/// <summary>
	/// Возвращает числовой индекс точки по её имени
	/// </summary>
	/// <param name="pointName">Имя точки</param>
	/// <returns>Индекс точки. -1 если такой точки не существует в графе</returns>
	int getPointIndex(QString pointName);

	/// <summary>
	/// Находит индекс минимального значения в списке
	/// </summary>
	/// <param name="list">Список значений</param>
	/// <returns>Индекс минимального значения</returns>
	int getIndexOfMin(QList<int> list);

	/// <summary>
	/// Возвращает дистанцию до всех точек из указанной
	/// </summary>
	/// <param name="toPoint">Индекс точки, из которой ищем дистанцию</param>
	/// <returns>Список дистанций до всех точек</returns>
	QList<int> getConnectedPoints(int toPoint);
};

