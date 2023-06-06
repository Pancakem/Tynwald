#include "parser.h"
#include "util.h"
#include <assert.h>
#include <stdlib.h>

mpc_result_t *parse_grammar(const char *input)
{
    char *grammar = read_file(GRAMMAR_FILE);
    assert(grammar != NULL);

    mpc_parser_t *Ident = mpc_new("ident");
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Character = mpc_new("character");
    mpc_parser_t *String = mpc_new("string");
    mpc_parser_t *Factor = mpc_new("factor");
    mpc_parser_t *Term = mpc_new("term");
    mpc_parser_t *Lexp = mpc_new("lexp");
    mpc_parser_t *Stmt = mpc_new("stmt");
    mpc_parser_t *Exp = mpc_new("exp");
    mpc_parser_t *Typeident = mpc_new("typeident");
    mpc_parser_t *Decls = mpc_new("decls");
    mpc_parser_t *Args = mpc_new("args");
    mpc_parser_t *Body = mpc_new("body");
    mpc_parser_t *Procedure = mpc_new("procedure");
    mpc_parser_t *Main = mpc_new("main");
    mpc_parser_t *Program = mpc_new("program");

    mpc_err_t *err = mpca_lang(
        MPCA_LANG_DEFAULT,
        grammar, Ident, Number, Character, String, Factor, Term, Lexp, Stmt, Exp,
        Typeident, Decls, Args, Body, Procedure, Main, Program, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }

    mpc_result_t *res = malloc(sizeof(mpc_result_t));
    if (mpc_parse_contents(input, Program, res))
    {
        mpc_ast_print(res->output);
        mpc_ast_delete(res->output);
    }
    else
    {
        mpc_err_print(res->error);
        mpc_err_delete(res->error);
    }

    mpc_cleanup(16, Ident, Number, Character, String, Factor, Term, Lexp, Stmt,
                Exp, Typeident, Decls, Args, Body, Procedure, Main, Program);

    return res;
}