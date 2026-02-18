/*=================================================================================
 * Trading project developed by Kieran Lang, started 19/01/2026
 * Given a set of price data, generate a trading signal using statistical patterns
 * ================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"
#include "agg.h"
#include "features.h"

int main()
{
	/* First read data from a csv using function */
	size_t nrows; // The number of rows in the csv will be stored in this variable
	trade *trade_data = read_csv("input/simulated_price.csv", &nrows);

	if (!trade_data)
	{
		fprintf(stderr, "Failed to read CSV\n");
		return EXIT_FAILURE;
	} else
		printf("Successfully retrieved %zu trades\n", nrows);

	/* Aggregate the data in OHLCV with granularity defined in parameters.h */
	feature *feature_data = agg_trades(trade_data, nrows, &nrows);

	if (!feature_data)
	{
		printf("Failed to calculate feature data\n");
		return EXIT_FAILURE;
	} else
		printf("Successfully calculated %zu features\n", nrows);

	/* Use the aggregated OHLCV data to create feature data */
	create_features(feature_data, nrows);

	int result = save_data("output/feature_data.csv", feature_data, nrows);

	free(feature_data);
	free(trade_data);
	return EXIT_SUCCESS;
}
