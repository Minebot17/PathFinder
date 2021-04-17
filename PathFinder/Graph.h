#pragma once
#include <QtCore/QCoreApplication>
#include <QList>
class Graph
{
public: 

	/// <summary>
	/// �������������� ����
	/// </summary>
	/// <param name="lines">�� ������ ������ ������ ����� �� �����, �� ������ ������� ����������</param>
	Graph(QStringList lines);

	/// <summary>
	/// ���������� ��������� �� ����� �� ����� � �����
	/// </summary>
	/// <param name="pointName">�������� �����</param>
	/// <returns>���������� �� ��������� ����� �� ���������. ���� ����� �� ����������, ���������� -1</returns>
	int getDistanceTo(QString fromPointName, QString toPointName);

	/// <summary>
	/// ���������� ������ ���������� ���� �� ����� �� ����� � �����
	/// </summary>
	/// <param name="pointName"></param>
	/// <returns>������ ��� �����, ������� ������������ ���������� ����. ���� ����� �� ����������, ���������� ������ ������</returns>
	QStringList getMinPathTo(QString fromPointName, QString toPointName);

private:
	QStringList pointNames;
	QList<QList<int>> distancesMatrix;
	int originPointIndex;
	QString originPointName;
	QList<int> pointLabels;

	/// <summary>
	/// ����������� ���������� �� ���� ����� ����� �� ������������ �����
	/// </summary>
	/// <param name="pointName">�����, �� ���� ����������� ����������</param>
	void calculateLabelsFromPoint(QString pointName);

	/// <summary>
	/// ���������� �������� ������ ����� �� � �����
	/// </summary>
	/// <param name="pointName">��� �����</param>
	/// <returns>������ �����. -1 ���� ����� ����� �� ���������� � �����</returns>
	int getPointIndex(QString pointName);

	/// <summary>
	/// ������� ������ ������������ �������� � ������
	/// </summary>
	/// <param name="list">������ ��������</param>
	/// <returns>������ ������������ ��������</returns>
	int getIndexOfMin(QList<int> list);

	/// <summary>
	/// ���������� ��������� �� ���� ����� �� ���������
	/// </summary>
	/// <param name="toPoint">������ �����, �� ������� ���� ���������</param>
	/// <returns>������ ��������� �� ���� �����</returns>
	QList<int> getConnectedPoints(int toPoint);
};

