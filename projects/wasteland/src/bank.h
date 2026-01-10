#pragma once

typedef struct pti_flags_t pti_flags_t;
typedef struct pti_palette_t pti_palette_t;
typedef struct pti_flags_t pti_flags_t;
typedef struct pti_tilemap_t pti_tilemap_t;
typedef struct pti_tileset_t pti_tileset_t;
typedef struct pti_bitmap_t pti_bitmap_t;

// this is the main graphics bank for the game
extern pti_flags_t *flags;
extern pti_palette_t *palette;
extern pti_tileset_t *tileset;
extern pti_tilemap_t *tilemap;
extern pti_bitmap_t *player_bitmap;