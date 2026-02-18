/* --------------------------------------------------/
 * C file containing functions used in other files   /
 * -------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
#include "structs.h"

trade *read_csv(const char *filename, size_t *out_nrows)
{
	// Open the file, if there is an error, return NULL
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		perror("fopen");
		return NULL;
	}

	size_t cap = 1024;
	size_t n = 0;

	// Allocate enough memory for the inital cap size, if there is an error return NULL
	trade *table = malloc(cap * sizeof(trade));
	if (!table)
	{
		fclose(fp);
		return NULL;
	}

	while (1)
	{
		// Scan each row from the csv file and assign the values to the variables in t
		trade t;
		int ret = fscanf(fp, "%d,%19[^,],%lf,%lf\n",
				&t.id, t.timestamp, &t.price, &t.volume);

		// If the csv line returns EOF (End Of File) exit the loop
		if (ret == EOF)
			break;

		// If the csv does not contain the expected number of variables, skip to the next row
		if (ret != 4)
		{
			fprintf(stderr, "Malformed row %zu\n", n);
			continue;
		}

		// If the current row exceeds the cap, reallocate more memory for the new trades
		if (n == cap)
		{
			cap *= 2;
			trade *tmp = realloc(table, cap*sizeof(trade));
			if (!tmp)
			{
				free(table);
				fclose(fp);
				return NULL;
			}
			table = tmp;
		}

		// Assign the row t to the n+1 value of the table
		table[n++] = t;
	}

	// Close the file and assign the number of rows to the out_nrows variable
	fclose(fp);

	*out_nrows = n;
	return table;
}

int save_data(const char *filename, feature *feature_data, size_t in_nrows)
{
	FILE *out = fopen(filename, "w");
	if (!out)
	{
		perror("fopen");
		return 0;
	}

	fprintf(out, "time_start,time_end,open,high,low,close,volume,mean,var,skew,kurt,period_return,sma\n");
	for (size_t i = 0; i < in_nrows; i++)
	{
		// Retrieve the feature
		feature f = feature_data[i];

		// Write the feature data to the file
		fprintf(out, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
				(double)f.time_start, (double)f.time_end,
				f.open, f.high, f.low, f.close, f.volume, 
				f.mean, f.var, f.skew, f.kurt,
				f.period_return, f.sma);
	}
	return 1;
}

int parse_datetime(const char *s, struct tm *tm)
{
	return sscanf(
			s,
			"%4d-%2d-%2d %2d:%2d:%2d",
			&tm->tm_year,
			&tm->tm_mon,
			&tm->tm_mday,
			&tm->tm_hour,
			&tm->tm_min,
			&tm->tm_sec
		     ) == 6
		? (tm->tm_year -= 1900, tm->tm_mon -= 1, 1)
		: 0;
}

time_t bucket_time(time_t t, int minutes)
{
	time_t interval = minutes * 60;
	return (t / interval) * interval;
}

