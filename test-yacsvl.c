/*
 * Test for YACSVL
 */

#include <stdio.h>

#include "lib/yacsvl.h"

int main(int argc, char **argv)
{
	CSV *csv = yacsvl_malloc_from_file("resources/test01.csv",',');
	printf("Rows: %ld\tColumns: %ld\n", csv->rows, csv->cols);
	yacsvl_print_pretty(csv);
	yacsvl_free(csv);

	csv = yacsvl_malloc_from_file("resources/test02.csv",',');
	printf("[%d, %d] = %lf\n",0,0,yacsvl_get_value(csv,0,0));
	printf("Change  value: ");
	yacsvl_set_value(csv,0,0,10.2);
	printf("[%d, %d] = %lf\n",0,0,yacsvl_get_value(csv,0,0));

	printf("Delimiter: %c\n",yacsvl_get_delimiter(csv));
	yacsvl_set_delimiter(csv,'|');

	/* test writing out the data */
	yacsvl_write_csv("output.csv",csv);

	yacsvl_print_csv(csv);

	/* copy to gsl matrix */
	gsl_matrix *mat = yacsvl_copy_to_gsl_matrix(csv);
	if (mat != NULL)
	{
		printf("gsl_matrix created\n");
	}
	else
	{
		printf("ERROR: gsl_matrix not created");
	}
	gsl_matrix_free(mat);

	yacsvl_free(csv);

	return 0;
}
