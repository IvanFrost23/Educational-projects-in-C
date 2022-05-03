#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ERROR_NOT_ENOUGH_MEMORY 1;

/*
    Функция, которая возвращает значение выражения (-1)^st
    Если st - четное, то функция вернет 1
    Если st - нечетное, то функция вернет 0
*/
int pow_one(int st)
{
    return (st % 2 == 0 ? 1 : -1);
}

/*
    Это рекурсивная функция, которая считает определитель разложением по 1-ой строке
    ans = (-1)^(0 + 0) * matrix[0][0] * M[0][0] + (-1)^(0 + 1) * matrix[0][1] * M[0][1] + ...
    , где M[i][j] - это минор элемента, который находится на пересечение i - ой строки и j - ого столбца.
*/
double count_determinant(int number_of_equations, double** matrix)
{
    if (number_of_equations == 1) // Условие выхода из рекурсии
    {
        return matrix[0][0];
    }
    double answer = 0; // Переменная, которая накапливает ответ
    for (int i = 0; i < number_of_equations; i++)
    {
        // Создаем матрицу-минор для элемента (0; i). Пытаемся выделить под нее память.
        double** new_matrix = NULL;
        new_matrix = (double**)malloc((number_of_equations - 1) * sizeof(double));
        if (new_matrix == NULL)
        {
            // Недостоточно памяти для выделения матрицы
            free(new_matrix);
            printf("Nedostatocno pamyti\n");
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        for (int j = 1; j < number_of_equations; j++)
        {
            new_matrix[j - 1] = NULL;
            new_matrix[j - 1] = (double*)malloc((number_of_equations - 1) * sizeof(double));
            if (new_matrix[j - 1] == NULL)
            {
                //Недостаточно памяти для выделения строки матрицы
                for (int jj = 0; jj < j; jj++)
                {
                    free(new_matrix[jj]);
                }
                free(new_matrix);
                printf("Nedostatocho pamyti\n");
                return ERROR_NOT_ENOUGH_MEMORY;
            }
            int ind = 0;
            for (int k = 0; k < number_of_equations; k++)
            {
                if (k == i) continue;
                new_matrix[j - 1][ind++] = matrix[j][k];
            }
        }
        answer += pow_one(i + 0) * matrix[0][i] * count_determinant(number_of_equations - 1, new_matrix);

        // Освобождаем память. Удаляем матрицу-минор.
        for (int i = 0; i < number_of_equations - 1; i++)
        {
            free(new_matrix[i]);
        }
        free(new_matrix);
    }
    return answer;
}

/*
    Эта функция, копирует матрицу matrix2 в matrix. То есть после
    выполнения этой функции matrix и matrix2 полностью совпадают.
*/
void update_matrix(int number_of_equations, double** matrix, double** matrix2)
{
    for (int i = 0; i < number_of_equations; i++)
    {
        for (int j = 0; j < number_of_equations; j++)
        {
            matrix[i][j] = matrix2[i][j];
        }
    }
}

/*
    Эта функция меняет столбец с номером column в матрице matrix на последний столбец matrix2.
    В терминологии метода Крамера, она заменяет поочердно каждый столбец на столбец значений.
*/
void change_column(int number_of_equations, double** matrix, double** matrix2, int column)
{
    for (int i = 0; i < number_of_equations; i++)
    {
        matrix[i][column] = matrix2[i][number_of_equations];
    }
}

int main()
{
    int number_of_equations; // Количество уравнений в системе

    // Считываем входные данные
    printf("Vvedite kolichestvo uravnenii v sisteme\n");
    scanf("%d", &number_of_equations);
    printf("Vvedite matricu\n");

    //Пытаемся выделить память под матрицу и ее копию

    double** matrix = NULL;
    matrix = (double**)malloc(number_of_equations * sizeof(double));
    double** changed_matrix = NULL;
    changed_matrix = (double**)malloc(number_of_equations * sizeof(double));
    if (matrix == NULL)
    {
        // Недостоточно памяти для выделения матрицы
        free(matrix);
        printf("Nedostatocno pamyti\n");
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    if (changed_matrix == NULL)
    {
        // Недостоточно памяти для выделения матрицы
        free(changed_matrix);
        printf("Nedostatocno pamyti\n");
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    for (int i = 0; i < number_of_equations; i++)
    {
        //Пытаемся выделить память под строку матрицы
        matrix[i] = NULL;
        matrix[i] = (double*)malloc((number_of_equations + 1) * sizeof(double));
        changed_matrix[i] = NULL;
        changed_matrix[i] = (double*)malloc((number_of_equations + 1) * sizeof(double));
        if (matrix[i] == NULL)
        {
            //Недостаточно памяти для выделения строки матрицы
            for (int j = 0; j <= i; j++)
            {
                free(matrix[j]);
            }
            free(matrix);
            printf("Nedostatocho pamyti\n");
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        if (changed_matrix[i] == NULL)
        {
            //Недостаточно памяти для выделения строки матрицы
            for (int j = 0; j <= i; j++)
            {
                free(changed_matrix[j]);
            }
            free(changed_matrix);
            printf("Nedostatocho pamyti\n");
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        for (int j = 0; j < number_of_equations + 1; j++)
        {
            printf("Vvedite element A(%d; %d)\n", i + 1, j + 1);
            scanf("%lf", &matrix[i][j]);
            changed_matrix[i][j] = matrix[i][j];
        }
    }

    double main_determinant = count_determinant(number_of_equations, matrix); // Считаем определитель исходной матрицы
    printf("Opredelitel matrici det = %lf\n", main_determinant);

    double* ans = NULL;
    ans = (double*)malloc(number_of_equations * sizeof(double));

    if (ans == NULL)
    {
        // Недостоточно памяти для выделения матрицы
        free(ans);
        printf("Nedostatocno pamyti\n");
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    //Решение СЛАУ методом Крамера
    int is_all_zeroes = 1; // Переменная, которая определяет все ли найденные определители являются нулями
    for (int i = 0; i < number_of_equations; i++)
    {
        if (fabs(main_determinant) == 0)
        {
            if (fabs(count_determinant(number_of_equations, changed_matrix) / main_determinant) != 0)
            {
                is_all_zeroes = 0;
            }
            continue;
        }
        change_column(number_of_equations, changed_matrix, matrix, i); // заменяем i-й столбец на столбец значений
        double det = count_determinant(number_of_equations, changed_matrix);
        printf("Opredelitel peremennoy det(x_%d) = %lf\n", i + 1, det);
        double x = det / main_determinant;
        ans[i] = x;
        update_matrix(number_of_equations, changed_matrix, matrix); // Возвращаем матрицу к исходному виду
    }

    printf("Reshenie systemi uravnenii\n");

    // Здесь обрабатываем случай, когда определитель исходной матрицы равен 0.
    if (fabs(main_determinant) == 0)
    {
        if (is_all_zeroes)
        {
            printf("Systema uravnenii imeet besconechno mnogo resenii\n");
        }
        else
        {
            printf("Systema uravnenii ne imeet resenii\n");
        }
    }
    else
    {
        for (int i = 0; i < number_of_equations; i++)
        {
            printf("x_%d = %lf\n", i + 1, ans[i]); // Выводим значение x_i
        }
    }
    return 0;
}
