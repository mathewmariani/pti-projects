#include "bank.h"
#include "batteries/assets.h"

// tilemap + tileset
pti_flags_t *flags = nullptr;
pti_tilemap_t *tilemap = nullptr;
pti_tileset_t *tileset = nullptr;

// entities
pti_bitmap_t *bitmap_bullet = nullptr;
pti_bitmap_t *bitmap_coin = nullptr;
pti_bitmap_t *bitmap_goomba = nullptr;
pti_bitmap_t *bitmap_player = nullptr;
pti_bitmap_t *bitmap_pop = nullptr;
pti_bitmap_t *bitmap_shooter = nullptr;
pti_bitmap_t *bitmap_platform = nullptr;
pti_bitmap_t *bitmap_zombie = nullptr;
pti_bitmap_t *bitmap_heart = nullptr;
pti_bitmap_t *bitmap_door = nullptr;

// font
pti_bitmap_t *bitmap_font = nullptr;

// effects
pti_bitmap_t *bitmap_fx_collect1 = nullptr;
pti_bitmap_t *bitmap_fx_collect2 = nullptr;
pti_bitmap_t *bitmap_fx_dust1 = nullptr;
pti_bitmap_t *bitmap_fx_dust2 = nullptr;

std::vector<pti_tilemap_t *> levels;