/* Header for YACSVL */
#ifndef YACSVL_H
#define YACSVL_H

#include <gsl/gsl_matrix.h>
#include <stdbool.h>

typedef struct {
	size_t   rows;
	size_t   cols;
	char	 delimiter;
	char   **header;
	double **data;
} CSV;

/* Get a value from a specific cell */
double yacsvl_get_value(CSV *csv, size_t row, size_t column);

/* Get a value on a specific cell */
void yacsvl_set_value(CSV *csv, size_t row, size_t column, double value);

/* Get delimiter value */
char yacsvl_get_delimiter(CSV *csv);

/* Set delimiter value */
void yacsvl_set_delimiter(CSV *csv, char delimiter);

/* Create a csv type */
CSV *yacsvl_malloc(size_t rows, size_t columns, char delimiter, bool header);

/* Create a CSV from a file */
CSV *yacsvl_malloc_from_file(char* filename, char delimiter, bool header);

/* Free memory allocated for the CSV */
void yacsvl_free(CSV *csv);

/* Tab delimited output of the CSV contents */
void yacsvl_print_pretty(CSV* csv);

/* Data formatted as CSV on stdout */
void yacsvl_print_csv(CSV* csv);

/* Output csv data to a file */
void yacsvl_write_csv(char* filename, CSV* csv);

/* Copy contents of a CSV to gsl matrix */
gsl_matrix *yacsvl_copy_to_gsl_matrix(CSV *csv);

/* Copy contents of a gsl_matrix to CSV */
CSV *yacsvl_copy_from_gsl_matrix(gsl_matrix *m, char delim);

#endif /* YACSVL_H */
