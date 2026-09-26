/*
====================================================================
      MINI ADVANCED INTERMEDIATE CODE GENERATOR & OPTIMIZER
                    COMPILER DESIGN LAB PROJECT
====================================================================

FEATURES
--------
1. Lexical Analysis
2. Symbol Table
3. Recursive-Descent Expression Parser
4. Three Address Code (TAC)
5. Quadruples
6. Triples
7. If / Else / While
8. Constant Folding
9. Common Subexpression Elimination
10. Basic Block Analysis
11. Compilation Statistics
12. Menu-driven interface

SUPPORTED LANGUAGE
------------------
int declarations
assignment statements
+  -  *  /  %
<  <=  >  >=  ==  !=
if / else
while
( )
{ }
====================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==================== COLORFUL CONSOLE UI ==================== */
/*
   The project uses ANSI escape sequences for a modern, colorful
   terminal interface. Windows 10/11 and most modern terminals support
   these sequences.
*/

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif /* _WIN32 */

#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_DIM     "\033[2m"
#define C_RED     "\033[31m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BLUE    "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN    "\033[36m"
#define C_WHITE   "\033[37m"
#define C_BG_BLUE "\033[44m"

void enableColorSupport(void)
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (hOut != INVALID_HANDLE_VALUE &&
        GetConsoleMode(hOut, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }
#endif
}

void clearScreen(void)
{
    printf("\033[2J\033[H");
}

void printLine(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
        putchar(ch);
    putchar('\n');
}

void printTitle(const char *title)
{
    printf("\n%s%s", C_BOLD, C_CYAN);
    printLine('=', 68);
    printf("                    %s\n", title);
    printLine('=', 68);
    printf("%s", C_RESET);
}

void printSuccess(const char *msg)
{
    printf("%s%s[ OK ]%s %s\n", C_BOLD, C_GREEN, C_RESET, msg);
}

void printError(const char *msg)
{
    printf("%s%s[ ERROR ]%s %s\n", C_BOLD, C_RED, C_RESET, msg);
}

void printInfo(const char *msg)
{
    printf("%s%s[ INFO ]%s %s\n", C_BOLD, C_YELLOW, C_RESET, msg);
}

void showWelcomeBanner(void)
{
    clearScreen();

    printf("%s%s", C_BOLD, C_CYAN);
    printLine('=', 72);
    printf("        ADVANCED INTERMEDIATE CODE GENERATOR\n");
    printf("              & OPTIMIZATION SYSTEM\n");
    printLine('=', 72);
    printf("%s", C_RESET);

    printf("%s%s                 COMPILER DESIGN LAB%s\n",
           C_BOLD, C_MAGENTA, C_RESET);
    printf("%s\n", C_DIM);
    printf("        Lexical Analysis  ->  Parsing  ->  TAC\n");
    printf("        TAC  ->  Quadruples  ->  Triples  ->  Optimization\n");
    printf("        Control Flow  ->  Basic Blocks  ->  Statistics\n");
    printf("%s\n", C_RESET);

    printf("%s%s   [ Compiler Design Project | C Language | Academic Edition ]%s\n",
           C_BOLD, C_YELLOW, C_RESET);
    printLine('-', 72);
}

void showProjectDashboard(void)
{
    printTitle("PROJECT DASHBOARD");

    printf("%s%s  PROJECT OVERVIEW%s\n", C_BOLD, C_MAGENTA, C_RESET);
    printf("  This system converts a small C-like source program into\n");
    printf("  intermediate representations and applies basic optimizations.\n\n");

    printf("%s%s  COMPILER WORKFLOW%s\n", C_BOLD, C_BLUE, C_RESET);
    printf("  %s1%s Source Input\n", C_CYAN, C_RESET);
    printf("       |\n");
    printf("       v\n");
    printf("  %s2%s Lexical Analysis\n", C_CYAN, C_RESET);
    printf("       |\n");
    printf("       v\n");
    printf("  %s3%s Symbol Table + Recursive-Descent Parser\n", C_CYAN, C_RESET);
    printf("       |\n");
    printf("       v\n");
    printf("  %s4%s Three Address Code\n", C_CYAN, C_RESET);
    printf("       |\n");
    printf("       +------> Quadruples / Triples\n");
    printf("       |\n");
    printf("       v\n");
    printf("  %s5%s Optimization\n", C_CYAN, C_RESET);
    printf("       |\n");
    printf("       v\n");
    printf("  %s6%s Basic Blocks + Compilation Statistics\n\n", C_CYAN, C_RESET);

    printf("%s%s  SUPPORTED CONSTRUCTS%s\n", C_BOLD, C_GREEN, C_RESET);
    printf("  int declarations, assignments, arithmetic operators,\n");
    printf("  relational operators, if/else, while loops and blocks.\n\n");

    printf("%s%s  OPTIMIZATION PASSES%s\n", C_BOLD, C_YELLOW, C_RESET);
    printf("  * Constant Folding\n");
    printf("  * Common Subexpression Elimination\n");
    printf("  * Repeated optimization pass for improved results\n");
}

void showColoredMenu(int loaded)
{
    printTitle("MAIN MENU");

    printf("%s%s  SOURCE MANAGEMENT%s\n", C_BOLD, C_MAGENTA, C_RESET);
    printf("  %s[1]%s Enter Source Program\n", C_CYAN, C_RESET);
    printf("  %s[2]%s Load Demo Program\n", C_CYAN, C_RESET);

    printf("\n%s%s  ANALYSIS & INTERMEDIATE CODE%s\n", C_BOLD, C_MAGENTA, C_RESET);
    printf("  %s[3]%s Lexical Analysis\n", C_CYAN, C_RESET);
    printf("  %s[4]%s Symbol Table\n", C_CYAN, C_RESET);
    printf("  %s[5]%s Three Address Code\n", C_CYAN, C_RESET);
    printf("  %s[6]%s Quadruples\n", C_CYAN, C_RESET);
    printf("  %s[7]%s Triples\n", C_CYAN, C_RESET);

    printf("\n%s%s  OPTIMIZATION & CONTROL FLOW%s\n", C_BOLD, C_MAGENTA, C_RESET);
    printf("  %s[8]%s Optimized TAC\n", C_CYAN, C_RESET);
    printf("  %s[9]%s Basic Block Analysis\n", C_CYAN, C_RESET);
    printf("  %s[10]%s Complete Pipeline\n", C_CYAN, C_RESET);

    printf("\n%s%s  INFORMATION%s\n", C_BOLD, C_MAGENTA, C_RESET);
    printf("  %s[11]%s Project Information & Features\n", C_CYAN, C_RESET);
    printf("  %s[12]%s Exit\n", C_RED, C_RESET);

    printf("\n");
    if (loaded)
        printf("  %sSTATUS:%s %s Source program loaded and ready.\n",
               C_BOLD, C_RESET, C_GREEN);
    else
        printf("  %sSTATUS:%s %s No source program loaded.\n",
               C_BOLD, C_RESET, C_YELLOW);

    printLine('-', 68);
}

