// T-39302: project-owned PS1 GPU model (VRAM, GP0 stream, rasterizer). Presentation only.
// Authored from the public hardware description (psx-spx, "GPU Rendering Commands", "GPU Render
// Attributes", "GPU Memory Transfer Commands"); no emulator source or data. No Godot dependency,
// so the unit tests and the capture comparison build with a plain compiler.
//
// The model never reads or writes anything but its own VRAM and environment. RAM is only read,
// by draw_otag(), through a const pointer.
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

namespace ps1 {

constexpr int VRAM_WIDTH = 1024;
constexpr int VRAM_HEIGHT = 512;

// Primitive and command kinds the statistics count. Order is the report order.
enum Kind : int {
    K_POLY_F3, K_POLY_F4, K_POLY_G3, K_POLY_G4, K_POLY_FT3, K_POLY_FT4, K_POLY_GT3, K_POLY_GT4,
    K_LINE_F2, K_LINE_G2, K_LINE_F_POLY, K_LINE_G_POLY,
    K_TILE, K_TILE_1, K_TILE_8, K_TILE_16, K_SPRT, K_SPRT_1, K_SPRT_8, K_SPRT_16,
    K_FILL, K_COPY_VRAM, K_LOAD_IMAGE, K_STORE_IMAGE,
    K_ENV_TPAGE, K_ENV_TWINDOW, K_ENV_AREA_TL, K_ENV_AREA_BR, K_ENV_OFFSET, K_ENV_MASK,
    K_TEX_4BIT, K_TEX_8BIT, K_TEX_15BIT, K_RAW_TEXTURE, K_MODULATED_TEXTURE,
    K_SEMI_0, K_SEMI_1, K_SEMI_2, K_SEMI_3, K_DITHERED, K_MASK_CHECK, K_MASK_SET,
    K_SKIPPED_OVERSIZE, K_UNKNOWN_COMMAND, K_KIND_COUNT
};

inline const char* kind_name(int kind) {
    static const char* names[K_KIND_COUNT] = {
        "POLY_F3", "POLY_F4", "POLY_G3", "POLY_G4", "POLY_FT3", "POLY_FT4", "POLY_GT3", "POLY_GT4",
        "LINE_F2", "LINE_G2", "LINE_F_POLY", "LINE_G_POLY",
        "TILE", "TILE_1", "TILE_8", "TILE_16", "SPRT", "SPRT_1", "SPRT_8", "SPRT_16",
        "FILL", "COPY_VRAM", "LOAD_IMAGE", "STORE_IMAGE",
        "ENV_TPAGE", "ENV_TWINDOW", "ENV_AREA_TL", "ENV_AREA_BR", "ENV_OFFSET", "ENV_MASK",
        "TEX_4BIT", "TEX_8BIT", "TEX_15BIT", "RAW_TEXTURE", "MODULATED_TEXTURE",
        "SEMI_0_AVG", "SEMI_1_ADD", "SEMI_2_SUB", "SEMI_3_QUARTER", "DITHERED", "MASK_CHECK", "MASK_SET",
        "SKIPPED_OVERSIZE", "UNKNOWN_COMMAND"};
    return kind >= 0 && kind < K_KIND_COUNT ? names[kind] : "?";
}

struct Environment {
    int area_x1 = 0, area_y1 = 0, area_x2 = 1023, area_y2 = 511;
    int offset_x = 0, offset_y = 0;
    int tpage_x = 0;          // texture page base, in units of 64 halfwords (0..15)
    int tpage_y = 0;          // 0 or 256
    int semi_mode = 0;        // 0: (B+F)/2, 1: B+F, 2: B-F, 3: B+F/4
    int tex_depth = 0;        // 0: 4-bit CLUT, 1: 8-bit CLUT, 2 and 3: 15-bit direct
    bool dither = false;
    bool draw_to_display = false;
    bool texture_disable = false;
    bool flip_x = false, flip_y = false;
    int window_mask_x = 0, window_mask_y = 0, window_off_x = 0, window_off_y = 0;  // 8-pixel units
    bool mask_set = false, mask_check = false;
};

struct DisplayState {
    int start_x = 0, start_y = 0;
    int width = 320, height = 240;
    bool enabled = true;
    bool true_color = false;
};

// Bounding box of the pixels a run of commands wrote (empty when x1 < x0).
struct DirtyRect {
    int x0 = 1024, y0 = 512, x1 = -1, y1 = -1;
    void add(int x, int y) { x0 = std::min(x0, x); y0 = std::min(y0, y); x1 = std::max(x1, x); y1 = std::max(y1, y); }
    bool empty() const { return x1 < x0 || y1 < y0; }
};

struct WalkResult {
    uint32_t nodes = 0;
    uint32_t words = 0;
    bool terminated = false;  // reached the end marker (bit 23 of a next pointer)
    bool node_limit = false;
};

class Gpu {
public:
    Gpu() : vram_(size_t(VRAM_WIDTH) * VRAM_HEIGHT, 0) { counts_.fill(0); }

