#include "parser.h"

int main(int argc, char **argv)
{
  if (argc < 1) {
    fprintf(stderr, "Usage: tynwald <source> ...");
    exit(1);
  }

  mpc_result_t *res = parse_grammar(argv[1]);

  return 0;
}