void showColoredSourcePreview(const char *source)
{
    int i = 0;
    int indent = 0;
    int lineStart = 1;

    printTitle("SOURCE PROGRAM PREVIEW");

    while (source[i] != '\0') {
        char ch = source[i];

        /* Skip whitespace, but preserve statement boundaries through ; and braces. */
        if (isspace((unsigned char)ch)) {
            i++;
            continue;
        }

        if (ch == '}') {
            if (!lineStart)
                printf("\n");

            if (indent > 0)
                indent--;

            for (int j = 0; j < indent; j++)
                printf("    ");

            printf("%s}%s", C_YELLOW, C_RESET);
            lineStart = 0;
            i++;
            continue;
        }

        if (ch == '{') {
            if (!lineStart)
                printf(" ");

            printf("%s{%s\n", C_YELLOW, C_RESET);
            indent++;
            lineStart = 1;
            i++;
            continue;
        }

        if (ch == ';') {
            printf("%s;%s\n", C_GREEN, C_RESET);
            lineStart = 1;
            i++;
            continue;
        }

        /* Print two-character relational/equality operators as one unit. */
        if ((ch == '=' || ch == '!' || ch == '<' || ch == '>') &&
            source[i + 1] == '=') {
            printf(" %s%c=%s ", C_MAGENTA, ch, C_RESET);
            i += 2;
            lineStart = 0;
            continue;
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '%' || ch == '<' || ch == '>' || ch == '=') {
            printf(" %s%c%s ", C_MAGENTA, ch, C_RESET);
            i++;
            lineStart = 0;
            continue;
        }

        if (ch == '(' || ch == ')') {
            printf("%s%c%s", C_CYAN, ch, C_RESET);
            i++;
            lineStart = 0;
            continue;
        }

        if (ch == ',') {
            printf("%s,%s ", C_YELLOW, C_RESET);
            i++;
            lineStart = 0;
            continue;
        }

        /* Read a complete identifier/number/keyword instead of printing char-by-char. */
        if (isalnum((unsigned char)ch) || ch == '_') {
            char word[32];
            int j = 0;

            while (isalnum((unsigned char)source[i]) || source[i] == '_') {
                if (j < 31)
                    word[j++] = source[i];
                i++;
            }
            word[j] = '\0';

            if (lineStart) {
                for (int k = 0; k < indent; k++)
                    printf("    ");
                lineStart = 0;
            }

            if (strcmp(word, "if") == 0 || strcmp(word, "else") == 0 ||
                strcmp(word, "while") == 0) {
                printf("%s%s%s", C_BLUE, word, C_RESET);
            } else if (strcmp(word, "int") == 0) {
                printf("%s%s%s", C_GREEN, word, C_RESET);
            } else {
                printf("%s", word);
            }
            continue;
        }

        /* Any unsupported character is shown clearly instead of silently ignored. */
        if (lineStart) {
            for (int j = 0; j < indent; j++)
                printf("    ");
            lineStart = 0;
        }

        printf("%s%c%s", C_RED, ch, C_RESET);
        i++;
    }

    if (!lineStart)
        printf("\n");

    printLine('-', 68);
}

#define MAX_SOURCE 12000
#define MAX_TOKENS 2000
#define MAX_SYMBOLS 200
#define MAX_TAC 1000
#define MAX_NAME 32
#define MAX_POOL 3000

/* ==================== TOKEN SYSTEM ==================== */

typedef enum {
    TOK_EOF, TOK_ID, TOK_NUM,
    TOK_INT, TOK_IF, TOK_ELSE, TOK_WHILE,
    TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_MOD,
    TOK_ASSIGN, TOK_EQ, TOK_NE, TOK_LT, TOK_LE, TOK_GT, TOK_GE,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_SEMI, TOK_COMMA, TOK_INVALID
} TokenKind;

