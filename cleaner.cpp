#include "stuff.h"

typedef enum TState
{
    ST_NORMAL = 0,
    ST_STRING = 1,
    ST_SINGLELINE_COMMENT = 2,
    ST_MULTILINE_COMMENT_A = 3,
    ST_MULTILINE_COMMENT_B = 4
} TState;


typedef enum TChanges
{
    CH_NONE = 0,
    CH_COMMENT_OPENED = 1,
    CH_COMMENT_CLOSED = 2,
    CH_COMMENT_B_CLOSED = 3
} TChanges;


TChanges setStateEx(TState *state, char buf[2], bool &skipApostriphe)
{
    TChanges result = CH_NONE;
    skipApostriphe = false;

    switch (buf[0])
    {
        case '{':   // open multiline comment '{'
            if (*state == ST_NORMAL)
            {
                *state = ST_MULTILINE_COMMENT_A;
                result = CH_COMMENT_OPENED;
            }
            break;

        case '}':   // close multiline comment '}'
            if (*state == ST_MULTILINE_COMMENT_A)
            {
                *state = ST_NORMAL;
                result = CH_COMMENT_CLOSED;
            }
            break;

        case '(':   // open multiline comment '(*'
            if ( (*state == ST_NORMAL) && (buf[1] == '*') )
            {
                *state = ST_MULTILINE_COMMENT_B;
                result = CH_COMMENT_OPENED;
            }
            break;

        case '*':   // close multiline comment '*)'
            if ( (*state == ST_MULTILINE_COMMENT_B) && (buf[1] == ')') )
            {
                *state = ST_NORMAL;
                result = CH_COMMENT_B_CLOSED;
            }
            break;

        case '/':   // open singleline comment '//'
            if ( (*state == ST_NORMAL) && (buf[1] == '/') )
            {
                *state = ST_SINGLELINE_COMMENT;
                result = CH_COMMENT_OPENED;
            }
            break;

        case '\n':  // end of the singleline comment '//'
            if (*state == ST_SINGLELINE_COMMENT)
            {
                *state = ST_NORMAL;
                result = CH_COMMENT_CLOSED;
            }
            break;

        case '\'':
            if (*state == ST_NORMAL)    // open string literal
            {
                *state = ST_STRING;
            }
            else
            {
                if (*state == ST_STRING)
                {
                    if (buf[1] == '\'')   // close string literal
                    {
                        skipApostriphe = true;  // skip apostrophe character
                    }
                    else
                    {
                       *state = ST_NORMAL;
                    }
                }
            }
            break;
    }

    return result;
}   /* setStateEx */

void deleteComments(QByteArray &srcCode)
{
    srcCode.append("\n ");  // to process comments at the eof
    TState state = ST_NORMAL;

    char buf[2];
    bool skipApostrophe; // to skip apostrophe character in string literals

    TChanges changes = CH_NONE;

    uint pos1 = 0,      // pos1 stores the 2nd symbol position => 1st symbol pos = po1-1
         commentStart = 0;

    buf[0] = 0;
    buf[1] = srcCode[pos1];

    while (buf[1])
    {
        changes = setStateEx(&state, buf, skipApostrophe);

        switch (changes)
        {
            case CH_COMMENT_OPENED:
                commentStart = pos1-1;  // 1st symbol position
                break;

            case CH_COMMENT_CLOSED:
                srcCode.replace(commentStart, pos1-commentStart, SPACE);
                pos1 = commentStart+1;
                break;

        case CH_COMMENT_B_CLOSED:       // buf[0] = '*', buf[1] = ')'
                srcCode.replace(commentStart, pos1-commentStart+1, SPACE);
                pos1 = commentStart+1;
                break;

            case CH_NONE:
                break;
        }

        if (skipApostrophe)  // skip apostrophe character
        {
            buf[0] = buf[1];
            buf[1] = srcCode[++pos1];

            skipApostrophe = false;
        }

        if (buf[1])
        {
            buf[0] = buf[1];
            buf[1] = srcCode[++pos1];
        }
    }

}   /* deleteComments */


QString deleteMultiSpaces(QString &source)
{
    while (source.indexOf(DOUBLE_SPACE) != -1)
        source.replace(DOUBLE_SPACE, SPACE);

    return source;
}   /* deleteMultiSpaces */


void deleteProgramCaption(QString &source)
{
    int posCap = source.indexOf("program", Qt::CaseInsensitive);
    if (posCap != -1)
    {                   // delete only caption, without semicolomn
        source.remove(posCap, source.indexOf(";", posCap) );
    }
}   /* deleteProgramCaption */
