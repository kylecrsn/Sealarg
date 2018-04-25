#include "sealarg.h"

static const char *SA_ERR_MSGS[] = {
  "\nSA_ERR: The value specified could not be stored as the type expected:",
  "\nSA_ERR: The value specified exceeded the limits for its type:"
};

void StrToVal(SealParser *, char *, int);
void ValParseCheck(char *, char *, int *);
void PrintUsage(SealParser *, char *);
char *StrToUpper(char *);
void DynamicAppend(char *, char **, int *);


void Initialize(SealParser *parser, char *about, char *prologue, char *epilogue, EParserSpec spec) {
  opterr = 0;
  parser->err = 0;
  parser->count = 0;
  parser->opt_count = 0;
  parser->pos_count = 0;
  parser->about = about;
  parser->prologue = prologue;
  parser->epilogue = epilogue;
  parser->spec = spec;
  parser->args = NULL;

  if(parser->spec == PS_SEAL) {
    AddArgument(parser, "h", "help", "Display this help message and exit", AK_OPTIONAL, AR_NONE, AA_PRESENCE, VT_INT);
  }
}

void AddArgument(SealParser *parser, char *flag, char *name, char *help, EArgKind kind, EArgReq req, EArgAction action, EValType type) {
  int i = parser->count;

  parser->count++;
  parser->args = realloc(parser->args, parser->count*sizeof(SealArg));
  parser->args[i].parsed = 0;
  parser->args[i].flag = flag;
  parser->args[i].name = name;
  parser->args[i].help = help;
  parser->args[i].kind = kind;
  parser->args[i].req = req;
  parser->args[i].action = action;
  parser->args[i].type = type;

  if(parser->args[i].kind == AK_OPTIONAL) {
    parser->opt_count++;
  }
  else {
    parser->pos_count++;
  }

  switch(parser->args[i].action) {
    case AA_PRESENCE: {
      parser->args[i].value.i = 0;
      break;
    }
    case AA_VALUE: {
      switch(parser->args[i].type) {
        case VT_VOID: {
          parser->args[i].value.vp = NULL;
          break;
        }
        case VT_SHORT: {
          parser->args[i].value.s = 0;
          break;
        }
        case VT_U_SHORT: {
          parser->args[i].value.us = 0;
          break;
        }
        case VT_INT: {
          parser->args[i].value.i = 0;
          break;
        }
        case VT_U_INT: {
          parser->args[i].value.ui = 0;
          break;
        }
        case VT_LONG: {
          parser->args[i].value.l = 0;
          break;
        }
        case VT_U_LONG: {
          parser->args[i].value.ul = 0;
          break;
        }
        case VT_LONG_LONG: {
          parser->args[i].value.ll = 0;
          break;
        }
        case VT_U_LONG_LONG: {
          parser->args[i].value.ull = 0;
          break;
        }
        case VT_FLOAT: {
          parser->args[i].value.f = 0.0;
          break;
        }
        case VT_DOUBLE: {
          parser->args[i].value.d = 0.0;
          break;
        }
        case VT_LONG_DOUBLE: {
          parser->args[i].value.ld = 0.0;
          break;
        }
        case VT_CHAR: {
          parser->args[i].value.c = 0;
          break;
        }
        case VT_S_CHAR: {
          parser->args[i].value.sc = 0;
          break;
        }
        case VT_U_CHAR: {
          parser->args[i].value.uc = 0;
          break;
        }
        case VT_STRING: {
          parser->args[i].value.cp = NULL;
          break;
        }
        case VT_BOOL: {
          parser->args[i].value.b = false;
          break;
        }
      }
      break;
    }
  }
}

