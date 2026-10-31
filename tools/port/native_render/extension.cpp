// T-39302 Godot binding for the PS1 GPU model (gpu.hpp). The class owns one VRAM and one drawing
// environment; it reads the game RAM that GDScript hands it and never writes anything but its own
// VRAM. Presentation only.
#include "gpu.hpp"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/rect2i.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class OriginalGpu : public RefCounted {
    GDCLASS(OriginalGpu, RefCounted)
    ps1::Gpu gpu;

    static bool valid_rect(int w, int h, int64_t limit_w, int64_t limit_h) {
        return w >= 1 && h >= 1 && w <= limit_w && h <= limit_h;
    }

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("reset"), &OriginalGpu::reset);
        ClassDB::bind_method(D_METHOD("set_vram", "bytes"), &OriginalGpu::set_vram);
        ClassDB::bind_method(D_METHOD("get_vram"), &OriginalGpu::get_vram);
        ClassDB::bind_method(D_METHOD("load_image", "x", "y", "w", "h", "bytes"), &OriginalGpu::load_image);
        ClassDB::bind_method(D_METHOD("store_image", "x", "y", "w", "h"), &OriginalGpu::store_image);
        ClassDB::bind_method(D_METHOD("move_image", "sx", "sy", "dx", "dy", "w", "h"), &OriginalGpu::move_image);
        ClassDB::bind_method(D_METHOD("fill_rect", "x", "y", "w", "h", "rgb24"), &OriginalGpu::fill_rect);
        ClassDB::bind_method(D_METHOD("gp0_word", "word"), &OriginalGpu::gp0_word);
        ClassDB::bind_method(D_METHOD("gp0_words", "bytes"), &OriginalGpu::gp0_words);
        ClassDB::bind_method(D_METHOD("gp1", "word"), &OriginalGpu::gp1);
        ClassDB::bind_method(D_METHOD("draw_otag", "ram", "address", "max_nodes"), &OriginalGpu::draw_otag, DEFVAL(1048576));
        ClassDB::bind_method(D_METHOD("read_rgba", "x", "y", "w", "h", "replicate_bits"), &OriginalGpu::read_rgba, DEFVAL(false));
        ClassDB::bind_method(D_METHOD("display_info"), &OriginalGpu::display_info);
        ClassDB::bind_method(D_METHOD("environment"), &OriginalGpu::environment);
        ClassDB::bind_method(D_METHOD("counters"), &OriginalGpu::counters);
        ClassDB::bind_method(D_METHOD("clear_counters"), &OriginalGpu::clear_counters);
        ClassDB::bind_method(D_METHOD("dirty_rect"), &OriginalGpu::dirty_rect);
        ClassDB::bind_method(D_METHOD("clear_dirty"), &OriginalGpu::clear_dirty);
    }

