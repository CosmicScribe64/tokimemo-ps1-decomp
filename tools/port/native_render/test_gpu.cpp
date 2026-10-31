// T-39302: unit tests for the PS1 GPU model with synthetic primitives. Every expected value is
// worked out by hand from the hardware rules (psx-spx) and written down in the test; none is
// produced by running the renderer. Build and run: tools/port/native_render/build.sh --test.
#include "gpu.hpp"

#include <cstdio>
#include <initializer_list>
#include <vector>

using ps1::Gpu;

static int g_checks = 0, g_failures = 0;
#define CHECK_EQ(actual, expected)                                                                    \
    do {                                                                                              \
        ++g_checks;                                                                                   \
        const long long a_ = (long long)(actual), e_ = (long long)(expected);                         \
        if (a_ != e_) {                                                                               \
            ++g_failures;                                                                             \
            std::printf("FAIL %s:%d %s: got %lld, expected %lld\n", __FILE__, __LINE__, #actual, a_, e_); \
        }                                                                                             \
    } while (0)

static int g_tests = 0;
#define TEST(name) static void name(); static struct Reg_##name { Reg_##name() { tests().push_back({#name, name}); } } reg_##name; static void name()
struct Case { const char* name; void (*fn)(); };
static std::vector<Case>& tests() { static std::vector<Case> t; return t; }

// ---- helpers ------------------------------------------------------------------------------------
static uint32_t xy(int x, int y) { return uint32_t(x & 0x7FF) | uint32_t(y & 0x7FF) << 16; }
static uint32_t rgb(int r, int g, int b) { return uint32_t(r) | uint32_t(g) << 8 | uint32_t(b) << 16; }
static uint16_t c15(int r, int g, int b) { return uint16_t(r | g << 5 | b << 10); }
static void send(Gpu& gpu, std::initializer_list<uint32_t> words) { for (uint32_t w : words) gpu.gp0(w); }
static void set_area(Gpu& gpu, int x1, int y1, int x2, int y2) {
    gpu.gp0(0xE3000000u | uint32_t(x1) | uint32_t(y1) << 10);
    gpu.gp0(0xE4000000u | uint32_t(x2) | uint32_t(y2) << 10);
}
static void set_tpage(Gpu& gpu, int tx, int ty256, int semi, int depth, bool dither = false) {
    gpu.gp0(0xE1000000u | uint32_t(tx) | uint32_t(ty256 ? 0x10 : 0) | uint32_t(semi) << 5 | uint32_t(depth) << 7 |
            (dither ? 0x200u : 0u));
}
static void put(Gpu& gpu, int x, int y, uint16_t v) { gpu.vram()[size_t(y) * 1024 + x] = v; }
static uint16_t px(const Gpu& gpu, int x, int y) { return gpu.pixel(x, y); }
// Number of pixels in [0,w)x[0,h) that differ from `background`.
static int count_changed(const Gpu& gpu, int w, int h, uint16_t background = 0) {
    int n = 0;
    for (int y = 0; y < h; ++y) for (int x = 0; x < w; ++x) if (px(gpu, x, y) != background) ++n;
    return n;
}

// ---- VRAM transfers -----------------------------------------------------------------------------
TEST(load_store_roundtrip_and_wrap) {
    Gpu gpu;
    const uint16_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    gpu.load_image(1022, 510, 4, 2, data);  // wraps both ways: x 1022,1023,0,1 and y 510,511
    CHECK_EQ(px(gpu, 1022, 510), 1); CHECK_EQ(px(gpu, 1023, 510), 2);
    CHECK_EQ(px(gpu, 0, 510), 3); CHECK_EQ(px(gpu, 1, 510), 4);
    CHECK_EQ(px(gpu, 1022, 511), 5); CHECK_EQ(px(gpu, 1, 511), 8);
    uint16_t back[8] = {};
    gpu.store_image(1022, 510, 4, 2, back);
    for (int i = 0; i < 8; ++i) CHECK_EQ(back[i], data[i]);
    // a height of 512 starting at y=100 wraps to the top: rows 100..511 then 0..99
    std::vector<uint16_t> tall(512, 0x1111);
    gpu.load_image(300, 100, 1, 512, tall.data());
    CHECK_EQ(px(gpu, 300, 0), 0x1111); CHECK_EQ(px(gpu, 300, 99), 0x1111);
}

TEST(gp0_cpu_to_vram_stream_odd_width) {
    Gpu gpu;
    // 3x1 image at (10,20): 3 halfwords = 2 words, the high half of the last word is ignored.
    send(gpu, {0xA0000000u, xy(10, 20), (1u << 16) | 3u, 0x00020001u, 0x7FFF0003u});
    CHECK_EQ(px(gpu, 10, 20), 1); CHECK_EQ(px(gpu, 11, 20), 2); CHECK_EQ(px(gpu, 12, 20), 3);
    CHECK_EQ(px(gpu, 13, 20), 0);
    // the stream is back at command level: a fill follows without desync
    send(gpu, {0x02FF0000u, xy(0, 0), (1u << 16) | 16u});
    CHECK_EQ(px(gpu, 0, 0), c15(0, 0, 31));
}

TEST(move_image_overlap_and_mask) {
    Gpu gpu;
    uint16_t row[4] = {10, 20, 30, 40};
    gpu.load_image(0, 0, 4, 1, row);
    // Overlapping copy right by 2, pixel by pixel: d2 = s0 = 10, d3 = s1 = 20, then d4 reads pixel 2, which
    // now holds 10, and d5 reads pixel 3, which now holds 20.
    gpu.move_image(0, 0, 2, 0, 4, 1);
    CHECK_EQ(px(gpu, 2, 0), 10); CHECK_EQ(px(gpu, 3, 0), 20); CHECK_EQ(px(gpu, 4, 0), 10); CHECK_EQ(px(gpu, 5, 0), 20);
    // Copy down by one row: row 1 gets row 0, row 2 reads row 1 (already overwritten), so it repeats row 0.
    {
        Gpu g2;
        uint16_t r0[2] = {1, 2}, r1[2] = {3, 4};
        g2.load_image(0, 0, 2, 1, r0); g2.load_image(0, 1, 2, 1, r1);
        g2.move_image(0, 0, 0, 1, 2, 2);
        CHECK_EQ(px(g2, 0, 1), 1); CHECK_EQ(px(g2, 1, 1), 2); CHECK_EQ(px(g2, 0, 2), 1); CHECK_EQ(px(g2, 1, 2), 2);
    }
    gpu.environment().mask_set = true;
    gpu.move_image(0, 0, 0, 5, 1, 1);
    CHECK_EQ(px(gpu, 0, 5), 10 | 0x8000);
    gpu.environment().mask_check = true;  // destination (0,5) now has bit 15: not overwritten
    gpu.environment().mask_set = false;
    gpu.move_image(1, 0, 0, 5, 1, 1);
    CHECK_EQ(px(gpu, 0, 5), 10 | 0x8000);
}

TEST(fill_rounds_x_and_width_to_16_and_ignores_area) {
    Gpu gpu;
    set_area(gpu, 100, 100, 101, 101);  // fill must ignore the draw area
    // x = 0x15 -> 0x10; width 0x11 -> 0x20; colour (255,0,0) -> 5 bit (31,0,0)
    send(gpu, {0x020000FFu, xy(0x15, 7), (2u << 16) | 0x11u});
    CHECK_EQ(px(gpu, 0x10, 7), 31); CHECK_EQ(px(gpu, 0x2F, 8), 31);
    CHECK_EQ(px(gpu, 0x0F, 7), 0); CHECK_EQ(px(gpu, 0x30, 7), 0); CHECK_EQ(px(gpu, 0x10, 9), 0);
    // fill wraps at the right edge: x=1008, w=32 covers 1008..1023 and 0..15
    send(gpu, {0x0200FF00u, xy(1008, 300), (1u << 16) | 32u});
    CHECK_EQ(px(gpu, 1023, 300), c15(0, 31, 0)); CHECK_EQ(px(gpu, 15, 300), c15(0, 31, 0)); CHECK_EQ(px(gpu, 16, 300), 0);
}

// ---- rectangles, clipping, offset ---------------------------------------------------------------
TEST(tile_covers_exactly_w_by_h) {
    Gpu gpu;
    send(gpu, {0x60000000u | rgb(255, 0, 0), xy(10, 10), (3u << 16) | 4u});
    CHECK_EQ(count_changed(gpu, 64, 64), 12);
    CHECK_EQ(px(gpu, 10, 10), 31); CHECK_EQ(px(gpu, 13, 12), 31);
    CHECK_EQ(px(gpu, 14, 10), 0); CHECK_EQ(px(gpu, 10, 13), 0); CHECK_EQ(px(gpu, 9, 10), 0);
}

TEST(fixed_size_tiles) {
    Gpu gpu;
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(5, 5)});                 // 1x1
    send(gpu, {0x70000000u | rgb(0, 255, 0), xy(20, 20)});               // 8x8
    send(gpu, {0x78000000u | rgb(0, 255, 0), xy(40, 40)});               // 16x16
    CHECK_EQ(count_changed(gpu, 128, 128), 1 + 64 + 256);
    CHECK_EQ(px(gpu, 5, 5), c15(0, 31, 0)); CHECK_EQ(px(gpu, 27, 27), c15(0, 31, 0)); CHECK_EQ(px(gpu, 28, 27), 0);
    CHECK_EQ(px(gpu, 55, 55), c15(0, 31, 0)); CHECK_EQ(px(gpu, 56, 55), 0);
}

