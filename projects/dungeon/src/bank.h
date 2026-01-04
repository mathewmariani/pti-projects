#pragma once

typedef struct pti_tilemap_t pti_tilemap_t;
typedef struct pti_tileset_t pti_tileset_t;
typedef struct pti_bitmap_t pti_bitmap_t;

// tilemap + tileset
extern pti_tilemap_t *tilemap;
extern pti_tileset_t *tileset;

// entities
extern pti_bitmap_t *bitmap_player;