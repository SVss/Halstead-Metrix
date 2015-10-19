#include "stuff.h"
#include "regexes.h"

#include "pascal_counter.h"

#ifdef DEBUG
    #include <QDebug>
#endif

typedef QHash<QString, int> TDict;

TExtractionResult count(QString &source, ConstRegExp RE, const uint CAP_GROUP = 0, ConstRegExp RE_LEFTOVERS)
{
    TExtractionResult result;
    TDict dict;

    QRegularExpressionMatchIterator currMatchIter = RE.globalMatch(source);
    QString buf;

    if (RE == CO_NUMERIC_LITERALS)
    {
        while (currMatchIter.hasNext() )
        {
            buf = currMatchIter.next().captured(0);
            buf.remove(QRegularExpression("[+\\s]") );  // delete unneeded whitespaces
                                                            // and plus-signs (in positive numbers)
            ++dict[buf];
        }
    }
    else
    {
        while (currMatchIter.hasNext() )
        {
            ++dict[currMatchIter.next().captured(CAP_GROUP).replace(WHITE_SPACES, SPACE)];
        }
    }

// removing matches and leftovers
    source.replace(RE, SPACE);

    if (RE_LEFTOVERS.pattern() != "")
        source.replace(RE_LEFTOVERS, SPACE);

    #ifdef DEBUG
        qDebug() << "~~~~~~";
    #endif

    TDict::iterator currLiteral;
    for (currLiteral = dict.begin(); currLiteral != dict.end(); ++currLiteral)
    {
        ++result.unique;
        result.overall += currLiteral.value();

        #ifdef DEBUG
            qDebug() << currLiteral.key() << ": " << currLiteral.value() << "\n";
        #endif
    }

    deleteMultiSpaces(source);

    return result;
}   /* count */


TExtractionResult countEnumConsts(QString &source)
{
    TExtractionResult result;

    QStringList enumConstsDefs;
    enumConstsDefs = getEnumConsts(source);

    if (!enumConstsDefs.isEmpty())
        result += count(source, makeListRegExp(enumConstsDefs) );

    return result;
}   /* countEnumConsts */

TExtractionResult countUserTypes(QString &source)
{
    TExtractionResult result;

    QStringList localTypesList;
    localTypesList = getLocalDefs(source, BL_TYPE_SECTION, 0, BL_LOCAL_TYPES, BL_LOCAL_TYPES_GROUP);

    if (!localTypesList.isEmpty())
        result += count(source, makeListRegExp(localTypesList) );

    return result;
}   /* countUserTypes */

TExtractionResult countVarConsts(QString &source)
{
    TExtractionResult result;
    QStringList localsDefsList;

    localsDefsList = getLocalDefs(source, BL_CAPTION, BL_CAPTION_GROUP,
        BL_CAP_PARAMS, BL_CAP_PARAMS_GROUP, CO_PARAM_OPERATORS);

    source.remove(BL_CAPTION);
    localsDefsList += getLocalDefs(source, BL_VAR_CONST_SECTION, 0,
        BL_VARCONST_IDENTIFIERS, BL_VARCONST_IDENTIFIERS_GROUP);

    if (!localsDefsList.isEmpty())
        result += count(source, makeListRegExp(localsDefsList) );

    result += count(source, BL_STANDARD_VARIABLES_CONSTANTS);

    return result;
}   /* countVarConsts */


void countProcFuncsCalls(QString &source, TExtractionResult &operands,
                                         TExtractionResult &operators)
{
    TExtractionResult result;
    QStringList subroutinesList;

    // functions calls
    subroutinesList = getLocalDefs(source, REG_ALL, 0, CO_FUNC_CALLS, CO_FUNC_CALLS_GROUP, CALLS_EXCEPTIONS);

    if (!subroutinesList.isEmpty() )
        result = count(source, makeListRegExp(subroutinesList) );

    operands += result;
    operators += result;

    // procedures calls
    subroutinesList = getLocalDefs(source, REG_ALL, 0, CO_VALID_IDS, 0, CO_STANDARD_OPERATORS_ALL);

    if (!subroutinesList.isEmpty() )
        result = count(source, makeListRegExp(subroutinesList) );

    operators += result;
}   /* countProcFuncsCalls */


uint countInputVariablesPascal(QByteArray srcCode)
{
    QStringList inputVars = getLocalDefs(QString(srcCode), REG_ALL, 0, INPUT_VARIABLES, INPUT_VARIABLES_GROUP);

    return inputVars.count();
}   /* countInputVariablesPascal */

void analyzeBlock(QString &block, TExtractionResult &operands,
                                  TExtractionResult &operators)
{
    #ifdef DEBUG
        qDebug() << "\n\nBlock:" << block.toLatin1().constData() << "\n===============\n";
    #endif

    operands += countEnumConsts(block);

    operators += countUserTypes(block);

    operands += countVarConsts(block);

}   /* analyzeBlock */


void countOpsPascal(QByteArray srcCode, uint &uniqOperators, uint &allOperators,
                                        uint &uniqOperands, uint &allOperands)
{
    TExtractionResult operators, operands;

    deleteComments(srcCode);

    QString source(srcCode);
    deleteProgramCaption(source);

    operands = count(source, CO_STRING_LITERALS);

    source = source.toLower();

    operators = count(source, CO_SEPARATORS_1, 0, CO_SEPARATORS_1_LEFT);
    operands += count(source, CO_NUMERIC_LITERALS);
    operators += count(source, CO_STANDARD_TYPES, CO_STANDARD_TYPES_GROUP);

    source.remove(BL_FORWARD_DECLARATIONS);

    QStringList blocks;
    blocks = extractBlocks(source);

    int i;
    for (i = 0; i < blocks.size(); ++i)
    {
        analyzeBlock(blocks[i], operands, operators);
    }

    source += blocks.join("\n\n");

    operands += countEnumConsts(source);
    operators += countUserTypes(source);
    operands += countVarConsts(source);

    countProcFuncsCalls(source, operands, operators);

    operators += count(source, CO_STANDARD_OPERATORS, 0, CO_STANDARD_OPERATORS_LEFT);
    operators += count(source, CO_SEPARATORS_2, 0, CO_SEPARATORS_2_LEFT);

    uniqOperators = operators.unique;
    allOperators = operators.overall;

    uniqOperands = operands.unique;
    allOperands = operands.overall;

}   /* countOpsPascal */