TEST(draw_area_clips_and_offset_shifts) {
    Gpu gpu;
    set_area(gpu, 2, 2, 5, 5);
    gpu.gp0(0xE5000000u | 3u | (3u << 11));  // offset (3,3)
    send(gpu, {0x60000000u | rgb(255, 255, 255), xy(0, 0), (4u << 16) | 4u});  // lands at 3..6, clipped to <=5
    CHECK_EQ(count_changed(gpu, 16, 16), 9);
    CHECK_EQ(px(gpu, 3, 3), c15(31, 31, 31)); CHECK_EQ(px(gpu, 5, 5), c15(31, 31, 31));
    CHECK_EQ(px(gpu, 6, 3), 0); CHECK_EQ(px(gpu, 2, 3), 0);
    // a negative offset: sign extension of the 11-bit fields
    Gpu g2;
    g2.gp0(0xE5000000u | 0x7FDu | (0x7FEu << 11));  // (-3,-2)
    send(g2, {0x68000000u | rgb(255, 0, 0), xy(10, 10)});
    CHECK_EQ(px(g2, 7, 8), 31); CHECK_EQ(count_changed(g2, 32, 32), 1);
}

// ---- polygons: fill rules ---------------------------------------------------------------------
TEST(triangle_top_left_rule_right_triangle) {
    Gpu gpu;
    // (0,0),(4,0),(0,4): a pixel (x,y) is covered when x+y < 4. The hypotenuse x+y=4 belongs to the
    // neighbouring triangle, the top row y=0 and the left column x=0 belong to this one.
    send(gpu, {0x20000000u | rgb(255, 0, 0), xy(0, 0), xy(4, 0), xy(0, 4)});
    int covered = 0;
    for (int y = 0; y < 6; ++y)
        for (int x = 0; x < 6; ++x) {
            const bool expect = x + y < 4;
            CHECK_EQ(px(gpu, x, y) != 0, expect);
            covered += expect;
        }
    CHECK_EQ(covered, 10);
}

