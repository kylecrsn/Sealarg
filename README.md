# Sealarg
> A lightweight, easy-to-use command-line argument parsing library for C

## About

Sealarg is a command-line parsing library designed to be as straightforward as possible to use. It is a small wrapper around the POSIX `getopt`/GNU `getopt_long` utilities, written to hide the grunt work and learning curve associated with the getopt interface. Skip to the [Getting Started](#getting-started) section below for how to quickly begin integrating the library into your project.

While getopt itself is a solid, standardized approach to command-line parsing, learning enough about it to attain a desirable level of robustness can often get in the way of the actual project it's being used in. Getopt has been widely used for decades and inspired many similar libraries in [other languages](https://en.wikipedia.org/wiki/Getopt#Other_languages), however other libraries such as Python's [argparse](https://docs.python.org/3/howto/argparse.html) have been developed with the intent of providing a much simpler alternative to verbose argument parsing. As such, this library lends much of its design choices to these newer alternative libraries.

## Getting Started


*For additional examples, check out `example/`*
*For the full documentation of Sealarg, including its more advanced semantics, check out `doc/`*

## References

Want to learn more about getopt? Click the links below:

- [GNU docs/examples on getopt/getopt_long](https://www.gnu.org/software/libc/manual/html_node/Getopt.html)
- [Man page getopt(3)](http://man7.org/linux/man-pages/man3/getopt.3.html)
- [Getopt on Wikipedia](https://en.wikipedia.org/wiki/Getopt)
- [Article on the history/usage of getopt/getopt_long](https://www.ibm.com/developerworks/aix/library/au-unix-getopt.html)
- [Informit article](http://www.informit.com/articles/article.aspx?p=175771&seqNum=3)
- [Rutgers getopt tutorial](https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/getopt.html)
- [Another getopt tutorial](https://www.theurbanpenguin.com/using-getopt-parse-arguments-c/)
