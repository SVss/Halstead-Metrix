#ifndef REGEXES_H_
#define REGEXES_H_

#include <QString>
#include <QRegularExpression>

const QRegularExpression::PatternOptions STD_PTRN_OPTIONS =
(
    QRegularExpression::DotMatchesEverythingOption |
    QRegularExpression::CaseInsensitiveOption
    );

typedef const QRegularExpression ConstRegExp;
#define CONST_REGEX(const_regex_name, regex_text) ConstRegExp const_regex_name(regex_text, STD_PTRN_OPTIONS)


CONST_REGEX(WHITE_SPACES, "\\s" );

CONST_REGEX(REG_ALL, ".*");


//* COMMON REGEXES:

CONST_REGEX(CO_STRING_LITERALS, "'([^']+|'')*'" );


CONST_REGEX(CO_SEPARATORS_1, "\\[|\\.{2}" );

CONST_REGEX(CO_SEPARATORS_1_LEFT, "\\]" );

CONST_REGEX(CO_SEPARATORS_2, "[\\(,;]|:(?!=)|\\.{2}|\\.(?!\\d)" );

CONST_REGEX(CO_SEPARATORS_2_LEFT, "\\)" );


CONST_REGEX(CO_NUMERIC_LITERALS,
        "(?:(?<=\\(|=|\\+|-|\\*|\\/|\\bdiv\\b|\\bmod\\b|\\bto\\b|\\bdownto\\b)\\s*"\
        "[-+]|(?<=\\W))\\s*[#]?\\d+\\.?\\d*"
    );

CONST_REGEX(CO_STANDARD_TYPES,
        "(?:|\\bof\\b)\\s*\\b((array|file|set)\\s*of|packed|int64|cardinal|"\
        "boolean|byte|integer|smallint|shortint|longint|word|longword|"\
        "real|float|double|char|pchar|string|ansistring|text|pointer)\\b"
    );

const uint CO_STANDARD_TYPES_GROUP = 1;


CONST_REGEX(CO_STANDARD_OPERATORS,
        "\\b(shl|shr|div|mod|and|or|xor|not|begin|if|for|while|repeat|case|with|"\
        "break|continue|exit|goto|read(ln)?|write(ln)?)\\b|(\\+|\\-|\\*|\\/|@|\\^|"\
        ":=|<|>|<=|>=|=|<>)"
    );

CONST_REGEX(CO_STANDARD_OPERATORS_LEFT,
        "\\b(else|then|do|downto|to|until|end)\\b"
    );

CONST_REGEX(CO_STANDARD_OPERATORS_ALL,
        CO_STANDARD_OPERATORS.pattern() + "|" + CO_STANDARD_OPERATORS_LEFT.pattern()
    );


CONST_REGEX(CO_PARAM_OPERATORS, "\\b(var|const|type|out)\\b" );


CONST_REGEX(CO_BLOCK,
        "(?:\\bprocedure\\b|\\bfunction\\b).*?(?:\\bend\\b\\s*;\\s*(?=\\bvar\\b|"\
        "\\bconst\\b|\\bprocedure\\b|\\bfunction\\b|\\bbegin\\b|\\btype\\b))"
    );

CONST_REGEX(CO_FUNC_CALLS,
        "(:=|\\b(while|\\(|to|downto))[^;]*?([\\w][\\w\\d]*)"
    );

const uint CO_FUNC_CALLS_GROUP = 3;


CONST_REGEX(CO_VALID_IDS,
        "\\b[\\w][\\w\\d]*\\b"
    );

CONST_REGEX(CALLS_EXCEPTIONS,
        "\\b(shl|shr|div|mod|and|or|xor|not|begin|if|for|while|repeat|case|with|"\
        "break|continue|exit|goto|read(ln)?|write(ln)?|else|then|do|downto|to|until|end)\\b"
    );


//* BLOCK REGEXES:

CONST_REGEX(BL_CAPTION,
        "(\\bprocedure\\b|\\bfunction\\b)\\s*[\\w][\\w\\d]*\\s*"\
        "(\\(.*?\\)\\s*|):?\\s*([\\w][\\w\\d]*|)\\s*;"
    );

const uint BL_CAPTION_GROUP = 2;


CONST_REGEX(BL_FORWARD_DECLARATIONS,
        "(\\bprocedure\\b|\\bfunction\\b)\\s*[\\w][\\w\\d]*\\s*"\
        "(\\(.*?\\)\\s*|):?\\s*([\\w][\\w\\d]*|)\\s*;\\s*forward\\s*;"
    );

CONST_REGEX(BL_CAP_PARAMS, "(\\(|;)\\s*(.*?(?=(\\)|:)))" );

const uint BL_CAP_PARAMS_GROUP = 2;


CONST_REGEX(BL_VAR_TYPE_SECTION,
        "\\b(?<=var|type)\\b(.*?)\\b(?=begin|type|procedure|function|const|var|type)\\b"
    );

CONST_REGEX(BL_ENUMERATED, "[^\\w]\\((.*?)\\)");

CONST_REGEX(BL_TYPE_SECTION,
        "\\btype\\b(.*?)\\b(?=begin|type|procedure|function|const|var)\\b"
    );

CONST_REGEX(BL_LOCAL_TYPES, "(?<=\\btype|;)\\s*(.*?)=" );

const uint BL_LOCAL_TYPES_GROUP = 1;


CONST_REGEX(BL_VAR_CONST_SECTION,
        "\\b(var|const)\\b(.*?)\\b(?=begin|type|procedure|function|const|var|type)\\b"
    );

CONST_REGEX(BL_VARCONST_IDENTIFIERS,
        "(?<=\\bvar\\b|\\bconst\\b|\\brecord\\b|;)\\s*"\
        "([\\w,\\s]*?)\\s*(?::)"
    );

const uint BL_VARCONST_IDENTIFIERS_GROUP = 1;

CONST_REGEX(BL_STANDARD_VARIABLES_CONSTANTS,
        "\\b(ioresult|result|maxint|pi|true|false|nil)\\b"
    );


//* INPUT VARIABLES REGEX

CONST_REGEX(INPUT_VARIABLES,
        "read(ln)?\\((.*?)\\)"
    );

const uint INPUT_VARIABLES_GROUP = 2;


// StringList to Regex converter
ConstRegExp makeListRegExp(QStringList list);

#endif // REGEXES_H_