TEST(triangle_winding_does_not_matter) {
    Gpu a, b;
    send(a, {0x20000000u | rgb(255, 0, 0), xy(1, 1), xy(9, 3), xy(4, 8)});
    send(b, {0x20000000u | rgb(255, 0, 0), xy(1, 1), xy(4, 8), xy(9, 3)});
    for (int y = 0; y < 12; ++y) for (int x = 0; x < 12; ++x) CHECK_EQ(px(a, x, y), px(b, x, y));
    CHECK_EQ(px(a, 4, 5) != 0, true);       // an interior pixel is drawn
    CHECK_EQ(px(a, 9, 3) != 0, false);      // the right vertex pixel is not (it lies on a right edge)
}

TEST(quad_is_two_triangles_without_gap_or_overlap) {
    Gpu gpu;
    gpu.gp0(0xE1000000u | (1u << 5));  // semi mode 1: B + F, so a double write would show as 2
    // additive 5-bit 1 (8-bit 8) over a black background; each pixel of the 8x8 square exactly once
    send(gpu, {0x2A000000u | rgb(8, 8, 8), xy(0, 0), xy(8, 0), xy(0, 8), xy(8, 8)});
    for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) CHECK_EQ(px(gpu, x, y), c15(1, 1, 1));
    CHECK_EQ(count_changed(gpu, 16, 16), 64);
}

TEST(degenerate_and_oversized_polygons_draw_nothing) {
    Gpu gpu;
    send(gpu, {0x20000000u | rgb(255, 255, 255), xy(3, 3), xy(9, 3), xy(6, 3)});  // zero area
    CHECK_EQ(count_changed(gpu, 32, 32), 0);
    send(gpu, {0x20000000u | rgb(255, 255, 255), xy(0, 0), xy(1024, 0), xy(0, 4)});  // 1024 apart
    CHECK_EQ(gpu.count(ps1::K_SKIPPED_OVERSIZE), 1);
    CHECK_EQ(count_changed(gpu, 64, 8), 0);
    send(gpu, {0x20000000u | rgb(255, 255, 255), xy(0, 0), xy(1023, 0), xy(0, 4)});  // 1023 apart is drawn
    CHECK_EQ(px(gpu, 0, 0) != 0, true);
}

TEST(gouraud_gradient_is_exact_on_an_integer_plane) {
    Gpu gpu;
    // r: 0 at (0,0) and (0,8), 248 at (8,0): 31 per pixel in x, flat in y. g and b stay 0.
    send(gpu, {0x30000000u | rgb(0, 0, 0), xy(0, 0), rgb(248, 0, 0), xy(8, 0), rgb(0, 0, 0), xy(0, 8)});
    CHECK_EQ(px(gpu, 0, 0), 0);
    CHECK_EQ(px(gpu, 4, 0), 15);      // 31*4 = 124 -> 124 >> 3 = 15
    CHECK_EQ(px(gpu, 7, 0), 27);      // 31*7 = 217 -> 27
    CHECK_EQ(px(gpu, 3, 2), 11);      // 93 -> 11
    CHECK_EQ(px(gpu, 0, 6), 0);
    CHECK_EQ(px(gpu, 7, 1), 0);       // x + y = 8 is on the excluded hypotenuse
}

// ---- semi-transparency -----------------------------------------------------------------------
static void semi_case(int mode, int er, int eg, int eb) {
    Gpu gpu;
    uint16_t back = c15(10, 20, 30);
    gpu.load_image(0, 0, 1, 1, &back);
    gpu.gp0(0xE1000000u | uint32_t(mode) << 5);
    // front 8-bit (64,32,16) -> 5-bit (8,4,2); semi-transparent tile (command bit 1)
    send(gpu, {0x62000000u | rgb(64, 32, 16), xy(0, 0), (1u << 16) | 1u});
    CHECK_EQ(px(gpu, 0, 0), c15(er, eg, eb));
}
TEST(semi_transparency_four_modes) {
    semi_case(0, 9, 12, 16);    // (B+F)/2: (10+8)/2, (20+4)/2, (30+2)/2
    semi_case(1, 18, 24, 31);   // B+F, blue clamps (32 -> 31)
    semi_case(2, 2, 16, 28);    // B-F
    semi_case(3, 12, 21, 30);   // B+F/4: 10+2, 20+1, 30+0
}
TEST(semi_dithered_blends_at_8_bit_then_dithers) {
    // Background 5-bit 28 (8-bit 224), subtract source 128 (gray), dither on, Gouraud so it dithers:
    // result8 = (224 - 128) + T, 5-bit = result8 >> 3: T=-3 (x=0,y=2) -> 93 >> 3 = 11, T=+1 (x=1,y=2) -> 97 >> 3 = 12.
    Gpu gpu;
    for (int x = 0; x < 4; ++x) for (int y = 0; y < 4; ++y) put(gpu, x, y, c15(28, 28, 28));
    set_tpage(gpu, 0, 0, 2, 0, true);
    send(gpu, {0x3A000000u | rgb(128, 128, 128), xy(0, 0), rgb(128, 128, 128), xy(4, 0), rgb(128, 128, 128), xy(0, 4),
               rgb(128, 128, 128), xy(4, 4)});
    CHECK_EQ(px(gpu, 0, 2), c15(11, 11, 11)); CHECK_EQ(px(gpu, 1, 2), c15(12, 12, 12));
    CHECK_EQ(px(gpu, 2, 2), c15(11, 11, 11));  // T=-4: 92 >> 3 = 11
    CHECK_EQ(px(gpu, 3, 2), c15(12, 12, 12));  // T=0: 96 >> 3 = 12
}

