/* Header for YACSVL */
#ifndef YACSVL_H
#define YACSVL_H


typedef struct {
	int      rows;
	int      cols;
	char	 delimiter;
	double **data;
} CSV;

CSV *yacsvl_malloc_from_file(char* filename, char delimiter);

void yacsvl_free(CSV *csv);

void yacsvl_print_pretty(CSV* csv);

void yacsvl_print_csv(CSV* csv);

#endif /* YACSVL_H */
