/* ================================================================== /
 * Collection of functions used to calculate features from OHLCV data /
 * ================================================================= */
#include <stdlib.h>
#include "structs.h"
#include "parameters.h"
#include "features.h"
#include <stdio.h>

// Function to calculate the simple moving average using the span defined in the parameters.h file
void calc_sma(feature *feature_data, size_t in_nrows)
{
	// Pre-compute the inverse of the sma span
	double inv_span = 1 / (double)sma_span;

	// Initialise the sma
	double sma = 0;

	// Calculate the initial value of the sma
	for (size_t f = 0; f < sma_span; f++)
	{
		sma += feature_data[f].close;
		feature_data[f].sma = sma * inv_span;
	}
	// Assign the value of the sma
	feature_data[sma_span-1].sma = sma * inv_span;

	// Iterate through the remaining features in the feature data
	for (size_t f = sma_span; f < in_nrows; f++)
	{
		// Subtract the close price that is no longer include in the span
		sma -= feature_data[f - sma_span].close;

		// Add the new close price
		sma += feature_data[f].close;

		// Assign the value of the sma
		feature_data[f].sma = sma * inv_span;
	}
}

void calc_return(feature *feature_data, size_t in_nrows)
{
	// Iterate through the feature data and calculate the percentage return between open and close
	for (size_t f = 0; f < in_nrows; f++)
	{
		feature_data[f].period_return = 100 * (feature_data[f].close - feature_data[f].open) / feature_data[f].open;
	}
}

void create_features(feature *ohlcv_data, size_t in_nrows)
{

	// Calculate the sma using the function
	calc_sma(ohlcv_data, in_nrows);
	calc_return(ohlcv_data, in_nrows);

}


