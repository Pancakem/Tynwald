
#include <stdio.h>
#include <assert.h>
#include "stack.h"
#include "token.h"
#include "error.h"
#include "ast.h"
#include "parse.h"
#include "hashtable.h"
#include "bintree.h"
#include "list.h"
#include "util.h"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <file> ...", argv[0]);
    exit(1);
  }
  char *buff = read_file(argv[1]);
  SYSEXPECT(buff != NULL);
  parse_exp_cxt_t *cxt = parse_new(buff);
  token_t *basetype = parse(cxt);
  assert(token_get_next(cxt->token_cxt) == NULL);
  _ast_print(basetype, 0);
  ast_destroy(basetype);
  parse_exp_destroy(cxt);
  free(buff);
  return 0;
}