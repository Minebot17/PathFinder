#include "pch.h"
#include "CppUnitTest.h"
#include "../PathFinder/Graph.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	private:

		void AssertGraph(Graph graph, QStringList pointNames, QList<QList<int>> distances) {
			Assert::AreEqual(pointNames.length(), graph.getPointNames().length());
			for (int i = 0; i < graph.getPointNames().length(); i++)
				Assert::IsTrue(pointNames[i] == graph.getPointNames()[i]);

			Assert::AreEqual(distances.length(), graph.getDistanceMatrix().length());
			for (int i = 0; i < graph.getDistanceMatrix().length(); i++) {
				Assert::AreEqual(distances[i].length(), graph.getDistanceMatrix()[i].length());
				for (int j = 0; j < graph.getDistanceMatrix()[i].length(); j++)
					Assert::AreEqual(distances[i][j], graph.getDistanceMatrix()[i][j]);
			}
		}

		Graph getDefaultGraph() {
			return Graph({
					"Moskow;Volgograd;Krasnadar;Tombow;Voronez;New-York", 
					"0;1000;0;0;0;0",
					"2000;0;5000;0;0;0",
					"0;0;0;3000;0;0",
					"0;7000;0;0;0;0",
					"0;0;0;1000;0;0",
					"0;0;0;0;0;0",
				});
		}

	public:
		
		TEST_METHOD(Graph_TwoPoints)
		{
			QStringList sheme = { 
				"Moskow;Volgograd",
				"0;2000",
				"3000;0"
			};

			QStringList pointNames = { "Moskow", "Volgograd" };
			QList<QList<int>> distances = {
				{ 0, 2000 },
				{ 3000, 0 }
			};

			Graph graph(sheme);
			AssertGraph(graph, pointNames, distances);
		}

		TEST_METHOD(Graph_ThreePoints)
		{
			QStringList sheme = {
				"Moskow;Volgograd;Kazan",
				"0;2000;4000",
				"3000;0;0",
				"0;200;0"
			};

			QStringList pointNames = { "Moskow", "Volgograd", "Kazan" };
			QList<QList<int>> distances = {
				{ 0, 2000, 4000 },
				{ 3000, 0, 0 },
				{ 0, 200, 0 }
			};

			Graph graph(sheme);
			AssertGraph(graph, pointNames, distances);
		}

		TEST_METHOD(Graph_OnePoint)
		{
			QStringList sheme = {
				"Moskow",
				"0",
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(1, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(Graph_WrongLinesCount)
		{
			QStringList sheme = {
				"Moskow;Volgograd",
				"0;2000"
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(0, err.errorCode);
				Assert::AreEqual(2, err.data[0]);
				Assert::AreEqual(1, err.data[1]);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(Graph_WrongElementsCount)
		{
			QStringList sheme = {
				"Moskow;Volgograd",
				"0;2000",
				"3000;0;1000"
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(2, err.errorCode);
				Assert::AreEqual(2, err.data[0]);
				Assert::AreEqual(3, err.data[1]);
				Assert::AreEqual(2, err.data[2]);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(Graph_WrongElement)
		{
			QStringList sheme = {
				"Moskow;Volgograd",
				"0;2000",
				"Volgograd;0"
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(3, err.errorCode);
				Assert::AreEqual(2, err.data[0]);
				Assert::AreEqual(1, err.data[1]);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(Graph_WrongConnectionLength)
		{
			QStringList sheme = {
				"Moskow;Volgograd",
				"0;-100",
				"3000;0"
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(3, err.errorCode);
				Assert::AreEqual(1, err.data[0]);
				Assert::AreEqual(2, err.data[1]);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(Graph_DiagonalConnection)
		{
			QStringList sheme = {
				"Moskow;Volgograd",
				"0;2000",
				"3000;10"
			};

			try {
				Graph graph(sheme);
			}
			catch (GraphError err) {
				Assert::AreEqual(4, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getDistanceTo_PointsConnected)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Moskow"), QString("Volgograd")), 1000);
		}

		TEST_METHOD(getDistanceTo_PointsNotConnected)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Moskow"), QString("New-York")), -1);
		}

		TEST_METHOD(getDistanceTo_PointsInvertedConnected)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Tombow"), QString("Voronez")), -1);
		}

		TEST_METHOD(getDistanceTo_IntermediatePointsIs0)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Volgograd"), QString("Krasnadar")), 5000);
		}

		TEST_METHOD(getDistanceTo_IntermediatePointsIs1)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Volgograd"), QString("Tombow")), 8000);
		}

		TEST_METHOD(getDistanceTo_IntermediatePointsIs2)
		{
			Graph graph = getDefaultGraph();
			Assert::AreEqual(graph.getDistanceTo(QString("Voronez"), QString("Krasnadar")), 13000);
		}

		TEST_METHOD(Graph_PointFromNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getDistanceTo(QString("Rezan"), QString("Moskow"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getDistanceTo_PointToNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getDistanceTo(QString("Moskow"), QString("Rezan"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getDistanceTo_PointNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getDistanceTo(QString("Rezan"), QString("Nur-Sultan"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getMinPathTo_PointsConnected)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Moskow"), QString("Volgograd"));
			QStringList expected = { "Moskow", "Volgograd" };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_PointsNotConnected)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Moskow"), QString("New-York"));
			QStringList expected = { };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_PointsInvertedConnected)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Tombow"), QString("Voronez"));
			QStringList expected = { };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_IntermediatePointsIs0)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Volgograd"), QString("Krasnadar"));
			QStringList expected = { "Volgograd", "Krasnadar" };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_IntermediatePointsIs1)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Volgograd"), QString("Tombow"));
			QStringList expected = { "Volgograd", "Krasnadar", "Tombow" };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_IntermediatePointsIs2)
		{
			Graph graph = getDefaultGraph();
			QStringList result = graph.getMinPathTo(QString("Voronez"), QString("Krasnadar"));
			QStringList expected = { "Voronez", "Tombow", "Volgograd", "Krasnadar" };

			Assert::AreEqual(result.length(), expected.length());
			for (int i = 0; i < result.length(); i++)
				Assert::IsTrue(result[i] == expected[i]);
		}

		TEST_METHOD(getMinPathTo_PointFromNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getMinPathTo(QString("Rezan"), QString("Moskow"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getMinPathTo_PointToNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getMinPathTo(QString("Moskow"), QString("Rezan"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}

		TEST_METHOD(getMinPathTo_PointNotExists)
		{
			Graph graph = getDefaultGraph();

			try {
				graph.getMinPathTo(QString("Rezan"), QString("Nur-Sultan"));
			}
			catch (GraphError err) {
				Assert::AreEqual(5, err.errorCode);
				return;
			}

			Assert::Fail();
		}
	};
}