    void reset() {
        std::fill(vram_.begin(), vram_.end(), uint16_t(0));
        env_ = Environment();
        display_ = DisplayState();
        mode_ = M_COMMAND;
        param_count_ = 0;
        counts_.fill(0);
        dirty_ = DirtyRect();
    }

    // ---- VRAM access ------------------------------------------------------------------------
    uint16_t* vram() { return vram_.data(); }
    const uint16_t* vram() const { return vram_.data(); }
    uint16_t pixel(int x, int y) const { return vram_[size_t(y & 511) * VRAM_WIDTH + (x & 1023)]; }

    // CPU to VRAM rectangle (the effect of GP0(A0h) plus its data words). Coordinates wrap at the
    // VRAM edges; mask check and mask set apply like they do for the GP0 command.
    void load_image(int x, int y, int w, int h, const uint16_t* data) {
        w = fix_extent(w, 1024); h = fix_extent(h, 512);
        for (int j = 0; j < h; ++j)
            for (int i = 0; i < w; ++i)
                put_transfer(x + i, y + j, data[size_t(j) * w + i]);
        ++counts_[K_LOAD_IMAGE];
    }
    void store_image(int x, int y, int w, int h, uint16_t* out) const {
        w = fix_extent(w, 1024); h = fix_extent(h, 512);
        for (int j = 0; j < h; ++j)
            for (int i = 0; i < w; ++i) out[size_t(j) * w + i] = pixel(x + i, y + j);
    }
    // VRAM to VRAM copy (GP0(80h)): pixel by pixel in row-major order, reading the source live, so an
    // overlapping copy sees pixels it already wrote (the hardware streams the rectangle; the
    // reference emulator does the same).
    void move_image(int sx, int sy, int dx, int dy, int w, int h) {
        w = fix_extent(w, 1024); h = fix_extent(h, 512);
        for (int j = 0; j < h; ++j)
            for (int i = 0; i < w; ++i) put_transfer(dx + i, dy + j, pixel(sx + i, sy + j));
        ++counts_[K_COPY_VRAM];
    }
    // Fill rectangle (GP0(02h)): ignores draw area, mask bits and texture state. x is rounded down
    // to 16 pixels, width up to 16 pixels, as the hardware does.
    void fill_rect(int x, int y, int w, int h, uint32_t rgb24) {
        x &= 0x3F0; y &= 0x1FF;
        w = ((w & 0x3FF) + 0xF) & ~0xF; h &= 0x1FF;
        const uint16_t c = uint16_t(((rgb24 & 0xFF) >> 3) | (((rgb24 >> 8) & 0xFF) >> 3) << 5 |
                                    (((rgb24 >> 16) & 0xFF) >> 3) << 10);
        if (w > 0 && h > 0) { dirty_.add(x, y); dirty_.add((x + w - 1) & 1023, (y + h - 1) & 511); }
        for (int j = 0; j < h; ++j)
            for (int i = 0; i < w; ++i) vram_[size_t((y + j) & 511) * VRAM_WIDTH + ((x + i) & 1023)] = c;
        ++counts_[K_FILL];
    }

    // ---- environment ------------------------------------------------------------------------
    const Environment& environment() const { return env_; }
    Environment& environment() { return env_; }
    const DisplayState& display() const { return display_; }
    DisplayState& display() { return display_; }
    uint64_t count(int kind) const { return counts_[kind]; }
    const std::array<uint64_t, K_KIND_COUNT>& counts() const { return counts_; }
    void clear_counts() { counts_.fill(0); }
    const DirtyRect& dirty() const { return dirty_; }
    void clear_dirty() { dirty_ = DirtyRect(); }

    // ---- GP1 (display control) --------------------------------------------------------------
    void gp1(uint32_t word) {
        const uint32_t arg = word & 0xFFFFFF;
        switch (word >> 24) {
            case 0x00:  // reset: environment and display defaults, VRAM kept
                env_ = Environment(); display_ = DisplayState(); mode_ = M_COMMAND; break;
            case 0x01: mode_ = M_COMMAND; break;
            case 0x03: display_.enabled = (arg & 1) == 0; break;
            case 0x05: display_.start_x = int(arg & 0x3FF); display_.start_y = int((arg >> 10) & 0x1FF); break;
            case 0x08: {
                static const int widths[4] = {256, 320, 512, 640};
                display_.width = (arg & 0x40) ? 368 : widths[arg & 3];
                display_.height = ((arg & 4) && (arg & 0x20)) ? 480 : 240;
                display_.true_color = (arg & 0x10) != 0;
                break;
            }
            default: break;
        }
    }