typedef struct {
    TokenKind kind;
    char lexeme[MAX_NAME];
    int line;
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;
int currentToken = 0;

/* ==================== SYMBOL TABLE ==================== */

typedef struct {
    char name[MAX_NAME];
    char type[8];
    int scope;
    int line;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

/* ==================== INTERMEDIATE CODE ==================== */

typedef enum {
    IR_ASSIGN,
    IR_BINARY,
    IR_LABEL,
    IR_GOTO,
    IR_IFGOTO
} IRKind;

typedef struct {
    IRKind kind;
    char op[12];
    char arg1[MAX_NAME];
    char arg2[MAX_NAME];
    char result[MAX_NAME];
    int line;
} Instruction;

Instruction tac[MAX_TAC];
int tacCount = 0;

int tempCounter = 1;
int labelCounter = 1;

/* Parser string pool */
char pool[MAX_POOL][MAX_NAME];
int poolCount = 0;

/* ==================== BASIC UTILITIES ==================== */

void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pauseScreen(void)
{
    printf("\n%s%sPress ENTER to continue...%s", C_BOLD, C_YELLOW, C_RESET);
    getchar();
}

char *saveName(const char *s)
{
    if (poolCount >= MAX_POOL) {
        printf("Internal memory limit reached.\n");
        exit(1);
    }

    strcpy(pool[poolCount], s);

    return pool[poolCount++];
}

void resetCompiler(void)
{
    tokenCount = 0;
    currentToken = 0;

    symbolCount = 0;

    tacCount = 0;
    tempCounter = 1;
    labelCounter = 1;

    poolCount = 0;
}

/* ==================== LEXICAL ANALYSIS ==================== */

const char *tokenName(TokenKind k)
{
    switch (k) {

        case TOK_EOF:
            return "EOF";

        case TOK_ID:
            return "IDENTIFIER";

        case TOK_NUM:
            return "NUMBER";

        case TOK_INT:
            return "INT";

        case TOK_IF:
            return "IF";

        case TOK_ELSE:
            return "ELSE";

        case TOK_WHILE:
            return "WHILE";

        case TOK_PLUS:
            return "+";

        case TOK_MINUS:
            return "-";

        case TOK_MUL:
            return "*";

        case TOK_DIV:
            return "/";

        case TOK_MOD:
            return "%";

        case TOK_ASSIGN:
            return "=";

        case TOK_EQ:
            return "==";

        case TOK_NE:
            return "!=";

        case TOK_LT:
            return "<";

        case TOK_LE:
            return "<=";

        case TOK_GT:
            return ">";

        case TOK_GE:
            return ">=";

        case TOK_LPAREN:
            return "(";

        case TOK_RPAREN:
            return ")";

        case TOK_LBRACE:
            return "{";

        case TOK_RBRACE:
            return "}";

        case TOK_SEMI:
            return ";";

        case TOK_COMMA:
            return ",";

        default:
            return "INVALID";
    }
}

void addToken(TokenKind kind, const char *lexeme, int line)
{
    if (tokenCount >= MAX_TOKENS) {
        printf("Too many tokens.\n");
        exit(1);
    }

    tokens[tokenCount].kind = kind;

    strncpy(
        tokens[tokenCount].lexeme,
        lexeme,
        MAX_NAME - 1
    );

    tokens[tokenCount].lexeme[MAX_NAME - 1] = '\0';

    tokens[tokenCount].line = line;

    tokenCount++;
}

void tokenize(const char *src)
{
    int i = 0;
    int line = 1;

    tokenCount = 0;

    while (src[i] != '\0') {

        char ch = src[i];

        /* Whitespace */
        if (isspace((unsigned char)ch)) {

            if (ch == '\n')
                line++;

            i++;
            continue;
        }

        /* Single line comment */
        if (ch == '/' && src[i + 1] == '/') {

            while (src[i] != '\0' && src[i] != '\n')
                i++;

            continue;
        }

        /* Multi-line comment */
        if (ch == '/' && src[i + 1] == '*') {

            i += 2;

            while (
                src[i] != '\0' &&
                !(src[i] == '*' && src[i + 1] == '/')
            ) {

                if (src[i] == '\n')
                    line++;

                i++;
            }

            if (src[i] != '\0')
                i += 2;

            continue;
        }

        /* Identifier / keyword */
        if (
            isalpha((unsigned char)ch) ||
            ch == '_'
        ) {

            char word[MAX_NAME];
            int j = 0;

            while (
                isalnum((unsigned char)src[i]) ||
                src[i] == '_'
            ) {

                if (j < MAX_NAME - 1)
                    word[j++] = src[i];

                i++;
            }

            word[j] = '\0';

            if (strcmp(word, "int") == 0)
                addToken(TOK_INT, word, line);

            else if (strcmp(word, "if") == 0)
                addToken(TOK_IF, word, line);

            else if (strcmp(word, "else") == 0)
                addToken(TOK_ELSE, word, line);

            else if (strcmp(word, "while") == 0)
                addToken(TOK_WHILE, word, line);

            else
                addToken(TOK_ID, word, line);

            continue;
        }

        /* Number */
        if (isdigit((unsigned char)ch)) {

            char num[MAX_NAME];
            int j = 0;

            while (isdigit((unsigned char)src[i])) {

                if (j < MAX_NAME - 1)
                    num[j++] = src[i];

                i++;
            }

            num[j] = '\0';

            addToken(TOK_NUM, num, line);

            continue;
        }

        /* Two-character operators */

        if (ch == '=' && src[i + 1] == '=') {

            addToken(TOK_EQ, "==", line);

            i += 2;

            continue;
        }

        if (ch == '!' && src[i + 1] == '=') {

            addToken(TOK_NE, "!=", line);

            i += 2;

            continue;
        }

        if (ch == '<' && src[i + 1] == '=') {

            addToken(TOK_LE, "<=", line);

            i += 2;

            continue;
        }

        if (ch == '>' && src[i + 1] == '=') {

            addToken(TOK_GE, ">=", line);

            i += 2;

            continue;
        }

        /* Single-character operators */

        switch (ch) {

            case '+':
                addToken(TOK_PLUS, "+", line);
                break;

            case '-':
                addToken(TOK_MINUS, "-", line);
                break;

            case '*':
                addToken(TOK_MUL, "*", line);
                break;

            case '/':
                addToken(TOK_DIV, "/", line);
                break;

            case '%':
                addToken(TOK_MOD, "%", line);
                break;

            case '=':
                addToken(TOK_ASSIGN, "=", line);
                break;

            case '<':
                addToken(TOK_LT, "<", line);
                break;

            case '>':
                addToken(TOK_GT, ">", line);
                break;

            case '(':
                addToken(TOK_LPAREN, "(", line);
                break;

            case ')':
                addToken(TOK_RPAREN, ")", line);
                break;

            case '{':
                addToken(TOK_LBRACE, "{", line);
                break;

            case '}':
                addToken(TOK_RBRACE, "}", line);
                break;

            case ';':
                addToken(TOK_SEMI, ";", line);
                break;

            case ',':
                addToken(TOK_COMMA, ",", line);
                break;

            default:
            {
                char bad[2];

                bad[0] = ch;
                bad[1] = '\0';

                addToken(TOK_INVALID, bad, line);
            }
        }

        i++;
    }

    addToken(TOK_EOF, "EOF", line);
}

void showTokens(void)
{
    int i;

    printf("\n%s%s========== LEXICAL ANALYSIS ==========%s\n", C_BOLD, C_CYAN, C_RESET);

    printf(
        "%-5s %-7s %-18s %-15s\n",
        "No.",
        "Line",
        "Token",
        "Lexeme"
    );

    printf("-----------------------------------------------\n");

    for (i = 0; i < tokenCount; i++) {

        printf(
            "%-5d %-7d %-18s %-15s\n",
            i,
            tokens[i].line,
            tokenName(tokens[i].kind),
            tokens[i].lexeme
        );
    }

    printf("\nTotal tokens: %d\n", tokenCount);
}

/* ==================== SYMBOL TABLE ==================== */

int findSymbol(const char *name)
{
    int i;

    for (i = 0; i < symbolCount; i++) {

        if (strcmp(symbolTable[i].name, name) == 0)
            return i;
    }

    return -1;
}

void addSymbol(
    const char *name,
    int scope,
    int line
)
{
    if (findSymbol(name) >= 0)
        return;

    if (symbolCount >= MAX_SYMBOLS) {

        printf("Symbol table full.\n");

        exit(1);
    }

    strcpy(
        symbolTable[symbolCount].name,
        name
    );

    strcpy(
        symbolTable[symbolCount].type,
        "int"
    );

    symbolTable[symbolCount].scope = scope;
    symbolTable[symbolCount].line = line;

    symbolCount++;
}

void showSymbols(void)
{
    int i;

    printf("\n%s%s========== SYMBOL TABLE ==========%s\n", C_BOLD, C_CYAN, C_RESET);

    printf(
        "%-5s %-15s %-8s %-8s %-8s\n",
        "No.",
        "Name",
        "Type",
        "Scope",
        "Line"
    );

    printf("-----------------------------------------------\n");

    for (i = 0; i < symbolCount; i++) {

        printf(
            "%-5d %-15s %-8s %-8d %-8d\n",
            i,
            symbolTable[i].name,
            symbolTable[i].type,
            symbolTable[i].scope,
            symbolTable[i].line
        );
    }
}

/* ==================== TAC GENERATION ==================== */

void newTemp(char *s)
{
    sprintf(s, "t%d", tempCounter++);
}

void newLabel(char *s)
{
    sprintf(s, "L%d", labelCounter++);
}

void emit(
    IRKind kind,
    const char *op,
    const char *a1,
    const char *a2,
    const char *res,
    int line
)
{
    if (tacCount >= MAX_TAC) {

        printf("TAC buffer full.\n");

        exit(1);
    }

    tac[tacCount].kind = kind;

    strcpy(
        tac[tacCount].op,
        op ? op : ""
    );

    strcpy(
        tac[tacCount].arg1,
        a1 ? a1 : ""
    );

    strcpy(
        tac[tacCount].arg2,
        a2 ? a2 : ""
    );

    strcpy(
        tac[tacCount].result,
        res ? res : ""
    );

    tac[tacCount].line = line;

    tacCount++;
}

/* ==================== PARSER ==================== */

void syntaxError(const char *msg)
{
    printf("\n========== SYNTAX ERROR ==========\n");

    printf(
        "Line   : %d\n",
        tokens[currentToken].line
    );

    printf(
        "Found  : %s\n",
        tokens[currentToken].lexeme
    );

    printf(
        "Reason : %s\n",
        msg
    );

    exit(1);
}

const char *expectedTokenText(TokenKind kind)
{
    return tokenName(kind);
}

void expectToken(TokenKind kind)
{
    if (tokens[currentToken].kind != kind) {
        char msg[160];
        snprintf(msg, sizeof(msg),
                 "Expected '%s' but found '%s'.",
                 expectedTokenText(kind),
                 tokens[currentToken].lexeme);
        syntaxError(msg);
    }

    currentToken++;
}

int acceptToken(TokenKind kind)
{
    if (tokens[currentToken].kind == kind) {

        currentToken++;

        return 1;
    }

    return 0;
}

/* Forward declarations */

char *expression(void);
char *equality(void);
char *relational(void);
char *additive(void);
char *multiplicative(void);
char *primary(void);

void statement(int scope);
void block(int scope);

/* Primary */

char *primary(void)
{
    char value[MAX_NAME];

    if (
        tokens[currentToken].kind == TOK_ID ||
        tokens[currentToken].kind == TOK_NUM
    ) {

        strcpy(
            value,
            tokens[currentToken].lexeme
        );

        if (tokens[currentToken].kind == TOK_ID) {

            addSymbol(
                value,
                0,
                tokens[currentToken].line
            );
        }

        currentToken++;

        return saveName(value);
    }

    if (acceptToken(TOK_LPAREN)) {

        char *v;

        v = expression();

        expectToken(TOK_RPAREN);

        return saveName(v);
    }

    syntaxError(
        "Expected identifier, number or '('."
    );

    return NULL;
}

/* Multiplication / division / modulo */

char *multiplicative(void)
{
    char *left;

    left = primary();

    while (
        tokens[currentToken].kind == TOK_MUL ||
        tokens[currentToken].kind == TOK_DIV ||
        tokens[currentToken].kind == TOK_MOD
    ) {

        TokenKind k;

        const char *op;

        char *right;

        char temp[MAX_NAME];

        k = tokens[currentToken].kind;

        currentToken++;

        if (k == TOK_MUL)
            op = "*";

        else if (k == TOK_DIV)
            op = "/";

        else
            op = "%";

        right = primary();

        newTemp(temp);

        emit(
            IR_BINARY,
            op,
            left,
            right,
            temp,
            tokens[currentToken - 1].line
        );

        left = saveName(temp);
    }

    return left;
}

/* Addition / subtraction */

char *additive(void)
{
    char *left;

    left = multiplicative();

    while (
        tokens[currentToken].kind == TOK_PLUS ||
        tokens[currentToken].kind == TOK_MINUS
    ) {

        TokenKind k;

        const char *op;

        char *right;

        char temp[MAX_NAME];

        k = tokens[currentToken].kind;

        currentToken++;

        if (k == TOK_PLUS)
            op = "+";

        else
            op = "-";

        right = multiplicative();

        newTemp(temp);

        emit(
            IR_BINARY,
            op,
            left,
            right,
            temp,
            tokens[currentToken - 1].line
        );

        left = saveName(temp);
    }

    return left;
}

/* Relational */

char *relational(void)
{
    char *left;

    left = additive();

    while (
        tokens[currentToken].kind == TOK_LT ||
        tokens[currentToken].kind == TOK_LE ||
        tokens[currentToken].kind == TOK_GT ||
        tokens[currentToken].kind == TOK_GE
    ) {

        TokenKind k;

        const char *op;

        char *right;

        char temp[MAX_NAME];

        k = tokens[currentToken].kind;

        currentToken++;

        if (k == TOK_LT)
            op = "<";

        else if (k == TOK_LE)
            op = "<=";

        else if (k == TOK_GT)
            op = ">";

        else
            op = ">=";

        right = additive();

        newTemp(temp);

        emit(
            IR_BINARY,
            op,
            left,
            right,
            temp,
            tokens[currentToken - 1].line
        );

        left = saveName(temp);
    }

    return left;
}

/* Equality */

char *equality(void)
{
    char *left;

    left = relational();

    while (
        tokens[currentToken].kind == TOK_EQ ||
        tokens[currentToken].kind == TOK_NE
    ) {

        TokenKind k;

        const char *op;

        char *right;

        char temp[MAX_NAME];

        k = tokens[currentToken].kind;

        currentToken++;

        if (k == TOK_EQ)
            op = "==";

        else
            op = "!=";

        right = relational();

        newTemp(temp);

        emit(
            IR_BINARY,
            op,
            left,
            right,
            temp,
            tokens[currentToken - 1].line
        );

        left = saveName(temp);
    }

    return left;
}

char *expression(void)
{
    return equality();
}

/* Block */

void block(int scope)
{
    expectToken(TOK_LBRACE);

    while (
        tokens[currentToken].kind != TOK_RBRACE &&
        tokens[currentToken].kind != TOK_EOF
    ) {

        statement(scope);
    }

    expectToken(TOK_RBRACE);
}

/* IF / ELSE */

void parseIf(int scope)
{
    char *condition;

    char elseLabel[MAX_NAME];
    char endLabel[MAX_NAME];

    expectToken(TOK_IF);

    expectToken(TOK_LPAREN);

    condition = expression();

    expectToken(TOK_RPAREN);

    newLabel(elseLabel);
    newLabel(endLabel);

    emit(
        IR_IFGOTO,
        "ifFalse",
        condition,
        "",
        elseLabel,
        tokens[currentToken - 1].line
    );

    statement(scope);

    if (acceptToken(TOK_ELSE)) {

        emit(
            IR_GOTO,
            "goto",
            "",
            "",
            endLabel,
            tokens[currentToken - 1].line
        );

        emit(
            IR_LABEL,
            "label",
            "",
            "",
            elseLabel,
            tokens[currentToken - 1].line
        );

        statement(scope);

        emit(
            IR_LABEL,
            "label",
            "",
            "",
            endLabel,
            tokens[currentToken - 1].line
        );
    }

    else {

        emit(
            IR_LABEL,
            "label",
            "",
            "",
            elseLabel,
            tokens[currentToken - 1].line
        );
    }
}

/* WHILE */

void parseWhile(int scope)
{
    char startLabel[MAX_NAME];
    char endLabel[MAX_NAME];

    char *condition;

    expectToken(TOK_WHILE);

    newLabel(startLabel);
    newLabel(endLabel);

    emit(
        IR_LABEL,
        "label",
        "",
        "",
        startLabel,
        tokens[currentToken].line
    );

    expectToken(TOK_LPAREN);

    condition = expression();

    expectToken(TOK_RPAREN);

    emit(
        IR_IFGOTO,
        "ifFalse",
        condition,
        "",
        endLabel,
        tokens[currentToken - 1].line
    );

    statement(scope);

    emit(
        IR_GOTO,
        "goto",
        "",
        "",
        startLabel,
        tokens[currentToken - 1].line
    );

    emit(
        IR_LABEL,
        "label",
        "",
        "",
        endLabel,
        tokens[currentToken - 1].line
    );
}

/* Statement */

void statement(int scope)
{
    /* Declaration */

    if (tokens[currentToken].kind == TOK_INT) {

        currentToken++;

        do {

            if (tokens[currentToken].kind != TOK_ID)
                syntaxError(
                    "Expected identifier in declaration."
                );

            addSymbol(
                tokens[currentToken].lexeme,
                scope,
                tokens[currentToken].line
            );

            currentToken++;

        } while (acceptToken(TOK_COMMA));

        expectToken(TOK_SEMI);

        return;
    }

    /* IF */

    if (tokens[currentToken].kind == TOK_IF) {

        parseIf(scope);

        return;
    }

    /* WHILE */

    if (tokens[currentToken].kind == TOK_WHILE) {

        parseWhile(scope);

        return;
    }

    /* Block */

    if (tokens[currentToken].kind == TOK_LBRACE) {

        block(scope + 1);

        return;
    }

    /* Assignment */

    if (tokens[currentToken].kind == TOK_ID) {

        char variable[MAX_NAME];

        char *value;

        strcpy(
            variable,
            tokens[currentToken].lexeme
        );

        addSymbol(
            variable,
            scope,
            tokens[currentToken].line
        );

        currentToken++;

        expectToken(TOK_ASSIGN);

        value = expression();

        expectToken(TOK_SEMI);

        emit(
            IR_ASSIGN,
            "=",
            value,
            "",
            variable,
            tokens[currentToken - 1].line
        );

        return;
    }

    syntaxError(
        "Expected declaration, assignment, if, while or block."
    );
}

void parseProgram(void)
{
    currentToken = 0;

    while (tokens[currentToken].kind != TOK_EOF) {
        if (tokens[currentToken].kind == TOK_INVALID) {
            syntaxError("Invalid character/token in source program.");
            return;
        }

        statement(0);
    }
}

/* ==================== TAC DISPLAY ==================== */

void showInstruction(
    Instruction *x,
    int i
)
{
    printf("%3d : ", i);

    if (x->kind == IR_ASSIGN) {

        printf(
            "%s = %s",
            x->result,
            x->arg1
        );
    }

    else if (x->kind == IR_BINARY) {

        printf(
            "%s = %s %s %s",
            x->result,
            x->arg1,
            x->op,
            x->arg2
        );
    }

    else if (x->kind == IR_LABEL) {

        printf(
            "%s:",
            x->result
        );
    }

    else if (x->kind == IR_GOTO) {

        printf(
            "goto %s",
            x->result
        );
    }

    else if (x->kind == IR_IFGOTO) {

        printf(
            "ifFalse %s goto %s",
            x->arg1,
            x->result
        );
    }
}

void showTAC(
    Instruction *code,
    int count,
    const char *title
)
{
    int i;

    printf(
        "\n%s%s========== %s ==========%s\n",
        C_BOLD, C_CYAN, title, C_RESET
    );

    for (i = 0; i < count; i++) {

        showInstruction(
            &code[i],
            i
        );

        printf("\n");
    }

    printf(
        "\nTotal instructions: %d\n",
        count
    );
}

/* ==================== QUADRUPLES ==================== */

void showQuadruples(void)
{
    int i;

    printf(
        "\n%s%s========== QUADRUPLES ==========%s\n",
        C_BOLD, C_CYAN, C_RESET
    );

    printf(
        "%-5s %-12s %-15s %-15s %-15s\n",
        "No.",
        "Operator",
        "Arg1",
        "Arg2",
        "Result"
    );

    printf(
        "------------------------------------------------------------\n"
    );

    for (i = 0; i < tacCount; i++) {

        Instruction *x = &tac[i];

        if (x->kind == IR_BINARY) {

            printf(
                "%-5d %-12s %-15s %-15s %-15s\n",
                i,
                x->op,
                x->arg1,
                x->arg2,
                x->result
            );
        }

        else if (x->kind == IR_ASSIGN) {

            printf(
                "%-5d %-12s %-15s %-15s %-15s\n",
                i,
                "=",
                x->arg1,
                "-",
                x->result
            );
        }

        else if (x->kind == IR_LABEL) {

            printf(
                "%-5d %-12s %-15s %-15s %-15s\n",
                i,
                "label",
                "-",
                "-",
                x->result
            );
        }

        else if (x->kind == IR_GOTO) {

            printf(
                "%-5d %-12s %-15s %-15s %-15s\n",
                i,
                "goto",
                "-",
                "-",
                x->result
            );
        }

        else {

            printf(
                "%-5d %-12s %-15s %-15s %-15s\n",
                i,
                "ifFalse",
                x->arg1,
                "-",
                x->result
            );
        }
    }
}

/* ==================== TRIPLES ==================== */

int producerOf(const char *name)
{
    int i;

    for (i = 0; i < tacCount; i++) {

        if (
            strcmp(
                tac[i].result,
                name
            ) == 0
        )
            return i;
    }

    return -1;
}

void tripleArg(
    const char *arg,
    char *out
)
{
    int p;

    p = producerOf(arg);

    if (
        p >= 0 &&
        arg[0] == 't'
    ) {

        sprintf(
            out,
            "(%d)",
            p
        );
    }

    else {

        strcpy(
            out,
            arg
        );
    }
}

void showTriples(void)
{
    int i;

    printf(
        "\n%s%s========== TRIPLES ==========%s\n",
        C_BOLD, C_CYAN, C_RESET
    );

    printf(
        "%-5s %-12s %-18s %-18s\n",
        "No.",
        "Operator",
        "Arg1",
        "Arg2"
    );

    printf(
        "-------------------------------------------------------\n"
    );

    for (i = 0; i < tacCount; i++) {

        char a1[MAX_NAME];
        char a2[MAX_NAME];

        Instruction *x = &tac[i];

        if (x->kind == IR_BINARY) {

            tripleArg(
                x->arg1,
                a1
            );

            tripleArg(
                x->arg2,
                a2
            );

            printf(
                "%-5d %-12s %-18s %-18s\n",
                i,
                x->op,
                a1,
                a2
            );
        }

        else if (x->kind == IR_ASSIGN) {

            tripleArg(
                x->arg1,
                a1
            );

            printf(
                "%-5d %-12s %-18s %-18s\n",
                i,
                "=",
                a1,
                "-"
            );
        }

        else if (x->kind == IR_LABEL) {

            printf(
                "%-5d %-12s %-18s %-18s\n",
                i,
                "label",
                x->result,
                "-"
            );
        }

        else if (x->kind == IR_GOTO) {

            printf(
                "%-5d %-12s %-18s %-18s\n",
                i,
                "goto",
                x->result,
                "-"
            );
        }

        else {

            tripleArg(
                x->arg1,
                a1
            );

            printf(
                "%-5d %-12s %-18s %-18s\n",
                i,
                "ifFalse",
                a1,
                x->result
            );
        }
    }
}

/* ==================== OPTIMIZATION ==================== */

int isNumber(const char *s)
{
    int i = 0;

    if (s[0] == '\0')
        return 0;

    if (s[0] == '-')
        i = 1;

    if (s[i] == '\0')
        return 0;

    for (; s[i] != '\0'; i++) {

        if (
            !isdigit(
                (unsigned char)s[i]
            )
        )
            return 0;
    }

    return 1;
}

int isControl(Instruction *x)
{
    return (
        x->kind == IR_LABEL ||
        x->kind == IR_GOTO ||
        x->kind == IR_IFGOTO
    );
}

/* Constant Folding */

void constantFolding(
    Instruction *code,
    int count
)
{
    int i;

    for (i = 0; i < count; i++) {

        int a;
        int b;
        int result;
        int valid = 1;

        if (code[i].kind != IR_BINARY)
            continue;

        if (
            !isNumber(code[i].arg1) ||
            !isNumber(code[i].arg2)
        )
            continue;

        a = atoi(code[i].arg1);
        b = atoi(code[i].arg2);

        result = 0;

        if (strcmp(code[i].op, "+") == 0)
            result = a + b;

        else if (strcmp(code[i].op, "-") == 0)
            result = a - b;

        else if (strcmp(code[i].op, "*") == 0)
            result = a * b;

        else if (strcmp(code[i].op, "/") == 0) {

            if (b == 0)
                valid = 0;

            else
                result = a / b;
        }

        else if (strcmp(code[i].op, "%") == 0) {

            if (b == 0)
                valid = 0;

            else
                result = a % b;
        }

        else if (strcmp(code[i].op, "<") == 0)
            result = a < b;

        else if (strcmp(code[i].op, "<=") == 0)
            result = a <= b;

        else if (strcmp(code[i].op, ">") == 0)
            result = a > b;

        else if (strcmp(code[i].op, ">=") == 0)
            result = a >= b;

        else if (strcmp(code[i].op, "==") == 0)
            result = a == b;

        else if (strcmp(code[i].op, "!=") == 0)
            result = a != b;

        else
            valid = 0;

        if (valid) {

            char value[32];

            sprintf(
                value,
                "%d",
                result
            );

            code[i].kind = IR_ASSIGN;

            strcpy(
                code[i].op,
                "="
            );

            strcpy(
                code[i].arg1,
                value
            );

            code[i].arg2[0] = '\0';
        }
    }
}

/* Common Subexpression Elimination */

int sameExpression(
    Instruction *a,
    Instruction *b
)
{
    if (
        a->kind != IR_BINARY ||
        b->kind != IR_BINARY
    )
        return 0;

    if (
        strcmp(
            a->op,
            b->op
        ) != 0
    )
        return 0;

    if (
        strcmp(a->arg1, b->arg1) == 0 &&
        strcmp(a->arg2, b->arg2) == 0
    )
        return 1;

    /* + and * are commutative */

    if (
        (
            strcmp(a->op, "+") == 0 ||
            strcmp(a->op, "*") == 0
        ) &&
        strcmp(a->arg1, b->arg2) == 0 &&
        strcmp(a->arg2, b->arg1) == 0
    )
        return 1;

    return 0;
}

void commonSubexpressionElimination(
    Instruction *code,
    int count
)
{
    int i;
    int j;

    for (i = 0; i < count; i++) {

        if (code[i].kind != IR_BINARY)
            continue;

        for (j = i - 1; j >= 0; j--) {

            if (isControl(&code[j]))
                break;

            if (
                sameExpression(
                    &code[i],
                    &code[j]
                )
            ) {

                code[i].kind = IR_ASSIGN;

                strcpy(
                    code[i].op,
                    "="
                );

                strcpy(
                    code[i].arg1,
                    code[j].result
                );

                code[i].arg2[0] = '\0';

                break;
            }
        }
    }
}

void optimize(
    Instruction *input,
    int count,
    Instruction *output,
    int *newCount
)
{
    int i;
    int changed;
    Instruction work[MAX_TAC];
    Instruction compact[MAX_TAC];
    int workCount;
    int compactCount;

    memcpy(work, input, sizeof(Instruction) * count);
    workCount = count;

    /*
       Optimization passes:
       1. Constant folding
       2. Common subexpression elimination
       3. Constant folding again
       4. Temporary-result fusion
       5. Dead temporary elimination

       Unlike the original version, this optimizer actually removes
       redundant TAC instructions, so optimized TAC can be smaller.
    */
    for (i = 0; i < 2; i++) {
        constantFolding(work, workCount);
        commonSubexpressionElimination(work, workCount);
        constantFolding(work, workCount);
    }

    /*
       Fuse:
           t1 = a + b
           x  = t1
       into:
           x  = a + b

       This is safe because t1 is a compiler-generated temporary and
       the assignment immediately consumes it.
    */
    compactCount = 0;
    for (i = 0; i < workCount; i++) {
        Instruction cur = work[i];

        if (cur.kind == IR_BINARY && i + 1 < workCount &&
            work[i + 1].kind == IR_ASSIGN &&
            strcmp(work[i + 1].arg1, cur.result) == 0 &&
            cur.result[0] == 't') {

            Instruction fused = cur;
            strcpy(fused.result, work[i + 1].result);
            compact[compactCount++] = fused;
            i++;
        }
        else {
            compact[compactCount++] = cur;
        }
    }

    /*
       Remove dead temporary assignments/binary results.
       A temporary is dead when no later TAC instruction uses it.
       Repeat until no more removable instructions exist.
    */
    changed = 1;
    while (changed) {
        changed = 0;

        for (i = 0; i < compactCount; i++) {
            char *name = compact[i].result;
            int used = 0;
            int j;

            if (name[0] != 't')
                continue;

            for (j = i + 1; j < compactCount; j++) {
                if (strcmp(compact[j].arg1, name) == 0 ||
                    strcmp(compact[j].arg2, name) == 0) {
                    used = 1;
                    break;
                }
            }

            if (!used &&
                (compact[i].kind == IR_BINARY ||
                 compact[i].kind == IR_ASSIGN)) {

                int k;
                for (k = i; k < compactCount - 1; k++)
                    compact[k] = compact[k + 1];

                compactCount--;
                changed = 1;
                break;
            }
        }
    }

    for (i = 0; i < compactCount; i++)
        output[i] = compact[i];

    *newCount = compactCount;
}

/* ==================== BASIC BLOCKS ==================== */

void showBasicBlocks(void)
{
    int i;

    int blocks = 1;

    printf(
        "\n%s%s========== BASIC BLOCK ANALYSIS ==========%s\n",
        C_BOLD, C_CYAN, C_RESET
    );

    if (tacCount == 0) {

        printf(
            "No TAC available.\n"
        );

        return;
    }

    printf(
        "Basic Block 1 starts at instruction 0\n"
    );

    for (i = 1; i < tacCount; i++) {

        if (
            tac[i].kind == IR_LABEL
        ) {

            blocks++;

            printf(
                "Basic Block %d starts at instruction %d (%s)\n",
                blocks,
                i,
                tac[i].result
            );
        }
    }

    printf(
        "\nTotal instructions : %d\n",
        tacCount
    );

    printf(
        "Estimated blocks   : %d\n",
        blocks
    );
}

/* ==================== STATISTICS ==================== */

void showStatistics(
    int optimizedCount
)
{
    double reduction = 0.0;

    if (tacCount > 0) {

        reduction =
            (
                (double)(
                    tacCount -
                    optimizedCount
                )
                /
                (double)tacCount
            ) * 100.0;
    }

    printf(
        "\n%s%s========== COMPILATION STATISTICS ==========%s\n",
        C_BOLD, C_CYAN, C_RESET
    );

    printf(
        "Tokens                : %d\n",
        tokenCount
    );

    printf(
        "Symbols               : %d\n",
        symbolCount
    );

    printf(
        "Original TAC          : %d instructions\n",
        tacCount
    );

    printf(
        "Optimized TAC         : %d instructions\n",
        optimizedCount
    );

    printf(
        "Instruction reduction : %.2f%%\n",
        reduction
    );
}

/* ==================== DEMO PROGRAM ==================== */

const char *demoProgram(void)
{
    return
        "int a,b,c,d,x,y,z,i,j,total;"
        "a=10;"
        "b=20;"
        "c=a+b*2;"
        "d=c-5;"
        "if(c>20){"
            "x=c+10;"
            "if(x>50){"
                "y=x*2;"
            "}"
            "else{"
                "y=x+5;"
            "}"
        "}"
        "else{"
            "x=c-10;"
            "if(x<20){"
                "y=x+15;"
            "}"
            "else{"
                "y=x/2;"
            "}"
        "}"
        "i=0;"
        "total=0;"
        "while(i<5){"
            "total=total+i*a;"
            "if(total>50){"
                "total=total-10;"
            "}"
            "else{"
                "total=total+5;"
            "}"
            "i=i+1;"
        "}"
        "j=0;"
        "while(j<4){"
            "x=x+j*2;"
            "if(x>=60){"
                "y=y+x;"
            "}"
            "else{"
                "y=y-j;"
            "}"
            "j=j+1;"
        "}"
        "z=x+y*2;"
        "if(z==100){"
            "a=z+total;"
        "}"
        "else{"
            "if(z!=100){"
                "a=z-total;"
            "}"
        "}"
        "b=a+c*d;"
        "c=b-x/2;"
        "d=c%3;";
}

/* ==================== SOURCE INPUT ==================== */

void enterSource(
    char *source
)
{
    char line[500];

    source[0] = '\0';

    printf("\n%s%sEnter your C-like program%s\n", C_BOLD, C_GREEN, C_RESET);

    printf("%sType END on a separate line when finished.%s\n", C_DIM, C_RESET);

    while (
        fgets(
            line,
            sizeof(line),
            stdin
        ) != NULL
    ) {

        {
            char endWord[500];
            strcpy(endWord, line);

            /* Remove trailing CR/LF and surrounding spaces. */
            endWord[strcspn(endWord, "\r\n")] = '\0';

            {
                char *p = endWord;
                while (isspace((unsigned char)*p))
                    p++;

                char *q = p + strlen(p);
                while (q > p && isspace((unsigned char)q[-1]))
                    *--q = '\0';

                if (strcmp(p, "END") == 0 || strcmp(p, "end") == 0)
                    break;
            }
        }

        if (
            strlen(source) +
            strlen(line)
            <
            MAX_SOURCE - 1
        ) {

            strcat(
                source,
                line
            );
        }

        else {

            printf(
                "Source program too large.\n"
            );

            break;
        }
    }
}

/* ==================== BUILD COMPILER ==================== */

void build(
    const char *source
)
{
    tokenCount = 0;
    currentToken = 0;

    symbolCount = 0;

    tacCount = 0;

    tempCounter = 1;
    labelCounter = 1;

    poolCount = 0;

    tokenize(source);

    parseProgram();
}

/* ==================== MENU ==================== */

void showMenu(void)
{
    /* Kept as a wrapper so the rest of the original program remains simple. */
    /* The loaded-state indicator is displayed from main through showColoredMenu. */
}

/* ==================== COMPLETE PIPELINE ==================== */

void completePipeline(void)
{
    Instruction optimized[MAX_TAC];

    int optimizedCount;

    printf(
        "\n%s%s===== COMPLETE COMPILER PIPELINE =====%s\n",
        C_BOLD, C_MAGENTA, C_RESET
    );

    printf(
        "\n[1] Lexical Analysis..."
    );

    showTokens();

    printf(
        "\n[2] Symbol Table..."
    );

    showSymbols();

    printf(
        "\n[3] Three Address Code..."
    );

    showTAC(
        tac,
        tacCount,
        "THREE ADDRESS CODE"
    );

    printf(
        "\n[4] Quadruples..."
    );

    showQuadruples();

    printf(
        "\n[5] Triples..."
    );

    showTriples();

    printf(
        "\n[6] Optimization..."
    );

    optimize(
        tac,
        tacCount,
        optimized,
        &optimizedCount
    );

    showTAC(
        optimized,
        optimizedCount,
        "OPTIMIZED THREE ADDRESS CODE"
    );

    printf(
        "\n[7] Basic Blocks..."
    );

    showBasicBlocks();

    showStatistics(
        optimizedCount
    );

    printf(
        "\n%s%s===== COMPILATION COMPLETED =====%s\n",
        C_BOLD, C_GREEN, C_RESET
    );
}

/* ==================== MAIN ==================== */

int main(void)
{
    char source[MAX_SOURCE];

    int choice;

    int loaded = 0;

    source[0] = '\0';

    enableColorSupport();
    showWelcomeBanner();

    while (1) {

        showColoredMenu(loaded);

        printf(
            "\nEnter choice: "
        );

        if (
            scanf(
                "%d",
                &choice
            ) != 1
        ) {

            printf(
                "Please enter a number.\n"
            );

            clearInputBuffer();

            continue;
        }

        clearInputBuffer();

        /* Enter program */

        if (choice == 1) {

            resetCompiler();

            enterSource(
                source
            );

            build(
                source
            );

            loaded = 1;

            printSuccess("Your source program is loaded successfully.");
            showColoredSourcePreview(source);
        }

        /* Demo */

        else if (choice == 2) {

            resetCompiler();

            strcpy(
                source,
                demoProgram()
            );

            build(
                source
            );

            loaded = 1;

            printSuccess("Demo program loaded successfully.");
            showColoredSourcePreview(source);
        }

        /* Exit */

        else if (choice == 12) {

            printf("\n%s%sProject closed. Thank you for using the compiler!%s\n",
                   C_BOLD, C_GREEN, C_RESET);

            break;
        }

        /* Project information */

        else if (choice == 11) {

            showProjectDashboard();

            printf("\n%s%s  MODULES INCLUDED%s\n",
                   C_BOLD, C_GREEN, C_RESET);
            printf("  01. Lexical Analysis\n");
            printf("  02. Symbol Table\n");
            printf("  03. Recursive-Descent Parser\n");
            printf("  04. Three Address Code\n");
            printf("  05. Quadruples\n");
            printf("  06. Triples\n");
            printf("  07. Constant Folding\n");
            printf("  08. Common Subexpression Elimination\n");
            printf("  09. Basic Block Analysis\n");
            printf("  10. Compilation Statistics\n");
            printf("  11. Menu-Driven User Interface\n");

            pauseScreen();
        }

        /* No source loaded */

        else if (!loaded) {

            printInfo("First choose [1] to enter your program or [2] to load the demo.");
        }

        /* Lexical Analysis */

        else if (choice == 3) {

            showTokens();

            pauseScreen();
        }

        /* Symbol Table */

        else if (choice == 4) {

            showSymbols();

            pauseScreen();
        }

        /* TAC */

        else if (choice == 5) {

            showTAC(
                tac,
                tacCount,
                "THREE ADDRESS CODE"
            );

            pauseScreen();
        }

        /* Quadruples */

        else if (choice == 6) {

            showQuadruples();

            pauseScreen();
        }

        /* Triples */

        else if (choice == 7) {

            showTriples();

            pauseScreen();
        }

        /* Optimization */

        else if (choice == 8) {

            Instruction optimized[MAX_TAC];

            int optimizedCount;

            optimize(
                tac,
                tacCount,
                optimized,
                &optimizedCount
            );

            showTAC(
                tac,
                tacCount,
                "ORIGINAL TAC"
            );

            showTAC(
                optimized,
                optimizedCount,
                "OPTIMIZED TAC"
            );

            showStatistics(
                optimizedCount
            );

            pauseScreen();
        }

        /* Basic Blocks */

        else if (choice == 9) {

            showBasicBlocks();

            pauseScreen();
        }

        /* Complete Pipeline */

        else if (choice == 10) {

            completePipeline();

            pauseScreen();
        }

        else {

            printError("Invalid choice. Please select a menu option from 1 to 12.");
        }
    }

    return 0;
}