/*
 * Test for YACSVL
 */

#include <stdio.h>

#include "lib/yacsvl.h"

int main(int argc, char **argv)
{
	CSV *csv = yacsvl_malloc_from_file("resources/test01.csv",',');
	printf("Rows: %d\tColumns: %d\n", csv->rows, csv->cols);
	yacsvl_print_pretty(csv);
	yacsvl_free(csv);

	csv = yacsvl_malloc_from_file("resources/test02.csv",',');
	yacsvl_print_csv(csv);
	yacsvl_free(csv);

	return 0;
}
