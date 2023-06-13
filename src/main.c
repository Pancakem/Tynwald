#include "parser.h"
#include "gen.h"

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: tynwald <source> ...");
    exit(1);
  }

  struct parser psr;
  init_parser(&psr);
  mpc_result_t *res = parse_grammar(&psr, argv[1]);
  gen(res);
  mpc_ast_delete(res->output);
  delete_parser(&psr);
  free(res);
  return 0;
}
