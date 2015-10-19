#include <QCoreApplication>

#include <QtMath>

#include "file_work.h"
#include "pascal_counter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // чтение файла
    QString inputFileName = "input.txt";

    if (argc > 1)
        inputFileName = argv[1];

    QString result = QString("\nResults for file: \"%1\"\n").arg(getRelativeFileName(inputFileName) );

    QByteArray source;
    source = readSource(inputFileName);

    if (source.isEmpty() )
    {
        printf("File not found!");
        return a.exec();
    }

    // получение фактических значений
    uint    uniqOperators,  // n1
            uniqOperands,   // n2

            allOperators,   // N1
            allOperands;    // N2


    countOpsPascal(source, uniqOperators, allOperators, uniqOperands, allOperands);

    result += QString("\n[unique/overall]\n\nOperators (n1/N1): %1/%2\nOperands (n2/N2): %3/%4\n").
            arg(uniqOperators).arg(allOperators).arg(uniqOperands).arg(allOperands);


    // рассчет практических показателей:
    uint programVocabulary = uniqOperands + uniqOperators,                   // n
         programLength = allOperands + allOperators,                         // N
         programVolume = programLength * log2(programVocabulary);            // V

    result += QString("\nProgram vocabulary (n): %1\nProgram length (N): %2\nProgram volume (V): %3\n").
            arg(programVocabulary).arg(programLength).arg(programVolume);


    // и теоретических:
    uint theoreticProgramVocabulary =
            2*uniqOperands-1 + PERFECT_PROGRAM_UNIQUE_OPERATORS,             // n*

         theoreticProgramVolume =                                            // V*
            theoreticProgramVocabulary * log2(theoreticProgramVocabulary);

    result += QString("\nTheoretic program vocabulary (n*): %1\nTheoretic program volume (V*): %2\n").
            arg(theoreticProgramVocabulary).arg(theoreticProgramVolume);


    uint theoreticProgramLength =                                            // N^
            uniqOperators*log2(uniqOperators) + uniqOperands*log2(uniqOperands);

    result += QString("Theoretic program length (N^): %1\n").arg(theoreticProgramLength);


    double programQuality = double(theoreticProgramVolume) / programVolume;  // L = V* / V

    result += QString("\nProgramming quality level (L): %1").arg(QString::number(programQuality, 'g', 10) );



    double approximateProgramQuality =                                       // L^ = 2*n2 / (n1*N1)
            2*uniqOperands/double(uniqOperators*allOperands);

    result += QString("\nApproximate program quality (L^): %1\n").arg(QString::number(approximateProgramQuality, 'g', 10) );


    uint intellectualCost =                                                  // E = N^ * log(n/L)
            theoreticProgramLength*log2(programVocabulary/programQuality);

    result += QString("\nIntellectual cost (E): %1\n").arg(intellectualCost);

/*
    uint intellectaulPreceptionCost =                                        // E` = N * log(n/L)
            programLength*log2(programVocabulary/programQuality);

    result += QString("\nIntellectual preception cost (E): %1\n").arg(intellectaulPreceptionCost);
*/

    writeResult(result, "output.txt");

    return a.exec();
}