TEST(semi_subtract_clamps_at_zero) {
    Gpu gpu;
    uint16_t back = c15(1, 0, 5);
    gpu.load_image(0, 0, 1, 1, &back);
    gpu.gp0(0xE1000000u | (2u << 5));
    send(gpu, {0x62000000u | rgb(128, 128, 128), xy(0, 0), (1u << 16) | 1u});  // front 16
    CHECK_EQ(px(gpu, 0, 0), 0);
}

// ---- textures --------------------------------------------------------------------------------
// Texture page 8 (x=512, y=0). CLUT at x=0, y=480 (clut word: x/16 | y << 6).
static const int CLUT_X = 0, CLUT_Y = 480;
static const uint32_t CLUT_WORD = uint32_t(CLUT_X / 16) | uint32_t(CLUT_Y) << 6;

static void setup_clut4(Gpu& gpu) {
    put(gpu, CLUT_X + 0, CLUT_Y, 0x0000);          // index 0: transparent
    put(gpu, CLUT_X + 1, CLUT_Y, c15(31, 0, 0));    // index 1: red
    put(gpu, CLUT_X + 2, CLUT_Y, c15(0, 31, 0));    // index 2: green
    put(gpu, CLUT_X + 3, CLUT_Y, c15(0, 0, 31) | 0x8000);  // index 3: blue with the semi-transparency bit
    put(gpu, 512, 0, 0x3210);                       // texels u=0..3 -> indices 0,1,2,3
}
TEST(sprite_4bit_clut_lookup_and_transparency) {
    Gpu gpu;
    setup_clut4(gpu);
    set_tpage(gpu, 8, 0, 0, 0);
    // raw (command bit 0): the texel goes through unchanged
    send(gpu, {0x65000000u, xy(20, 20), uint32_t(CLUT_WORD) << 16 | 0u, (1u << 16) | 4u});
    CHECK_EQ(px(gpu, 20, 20), 0);                          // index 0: not drawn
    CHECK_EQ(px(gpu, 21, 20), c15(31, 0, 0));
    CHECK_EQ(px(gpu, 22, 20), c15(0, 31, 0));
    CHECK_EQ(px(gpu, 23, 20), c15(0, 0, 31) | 0x8000);     // bit 15 of the texel is stored
    CHECK_EQ(px(gpu, 24, 20), 0);
}

TEST(sprite_modulation_neutral_and_half) {
    Gpu gpu;
    setup_clut4(gpu);
    set_tpage(gpu, 8, 0, 0, 0);
    // colour 0x80 is neutral: (31*128)>>4 = 248 -> 31
    send(gpu, {0x64000000u | rgb(128, 128, 128), xy(0, 0), CLUT_WORD << 16, (1u << 16) | 4u});
    CHECK_EQ(px(gpu, 1, 0), c15(31, 0, 0));
    // colour 0x40: (31*64)>>4 = 124 -> 15; the blue texel keeps bit 15 and has b = (31*64>>4)>>3 = 15
    send(gpu, {0x64000000u | rgb(64, 64, 64), xy(0, 4), CLUT_WORD << 16, (1u << 16) | 4u});
    CHECK_EQ(px(gpu, 1, 4), c15(15, 0, 0));
    CHECK_EQ(px(gpu, 3, 4), c15(0, 0, 15) | 0x8000);
    // colour 0xFF doubles: (31*255)>>4 = 494 -> clamps to 255 -> 31
    send(gpu, {0x64000000u | rgb(255, 255, 255), xy(0, 8), CLUT_WORD << 16, (1u << 16) | 4u});
    CHECK_EQ(px(gpu, 2, 8), c15(0, 31, 0));
}

TEST(sprite_8bit_and_15bit_textures) {
    Gpu gpu;
    put(gpu, CLUT_X + 7, CLUT_Y, c15(5, 6, 7));
    put(gpu, CLUT_X + 200, CLUT_Y, c15(9, 8, 7));
    put(gpu, 512, 0, uint16_t(200 << 8 | 7));       // u=0 -> index 7, u=1 -> index 200
    set_tpage(gpu, 8, 0, 0, 1);
    send(gpu, {0x65000000u, xy(0, 0), CLUT_WORD << 16, (1u << 16) | 2u});
    CHECK_EQ(px(gpu, 0, 0), c15(5, 6, 7)); CHECK_EQ(px(gpu, 1, 0), c15(9, 8, 7));
    // 15-bit direct: texel is the pixel; u counts halfwords
    put(gpu, 512 + 3, 1, c15(1, 2, 3)); put(gpu, 512 + 4, 1, c15(4, 5, 6));
    set_tpage(gpu, 8, 0, 0, 2);
    send(gpu, {0x65000000u, xy(10, 10), 0u | (1u << 8) | 3u, (1u << 16) | 2u});  // u0=3, v0=1
    CHECK_EQ(px(gpu, 10, 10), c15(1, 2, 3)); CHECK_EQ(px(gpu, 11, 10), c15(4, 5, 6));
    // an all-zero direct texel is transparent
    put(gpu, 512 + 5, 1, 0);
    send(gpu, {0x65000000u, xy(10, 20), 0u | (1u << 8) | 5u, (1u << 16) | 1u});
    CHECK_EQ(px(gpu, 10, 20), 0);
}

TEST(sprite_texcoords_wrap_at_256) {
    Gpu gpu;
    set_tpage(gpu, 8, 0, 0, 2);
    for (int u : {254, 255, 0, 1}) put(gpu, 512 + u, 0, c15(u & 31, 1, 1));
    send(gpu, {0x65000000u, xy(0, 0), 254u, (1u << 16) | 4u});  // u0 = 254, width 4 -> 254,255,0,1
    CHECK_EQ(px(gpu, 0, 0), c15(254 & 31, 1, 1)); CHECK_EQ(px(gpu, 1, 0), c15(255 & 31, 1, 1));
    CHECK_EQ(px(gpu, 2, 0), c15(0, 1, 1)); CHECK_EQ(px(gpu, 3, 0), c15(1, 1, 1));
}

