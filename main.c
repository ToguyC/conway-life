#include "gfx/gfx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL_SIZE 5
#define CELL_COUNT 100
#define SCREEN_WIDTH (CELL_SIZE * CELL_COUNT)
#define SCREEN_HEIGHT (CELL_SIZE * CELL_COUNT)

uint8_t get(uint8_t *grid, uint32_t i, uint32_t j) {
    return grid[j * CELL_COUNT + i];
}

void copy(uint8_t *arr, uint8_t *copy, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        copy[i] = arr[i];
    }
}

void addGlider(uint32_t i, uint32_t j, uint8_t *grid) {
    uint8_t glider[9] = {0, 0, 255, 255, 0, 255, 0, 255, 255};

    for (uint32_t k = i; k < i + 3; k++) {
        for (uint32_t l = j; l < j + 3; l++)
            grid[l * CELL_COUNT + k] = glider[(l - j) * 3 + (k - i)];
    }
}

void add(uint8_t *grid, uint32_t i, uint32_t j, uint8_t val, uint32_t w) {
    grid[j * w + i] = val;
}

void addGliderGun(uint32_t i, uint32_t j, uint8_t *grid) {
    // Marche pas
    uint8_t *gun = malloc(sizeof(uint8_t) * (11 * 38));

    add(gun, 1, 5, 255, 38);
    add(gun, 1, 6, 255, 38);
    add(gun, 2, 5, 255, 38);
    add(gun, 2, 6, 255, 38);

    add(gun, 11, 5, 255, 38);
    add(gun, 11, 6, 255, 38);
    add(gun, 11, 7, 255, 38);
    add(gun, 12, 4, 255, 38);
    add(gun, 12, 8, 255, 38);
    add(gun, 13, 3, 255, 38);
    add(gun, 13, 9, 255, 38);
    add(gun, 14, 3, 255, 38);
    add(gun, 14, 9, 255, 38);

    add(gun, 15, 6, 255, 38);

    add(gun, 16, 4, 255, 38);
    add(gun, 16, 8, 255, 38);
    add(gun, 17, 5, 255, 38);
    add(gun, 17, 6, 255, 38);
    add(gun, 17, 7, 255, 38);
    add(gun, 18, 6, 255, 38);

    add(gun, 21, 3, 255, 38);
    add(gun, 21, 4, 255, 38);
    add(gun, 21, 5, 255, 38);
    add(gun, 22, 3, 255, 38);
    add(gun, 22, 4, 255, 38);
    add(gun, 22, 5, 255, 38);
    add(gun, 23, 2, 255, 38);
    add(gun, 23, 6, 255, 38);

    add(gun, 25, 1, 255, 38);
    add(gun, 25, 2, 255, 38);
    add(gun, 25, 6, 255, 38);
    add(gun, 25, 7, 255, 38);

    add(gun, 35, 3, 255, 38);
    add(gun, 35, 4, 255, 38);
    add(gun, 36, 3, 255, 38);
    add(gun, 36, 4, 255, 38);

    for (uint32_t k = i; k < i + 38; k++) {
        for (uint32_t l = j; l < j + 11; l++) {
            grid[l * CELL_COUNT + k] = gun[(l - j) * 38 + (k - i)];
        }
    }
}

void update(uint8_t *grid, uint32_t N, struct gfx_context_t *ctxt) {
    uint8_t *new_grid = malloc(sizeof(uint8_t) * (N * N));
    copy(grid, new_grid, N * N);

    for (uint8_t i = 0; i < N; i++) {
        for (uint8_t j = 0; j < N; j++) {
            uint32_t total = (uint32_t)(
                (get(grid, i, (j - 1) % N) + get(grid, i, (j + 1) % N) +
                 get(grid, (i - 1) % N, j) + get(grid, (i + 1) % N, j) +
                 get(grid, (i - 1) % N, (j - 1) % N) +
                 get(grid, (i - 1) % N, (j + 1) % N) +
                 get(grid, (i + 1) % N, (j - 1) % N) +
                 get(grid, (i + 1) % N, (j + 1) % N)) /
                255);

            if (grid[j * N + i] == 255) {
                if (total < 2 || total > 3)
                    new_grid[j * N + i] = 0;
            } else {
                if (total == 3)
                    new_grid[j * N + i] = 255;
            }
        }
    }

    for (uint32_t i = 0; i < N * N; i++) {
        grid[i] = new_grid[i];
    }

    free(new_grid);
}

int main(void) {
    struct gfx_context_t *ctxt =
        gfx_create("Conway's game of life", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ctxt) {
        fprintf(stderr, "Graphics initialization failed!\n");
        return EXIT_FAILURE;
    }

    uint8_t cells[CELL_COUNT * CELL_COUNT];

    for (uint8_t i = 0; i < CELL_COUNT; i++) {
        for (uint8_t j = 0; j < CELL_COUNT; j++)
            cells[j * CELL_COUNT + i] = 0;
    }

    SDL_Event event;
    int x, y;
    uint32_t buttons;
    bool run_game = false;
    uint32_t time_ticks = 0;
    uint32_t delta = 100;

    addGlider(10, 10, cells);
    addGliderGun(40, 40, cells);

    while (true) {
        gfx_clear(ctxt, COLOR_BLACK);

        show_grid(ctxt, SCREEN_WIDTH, SCREEN_HEIGHT, CELL_SIZE);

        SDL_PollEvent(&event);
        buttons = SDL_GetMouseState(&x, &y);

        x = (uint32_t)(x / CELL_SIZE);
        y = (uint32_t)(y / CELL_SIZE);

        draw_full_square(ctxt, x, y, CELL_SIZE, COLOR_RED);

        if (run_game && time_ticks % delta == 0)
            update(cells, CELL_COUNT, ctxt);

        for (uint8_t i = 0; i < CELL_COUNT; i++) {
            for (uint8_t j = 0; j < CELL_COUNT; j++) {
                if (cells[j * CELL_COUNT + i] == 255)
                    draw_full_square(ctxt, i, j, CELL_SIZE, COLOR_WHITE);
            }
        }

        if ((buttons & SDL_BUTTON_LMASK) != 0)
            cells[y * CELL_COUNT + x] = 255;

        if ((buttons & SDL_BUTTON_RMASK) != 0)
            cells[y * CELL_COUNT + x] = 0;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_r) {
                for (uint8_t i = 0; i < CELL_COUNT; i++) {
                    for (uint8_t j = 0; j < CELL_COUNT; j++)
                        cells[j * CELL_COUNT + i] = 0;
                }
            }

            if (event.key.keysym.sym == SDLK_SPACE)
                run_game = !run_game;
        }

        if (event.type == SDL_QUIT)
            break;

        gfx_present(ctxt);
        time_ticks++;
    }

    // Free your system
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}