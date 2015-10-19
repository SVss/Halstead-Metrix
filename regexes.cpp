#include "stuff.h"

const QRegularExpression makeListRegExp(QStringList list)
{
    QString regex_text = "\\b(" + list.join("|") + ")\\b";
    return QRegularExpression(regex_text);
}