TEST(sprite_flip_x_and_y) {
    Gpu gpu;
    set_tpage(gpu, 8, 0, 0, 2);
    for (int i = 0; i < 4; ++i) { put(gpu, 512 + i, 0, c15(i + 1, 0, 1)); put(gpu, 512 + i, 1, c15(i + 1, 0, 2)); }
    gpu.gp0(0xE1000000u | (2u << 7) | 8u | 0x1000u);  // flip x
    send(gpu, {0x65000000u, xy(0, 0), 3u, (1u << 16) | 4u});  // u0 = 3 mirrored: 3,2,1,0
    CHECK_EQ(px(gpu, 0, 0), c15(4, 0, 1)); CHECK_EQ(px(gpu, 3, 0), c15(1, 0, 1));
    gpu.gp0(0xE1000000u | (2u << 7) | 8u | 0x2000u);  // flip y
    send(gpu, {0x65000000u, xy(0, 10), (1u << 8) | 0u, (2u << 16) | 1u});  // v0 = 1 mirrored: 1,0
    CHECK_EQ(px(gpu, 0, 10), c15(1, 0, 2)); CHECK_EQ(px(gpu, 0, 11), c15(1, 0, 1));
}

TEST(texture_window_masks_and_offsets) {
    Gpu gpu;
    set_tpage(gpu, 8, 0, 0, 2);
    for (int u = 0; u < 32; ++u) put(gpu, 512 + u, 0, c15(u, 1, 1));
    // mask x = 1 (8 pixels), offset x = 2 (2*8 = 16): u' = (u & ~8) | (2 & 1)*8 = u & ~8
    gpu.gp0(0xE2000000u | 1u | (2u << 10));
    send(gpu, {0x65000000u, xy(0, 0), 8u, (1u << 16) | 1u});  // u=8 -> 8 & ~8 | 0 = 0
    CHECK_EQ(px(gpu, 0, 0), c15(0, 1, 1));
    // mask x = 1, offset x = 1: u' = (u & ~8) | 8
    gpu.gp0(0xE2000000u | 1u | (1u << 10));
    send(gpu, {0x65000000u, xy(0, 1), 3u, (1u << 16) | 1u});  // u=3 -> 3 | 8 = 11
    CHECK_EQ(px(gpu, 0, 1), c15(11, 1, 1));
}

TEST(semi_transparent_texture_blends_only_texels_with_bit15) {
    Gpu gpu;
    setup_clut4(gpu);
    uint16_t back = c15(10, 10, 10);
    for (int x = 20; x < 24; ++x) put(gpu, x, 30, back);
    gpu.gp0(0xE1000000u | 8u | (1u << 5));  // page 8, 4-bit, semi mode 1 (add)
    send(gpu, {0x67000000u, xy(20, 30), CLUT_WORD << 16, (1u << 16) | 4u});  // raw + semi
    CHECK_EQ(px(gpu, 20, 30), back);                            // transparent texel: untouched
    CHECK_EQ(px(gpu, 21, 30), c15(31, 0, 0));                   // red has no bit 15: opaque
    CHECK_EQ(px(gpu, 22, 30), c15(0, 31, 0));                   // green: opaque
    CHECK_EQ(px(gpu, 23, 30), c15(10, 10, 31) | 0x8000);        // blue with bit 15: 10+0, 10+0, 10+31 -> 31
}

TEST(textured_triangle_page_clut_and_one_to_one_mapping) {
    Gpu gpu;
    setup_clut4(gpu);
    put(gpu, 512, 1, 0x3333);  // row v=1: indices 3,3,3,3
    // raw FT3 whose vertex texcoords equal pixel offsets (u = x, v = y): a 1:1 mapping
    // words: cmd, xy0, clut|uv0, xy1, tpage|uv1, xy2, uv2; tpage 8 = page x 512, 4-bit
    send(gpu, {0x25000000u, xy(0, 0), CLUT_WORD << 16 | 0u, xy(4, 0), (8u << 16) | 4u, xy(0, 4), 0u | (4u << 8)});
    // row y=0 pixels x=0..3 take u=0..3 (indices 0..3): x=0 transparent, 1 red, 2 green, 3 blue
    CHECK_EQ(px(gpu, 0, 0), 0); CHECK_EQ(px(gpu, 1, 0), c15(31, 0, 0)); CHECK_EQ(px(gpu, 2, 0), c15(0, 31, 0));
    // v=1 row has index 3 everywhere (modulated with the flat colour 0: command colour is 0 and raw
    // is set, so the texel passes unchanged)
    CHECK_EQ(px(gpu, 0, 1), c15(0, 0, 31) | 0x8000); CHECK_EQ(px(gpu, 1, 1), c15(0, 0, 31) | 0x8000);
    CHECK_EQ(gpu.environment().tpage_x, 8);  // the polygon's attribute updated the texture page
}

// ---- dithering --------------------------------------------------------------------------------
TEST(dither_matrix_on_gouraud_polygon_only) {
    static const int T[4][4] = {{-4, 0, -3, 1}, {2, -2, 3, -1}, {-3, 1, -4, 0}, {3, -1, 2, -2}};
    Gpu gpu;
    set_tpage(gpu, 0, 0, 0, 0, true);
    // Gouraud 8x8 square, every vertex colour 6: 5-bit value is (6 + T) >> 3, i.e. 1 where T >= 2
    send(gpu, {0x38000000u | rgb(6, 6, 6), xy(0, 0), rgb(6, 6, 6), xy(8, 0), rgb(6, 6, 6), xy(0, 8),
               rgb(6, 6, 6), xy(8, 8)});
    int ones = 0;
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            const int expect = (6 + T[y & 3][x & 3]) >> 3;
            CHECK_EQ(px(gpu, x, y), c15(expect, expect, expect));
            ones += expect;
        }
    CHECK_EQ(ones, 16);  // 4 positions per 4x4 tile, 4 tiles
    // flat polygon and TILE: no dithering, 6 >> 3 = 0
    Gpu flat;
    set_tpage(flat, 0, 0, 0, 0, true);
    send(flat, {0x28000000u | rgb(6, 6, 6), xy(0, 0), xy(8, 0), xy(0, 8), xy(8, 8)});
    send(flat, {0x60000000u | rgb(6, 6, 6), xy(16, 0), (8u << 16) | 8u});
    CHECK_EQ(count_changed(flat, 32, 8), 0);
}

