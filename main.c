#include "gfx/gfx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL_SIZE 10
#define CELL_COUNT 50
#define SCREEN_WIDTH (CELL_SIZE * CELL_COUNT)
#define SCREEN_HEIGHT (CELL_SIZE * CELL_COUNT)

void show_grid(struct gfx_context_t *ctxt) {
    for (uint16_t i = 0; i < SCREEN_WIDTH; i += CELL_SIZE) {
        for (uint16_t j = 0; j < SCREEN_HEIGHT; j += CELL_SIZE) {
            gfx_putpixel(ctxt, i, j, COLOR_WHITE);
        }
    }
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
    while (true) {
        gfx_clear(ctxt, COLOR_BLACK);

        show_grid(ctxt);

        SDL_PollEvent(&event);
        buttons = SDL_GetMouseState(&x, &y);

        x = (uint32_t)(x / CELL_SIZE);
        y = (uint32_t)(y / CELL_SIZE);

        draw_full_square(ctxt, x, y, CELL_SIZE, COLOR_RED);

        for (uint8_t i = 0; i < CELL_COUNT; i++) {
            for (uint8_t j = 0; j < CELL_COUNT; j++) {
                if (cells[j * CELL_COUNT + i] == 1)
                    draw_full_square(ctxt, i, j, CELL_SIZE, COLOR_WHITE);
            }
        }

        if ((buttons & SDL_BUTTON_LMASK) != 0)
            cells[y * CELL_COUNT + x] = 1;

        if ((buttons & SDL_BUTTON_RMASK) != 0)
            cells[y * CELL_COUNT + x] = 0;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_r) {
                for (uint8_t i = 0; i < CELL_COUNT; i++) {
                    for (uint8_t j = 0; j < CELL_COUNT; j++)
                        cells[j * CELL_COUNT + i] = 0;
                }
            }
        }

        if (event.type == SDL_QUIT)
            break;

        gfx_present(ctxt);
    }

    // Free your system
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}