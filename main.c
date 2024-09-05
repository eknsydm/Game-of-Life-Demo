#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define CELL 'o'
#define BACKGROUND '-'

#define SPEED .1
#define HEIGHT 20
#define WIDTH 100
#define DENSITY 20

typedef enum
{
    DEAD,
    ALIVE,
} State;

typedef struct
{
    State state;
} Cell;

void init_grid(Cell *grid, size_t height, size_t width, int density)
{
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            if (rand() % 100 < density)
            {
                grid[i * width + j].state = ALIVE;
                continue;
            }
            grid[i * width + j].state = DEAD;
        }
    }
}

void gen_next(Cell *grid, size_t height, size_t width)
{
    Cell *new_grid = (Cell *)malloc(height * width * sizeof(Cell));

    memcpy(new_grid, grid, sizeof(Cell) * height * width);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t alive_count = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (k == 0 && l == 0)
                        continue;

                    int row = (i + k + height) % height;
                    int col = (j + l + width) % width;

                    if (grid[row * width + col].state == ALIVE)
                    {
                        alive_count++;
                    }
                }
            }
            switch (alive_count)
            {
            case 0:
            case 1:
                new_grid[i * width + j].state = DEAD;
                break;
            case 2:
            case 3:
                if (alive_count == 3)
                    new_grid[i * width + j].state = ALIVE;
                break;
            default:
                new_grid[i * width + j].state = DEAD;
                break;
            }
        }
    }
    memcpy(grid, new_grid, sizeof(Cell) * height * width);
    free(new_grid);
}

int print_grid(Cell *grid, size_t height, size_t width)
{
    int alive_count = 0;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            switch (grid[i * width + j].state)
            {
            case ALIVE:
                alive_count++;
                putc(CELL, stdout);
                break;
            case DEAD:
                putc(BACKGROUND, stdout);
                break;
            }
        }
        putc('\n', stdout);
    }
    return alive_count;
}

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_title()
{
    clear_screen();
    printf("\n\n\n\n");
    printf("===============================================\n");
    printf("       Conway's Game of Life Setup\n");
    printf("===============================================\n\n");
}

void setup(size_t *height, size_t *width, size_t *density)
{
    char buffer[10];

    print_title();

    printf("Enter the height of the grid (default: %zu):\n", *height);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (strlen(buffer) > 1)
        {
            if (sscanf(buffer, "%zu", height) != 1)
            {
                printf("Error: Invalid input for height\n");
                exit(1);
            }
        }
    }
    else
    {
        printf("Error reading input\n");
        exit(1);
    }

    print_title();
    printf("Enter the width of the grid (default: %zu):\n", *width);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (strlen(buffer) > 1)
        {
            if (sscanf(buffer, "%zu", width) != 1)
            {
                printf("Error: Invalid input for width\n");
                exit(1);
            }
        }
    }
    else
    {
        printf("Error reading input\n");
        exit(1);
    }

    print_title();

    printf("Enter the density of the grid (default: %zu):\n", *density);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (strlen(buffer) > 1)
        {
            if (sscanf(buffer, "%zu", density) != 1)
            {
                printf("Error: Invalid input for density\n");
                exit(1);
            }
        }
    }
    else
    {
        printf("Error reading input\n");
        exit(1);
    }

    print_title();
    printf("Setup complete! Press Enter to continue...\n");
    fgets(buffer, sizeof(buffer), stdin);
}

int main(int argc, char **argv)
{
    size_t height = HEIGHT, width = WIDTH, density = DENSITY;

    srand(time(NULL));
    setup(&height, &width, &density);

    Cell *grid = (Cell *)malloc(height * width * sizeof(Cell));
    init_grid(grid, height, width, density);

    while (print_grid(grid, height, width) != 0)
    {
        Sleep(1000 * SPEED);
        gen_next(grid, height, width);
        system("cls");
    }
    return 0;
}