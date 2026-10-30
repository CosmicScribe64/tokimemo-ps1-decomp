// SPU direct-volume register is signed Q14, including negative phase.
#include "core.hpp"
#include <cassert>
int main() {
    using original_audio::Engine;
    assert(Engine::master_sample(16384,0)==0);
    assert(Engine::master_sample(16384,0x3fff)==16383);
    assert(Engine::master_sample(16384,0x2000)==8192);
    assert(Engine::master_sample(16384,0x4000)==-16384);
    assert(Engine::master_sample(16384,0x7fff)==-1);
    assert(Engine::master_sample(-16384,0x2000)==-8192);
    bool rejected=false;
    try { Engine::master_sample(16384,0x8000); }
    catch(const std::runtime_error&) { rejected=true; }
    assert(rejected);
}
