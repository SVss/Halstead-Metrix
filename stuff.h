#ifndef STUFF_H_
#define STUFF_H_

//#define DEBUG

#include <QByteArray>
#include <QStringList>

#include "regexes.h"

// результаты анализа
struct TExtractionResult
{
    uint unique;
    uint overall;

    inline TExtractionResult()
    {
        unique = 0;
        overall = 0;
    }

    inline TExtractionResult operator=(TExtractionResult a)
    {
        unique = a.unique;
        overall = a.overall;

        return a;
    }

    inline TExtractionResult operator+(TExtractionResult a)
    {
        unique += a.unique;
        overall += a.overall;

        TExtractionResult result;
        result.unique = unique;
        result.overall = overall;

        return result;
    }

    inline TExtractionResult operator+=(TExtractionResult a)
    {
        return operator+(a);
    }

};

const QByteArray SPACE = " ";
const QString DOUBLE_SPACE = "  ";

// cleaner
void deleteComments(QByteArray &srcCode);
void deleteProgramCaption(QString &source);

QString deleteMultiSpaces(QString &source);

// extractor
QStringList extractBlocks(QString &source);

QStringList getEnumConsts(QString &block);
QStringList getUserTypesDefs(QString &source);
QStringList getVarConstDefs(QString &source);

QStringList getLocalDefs(QString block, ConstRegExp REG_OUTER, const uint outerCapGroup,
                         ConstRegExp REG_INNER, const uint innerCapGroup,
                         ConstRegExp REMOVE_REG = QRegularExpression() );

// counter
TExtractionResult count(QString &source, ConstRegExp FRACTION, const uint CAP_GROUP,
                        ConstRegExp RE_LEFTOVERS = QRegularExpression("") );

TExtractionResult countEnumConsts(QString &source);
TExtractionResult countUserTypes(QString &source);
TExtractionResult countVarConsts(QString &source);

void countProcFuncsCalls(QString &source, TExtractionResult &operands,
                                         TExtractionResult &operators);

void analyzeBlock(QString &block, TExtractionResult &operands,
                                  TExtractionResult &operators);


#endif // STUFF_H_
