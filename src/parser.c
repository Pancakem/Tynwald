#include "parser.h"
#include "util.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @brief Initialize each parser according to the grammar
 *
 * @param psr
 */
void init_parser(struct parser *psr)
{
    psr->Ident = mpc_new("ident");
    psr->Number = mpc_new("number");
    psr->Character = mpc_new("character");
    psr->String = mpc_new("string");
    psr->Factor = mpc_new("factor");
    psr->Term = mpc_new("term");
    psr->Lexp = mpc_new("lexp");
    psr->Stmt = mpc_new("stmt");
    psr->Exp = mpc_new("exp");
    psr->Typeident = mpc_new("typeident");
    psr->Decls = mpc_new("decls");
    psr->Args = mpc_new("args");
    psr->Body = mpc_new("body");
    psr->Procedure = mpc_new("procedure");
    psr->Program = mpc_new("program");
}

/**
 * @brief Parses the grammar
 * @note The parser must be initialized before it is parsed in to this function
 * @param psr
 * @param input
 * @return mpc_result_t*
 */
mpc_result_t *parse_grammar(struct parser *psr, const char *input)
{
    // we are not reading the grammar from file; till we bug out this issue
    // char *grammar = read_file(GRAMMAR_FILE);
    // assert(grammar != NULL);

    mpc_err_t *err = mpca_lang(
        MPCA_LANG_DEFAULT,
        " ident     : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                           \n"
        " number    : /[0-9]+/ ;                                           \n"
        " character : /'.'/ ;                                              \n"
        " string    : /\"(\\\\.|[^\"])*\"/ ;                               \n"
        "                                                                  \n"
        " factor    : '(' <lexp> ')'                                       \n"
        "           | <number>                                             \n"
        "           | <character>                                          \n"
        "           | <string>                                             \n"
        "           | <ident> '(' <lexp>? (',' <lexp>)* ')'                \n"
        "           | <ident> ;                                            \n"
        "                                                                  \n"
        " term      : <factor> (('*' | '/' | '%') <factor>)* ;             \n"
        " lexp      : <term> (('+' | '-') <term>)* ;                       \n"
        "                                                                  \n"
        " stmt      : '{' <stmt>* '}'                                      \n"
        "           | \"while\" '(' <exp> ')' <stmt>                       \n"
        "           | \"if\"    '(' <exp> ')' <stmt>                       \n"
        "           | <ident> '=' <lexp> ';'                               \n"
        "           | <typeident> '=' <lexp> ';'                           \n"
        "           | \"puts\" '(' <lexp>? ')' ';'                         \n"
        "           | \"return\" <lexp>? ';'                               \n"
        "           | <ident> '(' <ident>? (',' <ident>)* ')' ';' ;        \n"
        "                                                                  \n"
        " exp       : <lexp> '>' <lexp>                                    \n"
        "           | <lexp> '<' <lexp>                                    \n"
        "           | <lexp> \">=\" <lexp>                                 \n"
        "           | <lexp> \"<=\" <lexp>                                 \n"
        "           | <lexp> \"!=\" <lexp>                                 \n"
        "           | <lexp> \"==\" <lexp> ;                               \n"
        "                                                                  \n"
        " typeident : (\"int\" | \"char\" | \"float\") <ident> ;           \n"
        " decls     : (<typeident> ';')* ;                                 \n"
        " args      : <typeident>? (',' <typeident>)* ;                    \n"
        " body      : '{' <decls> <stmt>* '}' ;                            \n"
        " procedure : <typeident> '(' <args> ')' <body> ; \n"
        " program    : /^/ <decls> <procedure>+ /$/ ;     \n",
        psr->Ident, psr->Number, psr->Character, psr->String, psr->Factor, psr->Term, psr->Lexp, psr->Stmt, psr->Exp,
        psr->Typeident, psr->Decls, psr->Args, psr->Body, psr->Procedure, psr->Program, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }

    mpc_result_t *res = malloc(sizeof(mpc_result_t));
    if (mpc_parse_contents(input, psr->Program, res))
    {
        mpc_ast_print(res->output);
        mpc_ast_delete(res->output);
    }
    else
    {
        mpc_err_print(res->error);
        mpc_err_delete(res->error);
    }

    return res;
}

/**
 * @brief Clean up parsers
 *
 * @param psr
 */
void delete_parser(struct parser *psr)
{
    mpc_cleanup(15, psr->Ident, psr->Number, psr->Character, psr->String, psr->Factor, psr->Term, psr->Lexp, psr->Stmt,
                psr->Exp, psr->Typeident, psr->Decls, psr->Args, psr->Body, psr->Procedure, psr->Program);
}