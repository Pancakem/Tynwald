#include "mpc.h"

int main(int argc, char **argv) {

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
      "           | \"print\" '(' <lexp>? ')' ';'                        \n"
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
      " typeident : (\"int\" | \"char\") <ident> | \"void\";             \n"
      " decls     : (<typeident> ';')* ;                                 \n"
      " args      : <typeident>? (',' <typeident>)* ;                    \n"
      " body      : '{' <decls> <stmt>* '}' ;                            \n"
      " procedure : (\"int\" | \"char\") <ident> '(' <args> ')' <body> ; \n"
      " main      : <typeident> \"main\" '(' <args> ')' <body> ;         \n"
      " includes  : (\"#include\" <string>)* ;                           \n"
      " program    : /^/ <decls> <procedure>* <main> /$/ ;     \n",
      Ident, Number, Character, String, Factor, Term, Lexp, Stmt, Exp,
      Typeident, Decls, Args, Body, Procedure, Main, Program, NULL);

  if (err != NULL) {
    mpc_err_print(err);
    mpc_err_delete(err);
    exit(1);
  }

  if (argc > 1) {

    mpc_result_t r;
    if (mpc_parse_contents(argv[1], Program, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
  }

  mpc_cleanup(17, Ident, Number, Character, String, Factor, Term, Lexp, Stmt,
              Exp, Typeident, Decls, Args, Body, Procedure, Main, Program);

  return 0;
}
