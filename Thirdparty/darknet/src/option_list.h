#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#include "darknet.h"
#include "list.h"

typedef struct{
    char *key;
    char *val;
    int used;
} kvp;

#ifdef __cplusplus
extern "C" {
#endif

dlist *read_data_cfg(char *filename);
int read_option(char *s, dlist *options);
void option_insert(dlist *l, char *key, char *val);
char *option_find(dlist *l, char *key);
char *option_find_str(dlist *l, char *key, char *def);
int option_find_int(dlist *l, char *key, int def);
int option_find_int_quiet(dlist *l, char *key, int def);
float option_find_float(dlist *l, char *key, float def);
float option_find_float_quiet(dlist *l, char *key, float def);
void option_unused(dlist *l);

//typedef struct {
//	int classes;
//	char **names;
//} metadata;

//LIB_API metadata get_metadata(char *file);

#ifdef __cplusplus
}
#endif
#endif