public:
    void reset() { gpu.reset(); }

    // Whole VRAM as 1024x512 little-endian 16-bit words. Returns an error text, "" on success.
    String set_vram(const PackedByteArray& bytes) {
        if (bytes.size() != int64_t(ps1::VRAM_WIDTH) * ps1::VRAM_HEIGHT * 2) return "VRAM image must be 1048576 bytes";
        std::memcpy(gpu.vram(), bytes.ptr(), size_t(bytes.size()));
        return "";
    }
    PackedByteArray get_vram() const {
        PackedByteArray out;
        out.resize(int64_t(ps1::VRAM_WIDTH) * ps1::VRAM_HEIGHT * 2);
        std::memcpy(out.ptrw(), gpu.vram(), size_t(out.size()));
        return out;
    }
    String load_image(int x, int y, int w, int h, const PackedByteArray& bytes) {
        if (!valid_rect(w, h, 1024, 512)) return "rectangle must be 1..1024 by 1..512";
        if (bytes.size() != int64_t(w) * h * 2) return "image data must be w*h little-endian 16-bit words";
        std::vector<uint16_t> data(size_t(w) * h);
        std::memcpy(data.data(), bytes.ptr(), size_t(bytes.size()));
        gpu.load_image(x, y, w, h, data.data());
        return "";
    }
    PackedByteArray store_image(int x, int y, int w, int h) const {
        PackedByteArray out;
        if (!valid_rect(w, h, 1024, 512)) return out;
        out.resize(int64_t(w) * h * 2);
        gpu.store_image(x, y, w, h, reinterpret_cast<uint16_t*>(out.ptrw()));
        return out;
    }
    String move_image(int sx, int sy, int dx, int dy, int w, int h) {
        if (!valid_rect(w, h, 1024, 512)) return "rectangle must be 1..1024 by 1..512";
        gpu.move_image(sx, sy, dx, dy, w, h);
        return "";
    }
    void fill_rect(int x, int y, int w, int h, int64_t rgb24) { gpu.fill_rect(x, y, w, h, uint32_t(rgb24 & 0xFFFFFF)); }
    void gp0_word(int64_t word) { gpu.gp0(uint32_t(word & 0xFFFFFFFF)); }
    // A raw GP0 word stream, little-endian 32-bit words.
    String gp0_words(const PackedByteArray& bytes) {
        if (bytes.size() % 4 != 0) return "GP0 stream must be a whole number of 32-bit words";
        const uint8_t* p = bytes.ptr();
        for (int64_t i = 0; i + 3 < bytes.size(); i += 4)
            gpu.gp0(uint32_t(p[i]) | uint32_t(p[i + 1]) << 8 | uint32_t(p[i + 2]) << 16 | uint32_t(p[i + 3]) << 24);
        return "";
    }
    void gp1(int64_t word) { gpu.gp1(uint32_t(word & 0xFFFFFFFF)); }

    // Walk the ordering table at `address` in `ram` (a power-of-two sized PackedByteArray, the
    // game's main RAM) exactly as DrawOTag's DMA would. Returns nodes, words, terminated, node_limit, error.
    Dictionary draw_otag(const PackedByteArray& ram, int64_t address, int64_t max_nodes) {
        Dictionary result;
        const int64_t size = ram.size();
        if (size < 8 || (size & (size - 1)) != 0) { result["error"] = "RAM size must be a power of two"; return result; }
        if (max_nodes < 1) { result["error"] = "max_nodes must be positive"; return result; }
        const ps1::WalkResult walk = gpu.draw_otag(ram.ptr(), size_t(size), uint32_t(address & 0xFFFFFFFF), uint32_t(max_nodes));
        result["nodes"] = int64_t(walk.nodes);
        result["words"] = int64_t(walk.words);
        result["terminated"] = walk.terminated;
        result["node_limit"] = walk.node_limit;
        result["error"] = "";
        return result;
    }

    // RGBA8 bytes (w*h*4) of a VRAM rectangle; empty when the size is invalid.
    PackedByteArray read_rgba(int x, int y, int w, int h, bool replicate_bits) const {
        PackedByteArray out;
        if (!valid_rect(w, h, 1024, 512)) return out;
        out.resize(int64_t(w) * h * 4);
        gpu.read_rgba(x, y, w, h, out.ptrw(), replicate_bits);
        return out;
    }

    // The display area the GP1 commands selected: x, y, width, height, enabled, true_color.
    Dictionary display_info() const {
        const ps1::DisplayState& d = gpu.display();
        Dictionary r;
        r["x"] = d.start_x; r["y"] = d.start_y; r["width"] = d.width; r["height"] = d.height;
        r["enabled"] = d.enabled; r["true_color"] = d.true_color;
        return r;
    }

    Dictionary environment() const {
        const ps1::Environment& e = gpu.environment();
        Dictionary r;
        r["area"] = Rect2i(e.area_x1, e.area_y1, e.area_x2 - e.area_x1 + 1, e.area_y2 - e.area_y1 + 1);
        r["offset_x"] = e.offset_x; r["offset_y"] = e.offset_y;
        r["tpage_x"] = e.tpage_x; r["tpage_y"] = e.tpage_y; r["semi_mode"] = e.semi_mode; r["tex_depth"] = e.tex_depth;
        r["dither"] = e.dither; r["draw_to_display"] = e.draw_to_display; r["texture_disable"] = e.texture_disable;
        r["flip_x"] = e.flip_x; r["flip_y"] = e.flip_y;
        r["window_mask_x"] = e.window_mask_x; r["window_mask_y"] = e.window_mask_y;
        r["window_off_x"] = e.window_off_x; r["window_off_y"] = e.window_off_y;
        r["mask_set"] = e.mask_set; r["mask_check"] = e.mask_check;
        return r;
    }

    // Primitive and command counts since the last clear, by name (see ps1::kind_name); zero entries omitted.
    Dictionary counters() const {
        Dictionary r;
        for (int k = 0; k < ps1::K_KIND_COUNT; ++k)
            if (gpu.count(k)) r[ps1::kind_name(k)] = int64_t(gpu.count(k));
        return r;
    }
    void clear_counters() { gpu.clear_counts(); }

    // Bounding box of the pixels written since the last clear; zero size when nothing was drawn.
    Rect2i dirty_rect() const {
        const ps1::DirtyRect& d = gpu.dirty();
        return d.empty() ? Rect2i() : Rect2i(d.x0, d.y0, d.x1 - d.x0 + 1, d.y1 - d.y0 + 1);
    }
    void clear_dirty() { gpu.clear_dirty(); }
};

void initialize_original_render(ModuleInitializationLevel level) {
    if (level == MODULE_INITIALIZATION_LEVEL_SCENE) ClassDB::register_class<OriginalGpu>();
}
void uninitialize_original_render(ModuleInitializationLevel) {}

extern "C" {
GDExtensionBool GDE_EXPORT original_render_init(GDExtensionInterfaceGetProcAddress get_proc, GDExtensionClassLibraryPtr library,
                                                GDExtensionInitialization* initialization) {
    GDExtensionBinding::InitObject init(get_proc, library, initialization);
    init.register_initializer(initialize_original_render);
    init.register_terminator(uninitialize_original_render);
    init.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
    return init.init();
}
}
