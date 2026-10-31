// T-39302: draw one captured ordering table with the renderer and compare the result with the
// emulator's VRAM after the same DrawOTag, pixel for pixel.
//
//   compare_capture --ram ram.bin --before vram_before.bin --after vram_after.bin --ot 800E949C
//                   [--status HEX] [--env-addr HEX] [--max-report N] [--vram-out FILE]
//
// ram.bin is the 2 MiB RAM image, the VRAM files are raw 1024x512 little-endian 16-bit words.
// --status is the GPUSTAT word of the emulator before the call (texture page, semi-transparency
// mode, dither and mask bits seed the environment). --env-addr is the address of the PutDrawEnv
// words (E3, E4, E5, ...) that libgpu keeps in RAM: they set the draw area and offset the GPU
// had when DrawOTag started. Prints one JSON object. No game data is embedded; inputs stay under
// .asset-cache and are never committed.
// Prints one JSON object. No game data is embedded; inputs stay under .asset-cache.
#include "gpu.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>

static std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) { std::fprintf(stderr, "cannot read %s\n", path.c_str()); std::exit(2); }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

int main(int argc, char** argv) {
    std::string ram_path, before_path, after_path;
    std::string vram_out;
    uint32_t ot = 0, status = 0, env_addr = 0;
    int max_report = 12;
    bool have_ot = false;
    for (int i = 1; i + 1 < argc; i += 2) {
        const std::string key = argv[i];
        const char* value = argv[i + 1];
        if (key == "--ram") ram_path = value;
        else if (key == "--before") before_path = value;
        else if (key == "--after") after_path = value;
        else if (key == "--ot") { ot = uint32_t(std::strtoul(value, nullptr, 16)); have_ot = true; }
        else if (key == "--vram-out") vram_out = value;
        else if (key == "--env-addr") env_addr = uint32_t(std::strtoul(value, nullptr, 16));
        else if (key == "--status") status = uint32_t(std::strtoul(value, nullptr, 16));
        else if (key == "--max-report") max_report = std::atoi(value);
        else { std::fprintf(stderr, "unknown option %s\n", key.c_str()); return 2; }
    }
    if (ram_path.empty() || before_path.empty() || !have_ot) {
        std::fprintf(stderr, "need --ram, --before and --ot\n");
        return 2;
    }
    const std::vector<uint8_t> ram = read_file(ram_path);
    const std::vector<uint8_t> before = read_file(before_path);
    if (ram.size() != 0x200000 || before.size() != 0x100000) { std::fprintf(stderr, "bad input sizes\n"); return 2; }

    ps1::Gpu gpu;
    std::memcpy(gpu.vram(), before.data(), before.size());
    ps1::Environment& env = gpu.environment();
    env.tpage_x = int(status & 15); env.tpage_y = (status & 0x10) ? 256 : 0;
    env.semi_mode = int((status >> 5) & 3); env.tex_depth = int((status >> 7) & 3);
    env.dither = (status & 0x200) != 0; env.draw_to_display = (status & 0x400) != 0;
    env.mask_set = (status & 0x800) != 0; env.mask_check = (status & 0x1000) != 0;

    // The draw area, offset, texture window and mask words the game last sent with PutDrawEnv are
    // kept in RAM by libgpu; feeding them first reproduces the GPU environment at DrawOTag entry.
    if (env_addr) {
        for (uint32_t at = env_addr & 0x1FFFFC; at + 4 <= ram.size(); at += 4) {
            const uint32_t word = uint32_t(ram[at]) | uint32_t(ram[at + 1]) << 8 | uint32_t(ram[at + 2]) << 16 | uint32_t(ram[at + 3]) << 24;
            if ((word >> 24) < 0xE1 || (word >> 24) > 0xE6) break;
            gpu.gp0(word);
        }
        gpu.clear_counts();
    }
    const ps1::WalkResult walk = gpu.draw_otag(ram.data(), ram.size(), ot);
    const ps1::DirtyRect dirty = gpu.dirty();
    const ps1::Environment final_env = gpu.environment();

    if (!vram_out.empty()) {
        std::ofstream out(vram_out, std::ios::binary);
        out.write(reinterpret_cast<const char*>(gpu.vram()), 0x100000);
    }

    std::printf("{\"nodes\":%u,\"words\":%u,\"terminated\":%s,\"dirty\":[%d,%d,%d,%d],\"area\":[%d,%d,%d,%d],\"offset\":[%d,%d]",
                walk.nodes, walk.words, walk.terminated ? "true" : "false", dirty.x0, dirty.y0, dirty.x1, dirty.y1,
                final_env.area_x1, final_env.area_y1, final_env.area_x2, final_env.area_y2, final_env.offset_x,
                final_env.offset_y);
    std::printf(",\"kinds\":{");
    bool first = true;
    for (int k = 0; k < ps1::K_KIND_COUNT; ++k) {
        if (!gpu.count(k)) continue;
        std::printf("%s\"%s\":%llu", first ? "" : ",", ps1::kind_name(k), (unsigned long long)gpu.count(k));
        first = false;
    }
    std::printf("}");

    if (!after_path.empty()) {
        const std::vector<uint8_t> after = read_file(after_path);
        if (after.size() != 0x100000) { std::fprintf(stderr, "bad after size\n"); return 2; }
        const uint16_t* got = gpu.vram();
        long changed_emulator = 0, changed_renderer = 0, diff_all = 0, diff_area = 0, area_pixels = 0;
        int bx0 = 1024, by0 = 512, bx1 = -1, by1 = -1;
        std::string report;
        int reported = 0;
        for (int y = 0; y < 512; ++y) {
            for (int x = 0; x < 1024; ++x) {
                const size_t at = size_t(y) * 1024 + x;
                uint16_t b, a;
                std::memcpy(&b, &before[at * 2], 2);
                std::memcpy(&a, &after[at * 2], 2);
                const uint16_t g = got[at];
                if (a != b) ++changed_emulator;
                if (g != b) ++changed_renderer;
                const bool in_area = x >= final_env.area_x1 && x <= final_env.area_x2 && y >= final_env.area_y1 &&
                                     y <= final_env.area_y2;
                if (in_area) ++area_pixels;
                if (g == a) continue;
                ++diff_all;
                if (in_area) ++diff_area;
                bx0 = std::min(bx0, x); by0 = std::min(by0, y); bx1 = std::max(bx1, x); by1 = std::max(by1, y);
                if (reported < max_report) {
                    char buf[128];
                    std::snprintf(buf, sizeof buf, "%s{\"x\":%d,\"y\":%d,\"before\":%u,\"emulator\":%u,\"renderer\":%u}",
                                  reported ? "," : "", x, y, b, a, g);
                    report += buf;
                    ++reported;
                }
            }
        }
        std::printf(",\"compared_vram\":524288,\"compared_area\":%ld,\"diff_vram\":%ld,\"diff_area\":%ld,"
                    "\"changed_by_emulator\":%ld,\"changed_by_renderer\":%ld,\"diff_bbox\":[%d,%d,%d,%d],\"first_diffs\":[%s]",
                    area_pixels, diff_all, diff_area, changed_emulator, changed_renderer, bx0, by0, bx1, by1, report.c_str());
    }
    std::printf("}\n");
    return 0;
}
