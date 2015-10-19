#ifndef PASCAL_COUNTER_H_
#define PASCAL_COUNTER_H_

#include <QString>

const uint PERFECT_PROGRAM_UNIQUE_OPERATORS = 9;
/*
const QString perfectProgram =
        "program do_everything;"\
        "var"                   \
        "   ;"                  \
        "begin"\
        "   readln();"          \
        "   do_work();"         \
        "   writeln();"         \
        "end."                  ;
*/

// counter
uint countInputVariablesPascal(QByteArray srcCode);

void countOpsPascal(QByteArray srcCode, uint &uniqOperators, uint &allOperators,
                                        uint &uniqOperands, uint &allOperands);

#endif // PASCAL_COUNTER_H_