TEST(dither_applies_to_modulated_polygons_but_not_raw_or_sprites) {
    static const int T[4][4] = {{-4, 0, -3, 1}, {2, -2, 3, -1}, {-3, 1, -4, 0}, {3, -1, 2, -2}};
    Gpu gpu;
    for (int v = 0; v < 2; ++v) for (int u = 0; u < 4; ++u) put(gpu, 512 + u, v, c15(16, 16, 16));
    set_tpage(gpu, 8, 0, 0, 2, true);
    const uint32_t tpage_attr = 8u | (2u << 7);
    // modulated FT4 by 0x80: (16*128)>>4 = 128, plus the dither offset, >> 3 (15 or 16); u = x, v = y
    send(gpu, {0x2C000000u | rgb(128, 128, 128), xy(0, 0), 0u, xy(4, 0), (tpage_attr << 16) | 4u, xy(0, 2),
               (2u << 8), xy(4, 2), (2u << 8) | 4u});
    for (int y = 0; y < 2; ++y)
        for (int x = 0; x < 4; ++x) { const int e = (128 + T[y][x]) >> 3; CHECK_EQ(px(gpu, x, y), c15(e, e, e)); }
    CHECK_EQ(px(gpu, 0, 0), c15(15, 15, 15)); CHECK_EQ(px(gpu, 1, 0), c15(16, 16, 16));
    CHECK_EQ(px(gpu, 0, 1), c15(16, 16, 16)); CHECK_EQ(px(gpu, 1, 1), c15(15, 15, 15));
    // raw FT4: the texel unchanged, no dithering
    send(gpu, {0x2D000000u, xy(0, 4), 0u, xy(4, 4), (tpage_attr << 16) | 4u, xy(0, 6), (2u << 8), xy(4, 6), (2u << 8) | 4u});
    for (int y = 4; y < 6; ++y) for (int x = 0; x < 4; ++x) CHECK_EQ(px(gpu, x, y), c15(16, 16, 16));
    // a modulated sprite is not dithered either: every pixel 128 >> 3 = 16
    send(gpu, {0x64000000u | rgb(128, 128, 128), xy(0, 8), 0u, (1u << 16) | 4u});
    for (int x = 0; x < 4; ++x) CHECK_EQ(px(gpu, x, 8), c15(16, 16, 16));
}

// ---- mask bits -------------------------------------------------------------------------------
TEST(mask_set_and_check) {
    Gpu gpu;
    gpu.gp0(0xE6000001u);  // set bit 15 on drawn pixels
    send(gpu, {0x68000000u | rgb(255, 0, 0), xy(1, 1)});
    CHECK_EQ(px(gpu, 1, 1), c15(31, 0, 0) | 0x8000);
    gpu.gp0(0xE6000002u);  // check: do not draw over pixels with bit 15
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(1, 1)});
    CHECK_EQ(px(gpu, 1, 1), c15(31, 0, 0) | 0x8000);
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(2, 1)});  // unmasked neighbour is drawn (and not set)
    CHECK_EQ(px(gpu, 2, 1), c15(0, 31, 0));
}

// ---- lines -----------------------------------------------------------------------------------
TEST(lines_include_both_end_points) {
    Gpu gpu;
    send(gpu, {0x40000000u | rgb(255, 0, 0), xy(2, 5), xy(6, 5)});
    CHECK_EQ(count_changed(gpu, 16, 16), 5);
    for (int x = 2; x <= 6; ++x) CHECK_EQ(px(gpu, x, 5), 31);
    Gpu diag;
    send(diag, {0x40000000u | rgb(255, 0, 0), xy(0, 0), xy(4, 4)});
    CHECK_EQ(count_changed(diag, 16, 16), 5);
    for (int i = 0; i <= 4; ++i) CHECK_EQ(px(diag, i, i), 31);
    Gpu back;  // drawn from right to left, the same pixels
    send(back, {0x40000000u | rgb(255, 0, 0), xy(6, 5), xy(2, 5)});
    CHECK_EQ(count_changed(back, 16, 16), 5);
}

TEST(gouraud_line_ramps_colour) {
    Gpu gpu;
    send(gpu, {0x50000000u | rgb(0, 0, 0), xy(0, 0), rgb(240, 0, 0), xy(4, 0)});
    // 4 steps: r = 0, 60, 120, 180, 240 -> >> 3 = 0, 7, 15, 22, 30
    CHECK_EQ(px(gpu, 0, 0), 0); CHECK_EQ(px(gpu, 1, 0), 7); CHECK_EQ(px(gpu, 2, 0), 15);
    CHECK_EQ(px(gpu, 3, 0), 22); CHECK_EQ(px(gpu, 4, 0), 30);
}

TEST(polyline_runs_until_terminator) {
    Gpu gpu;
    send(gpu, {0x48000000u | rgb(255, 0, 0), xy(0, 0), xy(4, 0), xy(4, 3), 0x55555555u});
    CHECK_EQ(count_changed(gpu, 16, 16), 5 + 3);  // (0..4,0) and (4,1..3); the shared corner once
    CHECK_EQ(px(gpu, 4, 3), 31);
    // the stream is back at command level
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(9, 9)});
    CHECK_EQ(px(gpu, 9, 9), c15(0, 31, 0));
}

