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
