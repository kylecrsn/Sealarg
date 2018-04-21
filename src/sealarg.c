#include "sealarg.h"

void Initialize(SealParser *parser, EParserSpec spec) {
  opterr = 0;
  parser->count = 0;
  parser->spec = spec;
  parser->args = malloc(0);
}

void AddArgument(SealParser *parser, char *flag, char *name, char *description, 
                EArgType argType, EValType valType, EPosArgType posArgType, EOptArgType optArgType, EOptArgReq optArgReq) {
  int i = parser->count;

  parser->count++;
  parser->args = realloc(parser->args, parser->count*sizeof(SealArg));
  parser->args[i].flag = flag;
  parser->args[i].name = name;
  parser->args[i].description = description;
  parser->args[i].arg_type = argType;
  parser->args[i].val_type = valType;
  parser->args[i].pos_arg_type = posArgType;
  parser->args[i].opt_arg_type = optArgType;
  parser->args[i].opt_arg_req = optArgReq;

  switch(parser->args[i].opt_arg_type) {
    case OAT_VALUE: {
      switch(parser->args[i].val_type) {
        case VT_CHAR: {
          parser->args[i].value = malloc(sizeof(char));
          *(char *)parser->args[i].value = 0;
          break;
        }
        case VT_INT: {
          parser->args[i].value = malloc(sizeof(int));
          *(int *)parser->args[i].value = 0;
          break;
        }
        case VT_STRING: {
          parser->args[i].value = malloc(sizeof(char));
          *(char *)parser->args[i].value = 0;
          break;
        }
      }
      break;
    }
    case OAT_PRESENCE: {
      parser->args[i].value = malloc(sizeof(int));
      *(int *)parser->args[i].value = 0;
      break;
    }
  }
}

void Parse(SealParser *parser, int argc, char **argv) {
  int i;
  int res;
  int opt_handled;
  int opt_index;
  char *short_flags = malloc(0);
  char **end;
  struct option opts[parser->count+1];

  for(i = 0; i < parser->count; i++) {
    switch(parser->args[i].opt_arg_req) {
      case OAR_NONE: {
        opts[i] = (struct option){parser->args[i].name, no_argument, 0, *parser->args[i].flag};
        short_flags = realloc(short_flags, sizeof(char));
        strcat(short_flags, parser->args[i].flag);
        break;
      }
      case OAR_OPTIONAL: {
        opts[i] = (struct option){parser->args[i].name, optional_argument, 0, *parser->args[i].flag};
        short_flags = realloc(short_flags, 3*sizeof(char));
        strcat(short_flags, parser->args[i].flag);
        strcat(short_flags, "::");
        break;
      }
      case OAR_REQUIRED: {
        opts[i] = (struct option){parser->args[i].name, required_argument, 0, *parser->args[i].flag};
        short_flags = realloc(short_flags, 2*sizeof(char));
        strcat(short_flags, parser->args[i].flag);
        strcat(short_flags, ":");
        break;
      }
    }
  }
  opts[i] = (struct option){0, 0, 0, 0};

  while(1) {
    opt_handled = 0;
    opt_index = 0;
    res = getopt_long(argc, argv, short_flags, opts, &opt_index);
    if(res == -1) {
      fprintf(stdout, "end of optional options\n");
      break;
    }
    else if(res == '?') {
      fprintf(stdout, "handled error detected\n");
      break;
    }

    for(i = 0; i < parser->count; i++) {
      if(res == *parser->args[i].flag) {
        switch(parser->args[i].opt_arg_type) {
          case OAT_VALUE: {
            switch(parser->args[i].val_type) {
              case VT_CHAR: {
                *(char *)parser->args[i].value = *optarg;
                break;
              }
              case VT_INT: {
                *(int *)parser->args[i].value = strtol(optarg, end, 10);
                break;
              }
              case VT_STRING: {
                parser->args[i].value = realloc(parser->args[i].value, strlen(optarg)*sizeof(char));
                parser->args[i].value = (char *)optarg;
                break;
              }
            }
            break;
          }
          case OAT_PRESENCE: {
            *(int *)parser->args[i].value = 1;
            break;
          }
        }
        opt_handled = 1;
        break;
      }
    }

    if(opt_handled == 0) {
      fprintf(stdout, "unhandled error detected\n");
      abort();
    }
  }

  free(short_flags);
}
