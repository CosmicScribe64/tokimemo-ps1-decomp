// native-bank0: drives core.hpp along the M3 route (.asset-cache/emu/finish-landing-m3/r1)
// from the Rei start through stage16 audio_bank_start(0,1), stage20/21 D1, the stage21
// COM_MIX/SPRING landing, the 0x11 transfer and the spring start. Prints the mix-time
// pump log and event log for tools/port/native_audio/compare_bank0.py, and runs negatives.
// usage: test_bank0 <first-arrival dir> <opening pack reads dir> <finish reads dir> <schedule.txt> <out dir>
#include "core.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
using original_audio::Engine;
static std::vector<uint8_t> slurp(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("missing "+path);return {std::istreambuf_iterator<char>(f),{}};}
static int failures=0,checks=0;
static void check(bool ok,const std::string& label){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<label<<"\n";}}
template<class F> static std::string error_of(F f){try{f();return "";}catch(const std::exception& e){return e.what();}}
// Pump distances from the M3 r1 events (C1 call R=2206): stage16 bank_start 6964, stage20 D1 9315,
// stage21 load 9316, 0x11 at 9447 (L+131), audio_bank_start(0,0) at 9449 (L+133).
constexpr uint32_t PREFIX=1949,SHIORI_POLL=181,STAGE16=4758,STAGE20=7109,STAGE21=7110,TRANSFER=131,SPRING=133;
struct Inputs{std::array<std::vector<uint8_t>,6> boot;std::array<std::vector<uint8_t>,3> rei,shiori;std::array<std::vector<uint8_t>,5> finish;std::vector<Engine::Landing> shiori_schedule,finish_schedule;};
static std::vector<Engine::Landing> schedule(const std::string& path){std::ifstream f(path);std::vector<Engine::Landing> rows;uint32_t p;unsigned r,c;while(f>>p>>r>>c)rows.push_back({p,r,c});return rows;}
// prologue_shiori_bank_resources.gd LANDING_SCHEDULE (M2, verbatim).
static const unsigned SHIORI[][3]={{3,0,1},{15,0,4},{16,0,6},{17,0,9},{18,0,11},{19,0,14},{20,0,16},{21,0,19},{22,0,21},{23,0,24},{24,0,26},{25,0,29},{26,0,31},{27,0,34},{28,0,36},{29,0,37},{31,1,1},{43,1,2},{44,1,4},{45,1,7},{46,1,9},{47,1,12},{48,1,14},{49,1,17},{50,1,19},{51,1,22},{52,1,24},{53,1,27},{54,1,29},{55,1,32},{56,1,34},{57,1,37},{58,1,39},{59,1,42},{60,1,44},{61,1,47},{62,1,49},{63,1,52},{64,1,54},{65,1,57},{66,1,59},{67,1,62},{68,1,64},{69,1,67},{70,1,69},{71,1,72},{72,1,74},{73,1,77},{74,1,79},{75,1,80},{77,2,1},{89,2,2},{90,2,4},{91,2,6}};
static void render(Engine& e,uint32_t n){for(uint32_t i=0;i<n;++i)e.render();}
// Engine at the stage21 load call (after STAGE21 pumps from R), or nullptr-equivalent on error.
static std::string to_stage16(Engine& e,const Inputs& in){
    e.initialize(in.boot,93);e.load_rei(in.rei);e.begin_rei_fade();
    for(int i=0;i<10;++i){e.tick_rei_fade(false);e.render();}
    e.start_rei(false);render(e,PREFIX);
    e.begin_fade(0xc0,1,true);e.begin_shiori_load(in.shiori,in.shiori_schedule);
    render(e,SHIORI_POLL);e.finish_shiori();render(e,STAGE16-SHIORI_POLL);
    return "";
}
int main(int argc,char** argv){
    if(argc!=6){std::cerr<<"usage\n";return 2;}
    Inputs in;std::string boot=argv[1],pack=argv[2],fin=argv[3],out=argv[5];
    const char* names[]={"executable.bin","SYS_MIX.VH","SYS_MIX.VB","PRO_VOL.VH","PRO_VOL.VB","PRO_VOL.SEQ"};
    for(int i=0;i<6;++i)in.boot[i]=slurp(boot+"/"+names[i]);
    in.rei={slurp(pack+"/prologue_rei-music_header.bin"),slurp(pack+"/prologue_rei-music_body.bin"),slurp(pack+"/prologue_rei-sequence.bin")};
    in.shiori={slurp(pack+"/prologue_shiori-sequence_pack.bin"),slurp(pack+"/prologue_shiori-music_body.bin"),slurp(pack+"/prologue_shiori-music_header.bin")};
    const char* fields[]={"se_header","se_body","sequence","music_body","music_header"};
    for(int i=0;i<5;++i)in.finish[i]=slurp(fin+"/"+fields[i]+".bin");
    for(auto& r:SHIORI)in.shiori_schedule.push_back({r[0],r[1],r[2]});
    in.finish_schedule=schedule(argv[4]);
    check(in.finish_schedule.size()>5,"finish landing schedule is empty");
    // ---- main route
    Engine e;std::string err=error_of([&]{to_stage16(e,in);});
    check(err.empty(),"route to stage16 failed: "+err);
    if(!err.empty()){std::cerr<<checks<<" checks, "<<failures<<" failures\n";return 1;}
    uint64_t r_abs=e.pumps-STAGE16;  // pumps value at the C1 call frame R
    int status=0;err=error_of([&]{status=e.bank0_start(1,1);});
    check(err.empty() && status==1,"stage16 audio_bank_start(0,1) did not queue on PRO_VOL.SEQ: "+err);
    err=error_of([&]{render(e,STAGE20-STAGE16);e.submit(5);e.submit(0xd1);e.render();e.submit(5);e.submit(0xd1);e.begin_bank0_load(in.finish,in.finish_schedule);});
    check(err.empty(),"stage20/21 commands or load failed: "+err);
    uint32_t polls_ok=0;
    err=error_of([&]{
        for(uint32_t p=1;p<=SPRING+12;++p){
            e.render();
            if(p<TRANSFER)polls_ok+=!e.bank0_complete();
            if(p==TRANSFER){check(e.bank0_complete(),"reads not complete at the measured 0x11 poll");e.submit(0x11);}
            if(p==TRANSFER+1)check(!e.bank0_transfer_done(),"slot1 transfer done one pump early");
            if(p==SPRING){check(e.bank0_transfer_done(),"bank0 transfer not done at the measured poll");check(e.bank0_start(0,0)==1,"spring audio_bank_start(0,0) saw no pack header");}
        }
    });
    check(err.empty(),"stage21..spring run failed: "+err);
    check(polls_ok==TRANSFER-1,"reads completed before the measured +131 poll");
    check(!e.stopped && e.current_gain==94 && e.bgm_sequence==0,"spring sequence 0 not playing at gain 94: "+std::to_string(e.current_gain));
    // ---- logs for the comparator
    std::ofstream pl(out+"/pump_log.txt"),el(out+"/event_log.txt");
    for(auto& row:e.bank0_pump_log)pl<<int64_t(row[0]-r_abs)<<' '<<row[1]<<' '<<row[2]<<' '<<row[3]<<'\n';
    for(auto& row:e.bank0_log)el<<int64_t(row[0]-r_abs)<<' '<<row[1]<<' '<<row[2]<<' '<<row[3]<<'\n';
    std::ofstream kl(out+"/kon_log.txt");for(auto& row:e.bank0_kon_log)kl<<int64_t(row[0]-r_abs)<<' '<<row[1]<<' '<<row[2]<<'\n';
    // Slot inventory after the transfer: COM_MIX in slot0 SPU 0x1010, SPRING in slot1 SPU 0x11010.
    bool slot0=false,slot1=false,stale=false;
    for(auto& s:e.samples){if(s.first>=0x1010 && s.first<0x11010)slot0=true;if(s.first>=0x11010 && s.first<0x39010)slot1=true;}
    check(slot0 && slot1,"COM_MIX/SPRING sample inventory missing after transfer");
    check(std::equal(in.finish[1].begin(),in.finish[1].begin()+62400,e.sound.begin()+0x1010),"SPU slot0 differs from COM_MIX.VB");
    check(std::equal(in.finish[3].begin(),in.finish[3].begin()+158992,e.sound.begin()+0x11010),"SPU slot1 differs from SPRING.VB");
    (void)stale;
    // PCM: spring audible after start.
    int peak=0;for(int i=0;i<60;++i){auto pcm=e.render();for(auto v:pcm)peak=std::max(peak,std::abs(int(v)));}
    check(peak>0,"spring BGM renders silence");
    // ---- gameplay: the command select confirm queues audio_command(3) and (0x500)
    // (golden route-golden-smoke-v1 enter-30-001 queue [3, 0x500]); both run in the next pump.
    {
        auto ram0=e.cpu.ram;size_t kons=e.bank0_kon_log.size();
        std::string gerr=error_of([&]{e.submit(3);e.submit(0x500);e.render();});
        check(gerr.empty(),"gameplay audio_command(3)/(0x500) failed: "+gerr);
        check(e.xa_forgets==1 && e.se_key_ons==1 && e.se_slot_a==0,"confirm did not run xa_forget and one effect key on");
        check(!e.system_flag,"audio_se_play left state_flags 0x100000 set");
        check(!e.stopped && e.bgm_sequence==0 && e.current_gain==94,"confirm effect disturbed the spring sequence");
        gerr=error_of([&]{render(e,2);});
        check(gerr.empty(),"pumps after the confirm effect failed: "+gerr);
        uint32_t effect_address=0;for(size_t i=kons;i<e.bank0_kon_log.size();++i){auto a=e.bank0_kon_log[i][2];if(a>=0x1010 && a<0x11010)effect_address=uint32_t(a);}
        check(effect_address!=0,"confirm effect keyed no COM_MIX (slot0) sample");
        std::ofstream ef(out+"/effect.txt");ef<<std::hex<<effect_address<<'\n';
        std::ofstream r0(out+"/ram_before_confirm.bin",std::ios::binary);r0.write((const char*)ram0.data(),ram0.size());
        std::ofstream r1(out+"/ram_after_confirm.bin",std::ios::binary);r1.write((const char*)e.cpu.ram.data(),e.cpu.ram.size());
        // Key off form 0x1500 (audio_se_stop) releases without a key on.
        uint64_t ons=e.se_key_ons;gerr=error_of([&]{e.submit(0x1500);e.render();});
        check(gerr.empty() && e.se_key_offs==1 && e.se_key_ons==ons,"0x1500 effect key off failed: "+gerr);
        check(!error_of([&]{e.submit(0x600);}).empty(),"unmodelled class-6 effect 0x600 accepted");
        // School day effects 0x501/0x502 (school_day.c:460-463): program 0, notes 0x1C/0x20.
        for(unsigned command:{0x501u,0x502u}){
            size_t k=e.bank0_kon_log.size();uint64_t n=e.se_key_ons;
            gerr=error_of([&]{e.submit(command);render(e,2);});
            bool slot0=false;for(size_t i=k;i<e.bank0_kon_log.size();++i){auto a=e.bank0_kon_log[i][2];if(a>=0x1010 && a<0x11010)slot0=true;}
            check(gerr.empty() && e.se_key_ons==n+1 && slot0,"school-day effect failed or keyed no slot0 sample: "+gerr);
        }
    }
    // ---- negatives (each would pass if its guard were removed)
    {Engine n;to_stage16(n,in);
     check(!error_of([&]{n.begin_bank0_load(in.finish,{{1,0,3}});}).empty(),"incomplete landing schedule accepted");}
    {Engine n;to_stage16(n,in);auto bad=in.finish_schedule;std::swap(bad[0],bad.back());
     check(!error_of([&]{n.begin_bank0_load(in.finish,bad);}).empty(),"out-of-order landing schedule accepted");}
    {Engine n;to_stage16(n,in);auto bad=in.finish;bad[2].pop_back();
     check(!error_of([&]{n.begin_bank0_load(bad,in.finish_schedule);}).empty(),"short SPRING.SEQ read accepted");}
    {Engine n;to_stage16(n,in);n.begin_bank0_load(in.finish,in.finish_schedule);render(n,TRANSFER-2);n.submit(0x11);
     check(!error_of([&]{n.render();}).empty(),"0x11 accepted before the music header landed");}
    {Engine n;to_stage16(n,in);
     check(!error_of([&]{n.submit(0x12);}).empty(),"unmodelled bank1 transfer 0x12 accepted");
     check(!error_of([&]{n.bank0_start(16,0);}).empty(),"sequence outside the pack index accepted");}
    {Engine n;to_stage16(n,in);n.cpu.write(0x8001c000,0x41);
     check(n.bank0_start(1,1)==-1 && n.command_queue.empty(),"audio_bank_start(0,1) queued over a non-0x40 pack header");}
    {Engine n;n.initialize(in.boot,93);
     check(!error_of([&]{n.begin_bank0_load(in.finish,in.finish_schedule);}).empty(),"bank0 load accepted before the Shiori start");}
    {Engine n;to_stage16(n,in);n.submit(5);n.submit(0xd1);n.render();uint64_t before=n.bank0_log.size();n.submit(0xd1);n.render();
     bool ignored=false;for(size_t i=before;i<n.bank0_log.size();++i)if(n.bank0_log[i][1]==8)ignored=true;
     check(ignored && n.fade_rate==0x80,"second D1 during a fade was not ignored");}
    // Effects before the slot0 transfer (0x21 assigned by audio_bank_start(0,1), no 0x11 yet).
    {Engine n;to_stage16(n,in);n.bank0_start(1,1);n.render();n.render();n.submit(0x500);
     check(error_of([&]{n.render();})=="effect command before the slot0 bank transfer","effect accepted before the slot0 transfer");}
    {Engine n;to_stage16(n,in);n.submit(0x500);
     check(error_of([&]{n.render();})=="effect command before audio_command_bank_assign(0x21)","effect accepted before the 0x21 assignment");}
    std::cerr<<checks<<" checks, "<<failures<<" failures\n";
    if(!failures)std::cout<<"PASS: native bank0 route\n";
    return failures?1:0;
}