    // ---- GP0 stream -------------------------------------------------------------------------
    void gp0_words(const uint32_t* words, size_t count) { for (size_t i = 0; i < count; ++i) gp0(words[i]); }

    void gp0(uint32_t word) {
        switch (mode_) {
            case M_PARAMS:
                params_[param_count_++] = word;
                if (param_count_ == param_need_) { mode_ = M_COMMAND; execute(); }
                return;
            case M_POLYLINE:
                if ((word & 0xF000F000) == 0x50005000) {  // terminator word
                    mode_ = M_COMMAND; execute_polyline(); return;
                }
                if (param_count_ >= params_.size()) params_.resize(params_.size() * 2);
                params_[param_count_++] = word;
                return;
            case M_LOAD_DATA: receive_load(word); return;
            default: break;
        }
        const uint32_t cmd = word >> 24;
        params_[0] = word;
        param_count_ = 1;
        if (cmd >= 0x20 && cmd <= 0x3F) {
            const bool quad = cmd & 0x08, tex = cmd & 0x04, gour = cmd & 0x10;
            const int n = quad ? 4 : 3;
            param_need_ = 1 + n * (1 + (tex ? 1 : 0)) + (gour ? n - 1 : 0);
            begin_params();
        } else if (cmd >= 0x40 && cmd <= 0x5F) {
            if (cmd & 0x08) {  // polyline: runs until a terminator word
                mode_ = M_POLYLINE;
            } else {
                param_need_ = (cmd & 0x10) ? 4 : 3;
                begin_params();
            }
        } else if (cmd >= 0x60 && cmd <= 0x7F) {
            param_need_ = 2 + ((cmd & 0x04) ? 1 : 0) + (((cmd >> 3) & 3) == 0 ? 1 : 0);
            begin_params();
        } else if (cmd == 0x02) { param_need_ = 3; begin_params(); }
        else if (cmd == 0x80) { param_need_ = 4; begin_params(); }
        else if (cmd == 0xA0 || cmd == 0xC0) { param_need_ = 3; begin_params(); }
        else if (cmd >= 0xE1 && cmd <= 0xE6) { environment_word(word); }
        else if (cmd == 0x00 || cmd == 0x01 || cmd == 0x1F || (cmd >= 0x03 && cmd <= 0x1E)) { /* no-op, cache clear, IRQ */ }
        else { ++counts_[K_UNKNOWN_COMMAND]; }
    }

    // Walk a DrawOTag-style linked list in `ram` (little-endian words) and feed each packet's
    // payload to the GP0 stream. A node is a tag word (next pointer in the low 24 bits, payload
    // word count in the high 8 bits) followed by that many words. The walk ends after the node
    // whose next pointer has bit 23 set (0xFFFFFF in the game). `ram_size` must be a power of two;
    // addresses are masked into it (KSEG0, KUSEG and KSEG1 alias the same RAM).
    WalkResult draw_otag(const uint8_t* ram, size_t ram_size, uint32_t address, uint32_t max_nodes = 1u << 20) {
        WalkResult result;
        const uint32_t mask = uint32_t(ram_size - 1) & ~3u;
        uint32_t node = address & mask;
        for (;;) {
            if (result.nodes >= max_nodes) { result.node_limit = true; break; }
            const uint32_t tag = read32(ram, node);
            ++result.nodes;
            const uint32_t length = tag >> 24;
            for (uint32_t i = 1; i <= length; ++i) gp0(read32(ram, (node + 4 * i) & mask));
            result.words += length;
            if (tag & 0x800000) { result.terminated = true; break; }
            node = tag & mask;
        }
        return result;
    }

    // ---- display read-out -------------------------------------------------------------------
    // RGBA8 for a VRAM rectangle (15-bit pixels). replicate_bits selects the 5 to 8 bit rule:
    // false: c5 << 3 (the emulator screenshots and most presenters of the port), true: bit
    // replication (c5 << 3) | (c5 >> 2). Alpha is 255. Rows and columns wrap at the VRAM edge.
    void read_rgba(int x, int y, int w, int h, uint8_t* out, bool replicate_bits = false) const {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                const uint16_t p = pixel(x + i, y + j);
                int r = p & 31, g = (p >> 5) & 31, b = (p >> 10) & 31;
                if (replicate_bits) { r = (r << 3) | (r >> 2); g = (g << 3) | (g >> 2); b = (b << 3) | (b >> 2); }
                else { r <<= 3; g <<= 3; b <<= 3; }
                uint8_t* o = out + (size_t(j) * w + i) * 4;
                o[0] = uint8_t(r); o[1] = uint8_t(g); o[2] = uint8_t(b); o[3] = 255;
            }
        }
    }

