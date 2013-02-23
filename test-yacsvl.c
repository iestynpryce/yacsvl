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
	printf("[%d, %d] = %lf\n",0,0,yacsvl_get_value(0,0,csv));
	printf("Change  value: ");
	yacsvl_set_value(0,0,csv,10.2);
	printf("[%d, %d] = %lf\n",0,0,yacsvl_get_value(0,0,csv));

	/* test writing out the data */
	yacsvl_write_csv("output.csv",csv);

	yacsvl_print_csv(csv);
	yacsvl_free(csv);

	return 0;
}
