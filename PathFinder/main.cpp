#include <QtCore/QCoreApplication>
#include <QDir>
#include <QTextStream>
#include "Graph.h"
#include <regex>

QRegExp rx("*.txt");

/// <summary>
/// Считывает файл по строкам
/// </summary>
/// <param name="absolutePath">Абсолютный путь до файла</param>
/// <returns>Считанные строки файла</returns>
QStringList readFile(QString absolutePath) {
    rx.setPatternSyntax(QRegExp::Wildcard);

    QStringList result;
    QFile inputFile(absolutePath);

    if (!rx.exactMatch(inputFile.fileName()))
        throw GraphError{ 6 };

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
        throw GraphError{ 7 };
    
    return result;
}

/// <summary>
/// Записывает в файл строки (с перезаписью)
/// </summary>
/// <param name="absolutePath">Абсолютный путь до файла</param>
/// <param name="lines">Строки для записи в файл</param>
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
        throw GraphError{ 8 };
}

/// <summary>
/// Преобразует относительный путь в абсолютный
/// </summary>
/// <param name="path">Относительный путь, в начале должна стоять точка</param>
/// <returns>Абсолютный путь</returns>
QString getPath(char* path) {
    QString result(path);

    if (result.startsWith(QChar('.')))
        result = QDir::currentPath() + result.mid(1);
    
    return result;
}

QString getErrorMessage(GraphError err) {
    switch (err.errorCode)
    {
        case 0:
            return QString::fromUtf8(u8"Кол-во точек должно быть столько же, сколько строк в матрице смежности. Кол-во точек: ‘%1’. Кол-во строк ‘%2’.")
                .arg(QString::number(err.data[0]), QString::number(err.data[1]));
        case 1:
            return QString::fromUtf8(u8"Точек в схеме должно быть минимум 2-е или более.");
        case 2:
            return QString::fromUtf8(u8"Кол-во точек должно быть столько же, сколько элементов в каждой строке матрицы смежности. Кол-во точек: '%1'. Кол-во элементов: ‘%2’ в строке ‘%3’.")
                .arg(QString::number(err.data[0]), QString::number(err.data[1]), QString::number(err.data[2]));
        case 3:
            return QString::fromUtf8(u8"Элемент матрицы в строке ‘%1’ под номером ‘%2’ имеет вид ‘%3’. Допустимые значения элементов – только положительные числа.")
                .arg(QString::number(err.data[0]), QString::number(err.data[1]), QString::number(err.data[2]));
        case 4:
            return QString::fromUtf8(u8"На главной диагонали матрицы расстояний может быть значение только “0”. Точки не могут быть соединены сами с собой.");
        case 5:
            return QString::fromUtf8(u8"Конечная или начальная точка не была найдена в списке всех точек.");
        case 6:
            return QString::fromUtf8(u8"Неверно указано расширение файла. Файл должен иметь расширение .txt");
        case 7:
            return QString::fromUtf8(u8"Неверно указан файл с входными данными. Возможно, файл не существует");
        case 8:
            return QString::fromUtf8(u8"Неверно указан файл для выходных данных. Возможно, указанного расположения не существует.");
        case 9:
            return QString::fromUtf8(u8"Конечная или начальная точка отсутствует во входном файле точек.");
        default:
            return QString::fromUtf8(u8"Неизвестная ошибка.");
    }
}

/// <summary>
/// Точка входа в программу
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
            throw GraphError{ 9 };

        Graph& graph = Graph(readFile(graphFilePath));

        int minDistance = graph.getDistanceTo(fromToPoints[0], fromToPoints[1]);
        QStringList minPath = minDistance == -1 ? QStringList() : graph.getMinPathTo(fromToPoints[0], fromToPoints[1]);

        if (minDistance == -1)
            writeToFile(outFilePath, QStringList(QString::fromUtf8(u8"Путь между указанными точками отсутствует")));
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
    catch (GraphError err) {
        try {
            QStringList outLines;
            outLines.append(getErrorMessage(err));
            writeToFile(outFilePath, outLines);
        }
        catch (GraphError err) {
            out << "\n" << getErrorMessage(err);
        }
    }
    
    return 0;
}
