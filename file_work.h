#ifndef FILE_WORK
#define FILE_WORK

#include <QByteArray>
#include <QFile>

QString getRelativeFileName(QString filename);

QByteArray readSource(QString fileName);
bool writeResult(QString res, QString fileName);

#endif // FILE_WORK

