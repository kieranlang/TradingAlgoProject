#ifndef STRUCTS_H

#define STRUCTS_H

typedef struct {
	int id;
	char timestamp[20];
	double price;
	double volume;
} trade;

typedef struct {
	time_t time_start;
	time_t time_end;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double mean;
	double var;
	double skew;
	double kurt;
	double period_return;
	double sma;
} feature;

#endif
