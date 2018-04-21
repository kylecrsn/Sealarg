#ifndef _SEALARG_H
#define _SEALARG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

typedef enum parser_spec_e {
  PS_SEAL = 0,
  PS_GNU  = 1
} EParserSpec;

typedef enum arg_type_e {
  AT_OPTIONAL   = 0,
  AT_POSITIONAL = 1
} EArgType;

typedef enum arg_pos_type_e {
  PAT_VALUE = 0,
  PAT_ARRAY = 1
} EPosArgType;

typedef enum arg_opt_type_e {
  OAT_VALUE    = 0,
  OAT_ARRAY    = 1,
  OAT_PRESENCE = 2,
  OAT_COUNT    = 3
} EOptArgType;

typedef enum opt_arg_req_e {
  OAR_NONE     = 0,
  OAR_OPTIONAL = 1,
  OAR_REQUIRED = 2
} EOptArgReq;

typedef enum val_type_e {
  VT_CHAR            = 0,
  VT_S_CHAR          = 1,
  VT_U_CHAR          = 2,
  VT_SHORT           = 3,
  VT_SHORT_INT       = VT_SHORT,
  VT_S_SHORT         = VT_SHORT,
  VT_S_SHORT_INT     = VT_SHORT,
  VT_U_SHORT         = 4,
  VT_U_SHORT_INT     = VT_U_SHORT,
  VT_INT             = 5,
  VT_S_INT           = VT_INT,
  VT_SIGNED          = VT_INT,
  VT_U_INT           = 6,
  VT_UNSIGNED        = VT_U_INT,
  VT_LONG            = 7,
  VT_S_LONG          = VT_LONG,
  VT_LONG_INT        = VT_LONG,
  VT_S_LONG_INT      = VT_LONG,
  VT_U_LONG          = 8,
  VT_U_LONG_INT      = VT_U_LONG,
  VT_LONG_LONG       = 9,
  VT_S_LONG_LONG     = VT_LONG_LONG,
  VT_LONG_LONG_INT   = VT_LONG_LONG,
  VT_S_LONG_LONG_INT = VT_LONG_LONG,
  VT_U_LONG_LONG     = 10,
  VT_U_LONG_LONG_INT = VT_U_LONG_LONG,
  VT_FLOAT           = 11,
  VT_DOUBLE          = 12,
  VT_LONG_DOUBLE     = 13,
  VT_STRING          = 14,
  VT_BOOLEAN         = 15,
  VT_NONE            = 16
} EValType;

typedef struct seal_arg_s {
  char *flag;
  char *name;
  char *description;
  EArgType arg_type;
  EValType val_type;
  EPosArgType pos_arg_type;
  EOptArgType opt_arg_type;
  EOptArgReq opt_arg_req;
  void *value;
} SealArg;

typedef struct seal_parser_s {
  int count;
  EParserSpec spec;
  SealArg *args;
} SealParser;

extern void Initialize(SealParser *, EParserSpec);
extern void AddArgument(SealParser *, char *, char *, char *, EArgType, EValType, EPosArgType, EOptArgType, EOptArgReq);
extern void Parse(SealParser *, int, char **);

#endif
