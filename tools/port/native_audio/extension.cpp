// T-29027 Godot binding. DSP and all mutable state are engine-instance owned.
#include "core.hpp"
#include <chrono>
#include <memory>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/hashing_context.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_int64_array.hpp>

using namespace godot;
class OriginalAudioEngine : public RefCounted {
    GDCLASS(OriginalAudioEngine, RefCounted)
    std::unique_ptr<original_audio::Engine> engine;
    bool poisoned=false;
protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("initialize_inputs","inputs","gain"),&OriginalAudioEngine::initialize_inputs);
        ClassDB::bind_method(D_METHOD("render_block"),&OriginalAudioEngine::render_block);
        ClassDB::bind_method(D_METHOD("stop_sequence"),&OriginalAudioEngine::stop_sequence);
        ClassDB::bind_method(D_METHOD("diagnostics"),&OriginalAudioEngine::diagnostics);
        ClassDB::bind_method(D_METHOD("dispose"),&OriginalAudioEngine::dispose);
        ClassDB::bind_method(D_METHOD("load_rei_bank","inputs"),&OriginalAudioEngine::load_rei_bank);
        ClassDB::bind_method(D_METHOD("begin_rei_fade"),&OriginalAudioEngine::begin_rei_fade);
        ClassDB::bind_method(D_METHOD("tick_rei_fade","xa_active"),&OriginalAudioEngine::tick_rei_fade);
        ClassDB::bind_method(D_METHOD("start_rei","xa_active"),&OriginalAudioEngine::start_rei);
        ClassDB::bind_method(D_METHOD("begin_fade","rate","decrement","per_pump"),&OriginalAudioEngine::begin_fade);
        ClassDB::bind_method(D_METHOD("begin_shiori_load","inputs","schedule"),&OriginalAudioEngine::begin_shiori_load);
        ClassDB::bind_method(D_METHOD("finish_shiori"),&OriginalAudioEngine::finish_shiori);
        ClassDB::bind_method(D_METHOD("submit_command","command"),&OriginalAudioEngine::submit_command);
        ClassDB::bind_method(D_METHOD("bank0_start","sequence","tick_skip"),&OriginalAudioEngine::bank0_start);
        ClassDB::bind_method(D_METHOD("begin_bank0_load","inputs","schedule"),&OriginalAudioEngine::begin_bank0_load);
        ClassDB::bind_method(D_METHOD("bank0_diagnostics"),&OriginalAudioEngine::bank0_diagnostics);
    }
