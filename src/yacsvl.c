/* Implementation of the csv library 		*
 * 						*
 * Iestyn Pryce					*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "yacsvl.h"

void _yacsvl_ingest_data_file(FILE *fp, CSV *csv);
/* Check boundaries and error if missed */
void _yacsvl_check_boundaries_v(int r, int c, CSV *csv);
/* Check boundaries and return boolean */
void _yacsvl_check_boundaries_b(int r, int c, CSV *csv);

/* Get a value from a specific cell */
double yacsvl_get_value(int r, int c, CSV *csv)
{
	/* Check boundaries */
	_yacsvl_check_boundaries_v(r, c, csv);
	return csv->data[r][c];
}

/* Set a value for a specific cell */
void yacsvl_set_value(int r, int c, CSV *csv, double value)
{
	_yacsvl_check_boundaries_v(r, c, csv);
	csv->data[r][c] = value;
}

/* Create a new csv from a given file name */
CSV *yacsvl_malloc_from_file(char* filename, char delimiter)
{
	/* Open the file */
	FILE *fp = NULL;
	fp = fopen(filename,"rb");
	if (fp == NULL)
	{
		fprintf(stderr,"Failed to open file: %s\n",filename);
		exit(1); // need a better way than this, so things get cleaned up correctly
	}

	int rows = 0;
	int columns_temp = 0;
	int columns = 0;
	char c;
	/* Read the file collecting the data for number of rows and columns */
	while((c=fgetc(fp)) != EOF) {
		if (c == delimiter)
		{
			columns_temp++;
		}
		if (c == '\n')
		{
			columns_temp++;
			rows++;
			if (columns_temp > columns)
			{
				columns = columns_temp;
			}
			columns_temp = 0;
		}
	}

	/* Create the CSV struct */
	CSV *csv = calloc(1,sizeof(CSV));
	if (csv == NULL)
	{
		fprintf(stderr,"Failed to allocate memory\n");
		exit(2);
	}
	csv->rows = rows;
	csv->cols = columns;
	csv->delimiter = delimiter;

	/* Create the data 'matrix' for the csv */
	double **data = calloc(rows,sizeof(double*)); 
	int i;
	for (i=0; i<rows; i++)
	{
		data[i] = calloc(columns,sizeof(double));
		if (data[i] == NULL)
		{
			fprintf(stderr,"Failed to allocate memory\n");
			exit(2);
		}
	}
	csv->data = data;
	_yacsvl_ingest_data_file(fp,csv);

	/* Close the file handle */
	fclose(fp);

	return csv;
}

void yacsvl_free(CSV *csv)
{
	/* Free the data: */
	int i;
	for(i=0; i < csv->rows; i++)
	{
		free(csv->data[i]);
	}

	/* Then free the CSV struct itself */
	free(csv);
}

void yacsvl_print_pretty(CSV* csv)
{
	int i = 0;
	int j = 0;

	for (i=0; i < csv->rows; i++)
	{
		for (j = 0; j < csv->cols; j++)
		{
			printf("%lf\t",csv->data[i][j]);
		}
		printf("\n");
	}
}

/* print to stdout the data in csv format */
void yacsvl_print_csv(CSV* csv)
{
	int i = 0;
	int j = 0;

	for (i=0; i < csv->rows; i++)
	{
		for (j = 0; j < csv->cols; j++)
		{
			printf("%lf",csv->data[i][j]);
			printf("%c",(j+1==csv->cols) ? '\n' : csv->delimiter);
		}
	}
}

/* write data to file in csv format */
void yacsvl_write_csv(char* filename, CSV* csv)
{
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		fprintf(stderr,"Failed to open file: %s\n",filename);
		exit(1);
	}

	int i = 0;
	int j = 0;
	for (i=0; i<csv->rows;i++)
	{
		for(j=0; j<csv->cols;j++)
		{
			fprintf(fp,"%lf",csv->data[i][j]);
			fprintf(fp,"%c",(j+1==csv->cols) ? '\n' : csv->delimiter);
		}
	}
	fclose(fp);
}

/* Puts the csv data from a file into CSV data structure */
void _yacsvl_ingest_data_file(FILE *fp, CSV *csv)
{
	/* Ensure that we are at the start of the file*/
	rewind(fp);

	double d = 0.0;
	char c;
	int i = 0; /* Row counter */
	int j = 0; /* Column counter */

	bool decimal = false;
	int  cdecimal = 1;
	int  sign = 1;

	while ((c=fgetc(fp))!=EOF)
	{
		if (c == csv->delimiter || c == '\n')
		{
			csv->data[i][j] = sign * d/cdecimal;
			d = 0;
			cdecimal = 1;
			decimal = false;
			sign = 1;

			j++;
		}
		if (c == '\n')
		{
			i++;
			j = 0;
			continue;
		}
		if (c >= '0' && c <= '9')
		{
			d = d*10 + (c-'0');
			if (decimal)
			{
				cdecimal*=10;
			}
			continue;
		}
		if (c == '.')
		{
			decimal = true;
		}
		if (c == '-')
		{
			sign = -1;
		}

	}
}

/* Check boundaries and error if missed */
void _yacsvl_check_boundaries_v(int r, int c, CSV *csv)
{
	if ( !( r < csv->rows) || !( c < csv->cols))
	{
		fprintf(stderr,"Index [%d,%d] out of bounds, CSV size is [%d,%d]\n", 
				r, c, csv->rows-1, csv->cols-1);
		exit(3);
	}
}
