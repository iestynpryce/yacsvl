/* Implementation of the csv library 		*
 * 						*
 * Iestyn Pryce					*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "yacsvl.h"

#define HEADER_TEXT_LEN 256

void _yacsvl_ingest_data_file(FILE *fp, CSV *csv, bool has_header);
/* Check boundaries and error if missed */
void _yacsvl_check_boundaries_v(CSV *csv, size_t r, size_t c);
/* Check boundaries and return boolean */
bool _yacsvl_check_boundaries_b(CSV *csv, size_t r, size_t c);

/* Get a value from a specific cell */
double yacsvl_get_value(CSV* csv, size_t r, size_t c)
{
	/* Check boundaries */
	_yacsvl_check_boundaries_v(csv,r, c);
	return csv->data[r][c];
}

/* Set a value for a specific cell */
void yacsvl_set_value(CSV *csv, size_t r, size_t c, double value)
{
	_yacsvl_check_boundaries_v(csv,r, c);
	csv->data[r][c] = value;
}

/* Get delimiter value */
char yacsvl_get_delimiter(CSV *csv)
{
	return csv->delimiter;
}

/* Set delimiter value */
void yacsvl_set_delimiter(CSV *csv, char delimiter)
{
	csv->delimiter = delimiter;
}

/* Create a new csv */
CSV *yacsvl_malloc(size_t rows, size_t columns, char delimiter,bool has_header)
{
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

	/* Create a header matrix if we have a header */
	char **header = NULL;
	if (has_header)
	{
		header = malloc(columns*sizeof(char*));
		if (header == NULL)
		{
			fprintf(stderr, "Failed to allocate memory\n");
			exit(2);
		}
	}
	csv->header = header;

	return csv;
}

/* Create a new csv from a given file name */
CSV *yacsvl_malloc_from_file(char* filename, char delimiter,bool header)
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

	/* If we have a header line then 'rows' will be one too many.
	 * Correct this by subtracting one. */
	if (header)
	{
		rows--;
	}

	CSV *csv = yacsvl_malloc(rows,columns,delimiter,header);
	_yacsvl_ingest_data_file(fp,csv,header);

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

	/* Print header */
	if (csv->header != NULL)
	{
		for (i=0; i <csv->cols; i++)
		{
			printf("%s\t",csv->header[i]);
		}
		printf("\n");
	}

	/* Print data */
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

/* Copy CSV type into a GSL matrix type */
gsl_matrix *yacsvl_copy_to_gsl_matrix(CSV *csv)
{
	int i=0;
	int j=0;

	gsl_matrix *m = gsl_matrix_alloc(csv->rows,csv->cols);

	for(i=0; i<csv->rows; i++)
	{
		for(j=0; j<csv->cols; j++)
		{
			gsl_matrix_set(m,i,j,yacsvl_get_value(csv,i,j));
		}
	}

	return m;
}

/* Copy CSV type from a GSL matrix type */
CSV *yacsvl_copy_from_gsl_matrix(gsl_matrix *m, char delim)
{
	int i = 0;
	int j = 0;

	/* Assume we can have a header */
	bool has_header = true;

	CSV *csv = yacsvl_malloc(m->size1, m->size2,delim,has_header);
	if (csv == NULL)
	{
		fprintf(stderr,"Out of memory\n");
		exit(3);
	}

	for (i=0; i<csv->rows; i++)
	{
		for (j=0; j<csv->cols; j++)
		{
			yacsvl_set_value(csv,i,j,gsl_matrix_get(m,i,j));
		}
	}

	return csv;
}

/* Puts the csv data from a file into CSV data structure */
void _yacsvl_ingest_data_file(FILE *fp, CSV *csv, bool has_header)
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

	/* Skip first line if header */
	if (has_header)
	{
		char text[HEADER_TEXT_LEN];
		int counter = 0;
		int header_column = 0;

		while((c=fgetc(fp))) 
		{
			/* Ingest header */
			if (c != csv->delimiter && c != '\n')
			{
				if (counter < HEADER_TEXT_LEN) 
				{
					text[counter] = c;
					counter++;
				}
			}
			else
			{
				char *header = malloc(HEADER_TEXT_LEN*sizeof(char));
				if (header == NULL)
				{
					fprintf(stderr,"Failed to allocate memory\n");
					exit(2);
				}
				text[counter] = '\0';
				strcpy(header,text);
				csv->header[header_column] = header;

				counter = 0;
				header_column++;
			}

			/* Break if we've reached the EOL */
			if ( c == '\n') 
			{
				break;
			}
		}
	}


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
void _yacsvl_check_boundaries_v(CSV* csv, size_t r, size_t c)
{
	if ( !( r < csv->rows) || !( c < csv->cols))
	{
		fprintf(stderr,"Index [%ld,%ld] out of bounds, CSV size is [%ld,%ld]\n", 
				r, c, csv->rows-1, csv->cols-1);
		exit(3);
	}
}

/* Check boundaries and return boolean */
bool _yacsvl_check_boundaries_b(CSV* csv, size_t r, size_t c)
{
	if ( !( r < csv->rows) || !( c < csv->cols))
	{
		fprintf(stderr,"Index [%ld,%ld] out of bounds, CSV size is [%ld,%ld]\n", 
				r, c, csv->rows-1, csv->cols-1);
		return false;
	}
	return true;
}
