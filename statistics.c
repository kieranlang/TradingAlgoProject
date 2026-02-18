/*=============================================================================== /
 * File containing all functions necessary for calculating statistical indicators /
 * ==============================================================================*/
#include <math.h>
#include <stdio.h>
#include "structs.h"
#include "statistics.h"

void moments(trade *trade_data, feature *bucket, int n_trades)
{
    /* 
    Function for calculating the standardized moments of a trade bucket 
    trade_data: the pointer to the earliest trade in the bucket
    n_trades: the number of trades in the bucket
    bucket: the feature of the current bucket
    */

    // Initialise the moments of the bucket
    bucket[0].mean = 0;
    bucket[0].var = 0;
    bucket[0].skew = 0;
    bucket[0].kurt = 0;

    double square_mean = 0; // Average square price of the bucket

    /* Iterate through the trades in the bucket and 
    calculate the sum of the price and the square prices */
    for (int i= 0; i < n_trades; i++)
    {
        // Sum the prices and the square prices
        bucket[0].mean += trade_data[i].price;
        square_mean += trade_data[i].price * trade_data[i].price;
    }

    /* Calculate the mean and the square mean */
    double inv_trades = 1 / (double)n_trades;
    bucket[0].mean = bucket[0].mean * inv_trades;
    square_mean = square_mean * inv_trades;

    // Calculate the variance
    bucket[0].var = (square_mean) - (bucket[0].mean * bucket[0].mean);
    // Calculate the standard deviation
    double std = sqrt(bucket[0].var);

    // Iterate through the trades in the bucket and calculate the skewness and kurtosis
    for (int i = 0; i < n_trades; i++)
    {
        double moment = (trade_data[i].price - bucket[0].mean);
        bucket[0].skew += moment * moment * moment;
        bucket[0].kurt += moment * moment * moment * moment;
    }

    // Calculate the skewness and kurtosis of the bucket
    bucket[0].skew = (bucket[0].skew * inv_trades) / (std * std * std);
    bucket[0].kurt = (bucket[0].kurt * inv_trades) / (std * std * std * std);
    
}
