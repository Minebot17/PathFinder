#include <QtCore/QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QList>
#include "Graph.h"

QStringList readFile(QString absolutePath) {
    QStringList result;
    QFile inputFile(absolutePath);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            result.append(line);
        }

        inputFile.close();
    }
    
    return result;
}

void writeToFile(QString absolutePath, QStringList lines) {
    QFile outputFile(absolutePath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&outputFile);

        for (int i = 0; i < lines.length(); i++)
            stream << lines[i] << endl;

        outputFile.close();
    }
}

QString getPath(char* path) {
    QString result(path);

    if (result.startsWith(QChar('.')))
        result = QDir::currentPath() + result.mid(1);
    
    return result;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QTextStream out(stdout);

    QString graphFilePath = getPath(argv[1]);
    QString pointsFilePath = getPath(argv[2]);
    QString outFilePath = getPath(argv[3]);

    QStringList fromToPoints = readFile(pointsFilePath)[0].split(QChar(';'));
    Graph& graph = Graph(readFile(graphFilePath));
    graph.calculateLabelsFromPoint(fromToPoints[0]);
    
    int minDistance = graph.getDistanceTo(fromToPoints[1]);
    QStringList minPath = graph.getMinPathTo(fromToPoints[1]);

    if (minDistance == -1)
        writeToFile(outFilePath, QStringList(QString("Путь между указанными точками отсутствует")));
    else {
        QStringList outLines;
        QString pathLine;

        for (int i = 0; i < minPath.length(); i++)
            pathLine += minPath[i] + "-";

        pathLine = pathLine.mid(0, pathLine.length() - 1);
        outLines.append(pathLine);
        outLines.append(QString::number(minDistance));

        writeToFile(outFilePath, outLines);
    }
    
    return 0;
}