void Parse(SealParser *parser, int argc, char **argv) {
  int i;
  int f = 0;
  int res;
  int opt_handled;
  int opt_index;
  char *short_flags = NULL;
  struct option opts[parser->count+1];

  // Build struct options object for getopt_long
  for(i = 0; i < parser->count; i++) {
    if(parser->args[i].kind == AK_OPTIONAL) {
      switch(parser->args[i].req) {
        case AR_NONE: {
          opts[i] = (struct option){parser->args[i].name, no_argument, 0, *parser->args[i].flag};
          DynamicAppend(parser->args[i].flag, &short_flags, &f);
          break;
        }
        case AR_OPTIONAL: {
          opts[i] = (struct option){parser->args[i].name, optional_argument, 0, *parser->args[i].flag};
          DynamicAppend(parser->args[i].flag, &short_flags, &f);
          DynamicAppend("::", &short_flags, &f);
          break;
        }
        case AR_REQUIRED: {
          opts[i] = (struct option){parser->args[i].name, required_argument, 0, *parser->args[i].flag};
          DynamicAppend(parser->args[i].flag, &short_flags, &f);
          DynamicAppend(":", &short_flags, &f);
          break;
        }
      }
    }
  }
  opts[i] = (struct option){0, 0, 0, 0};

  // Parse all AK_OPTIONAL args
  while(1) {
    opt_handled = 0;
    opt_index = 0;
    res = getopt_long(argc, argv, short_flags, opts, &opt_index);
    if(res == -1) {
      fprintf(stderr, "end of optional options\n");
      break;
    }
    else if(res == '?') {
      fprintf(stderr, "handled error detected\n");
      break;
    }

    for(i = 0; i < parser->count; i++) {
      if(res == *parser->args[i].flag) {
        if(res == 'h') {
          PrintUsage(parser, argv[0]);
          parser->err = -1;
          free(short_flags);
          return;
        }
        switch(parser->args[i].action) {
          case AA_PRESENCE: {
            parser->args[i].value.i = 1;
            parser->args[i].parsed = 1;
            break;
          }
          case AA_VALUE: {
            StrToVal(parser, optarg, i);
            if(parser->err != 0) {
              PrintUsage(parser, argv[0]);
              free(short_flags);
              return;
            }
            parser->args[i].parsed = 1;
            break;
          }
        }
        opt_handled = 1;
        break;
      }
    }

    if(opt_handled == 0) {
      fprintf(stderr, "unhandled error detected\n");
      abort();
    }
  }
  free(short_flags);

  // Check if there were only AK_OPTIONAL args
  if(optind == argc) {
    fprintf(stderr, "detected end of args\n");
    return;
  }

  // Parse all AK_POSITIONAL args
  for(i = 0; i < parser->count; i++) {
    if(parser->args[i].kind == AK_POSITIONAL) {
      switch(parser->args[i].req) {
        case AR_OPTIONAL: {
          break;
        }
        case AR_REQUIRED: {
          StrToVal(parser, argv[optind], i);
          if(parser->err != 0) {
            PrintUsage(parser, argv[0]);
            return;
          }
          parser->args[i].parsed = 1;
          optind++;
          break;
        }
        default: {
          fprintf(stderr, "Incompatible AR detected\n");
          break;
        }
      }
    }
  }

  if(optind != argc) {
    fprintf(stderr, "Leftover arguments not parsed\n");
  }
}

