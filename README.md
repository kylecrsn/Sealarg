# Sealarg
> An easy-to-use command-line argument parsing library for C

## About

Sealarg is a command-line parsing library designed to be as straightforward as possible to use. It is a small wrapper around the POSIX `getopt`/GNU `getopt_long` utilities, written to hide the grunt work and learning curve associated with the getopt interface. Skip to the [Getting Started](#getting-started) section below for a demonstration of Sealarg in action and how to quickly begin integrating the library into your project.

While getopt itself is a solid, standardized approach to command-line parsing, learning enough about it to attain a desirable level of robustness can often get in the way of the actual project it's being used in. Getopt has been widely used for decades and inspired many similar libraries in [other languages](https://en.wikipedia.org/wiki/Getopt#Other_languages), however other libraries such as Python's [argparse](https://docs.python.org/3/howto/argparse.html) have been developed with the intent of providing a much simpler alternative to verbose argument parsing. As such, this library lends much of its design choices to these newer alternative libraries.

## Getting Started

The sample code below illustrates just how easy Sealarg is to use in 5 steps. Here our program takes a string as a positional argument and prints it. If the optioanl argument `-m`/`--multiple` is used, the program prints out the string as many times as specified by the value passed to the option.

```c
#include "sealarg.h"

int main(int argc, char **argv) {
    int i;
    int count = 0;
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
```

When compiled and run with the `-h`/`--help` option, the following usage statement is automatically built and printed:

```sh
$ ./sample_program -h
Usage: [-h] [-m MULTIPLE] STRING
Prints a string some number of times

This is a prologue

Positional Arguments:
  STRING                    String to be printed
  
Optional Arguments:
  -h, --help                Display this help message and exit
  -m, --multiple            Number of string instances to print

This is an epilogue
$
```

Things like missing/excessive arguments, invalid arguments, and basic type/limit checking are all handled automatically.

**To integrate Sealarg into your project:**

- Copy `src/sealarg.c` and `include/sealarg.h` into your project folder and add them as dependenices in your Makefile, just like the rest of your code files
- Add `#include "selarg.h"` to wherever you'll be dealing with command-line arguments- typically this would be the .c file containing your `main` function
- Try compiling to make sure the library can be built alongside your code without any issues. If so, you're ready to start making calls to Sealarg's API and parse some arguments!

*For additional examples, check out [`example/`](example)*

*For the full documentation of Sealarg's API, including its more advanced semantics and finer-grained control, check out [`doc/`](doc)*

## References

Want to learn more about getopt? Click the links below:

- [GNU docs/examples on getopt/getopt_long](https://www.gnu.org/software/libc/manual/html_node/Getopt.html)
- [Man page getopt(3)](http://man7.org/linux/man-pages/man3/getopt.3.html)
- [Getopt on Wikipedia](https://en.wikipedia.org/wiki/Getopt)
- [IBM article](https://www.ibm.com/developerworks/aix/library/au-unix-getopt.html)
- [Informit article](http://www.informit.com/articles/article.aspx?p=175771&seqNum=3)
- [Rutgers article](https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/getopt.html)
- [SO posts](https://stackoverflow.com/questions/tagged/c+getopt)