// ---- ordering tables -------------------------------------------------------------------------
static void put32(std::vector<uint8_t>& ram, uint32_t at, uint32_t v) {
    for (int i = 0; i < 4; ++i) ram[at + i] = uint8_t(v >> (8 * i));
}
TEST(draw_otag_walks_the_linked_list) {
    std::vector<uint8_t> ram(0x200000, 0);
    // node A @0x1000 -> B @0x2000 (a TILE: 3 payload words), B ends the list with 0xFFFFFF
    put32(ram, 0x1000, 0x03000000u | 0x2000u);
    put32(ram, 0x1004, 0x60000000u | rgb(255, 0, 0)); put32(ram, 0x1008, xy(1, 1)); put32(ram, 0x100C, (2u << 16) | 2u);
    put32(ram, 0x2000, 0x03000000u | 0xFFFFFFu);
    put32(ram, 0x2004, 0x60000000u | rgb(0, 255, 0)); put32(ram, 0x2008, xy(10, 10)); put32(ram, 0x200C, (1u << 16) | 3u);
    Gpu gpu;
    const ps1::WalkResult r = gpu.draw_otag(ram.data(), ram.size(), 0x80001000u);  // KSEG0 alias
    CHECK_EQ(r.nodes, 2); CHECK_EQ(r.words, 6); CHECK_EQ(r.terminated, true);
    CHECK_EQ(px(gpu, 1, 1), c15(31, 0, 0)); CHECK_EQ(px(gpu, 2, 2), c15(31, 0, 0)); CHECK_EQ(px(gpu, 3, 1), 0);
    CHECK_EQ(px(gpu, 10, 10), c15(0, 31, 0)); CHECK_EQ(px(gpu, 12, 10), c15(0, 31, 0)); CHECK_EQ(px(gpu, 13, 10), 0);
    CHECK_EQ(gpu.count(ps1::K_TILE), 2);
}

TEST(draw_otag_loop_is_detected_and_limits_hold) {
    std::vector<uint8_t> ram(0x200000, 0);
    put32(ram, 0x100, 0x00000000u | 0x200u);   // empty node
    put32(ram, 0x200, 0x00000000u | 0x100u);   // loops back
    Gpu gpu;
    ps1::WalkResult r = gpu.draw_otag(ram.data(), ram.size(), 0x100);
    CHECK_EQ(r.cycle, true); CHECK_EQ(r.nodes, 2); CHECK_EQ(r.terminated, false);
    // a node that points at itself, carrying a full-screen semi-transparent quad, is drawn once
    put32(ram, 0x1000, 0x05000000u | 0x1000u);
    put32(ram, 0x1004, 0x2A000000u | rgb(8, 8, 8)); put32(ram, 0x1008, xy(0, 0)); put32(ram, 0x100C, xy(320, 0));
    put32(ram, 0x1010, xy(0, 240)); put32(ram, 0x1014, xy(320, 240));
    Gpu g2;
    g2.gp0(0xE1000000u | (1u << 5)); set_area(g2, 0, 0, 319, 239);
    r = g2.draw_otag(ram.data(), ram.size(), 0x1000);
    CHECK_EQ(r.cycle, true); CHECK_EQ(r.nodes, 1); CHECK_EQ(px(g2, 5, 5), c15(1, 1, 1));
    // a long chain without a loop stops at max_nodes, or at max_words
    for (uint32_t i = 0; i < 40; ++i) put32(ram, 0x4000 + i * 8, 0x01000000u | (0x4000 + (i + 1) * 8)), put32(ram, 0x4004 + i * 8, 0u);
    Gpu g3;
    r = g3.draw_otag(ram.data(), ram.size(), 0x4000, 10);
    CHECK_EQ(r.node_limit, true); CHECK_EQ(r.nodes, 10); CHECK_EQ(r.cycle, false);
    r = g3.draw_otag(ram.data(), ram.size(), 0x4000, 1000, 7);
    CHECK_EQ(r.word_limit, true); CHECK_EQ(r.words, 7);
    // RAM size must be a power of two
    r = g3.draw_otag(ram.data(), 0x1FFFFF, 0x4000);
    CHECK_EQ(r.bad_ram, true); CHECK_EQ(r.nodes, 0);
}

TEST(unterminated_polyline_is_capped_and_skipped) {
    Gpu gpu;
    gpu.gp0(0x48000000u | rgb(255, 0, 0));
    for (int i = 0; i < 9000; ++i) gpu.gp0(xy(i % 300, 5));
    CHECK_EQ(gpu.count(ps1::K_POLYLINE_TRUNCATED), 1);
    // the rest of the polyline is skipped until its terminator, then commands resume
    gpu.gp0(0x55555555u);
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(9, 9)});
    CHECK_EQ(px(gpu, 9, 9), c15(0, 31, 0));
    CHECK_EQ(gpu.count(ps1::K_TILE_1), 1);
}

TEST(extreme_coordinates_do_not_overflow) {
    Gpu gpu;
    const uint16_t data[4] = {1, 2, 3, 4};
    gpu.load_image(INT32_MAX - 3, INT32_MAX - 1, 2, 2, data);
    uint16_t out[4] = {};
    gpu.store_image(INT32_MAX - 3, INT32_MAX - 1, 2, 2, out);
    CHECK_EQ(out[0], 1); CHECK_EQ(out[3], 4);
    gpu.move_image(INT32_MIN, INT32_MIN, INT32_MAX, INT32_MAX, INT32_MIN, INT32_MAX);
    gpu.fill_rect(INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, 0xFFFFFF);
    gpu.load_image(-5, -5, 3, 1, data);
    CHECK_EQ(px(gpu, 1019, 507), 1);
}