void StrToVal(SealParser *parser, char *argStr, int i) {
  errno = 0;
  int err = 0;
  char *end;
  ArgVal test = {0};

  switch(parser->args[i].type) {
    case VT_VOID: {
      parser->args[i].value.vp = realloc(parser->args[i].value.vp, strlen(argStr)*sizeof(char));
      parser->args[i].value.vp = (void *)argStr;
      break;
    }
    case VT_SHORT: {
      test.l = strtol(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      if(err == 0 && (test.l < SHRT_MIN || test.l > SHRT_MAX)) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.s = (err == 0) ? (short)test.l : 0;
      break;
    }
    case VT_U_SHORT: {
      test.ul = strtoul(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      if(err == 0 && test.ul > USHRT_MAX) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.us = (err == 0) ? (unsigned short)test.ul : 0;
      break;
    }
    case VT_INT: {
      test.l = strtol(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      if(err == 0 && (test.l < INT_MIN || test.l > INT_MAX)) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.i = (err == 0) ? (int)test.l : 0;
      break;
    }
    case VT_U_INT: {
      test.ul = strtoul(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      if(err == 0 && test.ul > UINT_MAX) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.ui = (err == 0) ? (unsigned int)test.ul : 0;
      break;
    }
    case VT_LONG: {
      test.l = strtol(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.l = (err == 0) ? test.l : 0;
      break;
    }
    case VT_U_LONG: {
      test.ul = strtoul(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.ul = (err == 0) ? test.ul : 0;
      break;
    }
    case VT_LONG_LONG: {
      test.ll = strtoll(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.ll = (err == 0) ? test.ll : 0;
      break;
    }
    case VT_U_LONG_LONG: {
      test.ull = strtoull(argStr, &end, 10);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.ull = (err == 0) ? test.ull : 0;
      break;
    }
    case VT_FLOAT: {
      test.d = strtod(argStr, &end);
      ValParseCheck(argStr, end, &err);
      if(err == 0 && (test.d < FLT_MIN || test.d > FLT_MAX)) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.f = (err == 0) ? (float)test.d : 0.0;
      break;
    }
    case VT_DOUBLE: {
      test.d = strtod(argStr, &end);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.d = (err == 0) ? test.d : 0.0;
      break;
    }
    case VT_LONG_DOUBLE: {
      test.ld = strtold(argStr, &end);
      ValParseCheck(argStr, end, &err);
      parser->args[i].value.ld = (err == 0) ? test.ld : 0.0;
      break;
    }
    case VT_CHAR: {
      test.l = strtol(argStr, &end, 10);
      if(*end != 0) {
        if(strlen(argStr) == 1) {
          parser->args[i].value.c = argStr[0];
          break;
        }
        fprintf(stderr, "%s \"%s\"\n", SA_ERR_MSGS[0], argStr);
        err = 1;
      }
      else if(errno != 0 || test.l < CHAR_MIN || test.l > CHAR_MAX) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.c = (err == 0) ? (char)test.l : 0;
      break;
    }
    case VT_S_CHAR: {
      test.l = strtol(argStr, &end, 10);
      if(*end != 0) {
        if(strlen(argStr) == 1) {
          parser->args[i].value.sc = (signed char)argStr[0];
          break;
        }
        fprintf(stderr, "%s \"%s\"\n", SA_ERR_MSGS[0], argStr);
        err = 1;
      }
      else if(errno != 0 || test.l < SCHAR_MIN || test.l > SCHAR_MAX) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.sc = (err == 0) ? (signed char)test.l : 0;
      break;
    }
    case VT_U_CHAR: {
      test.ul = strtoul(argStr, &end, 10);
      if(*end != 0) {
        if(strlen(argStr) == 1) {
          parser->args[i].value.uc = (unsigned char)argStr[0];
          break;
        }
        fprintf(stderr, "%s \"%s\"\n", SA_ERR_MSGS[0], argStr);
        err = 1;
      }
      else if(errno != 0 || test.ul > UCHAR_MAX) {
        fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(ERANGE));
        err = 2;
      }
      parser->args[i].value.uc = (err == 0) ? (unsigned char)test.ul : 0;
      break;
    }
    case VT_STRING: {
      parser->args[i].value.cp = realloc(parser->args[i].value.cp, strlen(argStr)*sizeof(char));
      parser->args[i].value.cp = argStr;
      break;
    }
    case VT_BOOL: {
      if(strcmp("0", argStr) || strcmp("false", argStr)) {
        parser->args[i].value.b = false;
      }
      else if(strcmp("1", argStr) || strcmp("true", argStr)) {
        parser->args[i].value.b = true;
      }
      else {
        fprintf(stderr, "%s \"%s\"\n", SA_ERR_MSGS[0], argStr);
        err = 1;
      }
      break;
    }
  }
  parser->err = err;
}

void ValParseCheck(char *argStr, char *end, int *err) {
  if(*end != 0) {
    fprintf(stderr, "%s \"%s\"\n", SA_ERR_MSGS[0], argStr);
    *err = 1;
  }
  else if(errno != 0) {
    fprintf(stderr, "%s \"%s\"\n\t> %s\n", SA_ERR_MSGS[1], argStr, strerror(errno));
    *err = 2;
  }
}

void PrintUsage(SealParser *parser, char *prog) {
  int i;
  int n = 0, p = 0, v = 0;
  char *caps;
  char *usage = NULL;
  char *short_pres = NULL;
  char *short_val = NULL;

  switch(parser->spec) {
    case PS_SEAL: {
      DynamicAppend("Usage:", &usage, &n);
      for(i = 0; i < parser->count; i++) {
        if(parser->args[i].kind == AK_OPTIONAL && parser->args[i].flag != NULL) {
          switch(parser->args[i].action) {
            case AA_PRESENCE: {
              DynamicAppend(" [-", &short_pres, &p);
              DynamicAppend(parser->args[i].flag, &short_pres, &p);
              DynamicAppend("]", &short_pres, &p);
              break;
            }
            case AA_VALUE: {
              caps = StrToUpper(parser->args[i].name);
              DynamicAppend(" [-", &short_val, &v);
              DynamicAppend(parser->args[i].flag, &short_val, &v);
              DynamicAppend(" ", &short_val, &v);
              if(parser->args[i].req == AR_OPTIONAL) {
                DynamicAppend("[", &short_val, &v);
                DynamicAppend(caps, &short_val, &v);
                DynamicAppend("]", &short_val, &v);
              }
              else {
                DynamicAppend(caps, &short_val, &v);
              }
              DynamicAppend("]", &short_val, &v);
              free(caps);
              break;
            }
          }
        }
      }
      DynamicAppend(short_pres, &usage, &n);
      DynamicAppend(short_val, &usage, &n);
      free(short_pres);
      free(short_val);
      for(i = 0; i < parser->count; i++) {
        if(parser->args[i].kind == AK_POSITIONAL) {
          caps = StrToUpper(parser->args[i].name);
          DynamicAppend(" ", &usage, &n);
          DynamicAppend(caps, &usage, &n);
          free(caps);
        }
      }
      if(parser->about != NULL) {
        DynamicAppend("\n", &usage, &n);
        DynamicAppend(parser->about, &usage, &n);
      }
      DynamicAppend("\n", &usage, &n);
      if(parser->prologue != NULL) {
        DynamicAppend("\n", &usage, &n);
        DynamicAppend(parser->prologue, &usage, &n);
        DynamicAppend("\n", &usage, &n);
      }
      if(parser->pos_count != 0) {
        DynamicAppend("\nPositional Arguments:", &usage, &n);
        for(i = 0; i < parser->count; i++) {
          if(parser->args[i].kind == AK_POSITIONAL) {
            caps = StrToUpper(parser->args[i].name);
            DynamicAppend("\n  ", &usage, &n);
            DynamicAppend(caps, &usage, &n);
            DynamicAppend("\t\t\t", &usage, &n);
            DynamicAppend(parser->args[i].help, &usage, &n);
            free(caps);
          }
        }
        DynamicAppend("\n  ", &usage, &n);
      }
      if(parser->opt_count != 0) {
        DynamicAppend("\nOptional Arguments:", &usage, &n);
        for(i = 0; i < parser->count; i++) {
          if(parser->args[i].kind == AK_OPTIONAL) {
            DynamicAppend("\n  ", &usage, &n);
            if(parser->args[i].flag != NULL) {
              DynamicAppend("-", &usage, &n);
              DynamicAppend(parser->args[i].flag, &usage, &n);
              DynamicAppend(", ", &usage, &n);
            }
            else {
              DynamicAppend("    ", &usage, &n);
            }
            DynamicAppend("--", &usage, &n);
            DynamicAppend(parser->args[i].name, &usage, &n);
            DynamicAppend("\t\t\t", &usage, &n);
            DynamicAppend(parser->args[i].help, &usage, &n);
          }
        }
        DynamicAppend("\n", &usage, &n);
      }
      if(parser->epilogue != NULL) {
        DynamicAppend("\n", &usage, &n);
        DynamicAppend(parser->epilogue, &usage, &n);
        DynamicAppend("\n", &usage, &n);
      }
    }
    default: {
      printf("not yet handled usage\n");
      break;
    }
  }
  fprintf(stdout, "%s", usage);
  free(usage);
}

char *StrToUpper(char *str) {
  int i = 0;

  char *caps = malloc(strlen(str)*sizeof(char));
  while(str[i]) {
    caps[i] = toupper(str[i]);
    i++;
  }

  return caps;
}

void DynamicAppend(char *a, char **s, int *n) {
  int len = strlen(a);

  if(*n == 0) {
    *s = (char *)malloc((len+1)*sizeof(char));
    memcpy(*s, a, len);
  }
  else {
    *s = realloc(*s, (*n+len+1)*sizeof(char));
    memcpy(*s+*n, a, len);
  }
  *n += len;
  (*s)[*n] = 0;
}