public:
    String load_rei_bank(Array inputs) {
        if(!engine || poisoned)return "engine is disposed or failed";
        if(inputs.size()!=3)return "exact original IJYU_VOL inputs required";
        static constexpr const char* hashes[]={"f5ae3b750f10fec1a1cd0698ab1b2540de93bde3f5b4bb62dfa8da23848e489c","be8281abca7901556494ded265dd7f686cd5e58179db2f0d7dcb0ae95f90bb21","18dd4e4a9050cdf2fea53c4ab799c301a64c0bc36e4a059d9d5f3717b0628574"};
        std::array<std::vector<uint8_t>,3> data;
        for(int i=0;i<3;++i){
            if(inputs[i].get_type()!=Variant::PACKED_BYTE_ARRAY)return "Rei input must be original bytes";
            PackedByteArray bytes=inputs[i];if(bytes.is_empty() || bytes.size()>0x28000)return "Rei input extent differs";
            Ref<HashingContext> context;context.instantiate();
            if(context->start(HashingContext::HASH_SHA256)!=OK || context->update(bytes)!=OK || context->finish().hex_encode()!=hashes[i])return "Rei original input identity differs";
            data[i].assign(bytes.ptr(),bytes.ptr()+bytes.size());
        }
        try{engine->load_rei(data);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}
    }
    String begin_fade(int64_t rate,int64_t decrement,bool per_pump){if(!engine || poisoned)return "engine is disposed or failed";try{engine->begin_fade(int(rate),int(decrement),per_pump);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    String begin_shiori_load(Array inputs,PackedInt32Array schedule) {
        if(!engine || poisoned)return "engine is disposed or failed";
        if(inputs.size()!=3)return "exact original CBA5/CBAA/CBD4 sector reads required";
        // Exact whole-sector reads of LBA 0xCBA5 x0x25, 0xCBAA x0x50, 0xCBD4 x6.
        static constexpr const char* hashes[]={"0545fd982409dd4d105e2a327ca813f17edd7ab37b0494c7006397319fda796d","97f14d89b05e5c530a33f8e3ef2411ec03fa908c32fe38aecfc274fcba140450","622d0390afea51bccf234361a42ec93bbe3024874caa977d61b981af2d662bdd"};
        std::array<std::vector<uint8_t>,3> data;
        for(int i=0;i<3;++i){
            if(inputs[i].get_type()!=Variant::PACKED_BYTE_ARRAY)return "Shiori input must be original sector bytes";
            PackedByteArray bytes=inputs[i];if(bytes.is_empty() || bytes.size()>0x28000)return "Shiori input extent differs";
            Ref<HashingContext> context;context.instantiate();
            if(context->start(HashingContext::HASH_SHA256)!=OK || context->update(bytes)!=OK || context->finish().hex_encode()!=hashes[i])return "Shiori original input identity differs";
            data[i].assign(bytes.ptr(),bytes.ptr()+bytes.size());
        }
        if(schedule.size()==0 || schedule.size()%3 || schedule.size()>3*256)return "Shiori landing schedule shape differs";
        std::vector<original_audio::Engine::Landing> rows;
        for(int64_t i=0;i<schedule.size();i+=3){if(schedule[i]<0 || schedule[i+1]<0 || schedule[i+2]<0)return "Shiori landing schedule is malformed";rows.push_back({uint32_t(schedule[i]),unsigned(schedule[i+1]),unsigned(schedule[i+2])});}
        try{engine->begin_shiori_load(data,rows);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}
    }
    // native-bank0. Only audio_command(5), (0xD1) and (0x11) come from the owner;
    // 0x21/0x22/0x2F/0x200|n are queued by bank0_start (audio_bank_start(0,n)).
    String submit_command(int64_t command){
        if(!engine || poisoned)return "engine is disposed or failed";
        if(command!=5 && command!=0xd1 && command!=0x11)return "audio command outside the native bank0 owner set";
        try{engine->submit(unsigned(command));return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}
    }
    Dictionary bank0_start(int64_t sequence,int64_t tick_skip){
        Dictionary d;d["status"]=-1;
        if(!engine || poisoned){d["error"]="engine is disposed or failed";return d;}
        if(sequence<0 || sequence>15 || tick_skip<0 || tick_skip>1){d["error"]="bank0 start arguments outside the source/measured range";return d;}
        try{d["status"]=engine->bank0_start(unsigned(sequence),unsigned(tick_skip));d["error"]="";}catch(const std::exception& e){poisoned=true;d["error"]=String(e.what());}
        return d;
    }
    String begin_bank0_load(Array inputs,PackedInt32Array schedule) {
        if(!engine || poisoned)return "engine is disposed or failed";
        if(inputs.size()!=5)return "exact original BF98/BF79/D37D/D387/D3D5 sector reads required";
        // prologue_finish_bank_store.gd READS: whole-sector reads incl. over-read.
        static constexpr const char* hashes[]={"653eda42f86a4de8eef40a49a1efb19fa1d4cda67e8a2de9043a31e3faf87552","8a1034f0413cd0232e63448ac54573528bd18248636f5a129f5d34725af392a9","df78bc6a6237b90c5f5fe323ec2ae0760b84159f642665b76a543a7ba904965b","69ad06b80c84e616ead7b2132118c07116b02534bfd25a09bddba44c640afbb2","e3958c5a8e7918716f9d2062d43c878c828a2bb611ec242a4084360ee31a6e5c"};
        std::array<std::vector<uint8_t>,5> data;
        for(int i=0;i<5;++i){
            if(inputs[i].get_type()!=Variant::PACKED_BYTE_ARRAY)return "bank0 input must be original sector bytes";
            PackedByteArray bytes=inputs[i];if(bytes.is_empty() || bytes.size()>0x28000)return "bank0 input extent differs";
            Ref<HashingContext> context;context.instantiate();
            if(context->start(HashingContext::HASH_SHA256)!=OK || context->update(bytes)!=OK || context->finish().hex_encode()!=hashes[i])return "bank0 original input identity differs";
            data[i].assign(bytes.ptr(),bytes.ptr()+bytes.size());
        }
        if(schedule.size()==0 || schedule.size()%3 || schedule.size()>3*256)return "bank0 landing schedule shape differs";
        std::vector<original_audio::Engine::Landing> rows;
        for(int64_t i=0;i<schedule.size();i+=3){if(schedule[i]<0 || schedule[i+1]<0 || schedule[i+2]<0)return "bank0 landing schedule is malformed";rows.push_back({uint32_t(schedule[i]),unsigned(schedule[i+1]),unsigned(schedule[i+2])});}
        try{engine->begin_bank0_load(data,rows);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}
    }
    Dictionary bank0_diagnostics() const {
        Dictionary d;d["initialized"]=bool(engine);d["failed"]=poisoned;
        if(!engine)return d;
        d["pumps"]=int64_t(engine->pumps);d["bank0_load_pump"]=int64_t(engine->bank0_load_pump);d["bank0_loading"]=engine->bank0_loading;d["bank0_loaded"]=engine->bank0_loaded;
        d["bank0_complete"]=engine->bank0_complete();d["transfer_done"]=engine->bank0_transfer_done();d["transfer_requests"]=int64_t(engine->transfer_requests);
        d["system_flag"]=engine->system_flag;d["bgm_sequence"]=engine->bgm_sequence;d["bgm_next"]=engine->bgm_next;d["queued_commands"]=int64_t(engine->command_queue.size());
        d["stopped"]=engine->stopped;d["fade_active"]=engine->fade_active;d["fade_rate"]=engine->fade_rate;d["sequence_gain"]=engine->current_gain;d["seq_tick_skip"]=int64_t(engine->seq_tick_skip);
        d["pack_word0"]=int64_t(engine->cpu.read(0x8001c000));
        Array landed;for(auto c:engine->bank0_landed)landed.append(int64_t(c));d["bank0_landed"]=landed;
        Array log;for(auto& row:engine->bank0_log){PackedInt64Array r;for(auto v:row)r.append(v);log.append(r);}d["bank0_log"]=log;
        // Flat [pump, MVOL, gain, fade_active] and [pump, channel, sample address, 0] rows.
        PackedInt64Array pumps;for(auto& row:engine->bank0_pump_log)for(auto v:row)pumps.append(v);d["bank0_pump_log"]=pumps;
        PackedInt64Array kon;for(auto& row:engine->bank0_kon_log)for(auto v:row)kon.append(v);d["bank0_kon_log"]=kon;
        return d;
    }
    String finish_shiori(){if(!engine || poisoned)return "engine is disposed or failed";try{engine->finish_shiori();return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    String begin_rei_fade(){if(!engine || poisoned)return "engine is disposed or failed";try{engine->begin_rei_fade();return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    String tick_rei_fade(bool xa_active){if(!engine || poisoned)return "engine is disposed or failed";try{engine->tick_rei_fade(xa_active);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    String start_rei(bool xa_active){if(!engine || poisoned)return "engine is disposed or failed";try{engine->start_rei(xa_active);return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    String initialize_inputs(Array inputs,int64_t gain) {
        if(engine || poisoned)return "engine initialization is single-use";
        if(gain!=69 && gain!=93)return "unsupported exact native gain";
        if(inputs.size()!=6)return "exact original EXE/SYS_MIX/PRO_VOL inputs required";
        static constexpr const char* hashes[]={"bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae","3d38d81c115f91918c318d106fad8e03c0f7ed686eeb4f7bc9bb0efe46eef404","23df753d5e39259ce010f716707d151cba7a1a8c29fd2ebfdf5a0bf864bb0b86","05f7477864b4ca41a60fdbf943949b9f9a9cd83dcb74088d6529b3d957081032","dc7a06a4d0d90c7a21ac2415cc0f78c2c6bf3ef7f207c6443727fd3d69b588b9","16feb604805496721c8797d481fd4244dc6b501c8a5f609d46f51a22c3acc70a"};
        std::array<std::vector<uint8_t>,6> data;
        for(int i=0;i<6;++i){
            if(inputs[i].get_type()!=Variant::PACKED_BYTE_ARRAY)return "input must be original file bytes";
            PackedByteArray bytes=inputs[i];if(bytes.size()>0x200000 || bytes.is_empty())return "input size outside bounds";
            Ref<HashingContext> context;context.instantiate();
            if(context->start(HashingContext::HASH_SHA256)!=OK || context->update(bytes)!=OK || context->finish().hex_encode()!=hashes[i])return "original input identity differs";
            data[i].assign(bytes.ptr(),bytes.ptr()+bytes.size());
        }
        try{auto candidate=std::make_unique<original_audio::Engine>();candidate->initialize(data,static_cast<int>(gain));engine=std::move(candidate);return "";}
        catch(const std::exception& e){poisoned=true;return String(e.what());}
    }
    Dictionary render_block() {
        Dictionary result;
        if(!engine || poisoned){result["error"]="engine is disposed or failed";return result;}
        try{
            auto start=std::chrono::steady_clock::now();auto samples=engine->render();
            PackedVector2Array pcm;pcm.resize(735);for(int i=0;i<735;++i)pcm.set(i,Vector2(float(samples[2*i])/32768.0f,float(samples[2*i+1])/32768.0f));
            result["error"]="";result["pcm"]=pcm;result["end_sample"]=int64_t(engine->frames);
            result["render_us"]=double(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count())/1000.0;
        }catch(const std::exception& e){poisoned=true;result["error"]=String(e.what());}
        return result;
    }
    String stop_sequence(){if(!engine || poisoned)return "engine is disposed or failed";try{engine->stop();return "";}catch(const std::exception& e){poisoned=true;return String(e.what());}}
    Dictionary diagnostics() const {
        Dictionary d;d["initialized"]=bool(engine);d["failed"]=poisoned;d["hardware_PCM_parity"]=false;
        if(!engine)return d;
        d["rei_loaded"]=engine->rei_loaded;d["rei_started"]=engine->rei_started;d["fade_active"]=engine->fade_active;d["fade_ticks"]=engine->fade_ticks;d["sequence_gain"]=engine->current_gain;
        d["frames"]=int64_t(engine->frames);d["note_operations"]=int64_t(engine->cpu.notes);d["allocator_calls"]=int64_t(engine->cpu.allocations);d["maximum_voices"]=int(engine->maximum_voices);d["peak"]=engine->peak;d["wet_peak"]=engine->wet_peak;d["stopped"]=engine->stopped;d["replacements"]=int64_t(engine->replacements);d["busy_replacements"]=int64_t(engine->busy_replacements);
        d["fade_rate"]=engine->fade_rate;d["fade_decrement"]=engine->fade_step;d["fade_per_pump"]=engine->fade_per_pump;
        d["shiori_active"]=engine->shiori_active;d["shiori_started"]=engine->shiori_started;d["shiori_pump"]=int64_t(engine->shiori_pump);
        d["shiori_finish_pump"]=int64_t(engine->shiori_finish_pump);d["fade_finish_pump"]=int64_t(engine->fade_finish_pump);
        Array landed;for(auto c:engine->shiori_landed)landed.append(int64_t(c));d["shiori_landed"]=landed;
        Array kon;for(auto& row:engine->kon_log){PackedInt64Array r;for(auto v:row)r.append(int64_t(v));kon.append(r);}d["kon_log"]=kon;
        Array pumps;for(auto& row:engine->pump_log){PackedInt64Array r;for(auto v:row)r.append(int64_t(v));pumps.append(r);}d["pump_log"]=pumps;
        PackedByteArray header;header.resize(9760);std::memcpy(header.ptrw(),engine->cpu.ram.data()+0x12000,9760);d["mutated_music_header"]=header;
        PackedByteArray registers;registers.resize(512);std::memcpy(registers.ptrw(),engine->cpu.regs.data(),512);d["SPU_registers"]=registers;
        return d;
    }
    void dispose(){engine.reset();poisoned=true;}
};
void initialize_original_audio(ModuleInitializationLevel level){if(level==MODULE_INITIALIZATION_LEVEL_SCENE)ClassDB::register_class<OriginalAudioEngine>();}
void uninitialize_original_audio(ModuleInitializationLevel){}
extern "C" {
GDExtensionBool GDE_EXPORT original_audio_init(GDExtensionInterfaceGetProcAddress get_proc,GDExtensionClassLibraryPtr library,GDExtensionInitialization* initialization){
    GDExtensionBinding::InitObject init(get_proc,library,initialization);init.register_initializer(initialize_original_audio);init.register_terminator(uninitialize_original_audio);init.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);return init.init();
}
}
