#include "file_work.h"
#include <QTextStream>
#include <QFileInfo>

QString getRelativeFileName(QString filename)
{
    return QFileInfo(filename).fileName();
}   /* getRelativeFileName */


QByteArray readSource(QString fileName)
{
    QFile input(fileName);
    if (!input.open(QIODevice::ReadOnly) )
        return NULL;

    QByteArray result;
    result = input.readAll();

    input.close();

    return result;
}   /* readSource */

bool writeResult(QString res, QString fileName)
{
    QTextStream con(stdout);
    con << res;

    QFile output(fileName);
    if (!output.open(QIODevice::WriteOnly) )
        return false;

    output.write(res.toLatin1() );

    output.close();
    return true;
}   /* writeResult */
