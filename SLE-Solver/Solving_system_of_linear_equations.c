#include "return_codes.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const float eps = 1e-5;

int count_matrix_rank(int n, int m, float** matrix)
{
	for (int i = 0; i < n; i++)
	{
		if (fabs(matrix[i][i]) <= eps)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (fabs(matrix[j][i]) > eps)
				{
					for (int k = 0; k < m; k++)
					{
						float t = matrix[i][k];
						matrix[i][k] = matrix[j][k];
						matrix[j][k] = t;
					}
					break;
				}
			}
			if (fabs(matrix[i][i]) <= eps)
			{
				continue;
			}
		}

		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				continue;
			}

			float coef = -(matrix[j][i] / matrix[i][i]);

			for (int k = 0; k < m; k++)
			{
				matrix[j][k] += coef * matrix[i][k];
			}
		}
	}

	int rank = 0;

	for (int i = 0; i < n; i++)
	{
		_Bool f = true;

		for (int j = 0; j < m; j++)
		{
			if (fabs(matrix[i][j]) > eps)
			{
				f = false;
				break;
			}
		}

		rank += 1 - f;
	}

	return rank;
}

int solve_SLE(int n, int m, float** matrix, FILE* out)
{
	float* ans = NULL;

	ans = (float*)malloc(n * sizeof(float));

	if (ans == NULL)
	{
		free(ans);
		return -1;
	}

	int q = m - 2;

	for (int i = 0; i < n; i++)
	{
		ans[i] = 0;
	}

	for (int i = n - 1; i >= 0; i--)
	{
		float sum = 0;
		for (int j = 0; j < m - 1; j++)
		{
			if (j == q)
			{
				continue;
			}
			sum += matrix[i][j] * ans[j];
		}
		ans[i] = (matrix[i][m - 1] - sum) / matrix[i][q];
		q--;
	}

	for (int i = 0; i < n; i++)
	{
		fprintf(out, "%g\n", ans[i]);
	}

	free(ans);

	return 0;
}

void make_copy(int n, int m, float** a, float** b)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			a[i][j] = b[i][j];
		}
	}
	return;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Incorrect number of arguments!\nCorrect format: Source.exe <input file name> <output file name>");
		return ERROR_INVALID_PARAMETER;
	}

	FILE* in = fopen(argv[1], "r");

	if (in == NULL)
	{
		printf("File %s is not opened", argv[1]);
		return ERROR_FILE_NOT_FOUND;
	}

	int n;

	fscanf(in, "%d", &n);

	float** matrix = NULL;
	matrix = (float**)malloc(n * sizeof(float*));
	if (matrix == NULL)
	{
		fclose(in);
		free(matrix);
		printf("Not enough memory!");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	float** data = NULL;
	data = (float**)malloc(n * sizeof(float*));
	if (data == NULL)
	{
		fclose(in);
		free(data);
		printf("Not enough memory!");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	for (int i = 0; i < n; i++)
	{
		matrix[i] = NULL;
		matrix[i] = (float*)malloc((n + 1) * sizeof(float));
		if (matrix[i] == NULL)
		{
			for (int j = 0; j <= i; j++)
			{
				free(matrix[j]);
			}
			for (int j = 0; j < i; j++)
			{
				free(data[j]);
			}
			free(matrix);
			free(data);
			fclose(in);
			printf("Not enough memory!");
			return ERROR_NOT_ENOUGH_MEMORY;
		}

		data[i] = NULL;
		data[i] = (float*)malloc((n + 1) * sizeof(float));
		if (data[i] == NULL)
		{
			for (int j = 0; j <= i; j++)
			{
				free(matrix[j]);
			}
			for (int j = 0; j <= i; j++)
			{
				free(data[j]);
			}
			free(matrix);
			free(data);
			fclose(in);
			printf("Not enough memory!");
			return ERROR_NOT_ENOUGH_MEMORY;
		}

		for (int j = 0; j < n + 1; j++)
		{
			fscanf(in, "%f", &matrix[i][j]);
			data[i][j] = matrix[i][j];
		}
	}

	fclose(in);

	int rankMainMatrix = count_matrix_rank(n, n, matrix);

	make_copy(n, n + 1, matrix, data);

	for (int i = 0; i < n; i++)
	{
		free(data[i]);
	}

	free(data);

	int rankExtendedMatrix = count_matrix_rank(n, n + 1, matrix);

	FILE* out = fopen(argv[2], "w");

	if (out == NULL)
	{
		for (int i = 0; i < n; i++)
		{
			free(matrix[i]);
		}
		free(matrix);
		printf("File %s is not opened", argv[2]);
		return ERROR_FILE_NOT_FOUND;
	}

	if (rankMainMatrix != rankExtendedMatrix)
	{
		fprintf(out, "no solution");
	}
	else
	{
		if (rankMainMatrix < n)
		{
			fprintf(out, "many solutions");
		}

		if (rankMainMatrix == n)
		{
			int check = solve_SLE(n, n + 1, matrix, out);

			if (check == -1)
			{
				fclose(out);
				for (int i = 0; i < n; i++)
				{
					free(matrix[i]);
				}
				free(matrix);
				printf("Not enough memory!");
				return ERROR_NOT_ENOUGH_MEMORY;
			}
		}
	}

	fclose(out);

	for (int i = 0; i < n; i++)
	{
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}
