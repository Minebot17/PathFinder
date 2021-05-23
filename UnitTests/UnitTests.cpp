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
	};
}
