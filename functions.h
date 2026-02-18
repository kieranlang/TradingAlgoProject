#ifndef FILES_H
#define FILES_H

#include <time.h>
#include "structs.h"

trade *read_csv(const char *filename, size_t *out_nrows);
int save_data(const char *filename, feature *feature_data, size_t in_nrows);
int parse_datetime(const char *s, struct tm *tm);
time_t bucket_time(time_t t, int minutes);

#endif

