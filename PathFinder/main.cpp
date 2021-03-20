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

    Graph& graph = Graph(readFile(graphFilePath));
    
    return 0;
}
