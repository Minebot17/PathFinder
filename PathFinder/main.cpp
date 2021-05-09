#include <QtCore/QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QList>
#include "Graph.h"
#include <regex>

QRegExp rx("*.txt");

/// <summary>
/// ��������� ���� �� �������
/// </summary>
/// <param name="absolutePath">���������� ���� �� �����</param>
/// <returns>��������� ������ �����</returns>
QStringList readFile(QString absolutePath) {
    rx.setPatternSyntax(QRegExp::Wildcard);

    QStringList result;
    QFile inputFile(absolutePath);

    if (!rx.exactMatch(inputFile.fileName()))
        throw QString::fromUtf8(u8"������� ������� ���������� �����. ���� ������ ����� ���������� .txt");

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd())
        {
            QString line = in.readLine();
            result.append(line);
        }

        inputFile.close();
    }
    else
        throw QString::fromUtf8(u8"������� ������ ���� � �������� �������. ��������, ���� �� ����������");
    
    return result;
}

/// <summary>
/// ���������� � ���� ������ (� �����������)
/// </summary>
/// <param name="absolutePath">���������� ���� �� �����</param>
/// <param name="lines">������ ��� ������ � ����</param>
void writeToFile(QString absolutePath, QStringList lines) {
    QFile outputFile(absolutePath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&outputFile);
        stream.setGenerateByteOrderMark(true);
        stream.setCodec("UTF-8");

        for (int i = 0; i < lines.length(); i++)
            stream << lines[i] << endl;

        outputFile.close();
    }
    else
        throw QString::fromUtf8(u8"������� ������ ���� ��� �������� ������. ��������, ���������� ������������ �� ����������.");
}

/// <summary>
/// ����������� ������������� ���� � ����������
/// </summary>
/// <param name="path">������������� ����, � ������ ������ ������ �����</param>
/// <returns>���������� ����</returns>
QString getPath(char* path) {
    QString result(path);

    if (result.startsWith(QChar('.')))
        result = QDir::currentPath() + result.mid(1);
    
    return result;
}

/// <summary>
/// ����� ����� � ���������
/// </summary>
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QTextStream out(stdout);

    QString graphFilePath = getPath(argv[1]);
    QString pointsFilePath = getPath(argv[2]);
    QString outFilePath = getPath(argv[3]);

    try {
        QStringList fromToPoints = readFile(pointsFilePath)[0].split(QChar(';'));
        if (fromToPoints.length() < 2)
            throw QString::fromUtf8(u8"�������� ��� ��������� ����� ����������� �� ������� ����� �����.");

        Graph& graph = Graph(readFile(graphFilePath));

        int minDistance = graph.getDistanceTo(fromToPoints[0], fromToPoints[1]);
        QStringList minPath = minDistance == -1 ? QStringList() : graph.getMinPathTo(fromToPoints[0], fromToPoints[1]);

        if (minDistance == -1)
            writeToFile(outFilePath, QStringList(QString::fromUtf8(u8"���� ����� ���������� ������� �����������")));
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
    }
    catch (QString str) {
        try {
            QStringList outLines;
            outLines.append(str);
            writeToFile(outFilePath, outLines);
        }
        catch (QString strr) {
            out << "\n" << strr;
        }
    }
    
    return 0;
}
