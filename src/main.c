#include "parser.h"

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: tynwald <source> ...");
    exit(1);
  }

  struct parser psr;
  init_parser(&psr);
  mpc_result_t *res = parse_grammar(&psr, argv[1]);

  return 0;
}
