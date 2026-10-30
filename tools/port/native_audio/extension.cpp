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