private:
    enum Mode { M_COMMAND, M_PARAMS, M_POLYLINE, M_LOAD_DATA };

    struct Vertex {
        int x = 0, y = 0;
        int r = 0, g = 0, b = 0;
        int u = 0, v = 0;
    };
    struct Prim {
        bool textured = false, raw = false, semi = false, gouraud = false, dither = false;
        int clut_x = 0, clut_y = 0;
    };

    std::vector<uint16_t> vram_;
    Environment env_;
    DisplayState display_;
    Mode mode_ = M_COMMAND;
    std::vector<uint32_t> params_ = std::vector<uint32_t>(16);
    uint32_t param_count_ = 0, param_need_ = 0;
    std::array<uint64_t, K_KIND_COUNT> counts_;
    DirtyRect dirty_;
    // CPU to VRAM transfer in progress
    int load_x_ = 0, load_y_ = 0, load_w_ = 0, load_h_ = 0, load_i_ = 0;
    uint32_t load_total_ = 0, load_done_ = 0;

    static uint32_t read32(const uint8_t* ram, uint32_t at) {
        return uint32_t(ram[at]) | uint32_t(ram[at + 1]) << 8 | uint32_t(ram[at + 2]) << 16 | uint32_t(ram[at + 3]) << 24;
    }
    static int fix_extent(int v, int limit) { return ((v - 1) & (limit - 1)) + 1; }
    static int sext11(uint32_t v) { return int(int32_t(v << 21) >> 21); }

    void begin_params() { mode_ = M_PARAMS; }

    // Transfer-style write: mask check and mask set apply, no draw-area clip.
    void put_transfer(int x, int y, uint16_t value) {
        uint16_t& dst = vram_[size_t(y & 511) * VRAM_WIDTH + (x & 1023)];
        if (env_.mask_check && (dst & 0x8000)) return;
        dirty_.add(x & 1023, y & 511);
        dst = uint16_t(value | (env_.mask_set ? 0x8000 : 0));
    }

    void receive_load(uint32_t word) {
        for (int half = 0; half < 2 && load_done_ < load_total_; ++half) {
            const uint16_t value = uint16_t(word >> (16 * half));
            put_transfer(load_x_ + load_i_ % load_w_, load_y_ + load_i_ / load_w_, value);
            ++load_i_; ++load_done_;
        }
        if (load_done_ >= load_total_) mode_ = M_COMMAND;
    }

    void environment_word(uint32_t word) {
        switch (word >> 24) {
            case 0xE1:
                env_.tpage_x = int(word & 15); env_.tpage_y = (word & 0x10) ? 256 : 0;
                env_.semi_mode = int((word >> 5) & 3); env_.tex_depth = int((word >> 7) & 3);
                env_.dither = (word & 0x200) != 0; env_.draw_to_display = (word & 0x400) != 0;
                env_.texture_disable = (word & 0x800) != 0;
                env_.flip_x = (word & 0x1000) != 0; env_.flip_y = (word & 0x2000) != 0;
                ++counts_[K_ENV_TPAGE]; break;
            case 0xE2:
                env_.window_mask_x = int(word & 31); env_.window_mask_y = int((word >> 5) & 31);
                env_.window_off_x = int((word >> 10) & 31); env_.window_off_y = int((word >> 15) & 31);
                ++counts_[K_ENV_TWINDOW]; break;
            case 0xE3: env_.area_x1 = int(word & 0x3FF); env_.area_y1 = int((word >> 10) & 0x1FF); ++counts_[K_ENV_AREA_TL]; break;
            case 0xE4: env_.area_x2 = int(word & 0x3FF); env_.area_y2 = int((word >> 10) & 0x1FF); ++counts_[K_ENV_AREA_BR]; break;
            case 0xE5: env_.offset_x = sext11(word & 0x7FF); env_.offset_y = sext11((word >> 11) & 0x7FF); ++counts_[K_ENV_OFFSET]; break;
            case 0xE6: env_.mask_set = (word & 1) != 0; env_.mask_check = (word & 2) != 0; ++counts_[K_ENV_MASK]; break;
            default: break;
        }
    }

    // Texture-page attribute that rides in a textured polygon (high half of the second UV word).
    void apply_polygon_tpage(uint32_t attr) {
        env_.tpage_x = int(attr & 15); env_.tpage_y = (attr & 0x10) ? 256 : 0;
        env_.semi_mode = int((attr >> 5) & 3); env_.tex_depth = int((attr >> 7) & 3);
    }

    void execute() {
        const uint32_t cmd = params_[0] >> 24;
        if (cmd >= 0x20 && cmd <= 0x3F) { execute_polygon(cmd); return; }
        if (cmd >= 0x40 && cmd <= 0x5F) { execute_line(cmd); return; }
        if (cmd >= 0x60 && cmd <= 0x7F) { execute_rect(cmd); return; }
        switch (cmd) {
            case 0x02: fill_rect(int(params_[1] & 0xFFFF), int(params_[1] >> 16), int(params_[2] & 0xFFFF),
                                 int(params_[2] >> 16), params_[0] & 0xFFFFFF); break;
            case 0x80: move_image(int(params_[1] & 0xFFFF), int(params_[1] >> 16), int(params_[2] & 0xFFFF),
                                  int(params_[2] >> 16), int(params_[3] & 0xFFFF), int(params_[3] >> 16)); break;
            case 0xA0:
                load_x_ = int(params_[1] & 0x3FF); load_y_ = int((params_[1] >> 16) & 0x1FF);
                load_w_ = fix_extent(int(params_[2] & 0xFFFF), 1024); load_h_ = fix_extent(int(params_[2] >> 16), 512);
                load_i_ = 0; load_done_ = 0; load_total_ = uint32_t(load_w_) * uint32_t(load_h_);
                mode_ = M_LOAD_DATA; ++counts_[K_LOAD_IMAGE]; break;
            case 0xC0: ++counts_[K_STORE_IMAGE]; break;  // read-back happens through store_image()
            default: break;
        }
    }

    // ---- color and texture pipeline ---------------------------------------------------------
    static int clamp8(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }
    static int dither_offset(int x, int y) {
        static const int8_t table[4][4] = {{-4, 0, -3, 1}, {2, -2, 3, -1}, {-3, 1, -4, 0}, {3, -1, 2, -2}};
        return table[y & 3][x & 3];
    }

    uint16_t fetch_texel(const Prim& p, int u, int v) const {
        u &= 0xFF; v &= 0xFF;
        u = (u & ~(env_.window_mask_x * 8)) | ((env_.window_off_x & env_.window_mask_x) * 8);
        v = (v & ~(env_.window_mask_y * 8)) | ((env_.window_off_y & env_.window_mask_y) * 8);
        const int base_x = env_.tpage_x * 64, base_y = env_.tpage_y;
        if (env_.tex_depth == 0) {
            const uint16_t word = pixel(base_x + (u >> 2), base_y + v);
            return pixel(p.clut_x + ((word >> ((u & 3) * 4)) & 0xF), p.clut_y);
        }
        if (env_.tex_depth == 1) {
            const uint16_t word = pixel(base_x + (u >> 1), base_y + v);
            return pixel(p.clut_x + ((word >> ((u & 1) * 8)) & 0xFF), p.clut_y);
        }
        return pixel(base_x + u, base_y + v);
    }

    // Shade and store one pixel. (cr, cg, cb) is the 8-bit vertex or flat colour, (u, v) the
    // interpolated texel coordinate. px, py are VRAM coordinates already inside the draw area.
    //
    // Colour path: the 8-bit result of modulation (texel * colour / 128, clamped) or of the vertex
    // colour is dithered and truncated to 5 bits. A semi-transparent pixel without dithering blends
    // the truncated 5-bit source with the 5-bit background. With dithering the blend runs at 8-bit
    // precision (background << 3) and the dither offset is added to the blended value, as the
    // reference emulator does; the hardware order is not documented in a form that settles it.
    void plot(int px, int py, int cr, int cg, int cb, int u, int v, const Prim& p) {
        bool blend = p.semi;
        uint16_t texbit = 0;
        int r8, g8, b8;
        if (p.textured) {
            const uint16_t texel = fetch_texel(p, u, v);
            if (texel == 0) return;  // an all-zero texel is transparent
            blend = p.semi && (texel & 0x8000);
            texbit = texel & 0x8000;
            const int tr = texel & 31, tg = (texel >> 5) & 31, tb = (texel >> 10) & 31;
            if (p.raw) { r8 = tr << 3; g8 = tg << 3; b8 = tb << 3; }
            else { r8 = clamp8((tr * cr) >> 4); g8 = clamp8((tg * cg) >> 4); b8 = clamp8((tb * cb) >> 4); }
        } else {
            r8 = cr; g8 = cg; b8 = cb;
        }
        uint16_t& dst = vram_[size_t(py) * VRAM_WIDTH + px];
        if (env_.mask_check && (dst & 0x8000)) return;
        const int d = p.dither ? dither_offset(px, py) : 0;
        int r5, g5, b5;
        if (blend && p.dither) {
            r5 = clamp8(mix8((dst & 31) << 3, r8) + d) >> 3;
            g5 = clamp8(mix8(((dst >> 5) & 31) << 3, g8) + d) >> 3;
            b5 = clamp8(mix8(((dst >> 10) & 31) << 3, b8) + d) >> 3;
        } else {
            r5 = clamp8(r8 + d) >> 3; g5 = clamp8(g8 + d) >> 3; b5 = clamp8(b8 + d) >> 3;
            if (blend) {
                r5 = mix(dst & 31, r5); g5 = mix((dst >> 5) & 31, g5); b5 = mix((dst >> 10) & 31, b5);
            }
        }
        dirty_.add(px, py);
        dst = uint16_t(r5 | g5 << 5 | b5 << 10 | texbit | (env_.mask_set ? 0x8000 : 0));
    }

    int mix8(int back, int front) const {
        switch (env_.semi_mode) {
            case 0: return (back + front) >> 1;
            case 1: return std::min(255, back + front);
            case 2: return std::max(0, back - front);
            default: return std::min(255, back + (front >> 2));
        }
    }

    int mix(int back, int front) const {
        switch (env_.semi_mode) {
            case 0: return (back + front) >> 1;
            case 1: return std::min(31, back + front);
            case 2: return std::max(0, back - front);
            default: return std::min(31, back + (front >> 2));
        }
    }

    void note_prim(const Prim& p) {
        if (p.semi) ++counts_[K_SEMI_0 + env_.semi_mode];
        if (p.dither) ++counts_[K_DITHERED];
        if (env_.mask_check) ++counts_[K_MASK_CHECK];
        if (env_.mask_set) ++counts_[K_MASK_SET];
        if (p.textured) {
            ++counts_[env_.tex_depth == 0 ? K_TEX_4BIT : (env_.tex_depth == 1 ? K_TEX_8BIT : K_TEX_15BIT)];
            ++counts_[p.raw ? K_RAW_TEXTURE : K_MODULATED_TEXTURE];
        }
    }

    // ---- polygons ---------------------------------------------------------------------------
    void execute_polygon(uint32_t cmd) {
        const bool raw = cmd & 1, semi = cmd & 2, tex = cmd & 4, quad = cmd & 8, gour = cmd & 16;
        const int n = quad ? 4 : 3;
        Vertex v[4];
        Prim p;
        p.textured = tex && !env_.texture_disable; p.raw = raw; p.semi = semi; p.gouraud = gour;
        int at = 0;
        uint32_t color = params_[at++] & 0xFFFFFF;
        for (int i = 0; i < n; ++i) {
            if (i > 0 && gour) color = params_[at++] & 0xFFFFFF;
            v[i].r = int(color & 0xFF); v[i].g = int((color >> 8) & 0xFF); v[i].b = int((color >> 16) & 0xFF);
            const uint32_t xy = params_[at++];
            v[i].x = sext11(xy & 0x7FF) + env_.offset_x;
            v[i].y = sext11((xy >> 16) & 0x7FF) + env_.offset_y;
            if (tex) {
                const uint32_t uv = params_[at++];
                v[i].u = int(uv & 0xFF); v[i].v = int((uv >> 8) & 0xFF);
                if (i == 0) { p.clut_x = int((uv >> 16) & 0x3F) * 16; p.clut_y = int((uv >> 22) & 0x1FF); }
                if (i == 1) apply_polygon_tpage(uv >> 16);
            }
        }
        p.dither = env_.dither && (gour || (p.textured && !raw));
        static const Kind kinds[2][2][2] = {{{K_POLY_F3, K_POLY_F4}, {K_POLY_FT3, K_POLY_FT4}},
                                            {{K_POLY_G3, K_POLY_G4}, {K_POLY_GT3, K_POLY_GT4}}};
        ++counts_[kinds[gour ? 1 : 0][tex ? 1 : 0][quad ? 1 : 0]];
        note_prim(p);
        triangle(v[0], v[1], v[2], p);
        if (quad) triangle(v[1], v[2], v[3], p);
    }

    static int64_t edge(const Vertex& a, const Vertex& b, int x, int y) {
        return int64_t(b.x - a.x) * (y - a.y) - int64_t(b.y - a.y) * (x - a.x);
    }
    // Top-left rule for a triangle whose edge function is positive inside (screen y grows down):
    // an edge that runs up (dy < 0) is a left edge, one that is horizontal and runs right is a top
    // edge. Pixels exactly on any other edge belong to the neighbour.
    static bool top_left(const Vertex& a, const Vertex& b) {
        const int dy = b.y - a.y, dx = b.x - a.x;
        return dy < 0 || (dy == 0 && dx > 0);
    }

    // Attributes (colour, texel coordinate) are interpolated from the integer barycentric weights
    // the fill rule already computes: value = (w0*va + w1*vb + w2*vc) / area, rounded to nearest
    // with ties up. Exact in integers, so an attribute that is an integer at the pixel stays one.
    static int interpolate(int64_t w0, int64_t w1, int64_t w2, int64_t area, int va, int vb, int vc) {
        const int64_t numerator = w0 * va + w1 * vb + w2 * vc;
        return int((numerator + area / 2) / area);  // weights and attributes are non-negative
    }

    void triangle(Vertex a, Vertex b, Vertex c, const Prim& p) {
        const int minx = std::min({a.x, b.x, c.x}), maxx = std::max({a.x, b.x, c.x});
        const int miny = std::min({a.y, b.y, c.y}), maxy = std::max({a.y, b.y, c.y});
        if (maxx - minx > 1023 || maxy - miny > 511) { ++counts_[K_SKIPPED_OVERSIZE]; return; }
        int64_t area = edge(a, b, c.x, c.y);
        if (area == 0) return;
        if (area < 0) { std::swap(b, c); area = -area; }
        const int x0 = std::max(minx, std::max(env_.area_x1, 0)), x1 = std::min(maxx, std::min(env_.area_x2, 1023));
        const int y0 = std::max(miny, std::max(env_.area_y1, 0)), y1 = std::min(maxy, std::min(env_.area_y2, 511));
        if (x0 > x1 || y0 > y1) return;
        const bool flat = !p.gouraud;
        const bool tl0 = top_left(b, c), tl1 = top_left(c, a), tl2 = top_left(a, b);
        for (int y = y0; y <= y1; ++y) {
            for (int x = x0; x <= x1; ++x) {
                const int64_t w0 = edge(b, c, x, y), w1 = edge(c, a, x, y), w2 = edge(a, b, x, y);
                if (w0 < 0 || w1 < 0 || w2 < 0) continue;
                if ((w0 == 0 && !tl0) || (w1 == 0 && !tl1) || (w2 == 0 && !tl2)) continue;
                int cr = a.r, cg = a.g, cb = a.b;
                if (!flat) {
                    cr = clamp8(interpolate(w0, w1, w2, area, a.r, b.r, c.r));
                    cg = clamp8(interpolate(w0, w1, w2, area, a.g, b.g, c.g));
                    cb = clamp8(interpolate(w0, w1, w2, area, a.b, b.b, c.b));
                }
                int u = 0, v = 0;
                if (p.textured) {
                    u = interpolate(w0, w1, w2, area, a.u, b.u, c.u);
                    v = interpolate(w0, w1, w2, area, a.v, b.v, c.v);
                }
                plot(x, y, cr, cg, cb, u, v, p);
            }
        }
    }

    // ---- lines ------------------------------------------------------------------------------
    // Lines run along the major axis one pixel per step, both end points included; the minor axis
    // is a 16.16 DDA started half a pixel in. Gouraud colours interpolate along the same steps.
    void line(const Vertex& a, const Vertex& b, const Prim& p) {
        const int dx = b.x - a.x, dy = b.y - a.y;
        const int adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
        if (adx > 1023 || ady > 511) { ++counts_[K_SKIPPED_OVERSIZE]; return; }
        const int k = std::max(adx, ady);
        for (int i = 0; i <= k; ++i) {
            int x, y;
            if (k == 0) { x = a.x; y = a.y; }
            else if (adx >= ady) {
                x = a.x + (dx < 0 ? -i : i);
                y = a.y + int((int64_t(dy) * i * 2 + (dy >= 0 ? k : -k)) / (2 * int64_t(k)));
            } else {
                y = a.y + (dy < 0 ? -i : i);
                x = a.x + int((int64_t(dx) * i * 2 + (dx >= 0 ? k : -k)) / (2 * int64_t(k)));
            }
            if (x < env_.area_x1 || x > env_.area_x2 || y < env_.area_y1 || y > env_.area_y2 ||
                x < 0 || x > 1023 || y < 0 || y > 511) continue;
            int cr = a.r, cg = a.g, cb = a.b;
            if (p.gouraud && k > 0) {
                cr = a.r + int((int64_t(b.r - a.r) * i) / k); cg = a.g + int((int64_t(b.g - a.g) * i) / k);
                cb = a.b + int((int64_t(b.b - a.b) * i) / k);
            }
            plot(x, y, cr, cg, cb, 0, 0, p);
        }
    }

    Vertex line_vertex(uint32_t color, uint32_t xy) const {
        Vertex v;
        v.r = int(color & 0xFF); v.g = int((color >> 8) & 0xFF); v.b = int((color >> 16) & 0xFF);
        v.x = sext11(xy & 0x7FF) + env_.offset_x; v.y = sext11((xy >> 16) & 0x7FF) + env_.offset_y;
        return v;
    }

    void execute_line(uint32_t cmd) {  // single segment (F2 or G2)
        Prim p;
        p.semi = cmd & 2; p.gouraud = cmd & 0x10; p.dither = env_.dither && p.gouraud;
        Vertex a, b;
        if (p.gouraud) {
            a = line_vertex(params_[0] & 0xFFFFFF, params_[1]); b = line_vertex(params_[2] & 0xFFFFFF, params_[3]);
        } else {
            a = line_vertex(params_[0] & 0xFFFFFF, params_[1]); b = line_vertex(params_[0] & 0xFFFFFF, params_[2]);
            b.r = a.r; b.g = a.g; b.b = a.b;
        }
        ++counts_[p.gouraud ? K_LINE_G2 : K_LINE_F2];
        note_prim(p);
        line(a, b, p);
    }

    void execute_polyline() {
        const uint32_t cmd = params_[0] >> 24;
        Prim p;
        p.semi = cmd & 2; p.gouraud = cmd & 0x10; p.dither = env_.dither && p.gouraud;
        ++counts_[p.gouraud ? K_LINE_G_POLY : K_LINE_F_POLY];
        note_prim(p);
        const uint32_t base_color = params_[0] & 0xFFFFFF;
        std::vector<Vertex> pts;
        if (p.gouraud) {
            pts.push_back(line_vertex(base_color, params_[1]));
            for (uint32_t i = 2; i + 1 < param_count_; i += 2) pts.push_back(line_vertex(params_[i] & 0xFFFFFF, params_[i + 1]));
        } else {
            for (uint32_t i = 1; i < param_count_; ++i) pts.push_back(line_vertex(base_color, params_[i]));
        }
        for (size_t i = 0; i + 1 < pts.size(); ++i) line(pts[i], pts[i + 1], p);
    }

    // ---- rectangles -------------------------------------------------------------------------
    void execute_rect(uint32_t cmd) {
        const bool raw = cmd & 1, semi = cmd & 2, tex = cmd & 4;
        const int size = int((cmd >> 3) & 3);
        Prim p;
        p.textured = tex && !env_.texture_disable; p.raw = raw; p.semi = semi;
        int at = 0;
        const uint32_t color = params_[at++] & 0xFFFFFF;
        const uint32_t xy = params_[at++];
        int u0 = 0, v0 = 0;
        if (tex) {
            const uint32_t uv = params_[at++];
            u0 = int(uv & 0xFF); v0 = int((uv >> 8) & 0xFF);
            p.clut_x = int((uv >> 16) & 0x3F) * 16; p.clut_y = int((uv >> 22) & 0x1FF);
        }
        int w, h;
        if (size == 0) { const uint32_t wh = params_[at]; w = int(wh & 0x3FF); h = int((wh >> 16) & 0x1FF); }
        else { w = h = size == 1 ? 1 : (size == 2 ? 8 : 16); }
        static const Kind kinds[2][4] = {{K_TILE, K_TILE_1, K_TILE_8, K_TILE_16}, {K_SPRT, K_SPRT_1, K_SPRT_8, K_SPRT_16}};
        ++counts_[kinds[tex ? 1 : 0][size]];
        note_prim(p);
        const int x = sext11(xy & 0x7FF) + env_.offset_x, y = sext11((xy >> 16) & 0x7FF) + env_.offset_y;
        const int cr = int(color & 0xFF), cg = int((color >> 8) & 0xFF), cb = int((color >> 16) & 0xFF);
        for (int j = 0; j < h; ++j) {
            const int py = y + j;
            if (py < env_.area_y1 || py > env_.area_y2 || py < 0 || py > 511) continue;
            const int v = (v0 + (env_.flip_y ? -j : j)) & 0xFF;
            for (int i = 0; i < w; ++i) {
                const int px = x + i;
                if (px < env_.area_x1 || px > env_.area_x2 || px < 0 || px > 1023) continue;
                const int u = (u0 + (env_.flip_x ? -i : i)) & 0xFF;
                plot(px, py, cr, cg, cb, u, v, p);
            }
        }
    }
};

}  // namespace ps1
