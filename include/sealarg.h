#ifndef _SEALARG_H
#define _SEALARG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

typedef enum parser_spec_e {
  PS_SEAL  = 0,
  PS_POSIX = 1,
  PS_GNU   = 2
} EParserSpec;

typedef enum arg_kind_e {
  AK_OPTIONAL   = 0,
  AK_POSITIONAL = 1
} EArgKind;

typedef enum arg_req_e {
  AR_NONE     = 0,
  AR_OPTIONAL = 1,
  AR_REQUIRED = 2
} EArgReq;

typedef enum arg_action_e {
  AA_PRESENCE = 0,
  //AA_COUNT  = 1,
  AA_VALUE    = 2
  //AA_ARRAY  = 3
} EArgAction;

typedef enum val_type_e {
  VT_VOID        = 0,
  VT_SHORT       = 1,
  VT_U_SHORT     = 2,
  VT_INT         = 3,
  VT_U_INT       = 4,
  VT_LONG        = 5,
  VT_U_LONG      = 6,
  VT_LONG_LONG   = 7,
  VT_U_LONG_LONG = 8,
  VT_FLOAT       = 9,
  VT_DOUBLE      = 10,
  VT_LONG_DOUBLE = 11,
  VT_CHAR        = 12,
  VT_S_CHAR      = 13,
  VT_U_CHAR      = 14,
  VT_STRING      = 15,
  VT_BOOL        = 16
} EValType;

typedef union arg_val_u {
  void *             vp;
  short              s;
  unsigned short     us;
  int                i;
  unsigned int       ui;
  long               l;
  unsigned long      ul;
  long long          ll;
  unsigned long long ull;
  float              f;
  double             d;
  long double        ld;
  char               c;
  signed char        sc;
  unsigned char      uc;
  char *             cp;
  bool               b;
} ArgVal;

typedef struct seal_arg_s {
  int        parsed;
  char *     flag;
  char *     name;
  char *     help;
  EArgKind   kind;
  EArgReq    req;
  EArgAction action;
  EValType   type;
  ArgVal     value;
} SealArg;

typedef struct seal_parser_s {
  int         err;
  int         count;
  int         opt_count;
  int         pos_count;
  char *      about;
  char *      prologue;
  char *      epilogue;
  EParserSpec spec;
  SealArg *   args;
} SealParser;

extern void Initialize(SealParser *, char *, char *, char *, EParserSpec);
extern void AddArgument(SealParser *, char *, char *, char *, EArgKind, EArgReq, EArgAction, EValType);
extern void Parse(SealParser *, int, char **);

#endif
