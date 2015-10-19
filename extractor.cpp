#include "stuff.h"
#include "regexes.h"

#ifdef DEBUG
    #include <QDebug>
#endif

QStringList extractBlocks(QString &source)
{
    QStringList result;
    QRegularExpressionMatchIterator currMatchIter = CO_BLOCK.globalMatch(source);

    while (currMatchIter.hasNext() )
    {
        result.append(currMatchIter.next().captured(0).trimmed() );
    }

    source.remove(CO_BLOCK);

    return result;
}   /* extractBlocks */


QStringList getSectionEnums(QString &section)
{
    QStringList result;
    QRegularExpressionMatchIterator currMatchIter = BL_ENUMERATED.globalMatch(section);

    while (currMatchIter.hasNext() )
    {
        result.append(currMatchIter.next().captured(1).remove(QRegularExpression("\\s") ).split(",") );

        #ifdef DEBUG
            qDebug() << "\n~~~~~~\n";
        #endif

        int resIter;
        for (resIter = 0; resIter < result.size()-1; ++resIter)
        {
            section.remove(result.at(resIter) );

            #ifdef DEBUG
                qDebug() << result.at(resIter).toLatin1().constData() << "\n";
            #endif
        }

        #ifdef DEBUG
            qDebug() << result.last().toLatin1().constData() << "\n";
        #endif
    }

    return result;
}   /* getSectionEnums */

QStringList getEnumConsts(QString &block)
{
    QStringList result;
    QString currSection;

    QRegularExpressionMatchIterator currMatchIter = BL_VAR_TYPE_SECTION.globalMatch(block);

    while (currMatchIter.hasNext() )
    {
        currSection = currMatchIter.next().captured(0);
        result.append(getSectionEnums(currSection) );
    }

    return result;
}   /* getEnumConsts */


QStringList getLocalDefs(QString block, ConstRegExp REG_OUTER, const uint outerCapGroup,
                         ConstRegExp REG_INNER, const uint innerCapGroup,
                         ConstRegExp REMOVE_REG)
{
    QStringList result;
    QString params, currParam;

    #ifdef DEBUG
        qDebug() << "\n~~~~~~\n";
    #endif

    QRegularExpressionMatchIterator currMatchIter = REG_OUTER.globalMatch(block);
    if (!currMatchIter.hasNext() )
        return result;       // empty result if not found

    params = currMatchIter.next().captured(outerCapGroup);

    currMatchIter = REG_INNER.globalMatch(params);
    while (currMatchIter.hasNext() )
    {
        currParam = currMatchIter.next().captured(innerCapGroup).remove(REMOVE_REG).remove(WHITE_SPACES);

        if (!currParam.isEmpty() )
            result.append(currParam.split(",") );
    }

    #ifdef DEBUG
        for (int resIter = 0; resIter < result.size(); ++resIter)
            qDebug() << result.at(resIter).toLatin1().constData() << "\n";
    #endif

    return result;
}   /* getLocalDefs */

QStringList getVarConstDefs(QString &source)
{
    QStringList result;
    result = getLocalDefs(source, BL_VAR_CONST_SECTION, 0, BL_VARCONST_IDENTIFIERS, 1);

    return result;
}   /* getVarConsts */

QStringList getUserTypesDefs(QString &source)
{
    QStringList result;
    result = getLocalDefs(source, BL_TYPE_SECTION, 0, BL_LOCAL_TYPES, 1);

    return result;
}   /* getUserTypesDefs */
