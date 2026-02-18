/*---------------------------------------------------------------------------/
 * File containing all necessary functions to calculate trade-level features /
 * -------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include "structs.h"
#include "functions.h"
#include "agg.h"
#include "parameters.h"
#include "statistics.h"

feature *agg_trades(trade *trade_data, size_t in_nrows, size_t *out_nrows)
{
	size_t cap = 1024;
	size_t n = 0;

	// Allocate enough memory for the initial cap size and return NULL if there is an error
	feature *table = malloc(cap * sizeof(feature));
	if (!table)
	{
		return NULL;
	}

	// Calculate the numeric datetime for the first trade
	struct tm tm = {0};
	if (!parse_datetime(trade_data[0].timestamp, &tm))
	{
		printf("Error parsing datetime\n");
		return NULL;
	}
	time_t t = mktime(&tm);

	// Initialise the bucket size and first bucket
	time_t bucket_size = granularity_minutes * 60;
	time_t bucket_start = bucket_time(t, granularity_minutes);
	time_t bucket_end = bucket_start + bucket_size;

	// Initialise the trade and feature count
	int trade_count = 0;
	int n_features = 0;

	// Initialise variables for ohlcv calculations
	double open = -1;
	double high = DBL_MIN;
	double low = DBL_MAX;
	double close = -1;
	double volume_sum = 0;

	// Store the index of the earliest trade in the current bucket
	int ind = 0;

	for (int i = 0; i < in_nrows; i++)
	{
		struct tm tm = {0};
		if (!parse_datetime(trade_data[i].timestamp, &tm))
		{
			printf("Error parsing datetime\n");
			return NULL;
		}
		time_t t = mktime(&tm);

		// Advance the current bucket until it encompasses the current trade
		while (t >= bucket_end)
		{
			// If there have been any trades in the current bucket
			if (trade_count > 0)
			{
				// Assign ohlcv variables
				table[n_features].time_start = bucket_start;
				table[n_features].time_end = bucket_end;
				table[n_features].volume = volume_sum;
				table[n_features].close = close;
				table[n_features].high = high;
				table[n_features].low = low;
				table[n_features].open = open;
				table[n_features].high = high;
				table[n_features].low = low;
				table[n_features].close = close;
				table[n_features].volume = volume_sum;

				// Calculate inner bucket statistics
				moments(&trade_data[ind], &table[n_features], (i - ind));
				
				// Increase n_features by 1 and reset the trade_count
				n_features++;
				trade_count = 0;

			}
			
			// Reset ohlcv variables
			open = -1;
			high = DBL_MIN;
			low = DBL_MAX;
			volume_sum = 0;

			// Update the earliest trade index of the new bucket
			ind = i;

			// Shift bucket forward by 1 timestep
			bucket_start = bucket_end;
			bucket_end = bucket_start + bucket_size;
		}

		// Check if this is the first trade in a bucket
		if (open == -1)
		{
			open = trade_data[i].price;
		}

		// Check if the price falls above or below the current high or low
		if (trade_data[i].price > high)
		{
			high = trade_data[i].price;
		}
		if (trade_data[i].price < low)
		{
			low = trade_data[i].price;
		}

		// Assign close to the current price (the last update will be used for the close)
		close = trade_data[i].price;

		// Update the volume of the current bucket
		volume_sum += trade_data[i].volume;

		trade_count++;
	}
	*out_nrows = n_features;

	return table;
}
	

