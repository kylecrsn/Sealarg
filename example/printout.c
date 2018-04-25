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
