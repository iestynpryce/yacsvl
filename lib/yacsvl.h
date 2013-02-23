/* Header for YACSVL */
#ifndef YACSVL_H
#define YACSVL_H


typedef struct {
	int      rows;
	int      cols;
	char	 delimiter;
	double **data;
} CSV;

/* Get a value from a specific cell */
double yacsvl_get_value(int row, int column, CSV *csv);

/* Get a value on a specific cell */
void yacsvl_set_value(int row, int column, CSV *csv, double value);

/* Create a CSV from a file */
CSV *yacsvl_malloc_from_file(char* filename, char delimiter);

/* Free memory allocated for the CSV */
void yacsvl_free(CSV *csv);

/* Tab delimited output of the CSV contents */
void yacsvl_print_pretty(CSV* csv);

/* Data formatted as CSV on stdout */
void yacsvl_print_csv(CSV* csv);

/* Output csv data to a file */
void yacsvl_write_csv(char* filename, CSV* csv);

#endif /* YACSVL_H */