TEST(command_mirrors_and_texture_disable) {
    Gpu gpu;
    // 0x81..0x9F mirror 0x80, 0xA1..0xBF mirror 0xA0, 0xC1..0xDF mirror 0xC0
    send(gpu, {0xA5000000u, xy(10, 20), (1u << 16) | 2u, 0x00020001u});
    CHECK_EQ(px(gpu, 10, 20), 1); CHECK_EQ(px(gpu, 11, 20), 2);
    send(gpu, {0x9F000000u, xy(10, 20), xy(30, 40), (1u << 16) | 2u});
    CHECK_EQ(px(gpu, 30, 40), 1); CHECK_EQ(px(gpu, 31, 40), 2);
    send(gpu, {0xC3000000u, xy(0, 0), (1u << 16) | 1u});
    CHECK_EQ(gpu.count(ps1::K_STORE_IMAGE), 1);
    CHECK_EQ(gpu.count(ps1::K_UNKNOWN_COMMAND), 0);
    // texture disable (E1 bit 11) works only after GP1(09h) allows it
    Gpu g2;
    put(g2, 512, 0, c15(5, 5, 5)); g2.gp0(0xE1000000u | 8u | (2u << 7) | 0x800u);
    send(g2, {0x65000000u, xy(0, 0), 0u, (1u << 16) | 1u});
    CHECK_EQ(px(g2, 0, 0), c15(5, 5, 5));          // not allowed: texturing stays on
    g2.gp1(0x09000001u);
    send(g2, {0x65000000u | rgb(0, 255, 0) , xy(4, 0), 0u, (1u << 16) | 1u});
    CHECK_EQ(px(g2, 4, 0), c15(0, 31, 0));         // allowed: the primitive is untextured, colour 0x00FF00
    g2.gp1(0x09000000u);
    send(g2, {0x65000000u, xy(8, 0), 0u, (1u << 16) | 1u});
    CHECK_EQ(px(g2, 8, 0), c15(5, 5, 5));
}

TEST(draw_otag_packet_with_image_upload_and_env_commands) {
    std::vector<uint8_t> ram(0x200000, 0);
    // one node: E3 (area), E4, then a CPU->VRAM upload of 2x1 halfwords (A0 header 3 words + 1 data word)
    put32(ram, 0x40, 0x06000000u | 0xFFFFFFu);
    put32(ram, 0x44, 0xE3000000u | 0u | (0u << 10)); put32(ram, 0x48, 0xE4000000u | 63u | (63u << 10));
    put32(ram, 0x4C, 0xA0000000u); put32(ram, 0x50, xy(700, 300)); put32(ram, 0x54, (1u << 16) | 2u);
    put32(ram, 0x58, 0x22221111u);
    Gpu gpu;
    gpu.draw_otag(ram.data(), ram.size(), 0x40);
    CHECK_EQ(px(gpu, 700, 300), 0x1111); CHECK_EQ(px(gpu, 701, 300), 0x2222);
    CHECK_EQ(gpu.environment().area_x2, 63); CHECK_EQ(gpu.environment().area_y2, 63);
}

// ---- stream robustness and read-out -----------------------------------------------------------
TEST(noop_and_unknown_commands_do_not_desync) {
    Gpu gpu;
    send(gpu, {0x00000000u, 0x01000000u, 0x1F000000u, 0x03000000u});
    send(gpu, {0x68000000u | rgb(255, 0, 0), xy(4, 4)});
    CHECK_EQ(px(gpu, 4, 4), 31);
    send(gpu, {0xC0000000u, xy(0, 0), (1u << 16) | 1u});  // store-image header only, no data words follow
    send(gpu, {0x68000000u | rgb(0, 255, 0), xy(5, 4)});
    CHECK_EQ(px(gpu, 5, 4), c15(0, 31, 0));
}

TEST(read_rgba_expansion) {
    Gpu gpu;
    gpu.vram()[0] = 0x7FFF; gpu.vram()[1] = 0x0001; gpu.vram()[2] = c15(16, 8, 4);
    uint8_t out[12];
    gpu.read_rgba(0, 0, 3, 1, out, false);
    CHECK_EQ(out[0], 248); CHECK_EQ(out[1], 248); CHECK_EQ(out[2], 248); CHECK_EQ(out[3], 255);
    CHECK_EQ(out[4], 8); CHECK_EQ(out[5], 0);
    CHECK_EQ(out[8], 128); CHECK_EQ(out[9], 64); CHECK_EQ(out[10], 32);
    gpu.read_rgba(0, 0, 3, 1, out, true);  // replication: (c<<3)|(c>>2): 31 -> 255, 1 -> 8, 16 -> 132
    CHECK_EQ(out[0], 255); CHECK_EQ(out[4], 8); CHECK_EQ(out[8], 132); CHECK_EQ(out[9], 66); CHECK_EQ(out[10], 33);
}

TEST(gp1_display_mode_and_start) {
    Gpu gpu;
    gpu.gp1(0x08000001u);  // 320 wide, 240 high
    CHECK_EQ(gpu.display().width, 320); CHECK_EQ(gpu.display().height, 240);
    gpu.gp1(0x05000000u | 0u | (240u << 10));
    CHECK_EQ(gpu.display().start_y, 240);
    gpu.gp1(0x08000000u | 0x40u);  // hres2 set: 368
    CHECK_EQ(gpu.display().width, 368);
    gpu.gp1(0x08000000u | 3u | 4u | 0x20u);  // 640 x 480 interlaced
    CHECK_EQ(gpu.display().width, 640); CHECK_EQ(gpu.display().height, 480);
}

int main() {
    for (const Case& c : tests()) {
        const int before = g_failures;
        c.fn();
        ++g_tests;
        std::printf("%s %s\n", g_failures == before ? "ok  " : "FAIL", c.name);
    }
    std::printf("%d tests, %d checks, %d failures\n", g_tests, g_checks, g_failures);
    return g_failures ? 1 : 0;
}
