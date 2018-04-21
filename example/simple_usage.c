#include <stdlib.h>
#include <stdio.h>

#include "sealarg.h"

int main(int argc, char *argv[]) {
  int i;
  SealParser *parser = malloc(sizeof(SealParser));
  
  Initialize(parser, PS_SEAL);
  AddArgument(parser, "h", "help", "Usage of program", AT_OPTIONAL, VT_NONE, PAT_VALUE, OAT_PRESENCE, OAR_NONE);
  AddArgument(parser, "v", "verbose", "Verbose output", AT_OPTIONAL, VT_NONE, PAT_VALUE, OAT_PRESENCE, OAR_NONE);
  AddArgument(parser, "a", "alpha", "alpha option", AT_OPTIONAL, VT_CHAR, PAT_VALUE, OAT_VALUE, OAR_REQUIRED);
  AddArgument(parser, "b", "ebta", "beta option", AT_OPTIONAL, VT_INT, PAT_VALUE, OAT_VALUE, OAR_REQUIRED);
  AddArgument(parser, "c", "charlie", NULL, AT_OPTIONAL, VT_STRING, PAT_VALUE, OAT_VALUE, OAR_REQUIRED);
  Parse(parser, argc, argv);

  fprintf(stdout, "%d\n", parser->count);
  for(i = 0; i < parser->count; i++) {
    fprintf(stdout, 
      "%s %s %s %d %d %d %d %d ",
      parser->args[i].flag,
      parser->args[i].name,
      parser->args[i].description,
      parser->args[i].arg_type,
      parser->args[i].val_type,
      parser->args[i].pos_arg_type,
      parser->args[i].opt_arg_type,
      parser->args[i].opt_arg_req);
    fflush(stdout);
    switch(parser->args[i].opt_arg_type) {
      case OAT_PRESENCE: {
        fprintf(stdout, "Value: %d\n", *(int *)parser->args[i].value);
        break;
      }
      case OAT_VALUE: {
        switch(parser->args[i].val_type) {
          case VT_CHAR: {
            fprintf(stdout, "Value: %c\n", *(char *)parser->args[i].value);
            break;
          }
          case VT_INT: {
            fprintf(stdout, "Value: %d\n", *(int *)parser->args[i].value);
            break;
          }
          case VT_STRING: {
            fprintf(stdout, "Value: %s\n", (char *)parser->args[i].value);
            break;
          }
        }
      }
    }
  }

  free(parser);
  return 0;
}
