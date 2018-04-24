#include "sealarg.h"

int main(int argc, char **argv) {
    int i;
    int count;
    char *str;

    // 1. Allocate a SealParser instance
    SealParser *parser = malloc(sizeof(SealParser));

    // 2. Initialize the parser
    Initialize(parser, "Prints a string some number of times", "This is a prologue", "This is an epilogue", PS_SEAL);

    // 3. Add any number of optional or positional arguments
    AddArgument(parser, "m", "multiple", "Number of string instances to print", AK_OPTIONAL, AR_REQUIRED, AA_VALUE, VT_INT);
    AddArgument(parser, NULL, "string", "String to be printed", AK_POSITIONAL, AR_REQUIRED, AA_VALUE, VT_STRING);

    // 4. Parse all the arguments and check for any errors
    Parse(parser, argc, argv);
    if(parser->err != 0) {
        free(parser);
        return parser->err;
    }

    // 5. Access arguments by index in the order they were added to the parser
    count = parser->args[1].value.i;
    str = parser->args[2].value.cp;

    for(i = 0; i < count; i++) {
        printf("%s\n", str);
    }

    free(parser);
    return 0;
}

/*
#include "sealarg.h"

int main(int argc, char *argv[]) {
  int i;
  SealParser *parser = malloc(sizeof(SealParser));

  Initialize(parser, NULL, NULL, NULL, PS_SEAL);
  AddArgument(parser, "h", "help", "Usage of program", AK_OPTIONAL, AR_NONE, AA_PRESENCE, VT_INT);
  AddArgument(parser, "v", "verbose", "Verbose output", AK_OPTIONAL, AR_NONE, AA_PRESENCE, VT_INT);
  AddArgument(parser, "a", "alpha", "alpha option", AK_OPTIONAL, AR_NONE, AA_PRESENCE, VT_INT);
  AddArgument(parser, "b", "beta", "beta option", AK_OPTIONAL, AR_REQUIRED, AA_VALUE, VT_INT);
  AddArgument(parser, "c", "charlie", "charlie option", AK_OPTIONAL, AR_REQUIRED, AA_VALUE, VT_STRING);
  AddArgument(parser, NULL, "delta", "delta positional option", AK_POSITIONAL, AR_REQUIRED, AA_VALUE, VT_INT);
  AddArgument(parser, NULL, "epsilon", "epsilon positional option", AK_POSITIONAL, AR_REQUIRED, AA_VALUE, VT_STRING);
  Parse(parser, argc, argv);

  fprintf(stdout, "%d\n", parser->count);
  for(i = 0; i < parser->count; i++) {
    fprintf(stdout, 
      "%s %s %s %d %d %d %d %d ",
      parser->args[i].flag,
      parser->args[i].name,
      parser->args[i].help,
      parser->args[i].kind,
      parser->args[i].req,
      parser->args[i].action,
      parser->args[i].type,
      parser->args[i].parsed);
    fflush(stdout);
    switch(parser->args[i].action) {
      case AA_PRESENCE: {
        fprintf(stdout, "Value: %d\n", parser->args[i].value.i);
        break;
      }
      case AA_VALUE: {
        switch(parser->args[i].type) {
          case VT_CHAR: {
            fprintf(stdout, "Value: %c\n", parser->args[i].value.c);
            break;
          }
          case VT_INT: {
            fprintf(stdout, "Value: %d\n", parser->args[i].value.i);
            break;
          }
          case VT_STRING: {
            fprintf(stdout, "Value: %s\n", parser->args[i].value.cp);
            break;
          }
          default: {
            fprintf(stdout, "Not yet handled\n");
            break;
          }
        }
      }
    }
  }

  free(parser);
  return 0;
}

*/