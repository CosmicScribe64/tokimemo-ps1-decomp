// T-29027: project-owned original-file CPU and SPU model. No emulator code/data.
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "gaussian.hpp"

namespace original_audio {
inline int32_t clamp16(int64_t value) { return int32_t(std::clamp<int64_t>(value,-32768,32767)); }
inline int32_t signed16(uint32_t value) { return int16_t(value & 65535); }
inline int32_t fixed(int64_t value) { return int32_t(value >> 15); }

struct Cpu {
    std::vector<uint8_t> ram = std::vector<uint8_t>(0x200000);
    std::array<uint8_t,512> regs{};
    std::vector<std::pair<bool,uint32_t>> writes;
    std::function<bool(uint32_t,std::array<uint32_t,32>&)> hook;
    bool capture=false;
    uint64_t notes=0,allocations=0;
    uint8_t* region(uint32_t address,unsigned count) {
        address &= 0x1fffffff;
        if(count<=ram.size() && address<=ram.size()-count) return ram.data()+address;
        if(address>=0x1f801c00 && count<=512 && address-0x1f801c00<=512-count) return regs.data()+address-0x1f801c00;
        throw std::runtime_error("CPU address outside owned RAM/registers: "+std::to_string(address));
    }
    uint32_t read(uint32_t address,unsigned count=4) {
        auto p=region(address,count); uint32_t result=0;
        for(unsigned i=0;i<count;++i) result|=uint32_t(p[i])<<(8*i);
        return result;
    }
    void write(uint32_t address,uint32_t value,unsigned count=4) {
        auto p=region(address,count);
        for(unsigned i=0;i<count;++i) p[i]=uint8_t(value>>(8*i));
        if(capture && (address&0x1fffffff)>=0x1f801d88 && (address&0x1fffffff)<=0x1f801d8e) {
            uint32_t a=address&0x1fffffff;
            if(count!=2 || (a&1)) throw std::runtime_error("unsupported key mask write");
            writes.emplace_back(a<0x1f801d8c,(value<<((a&2)?16:0))&0xffffff);
        }
    }
    uint32_t call(uint32_t entry,std::initializer_list<uint32_t> args={}) {
        std::array<uint32_t,32> r{};r[29]=0x801ff000;r[31]=0x80000000;
        unsigned n=0; for(auto a:args) { if(n<4)r[n+4]=a;else write(r[29]+16+(n-4)*4,a);++n; }
        uint32_t pc=entry,next=entry+4,hi=0,lo=0,load_value=0;int load=-1;
        for(unsigned budget=0;budget<50000;++budget) {
            if(pc==0x80000000)return r[2];
            if(pc==0x8008e4b8 || pc==0x8008e8a8)++notes;
            if(pc==0x8008c810)++allocations;
            if(hook) {
                // Device hooks are only entry points; commit any pending load
                // before the host interface, matching the reviewed CPU model.
                if(pc==0x80095b70 || pc==0x80097948 || pc==0x800949b0) {
                    if(load>=0)r[load]=load_value;
                    load=-1;
                    if(hook(pc,r)){pc=r[31];next=pc+4;continue;}
                }
            }
            uint32_t ins=read(pc),op=ins>>26,rs=(ins>>21)&31,rt=(ins>>16)&31,rd=(ins>>11)&31,sh=(ins>>6)&31,fn=ins&63;
            int32_t imm=signed16(ins);uint32_t a=r[rs],b=r[rt],old_next=next;
            next+=4;int written=-1,new_load=-1;uint32_t new_value=0;
            auto put=[&](unsigned reg,uint32_t v){r[reg]=v;written=int(reg);};
            if(op==0) {
                switch(fn) {
                case 0:put(rd,b<<sh);break;case 2:put(rd,b>>sh);break;case 3:put(rd,uint32_t(int32_t(b)>>sh));break;
                case 4:put(rd,b<<(a&31));break;case 6:put(rd,b>>(a&31));break;case 7:put(rd,uint32_t(int32_t(b)>>(a&31)));break;
                case 8:next=a;break;case 9:put(rd,pc+8);next=a;break;
                case 16:put(rd,hi);break;case 18:put(rd,lo);break;
                case 24:case 25:{uint64_t p=fn==24?uint64_t(int64_t(int32_t(a))*int32_t(b)):uint64_t(a)*b;hi=p>>32;lo=uint32_t(p);break;}
                case 26:case 27:{int64_t aa=fn==26?int64_t(int32_t(a)):int64_t(a),bb=fn==26?int64_t(int32_t(b)):int64_t(b);if(!bb)throw std::runtime_error("CPU division by zero");lo=uint32_t(aa/bb);hi=uint32_t(aa%bb);break;}
                case 32:case 33:put(rd,a+b);break;case 34:case 35:put(rd,a-b);break;
                case 36:put(rd,a&b);break;case 37:put(rd,a|b);break;case 38:put(rd,a^b);break;case 39:put(rd,~(a|b));break;
                case 42:put(rd,int32_t(a)<int32_t(b));break;case 43:put(rd,a<b);break;
                default:throw std::runtime_error("unsupported CPU SPECIAL "+std::to_string(fn));
                }
            } else if(op==2 || op==3) {if(op==3)put(31,pc+8);next=((pc+4)&0xf0000000)|((ins&0x3ffffff)<<2);
            } else if(op==1 || (op>=4 && op<=7)) {
                if(op==1 && rt>1)throw std::runtime_error("unsupported REGIMM");
                bool take=op==1?(rt==0?int32_t(a)<0:int32_t(a)>=0):op==4?a==b:op==5?a!=b:op==6?int32_t(a)<=0:int32_t(a)>0;
                if(take)next=pc+4+uint32_t(imm*4);
            } else if(op==8 || op==9)put(rt,a+uint32_t(imm));
            else if(op==10)put(rt,int32_t(a)<imm);
            else if(op==11)put(rt,a<uint32_t(imm));
            else if(op==12)put(rt,a&(ins&65535));else if(op==13)put(rt,a|(ins&65535));else if(op==14)put(rt,a^(ins&65535));else if(op==15)put(rt,ins<<16);
            else if(op==32 || op==33 || op==35 || op==36 || op==37) {
                unsigned width=op==32||op==36?1:op==35?4:2;new_load=int(rt);new_value=read(a+uint32_t(imm),width);
                if(op==32)new_value=uint32_t(int32_t(int8_t(new_value)));
                if(op==33)new_value=uint32_t(signed16(new_value));
            } else if(op==40 || op==41 || op==43)write(a+uint32_t(imm),b,op==40?1:op==41?2:4);
            else throw std::runtime_error("unsupported CPU opcode "+std::to_string(op)+" at "+std::to_string(pc));
            if(load>=0 && load!=written)r[load]=load_value;
            load=new_load;load_value=new_value;r[0]=0;pc=old_next;
        }
        throw std::runtime_error("CPU instruction budget exhausted");
    }
};

struct Envelope {
    uint16_t a=0,b=0;int level=0,counter=0,phase=0;
    void release(){if(phase!=3){phase=3;counter=0;}}
    int tick() {
        int shift,step,all;bool exp,down;
        if(phase==0){shift=(a>>10)&31;step=(a>>8)&3;exp=a&0x8000;down=false;all=127;}
        else if(phase==1){shift=(a>>4)&15;step=0;exp=true;down=true;all=31;}
        else if(phase==2){shift=(b>>8)&31;step=(b>>6)&3;exp=b&0x8000;down=b&0x4000;all=127;}
        else{shift=b&31;step=0;exp=b&32;down=true;all=124;}
        int delta=(down?~(7-step):7-step)*(1<<std::max(0,11-shift));int increment=32768>>std::max(0,shift-11);
        if(exp && !down && level>0x6000){if(shift<10)delta>>=2;else if(shift>=11)increment>>=2;else{delta>>=1;increment>>=1;}}
        else if(exp && down)delta=(delta*level)>>15;
        if((step|(shift<<2))!=all)increment=std::max(increment,1);
        counter+=increment;
        if(counter&32768){counter&=32767;level=std::clamp(level+delta,0,32767);if(phase==0 && level==32767){phase=1;counter=0;}else if(phase==1 && level<=std::min(32767,((a&15)+1)*2048)){phase=2;counter=0;}}
        return level;
    }
};

struct Voice {
    bool live=false,ended=false;uint32_t begin=0,end=0,cursor=0,loop=0,next_cursor=0;
    int index=28,h1=0,h2=0,fraction=0,pitch=0,left=0,right=0;
    std::array<int,28> decoded{};std::array<int,4> history{};Envelope env;
    int next_sample(const std::vector<uint8_t>& ram) {
        if(index==28){
            if(ended)return 0;
            cursor=next_cursor;if(cursor<begin || cursor+16>end)throw std::runtime_error("ADPCM crossed sample extent");
            int predictor=ram[cursor]>>4,shift=ram[cursor]&15,flags=ram[cursor+1];
            if(predictor>4 || shift>12 || (flags&~7))throw std::runtime_error("unsupported ADPCM format");
            if(flags&4)loop=cursor;
            static constexpr int coef[5][2]={{0,0},{60,0},{115,-52},{98,-55},{122,-60}};
            for(int i=0;i<28;++i){int n=(ram[cursor+2+i/2]>>((i&1)*4))&15;if(n>=8)n-=16;int value=((n*4096)>>shift)+((h1*coef[predictor][0]+h2*coef[predictor][1]+32)>>6);decoded[i]=clamp16(value);h2=h1;h1=decoded[i];}
            next_cursor=cursor+16;if(flags&1){if(flags&2)next_cursor=loop;else ended=true;}index=0;
        }
        return decoded[index++];
    }
    std::pair<int,int> tick(const std::vector<uint8_t>& ram) {
        int level=env.tick(),i=(fraction>>4)&255;
        int value=((gaussian[255-i]*history[0])>>15)+((gaussian[511-i]*history[1])>>15)+((gaussian[256+i]*history[2])>>15)+((gaussian[i]*history[3])>>15);
        value=(clamp16(value)*level)>>15;fraction+=pitch;
        while(fraction>=4096){fraction-=4096;history[0]=history[1];history[1]=history[2];history[2]=history[3];history[3]=next_sample(ram);}
        if((env.phase==3 && level==0)||(ended && index==28))live=false;
        return {(value*left)>>14,(value*right)>>14};
    }
};

// Independently authored from PSX-SPX register equations and FIR facts.
// All delay/work/filter state belongs to this engine; no Redux source reused.
struct Reverb {
    uint32_t base=0,cursor=0;unsigned phase=0;
    std::array<std::array<int32_t,39>,2> input{},output{};
    static constexpr std::array<int,39> fir={-1,0,2,0,-10,0,35,0,-103,0,266,0,-616,0,1332,0,-2960,0,10246,16384,10246,0,-2960,0,1332,0,-616,0,266,0,-103,0,35,0,-10,0,2,0,-1};
    void reset(uint32_t address){base=cursor=address;phase=0;input={};output={};}
    uint32_t address(int64_t offset) const {
        int64_t span=0x80000-base,relative=int64_t(cursor)-base+offset;
        if(span<=0)throw std::runtime_error("invalid reverb work area");
        relative%=span;if(relative<0)relative+=span;return uint32_t(base+relative)&~1u;
    }
    int get(const std::vector<uint8_t>& ram,int64_t offset) const {auto p=address(offset);return signed16(uint32_t(ram[p])|(uint32_t(ram[p+1])<<8));}
    void put(std::vector<uint8_t>& ram,int64_t offset,int value){auto p=address(offset);auto v=uint16_t(clamp16(value));ram[p]=v;ram[p+1]=v>>8;}
    std::pair<int,int> tick(int left,int right,Cpu& cpu,std::vector<uint8_t>& ram) {
        for(int c=0;c<2;++c){for(int i=38;i>0;--i)input[c][i]=input[c][i-1];input[c][0]=c?right:left;}
        std::array<int,2> wet{};
        if((phase&1)==0){
            std::array<int,32> r{};for(int i=0;i<32;++i)r[i]=signed16(cpu.read(0x1f801dc0+i*2,2));
            std::array<int,2> in{};for(int c=0;c<2;++c){int64_t sum=0;for(int i=0;i<39;++i)sum+=int64_t(input[c][i])*fir[i];in[c]=fixed(sum);}
            auto read=[&](int index,int adjustment=0){return get(ram,int64_t(r[index])*8+adjustment);};
            bool enabled=cpu.read(0x1f801daa,2)&128;
            for(int c=0;c<2;++c){
                int source=fixed(int64_t(in[c])*r[30+c]);
                if(enabled){
                    int previous=read(10+c,-2);
                    int same=previous+fixed(int64_t(source+fixed(int64_t(read(16+c))*r[7])-previous)*r[2]);
                    previous=read(18+c,-2);
                    int different=previous+fixed(int64_t(source+fixed(int64_t(read(24+(1-c)))*r[7])-previous)*r[2]);
                    put(ram,int64_t(r[10+c])*8,same);put(ram,int64_t(r[18+c])*8,different);
                }
                int value=fixed(int64_t(r[3])*read(12+c)+int64_t(r[4])*read(14+c)+int64_t(r[5])*read(20+c)+int64_t(r[6])*read(22+c));
                for(int stage=0;stage<2;++stage){int dest=26+stage*2+c;int delayed=get(ram,int64_t(r[dest]-r[stage])*8);int v=clamp16(value-fixed(int64_t(r[8+stage])*delayed));if(enabled)put(ram,int64_t(r[dest])*8,v);value=clamp16(fixed(int64_t(v)*r[8+stage])+delayed);}
                wet[c]=fixed(int64_t(value)*signed16(cpu.read(0x1f801d84+c*2,2)));
            }
            cursor=address(2);
        }
        std::pair<int,int> result;
        for(int c=0;c<2;++c){for(int i=38;i>0;--i)output[c][i]=output[c][i-1];output[c][0]=(phase&1)?0:wet[c];int64_t sum=0;for(int i=0;i<39;++i)sum+=int64_t(output[c][i])*fir[i];int v=clamp16(sum>>14);if(c)result.second=v;else result.first=v;}
        ++phase;return result;
    }
};

struct Engine {
    Cpu cpu;std::vector<uint8_t> sound=std::vector<uint8_t>(0x80000);
    std::array<Voice,24> voices{};Reverb reverb;
    std::vector<std::pair<uint32_t,uint32_t>> occupied,samples;
    uint64_t frames=0,replacements=0,busy_replacements=0,nonzero=0;
    uint32_t maximum_voices=0;int peak=0,wet_peak=0;bool stopped=false,initialized=false;
    bool rei_loaded=false,rei_started=false,fade_active=false,fade_master=false;
    int current_gain=0,full_gain=0,attenuation=0,fade_accumulator=0,fade_ticks=0;
    // sound_driver.c::audio_bgm_fade_start rate/decrement pair; per_pump means
    // the fade advances once per render pump (audio_bgm_update), not per event.
    int fade_rate=0,fade_step=0;bool fade_per_pump=false;
    // T-39002 stage10/substep8 prologue_shiori bank load. Three nowait CD reads
    // land sector by sector over the live Rei score/header (measured schedule).
    struct Landing{uint32_t pump;unsigned region;unsigned count;};
    static constexpr uint32_t shiori_target[3]={0x8002e800,0x801b8000,0x80016000};
    static constexpr unsigned shiori_sectors[3]={0x25,0x50,6};
    std::array<std::vector<uint8_t>,3> shiori_data;std::array<unsigned,3> shiori_landed{};
    std::vector<Landing> shiori_schedule;size_t shiori_next=0;
    bool shiori_active=false,shiori_finish_pending=false,shiori_started=false;
    uint32_t shiori_pump=0,shiori_finish_pump=0,fade_finish_pump=0;
    std::vector<std::array<uint32_t,4>> kon_log;std::vector<std::array<int32_t,3>> pump_log;
    void copy(uint32_t target,const std::vector<uint8_t>& data){auto p=cpu.region(target,unsigned(data.size()));std::copy(data.begin(),data.end(),p);}
    void initialize(const std::array<std::vector<uint8_t>,6>& data,int gain) {
        if(gain!=93 && gain!=69)throw std::runtime_error("unsupported gain");
        if(initialized)throw std::runtime_error("engine initialization is single-use");
        if(data[0].size()<0x800 || std::memcmp(data[0].data(),"PS-X EXE",8))throw std::runtime_error("invalid EXE");
        auto word=[&](size_t p){return uint32_t(data[0][p])|(uint32_t(data[0][p+1])<<8)|(uint32_t(data[0][p+2])<<16)|(uint32_t(data[0][p+3])<<24);};
        auto base=word(24)&0x1fffffff,size=word(28);if(size>0x200000 || base>0x200000-size || data[0].size()<0x800+size)throw std::runtime_error("EXE extent");
        std::copy_n(data[0].data()+0x800,size,cpu.ram.data()+base);
        for(int ch=0;ch<24;++ch)std::copy_n(cpu.ram.data()+0xdc990,16,cpu.regs.data()+ch*16);
        std::copy_n(cpu.ram.data()+0xdc9a0,32,cpu.regs.data()+0x180);
        cpu.hook=[&](uint32_t pc,std::array<uint32_t,32>& r){
            if(pc==0x80097948){r[2]=0;for(auto s:occupied)if(r[4]>=s.first && r[4]<s.second)r[2]=1;return true;}
            // SpuClearReverbWorkArea(3) (audio_voices_kill): the libspu DMA zero fill of
            // the mode-3 work area, _spu_rev_startaddr[3] 0xF6F8 (x8) to the end of SPU RAM.
            // Only reached by native-bank0 audio_bgm_start; other modes are refused.
            if(pc==0x800949b0){
                if(r[4]!=3 || cpu.read(0x1f801da2,2)!=0xf6f8)throw std::runtime_error("reverb work-area clear outside mode 3");
                std::fill(sound.begin()+0x7b7c0,sound.end(),uint8_t(0));r[2]=0;return true;
            }
            if(pc!=0x80095b70)return false;
            uint32_t dest=cpu.read(0x800dcbd4,2)*8,count=r[5],source=r[4]&0x1fffffff;
            if(count>sound.size() || dest>sound.size()-count)throw std::runtime_error("SPU transfer extent");
            for(auto s:occupied)if(std::max(dest,s.first)<std::min(dest+count,s.second))throw std::runtime_error("SPU transfer overlap");
            std::copy_n(cpu.region(source,count),count,sound.data()+dest);occupied.emplace_back(dest,dest+count);cpu.call(0x80095730,{0});r[2]=count;return true;
        };
        cpu.call(0x8008dad4,{24});cpu.call(0x80090df0,{24});cpu.call(0x8008baa4,{1});cpu.call(0x8008b904,{0x801a0000,1,1});cpu.call(0x80090e30,{1});cpu.call(gain==69?0x80090e40:0x80090e50);cpu.call(0x80095730,{0});
        for(unsigned slot=0;slot<2;++slot){uint32_t hp=slot?0x80012000:0x80010000,bp=slot?0x801c8000:0x801b8000,spu=slot?0x11010:0x1010;copy(hp,data[1+slot*2]);copy(bp,data[2+slot*2]);if(cpu.call(0x8008c1b0,{hp,slot,spu})!=slot || cpu.read(0x801293b0+slot,1)!=2)throw std::runtime_error("VAB open failed");if(cpu.call(0x8008c620,{bp,slot})!=slot || cpu.read(0x801293b0+slot,1)!=1)throw std::runtime_error("VAB transfer failed");}
        copy(0x8001c000,data[5]);
        cpu.call(0x8008fef0,{3});cpu.call(0x8008ff5c,{60,60});cpu.call(0x8008feb0);cpu.call(0x8008b750,{127,127});
        if(cpu.call(0x800900a0,{0x8001c050,1})!=0)throw std::runtime_error("score open failed");
        cpu.call(0x8008c720,{0,uint32_t(gain),uint32_t(gain)});cpu.call(0x80090350,{0,1,1});
        // Original mutated header retains its VAG size table. Bound each voice
        // to an actual original sample, not the complete body or next sample.
        const auto& header=data[3];auto half=[&](size_t p){if(p+2>header.size())throw std::runtime_error("VAB size table");return uint32_t(header[p])|(uint32_t(header[p+1])<<8);};
        size_t table=0x820+half(0x12)*512;uint32_t address=0x11010;
        for(unsigned i=0;i<=half(0x16);++i){uint32_t bytes=half(table+i*2)*8;if(bytes){if(address+bytes>0x11010+data[4].size())throw std::runtime_error("VAG extent");samples.emplace_back(address,address+bytes);}address+=bytes;}
        reverb.reset(cpu.read(0x1f801da2,2)*8);initialized=true;cpu.capture=true;
        current_gain=full_gain=gain;attenuation=gain==69?24:0;
    }
    void load_rei(const std::array<std::vector<uint8_t>,3>& data) {
        if(!initialized || rei_loaded)throw std::runtime_error("Rei bank load requires one initialized arrival owner");
        if(data[0].size()!=10272 || data[1].size()!=139680 || data[2].size()!=6416)throw std::runtime_error("Rei bank extent differs");
        // Alternate source bank: SDK slot2, header0x16000, SPU0x39010.
        // Keep slot1 and its sequence alive while the new bank is transferred.
        copy(0x80016000,data[0]);copy(0x801b8000,data[1]);copy(0x8002e800,data[2]);
        if(cpu.call(0x8008c1b0,{0x80016000,2,0x39010})!=2 || cpu.read(0x801293b2,1)!=2)throw std::runtime_error("Rei VAB open failed");
        if(cpu.call(0x8008c620,{0x801b8000,2})!=2 || cpu.read(0x801293b2,1)!=1)throw std::runtime_error("Rei VAB transfer failed");
        const auto& header=data[0];auto half=[&](size_t p){if(p+2>header.size())throw std::runtime_error("Rei VAB table");return uint32_t(header[p])|(uint32_t(header[p+1])<<8);};
        size_t table=0x820+half(0x12)*512;uint32_t address=0x39010;
        for(unsigned i=0;i<=half(0x16);++i){uint32_t bytes=half(table+i*2)*8;if(bytes){if(address+bytes>0x39010+data[1].size())throw std::runtime_error("Rei VAG extent");samples.emplace_back(address,address+bytes);}address+=bytes;}
        rei_loaded=true;
    }
    void begin_fade(int rate,int step,bool per_pump) {
        if(!initialized || stopped || fade_active)throw std::runtime_error("BGM fade requires an active sequence without an outstanding fade");
        // audio_bgm_fade_start: B1 0x100/10, C1 0xC0/1, D1 0x80/1. Nothing else.
        if(!((rate==0x100 && step==10) || (rate==0xc0 && step==1) || (rate==0x80 && step==1)))throw std::runtime_error("fade rate/decrement outside audio_bgm_fade_start");
        fade_active=true;fade_master=true;fade_accumulator=0;fade_ticks=0;fade_rate=rate;fade_step=step;fade_per_pump=per_pump;fade_sequence_volume=false;
    }
    void begin_rei_fade() {
        if(!initialized || stopped || rei_started || fade_active)throw std::runtime_error("Rei fade requires active arrival sequence");
        // audio_command5 + B1: fade accumulator0, rate256, decrement10.
        begin_fade(0x100,10,false);
    }
    void tick_rei_fade(bool xa_active) {
        if(fade_per_pump && fade_active)throw std::runtime_error("per-pump fade cannot take event ticks");
        tick_fade(xa_active);
    }
    void tick_fade(bool xa_active) {
        if(!fade_active || stopped)return;
        // audio_bgm_update: accumulator += rate; on carry gain -= step.
        fade_accumulator+=fade_rate;
        if(!(fade_accumulator&0xff00))return;
        fade_accumulator&=255;current_gain-=fade_step;++fade_ticks;fade_master=!xa_active && !fade_sequence_volume;
        // audio_bgm_fade_finish: SsSetMVol(0,0) then audio_bgm_control(0x71).
        if(current_gain<0){if(fade_master){cpu.call(0x8008fd68,{0});cpu.call(0x8008b750,{0,0});}stop();fade_active=false;fade_per_pump=false;fade_finish_pump=shiori_pump;bank0_fade_finished();return;}
        int master=full_gain==0?0:(current_gain*127)/full_gain;
        if(fade_master)cpu.call(0x8008b750,{uint32_t(master),uint32_t(master)});
        else cpu.call(0x8008c720,{0,uint32_t(current_gain),uint32_t(current_gain)});
    }
    void start_rei(bool xa_active) {
        if(!initialized || !rei_loaded || rei_started)throw std::runtime_error("Rei start requires its completed alternate bank");
        // The source starts bank1/sequence0 after CUT09/O09 completion. It
        // selects SDKslot2 and preserves existing SPU/reverb instance state.
        if(!stopped){
            if(fade_active && !xa_active){cpu.call(0x8008fd68,{0});cpu.call(0x8008b750,{0,0});}
            stop();
        }
        uint32_t offset=cpu.read(0x8002e800),depth=cpu.read(0x8002e840,1);
        if(offset!=0x40 || depth!=58)throw std::runtime_error("Rei sequence index/reverb differs");
        cpu.call(0x8008ff5c,{depth,depth});
        if(cpu.call(0x800900a0,{0x8002e800+offset+0x10,2})!=0)throw std::runtime_error("Rei sequence open failed");
        current_gain=full_gain=int(cpu.read(0x80016018,1))-25-attenuation;
        cpu.call(0x8008b750,{127,127});cpu.call(0x8008c720,{0,uint32_t(current_gain),uint32_t(current_gain)});cpu.call(0x80090350,{0,1,1});
        stopped=false;rei_started=true;fade_active=false;fade_master=false;
    }
    static int volume(uint32_t raw){if(raw&0x8000)throw std::runtime_error("volume sweep unsupported");return int(raw&0x7fff)-((raw&0x4000)?0x8000:0);}
    static int master_sample(int sample,uint32_t raw){return int((int64_t(sample)*volume(raw))>>14);}
    void flush() {
        for(int i=0;i<24;++i)cpu.write(0x1f801c0c+i*16,voices[i].live?voices[i].env.level:0,2);
        cpu.writes.clear();cpu.call(0x8008e1d0);
        for(auto command:cpu.writes)for(int ch=0;ch<24;++ch)if(command.second&(1u<<ch)){
            auto& v=voices[ch];if(!command.first){if(v.live)v.env.release();continue;}
            uint32_t addr=cpu.read(0x1f801c06+ch*16,2)*8;
            if(kon_log.size()<4096)kon_log.push_back({uint32_t(frames/735),shiori_active?shiori_pump:0u,uint32_t(ch),addr});
            if(bank0_log_pumps && bank0_kon_log.size()<16384)bank0_kon_log.push_back({int64_t(pumps),int64_t(ch),int64_t(addr),0});
            auto s=std::find_if(samples.begin(),samples.end(),[&](auto row){return row.first==addr;});
            if(s==samples.end())throw std::runtime_error("key-on outside original sample inventory");
            if(v.live){++replacements;if(v.env.level>0)++busy_replacements;}
            v=Voice{};v.live=true;v.begin=v.cursor=v.next_cursor=v.loop=s->first;v.end=s->second;v.env.a=cpu.read(0x1f801c08+ch*16,2);v.env.b=cpu.read(0x1f801c0a+ch*16,2);v.history[3]=v.next_sample(sound);
        }
        for(int ch=0;ch<24;++ch){auto& v=voices[ch];v.pitch=std::min<uint32_t>(cpu.read(0x1f801c04+ch*16,2),0x4000);v.left=volume(cpu.read(0x1f801c00+ch*16,2));v.right=volume(cpu.read(0x1f801c02+ch*16,2));v.env.a=cpu.read(0x1f801c08+ch*16,2);v.env.b=cpu.read(0x1f801c0a+ch*16,2);}
    }
    std::array<int16_t,1470> render(bool wet_enabled=true) {
        if(!initialized)throw std::runtime_error("engine not initialized");
        ++pumps;
        if(shiori_active){++shiori_pump;land_shiori();}
        if(bank0_loading){++bank0_load_pump;land_bank0();}
        flush();if(!stopped){if(seq_tick_skip)--seq_tick_skip;else cpu.call(0x80092500,{0,0});}
        std::array<int16_t,1470> pcm{};uint32_t mask=cpu.read(0x1f801d98)&0xffffff;
        if(cpu.read(0x1f801d90)&0xfffffe || cpu.read(0x1f801d94))throw std::runtime_error("noise/pitch modulation unsupported");
        for(int i=0;i<735;++i){int l=0,r=0,sl=0,sr=0;uint32_t count=0;for(int ch=0;ch<24;++ch){auto& v=voices[ch];if(!v.live)continue;++count;auto p=v.tick(sound);l+=p.first;r+=p.second;if(mask&(1u<<ch)){sl+=p.first;sr+=p.second;}}maximum_voices=std::max(maximum_voices,count);auto wet=wet_enabled?reverb.tick(sl,sr,cpu,sound):std::pair<int,int>{0,0};wet_peak=std::max({wet_peak,std::abs(wet.first),std::abs(wet.second)});l+=wet.first;r+=wet.second;l=master_sample(l,cpu.read(0x1f801d80,2));r=master_sample(r,cpu.read(0x1f801d82,2));peak=std::max({peak,std::abs(l),std::abs(r)});pcm[2*i]=int16_t(clamp16(l));pcm[2*i+1]=int16_t(clamp16(r));if(l || r)++nonzero;++frames;}
        if(shiori_active && pump_log.size()<4096)pump_log.push_back({int32_t(shiori_pump),int32_t(cpu.read(0x1f801d80,2)),current_gain});
        // Mix-time state (before this pump's commands), the instant of an emulator timeline row.
        if(bank0_log_pumps && bank0_pump_log.size()<8192)bank0_pump_log.push_back({int64_t(pumps),int64_t(cpu.read(0x1f801d80,2)),current_gain,int64_t(fade_active)});
        // audio_command_pump runs after this pump's SPU tick: the queued poll
        // commands (0x15, 0x23/0x24/0x2E/0x200), then audio_pending_update with
        // the slot2 transfer and audio_bgm_update, whose C1/D1 fade advances
        // once per audio pump. Their effect is visible from the next pump.
        if(shiori_finish_pending)finish_shiori_now();
        bank0_pump_commands();
        if(fade_per_pump && fade_active)tick_fade(false);
        return pcm;
    }
    void begin_shiori_load(const std::array<std::vector<uint8_t>,3>& data,const std::vector<Landing>& schedule) {
        if(!initialized || !rei_started || stopped || shiori_active || shiori_started)throw std::runtime_error("Shiori bank load requires the playing Rei sequence");
        if(!fade_active || !fade_per_pump || fade_rate!=0xc0 || fade_step!=1 || fade_ticks!=0 || fade_accumulator!=0)throw std::runtime_error("Shiori bank load requires the just-started source C1 fade");
        for(int r=0;r<3;++r)if(data[r].size()!=size_t(shiori_sectors[r])*2048)throw std::runtime_error("Shiori CD read extent differs");
        // Validate the measured landing table: per region strictly increasing
        // cumulative sectors, regions complete in issue order (CBA5,CBAA,CBD4).
        std::array<unsigned,3> count{};uint32_t last=0;int region=0;
        for(auto& row:schedule){
            if(row.region>2 || row.pump==0 || row.pump<last || int(row.region)<region || row.count<=count[row.region] || row.count>shiori_sectors[row.region])throw std::runtime_error("Shiori landing schedule is malformed");
            if(int(row.region)>region){for(int r=region;r<int(row.region);++r)if(count[r]!=shiori_sectors[r])throw std::runtime_error("Shiori landing schedule is out of read order");region=int(row.region);if(row.pump==last)throw std::runtime_error("Shiori landing schedule overlaps reads");}
            count[row.region]=row.count;last=row.pump;
        }
        for(int r=0;r<3;++r)if(count[r]!=shiori_sectors[r])throw std::runtime_error("Shiori landing schedule is incomplete");
        shiori_data=data;shiori_schedule=schedule;shiori_next=0;shiori_landed={};shiori_pump=0;shiori_active=true;
    }
    void land_shiori() {
        // Sector-granular nowait reads. Bytes are resident before this pump's
        // SPU flush and SsSeqCalledTbyT read them, as the measurement shows.
        while(shiori_next<shiori_schedule.size() && shiori_schedule[shiori_next].pump<=shiori_pump){
            auto row=shiori_schedule[shiori_next++];
            if(row.pump!=shiori_pump)throw std::runtime_error("Shiori landing pump was skipped");
            auto first=size_t(shiori_landed[row.region])*2048,end=size_t(row.count)*2048;
            std::copy(shiori_data[row.region].begin()+first,shiori_data[row.region].begin()+end,cpu.region(shiori_target[row.region]+uint32_t(first),unsigned(end-first)));
            shiori_landed[row.region]=row.count;
        }
    }
    void finish_shiori() {
        // audio_bank_load_poll at stage11/substep2: queue 0x15, then
        // audio_bank_start(1,0) queues 0x23/0x24/0x2E/0x200 for the next pump.
        if(!shiori_active || shiori_finish_pending || shiori_started)throw std::runtime_error("Shiori bank poll has no in-flight load");
        for(int r=0;r<3;++r)if(shiori_landed[r]!=shiori_sectors[r])throw std::runtime_error("Shiori CD reads are incomplete at the bank poll");
        if(!stopped || fade_active)throw std::runtime_error("Shiori dispose before the C1 fade finished is outside the measured path");
        shiori_finish_pending=true;
    }
    void finish_shiori_now() {
        shiori_finish_pending=false;shiori_finish_pump=shiori_pump;
        // 0x15 audio_bank_dispose(2): the sequence already stopped at fade
        // finish (state_flags&0x100 clear), so only SsVabClose(2) remains.
        cpu.call(0x80090e60,{2});
        occupied.erase(std::remove_if(occupied.begin(),occupied.end(),[](auto row){return row.first>=0x39010 && row.first<0x39010+0x28000;}),occupied.end());
        samples.erase(std::remove_if(samples.begin(),samples.end(),[](auto row){return row.first>=0x39010 && row.first<0x39010+0x28000;}),samples.end());
        // 0x200 with request bit1 still set by C1: audio_bgm_fade_finish under
        // state_flags 0x100000 (no XA) = SsSetSerialAttr-off + SsSetMVol(0,0);
        // the start is deferred until audio_pending_update transfers slot2.
        cpu.call(0x8008fd68,{0});cpu.call(0x8008b750,{0,0});
        if(cpu.call(0x8008c1b0,{0x80016000,2,0x39010})!=2 || cpu.read(0x801293b2,1)!=2)throw std::runtime_error("Shiori VAB open failed");
        if(cpu.call(0x8008c620,{0x801b8000,2})!=2 || cpu.read(0x801293b2,1)!=1)throw std::runtime_error("Shiori VAB transfer failed");
        const auto& header=shiori_data[2];auto half=[&](size_t p){if(p+2>header.size())throw std::runtime_error("Shiori VAB table");return uint32_t(header[p])|(uint32_t(header[p+1])<<8);};
        size_t table=0x820+half(0x12)*512;uint32_t address=0x39010;
        for(unsigned i=0;i<=half(0x16);++i){uint32_t bytes=half(table+i*2)*8;if(bytes){if(address+bytes>0x39010+shiori_data[1].size())throw std::runtime_error("Shiori VAG extent");samples.emplace_back(address,address+bytes);}address+=bytes;}
        // 0x2E audio_sequence_pack_index(0x8002E800,1) + audio_bgm_start.
        uint32_t offset=cpu.read(0x8002e800),depth=cpu.read(0x8002e840,1);
        if(offset!=0x40 || depth!=shiori_data[0][0x40])throw std::runtime_error("Shiori sequence index/reverb differs");
        cpu.call(0x8008ff5c,{depth,depth});
        if(cpu.call(0x800900a0,{0x8002e800+offset+0x10,2})!=0)throw std::runtime_error("Shiori sequence open failed");
        current_gain=full_gain=int(cpu.read(0x80016018,1))-25-attenuation;
        cpu.call(0x8008b750,{127,127});cpu.call(0x8008c720,{0,uint32_t(current_gain),uint32_t(current_gain)});cpu.call(0x80090350,{0,1,1});
        stopped=false;shiori_started=true;fade_active=false;fade_master=false;fade_per_pump=false;
    }
    // ---- native-bank0: the bank0 sequence pack (0x8001C000) after the Shiori start.
    // stage16 audio_bank_start(0,1), stage20/21 audio_command(5)+(0xD1), the stage21
    // audio_bank_load_start(0,0xBF98,0xBF79,0xD3D5,0xD387,0xD37D) landing, the mode 0x34
    // stage 0xFF audio_command(0x11) transfer and audio_bank_start(0,0). Sources:
    // frame_render_audio.c (audio_bank_start/load_start/load_poll/transfer_start) and
    // sound_driver.c (audio_command_pump, audio_control_dispatch, audio_command_bank_*,
    // audio_command_bgm, audio_bgm_update/fade_start/fade_finish/control/start,
    // audio_voices_kill, audio_bank_dispose/request_transfer/transfer_schedule/transfer).
    // Commands are queued by the logic frame and run in the next pump after its SPU
    // tick, in submission order, followed by audio_pending_update.
    bool system_flag=false;          // state_flags 0x100000 (audio_command 5); cleared by audio_bgm_start
    bool fade_sequence_volume=false; // a queued-command fade whose steps use SsSeqSetVol (0x100000 clear)
    bool bank0_log_pumps=false,bgm_start_requested=false,pack0_indexed=false,bank0_loading=false,bank0_loaded=false;
    unsigned bgm_slot=1;             // D_80125D4C; 0x22 -> 1 (audio_state_init also 1)
    // MEASURED (M3, not source-defined): SsSeqCalledTbyT ticks lost because audio_bgm_start
    // (its SpuClearReverbWorkArea DMA wait) ran past the next vsync tick. Stage16: 1, spring: 0.
    unsigned seq_tick_skip=0,start_tick_skip=0;
    int bgm_sequence=-1,bgm_next=-1;   // bgm_sequence_id / bgm_next_sequence_id of a bank0 start
    std::vector<uint16_t> command_queue;
    std::array<uint32_t,16> pack0_address{};std::array<uint32_t,16> pack0_depth{};
    uint64_t pumps=0;uint32_t bank0_load_pump=0,transfer_requests=0;
    static constexpr uint32_t bank0_target[5]={0x80010000,0x801b8000,0x8001c000,0x801c8000,0x80012000};
    static constexpr unsigned bank0_sectors[5]={3,0x20,0x25,0x50,6};
    std::array<std::vector<uint8_t>,5> bank0_data;std::array<unsigned,5> bank0_landed{};
    std::vector<Landing> bank0_schedule;size_t bank0_next_row=0;
    // [pump, kind, a, b]: 1 commands(count,last) 2 fade_start(rate,step) 3 fade_finish(gain,master)
    // 4 bgm_start(sequence,gain) 5 voices_kill(released) 6 dispose(slot) 7 transfer(slot,bytes)
    // 8 fade_ignored(command) 9 bank0_load 10 landing_complete(region)
    std::vector<std::array<int64_t,4>> bank0_log,bank0_pump_log,bank0_kon_log;
    void note(int kind,int64_t a=0,int64_t b=0){if(bank0_log.size()<4096)bank0_log.push_back({int64_t(pumps),int64_t(kind),a,b});}
    static bool bank0_command(unsigned command){return command==5 || command==0xd1 || command==0x11 || command==0x21 || command==0x22 || command==0x2f || (command>=0x200 && command<=0x20f);}
    void submit(unsigned command) {
        if(!initialized)throw std::runtime_error("audio command requires an initialized engine");
        if(!bank0_command(command))throw std::runtime_error("audio command outside the modelled bank0 set");
        if(shiori_finish_pending || (shiori_active && !shiori_started))throw std::runtime_error("bank0 command during the in-flight Shiori load");
        if(command_queue.size()>=32)throw std::runtime_error("audio command queue overflow");
        command_queue.push_back(uint16_t(command));bank0_log_pumps=true;
    }
    // audio_bank_start(0,n): reads the pack index word at call time; -1 queues nothing.
    int bank0_start(unsigned sequence,unsigned tick_skip) {
        if(!initialized)throw std::runtime_error("bank0 start requires an initialized engine");
        if(sequence>15)throw std::runtime_error("bank0 sequence outside the pack index");
        if(tick_skip>1)throw std::runtime_error("bank0 start tick skip outside the measured values");
        if((cpu.read(0x8001c000)&0xffff)!=0x40)return -1;
        submit(0x22);submit(0x21);submit(0x2f);submit(0x200|sequence);start_tick_skip=tick_skip;
        return 1;
    }
    void begin_bank0_load(const std::array<std::vector<uint8_t>,5>& data,const std::vector<Landing>& schedule) {
        if(!initialized || bank0_loading || bank0_loaded)throw std::runtime_error("bank0 load is single-use and needs an initialized engine");
        if(!shiori_started)throw std::runtime_error("bank0 load is modelled only after the Shiori start");
        for(int r=0;r<5;++r)if(data[r].size()!=size_t(bank0_sectors[r])*2048)throw std::runtime_error("bank0 CD read extent differs");
        // Issue order b,c,f,e,d: regions complete in order, counts strictly increase.
        std::array<unsigned,5> count{};uint32_t last=0;int region=0;
        for(auto& row:schedule){
            if(row.region>4 || row.pump==0 || row.pump<last || int(row.region)<region || row.count<=count[row.region] || row.count>bank0_sectors[row.region])throw std::runtime_error("bank0 landing schedule is malformed");
            if(int(row.region)>region){for(int r=region;r<int(row.region);++r)if(count[r]!=bank0_sectors[r])throw std::runtime_error("bank0 landing schedule is out of read order");region=int(row.region);if(row.pump==last)throw std::runtime_error("bank0 landing schedule overlaps reads");}
            count[row.region]=row.count;last=row.pump;
        }
        for(int r=0;r<5;++r)if(count[r]!=bank0_sectors[r])throw std::runtime_error("bank0 landing schedule is incomplete");
        bank0_data=data;bank0_schedule=schedule;bank0_next_row=0;bank0_landed={};bank0_load_pump=0;bank0_loading=true;bank0_log_pumps=true;note(9);
    }
    void land_bank0() {
        while(bank0_next_row<bank0_schedule.size() && bank0_schedule[bank0_next_row].pump<=bank0_load_pump){
            auto row=bank0_schedule[bank0_next_row++];
            if(row.pump!=bank0_load_pump)throw std::runtime_error("bank0 landing pump was skipped");
            auto first=size_t(bank0_landed[row.region])*2048,end=size_t(row.count)*2048;
            std::copy(bank0_data[row.region].begin()+first,bank0_data[row.region].begin()+end,cpu.region(bank0_target[row.region]+uint32_t(first),unsigned(end-first)));
            bank0_landed[row.region]=row.count;
            if(row.count==bank0_sectors[row.region])note(10,row.region);
        }
    }
    bool bank0_complete() const {for(int r=0;r<5;++r)if(bank0_landed[r]!=bank0_sectors[r])return false;return bank0_loading;}
    void forget_slot(uint32_t begin,uint32_t end) {
        occupied.erase(std::remove_if(occupied.begin(),occupied.end(),[&](auto row){return row.first>=begin && row.first<end;}),occupied.end());
        samples.erase(std::remove_if(samples.begin(),samples.end(),[&](auto row){return row.first>=begin && row.first<end;}),samples.end());
    }
    // audio_voices_kill (not a TT game): SDK all-key-off and reverb reset calls.
    void voices_kill() {
        cpu.writes.clear();
        cpu.call(0x80090db0);cpu.call(0x8008fd68,{0});cpu.call(0x8008fed0);cpu.call(0x800949b0,{3});cpu.call(0x8008feb0);
        int released=0;
        for(auto command:cpu.writes)for(int ch=0;ch<24;++ch)if(command.second&(1u<<ch)){
            if(command.first)throw std::runtime_error("audio_voices_kill wrote a key-on");
            if(voices[ch].live){voices[ch].env.release();++released;}
        }
        cpu.writes.clear();
        if(cpu.read(0x1f801da2,2)*8!=reverb.base)reverb.reset(cpu.read(0x1f801da2,2)*8);
        note(5,released);
    }
    // audio_command_bank_assign 0x2F: audio_sequence_pack_index(0x8001C000, 0).
    void index_pack0() {
        for(int i=0;i<16;++i){uint32_t off=cpu.read(0x8001c000+i*4);if(off==0)off=cpu.read(0x8001c000);pack0_address[i]=0x8001c000+off+0x10;pack0_depth[i]=cpu.read(0x8001c040+i,1);}
        pack0_indexed=true;
    }
    // audio_bgm_start with D_80125D4C == 1 and D_80125D4A == 0.
    void bgm_start_bank0() {
        if(bgm_slot!=1 || !pack0_indexed || bgm_sequence<0)throw std::runtime_error("bank0 start lacks its slot/pack/sequence assignment");
        voices_kill();
        cpu.call(0x8008ff5c,{pack0_depth[bgm_sequence],pack0_depth[bgm_sequence]});
        if(cpu.call(0x800900a0,{pack0_address[bgm_sequence],1})!=0)throw std::runtime_error("bank0 sequence open failed (source error-report path is not modelled)");
        current_gain=full_gain=int(cpu.read(0x80012018,1))-25-attenuation;
        cpu.call(0x8008b750,{127,127});cpu.call(0x8008c720,{0,uint32_t(current_gain),uint32_t(current_gain)});cpu.call(0x80090350,{0,1,1});
        seq_tick_skip=start_tick_skip;start_tick_skip=0;
        stopped=false;system_flag=false;fade_active=false;fade_master=false;fade_per_pump=false;fade_sequence_volume=false;bgm_start_requested=false;
        note(4,bgm_sequence,current_gain);
    }
    void request_start() {
        if(transfer_requests&(1u<<bgm_slot)){bgm_start_requested=true;return;}
        bgm_start_bank0();
    }
    // audio_bgm_control(0x71): stop the sequence handle if state 0x100, then promote next.
    void control_71() {
        if(!stopped){cpu.call(0x8008c720,{0,0,0});cpu.call(0x80090580,{0});cpu.call(0x800907e0,{0});stopped=true;}
        bgm_sequence=bgm_next;bgm_next=-1;fade_active=false;fade_per_pump=false;fade_sequence_volume=false;
    }
    // audio_bgm_fade_finish outside a fade step (0x200 during a fade, 0x11 on the BGM slot).
    void fade_finish_source() {
        bool master=system_flag;  // && !(state_flags & 0x400): XA is refused by the owner.
        if(master){cpu.call(0x8008fd68,{0});cpu.call(0x8008b750,{0,0});}
        note(3,current_gain,master);
        control_71();
        if(bgm_sequence!=-1)request_start();
    }
    // Tail of tick_fade's finish (audio_bgm_fade_finish after the step underflow).
    void bank0_fade_finished() {
        note(3,current_gain,fade_master);
        bgm_sequence=bgm_next;bgm_next=-1;fade_sequence_volume=false;
        if(bgm_sequence!=-1)request_start();
    }
    void fade_command(int rate,int step) {
        // audio_bgm_fade_start: ignored while request bit 1 is set.
        if(fade_active){note(8,rate);return;}
        if(stopped)throw std::runtime_error("bank0 fade without a playing sequence is not modelled");
        fade_active=true;fade_accumulator=0;fade_ticks=0;fade_rate=rate;fade_step=step;fade_per_pump=true;fade_sequence_volume=!system_flag;fade_master=system_flag;
        note(2,rate,step);
    }
    void dispose(unsigned slot) {
        // audio_bank_dispose: the BGM slot stops/closes the score (state 0x100 stays set);
        // the SE slot clears serial attributes; then SsVabClose.
        if(slot==bgm_slot){if(!stopped){cpu.call(0x80090580,{0});cpu.call(0x800907e0,{0});}bgm_sequence=-1;bgm_next=-1;}
        else if(slot==0)cpu.call(0x8008fd68,{0});
        cpu.call(0x80090e60,{slot});
        if(slot==0)forget_slot(0x1010,0x11010);else forget_slot(0x11010,0x39010);
        note(6,slot);
    }
    void request_transfer(unsigned slot) {
        if(slot==bgm_slot && !stopped)fade_finish_source();
        transfer_requests|=1u<<slot;
    }
    void dispatch(unsigned command) {
        if(command==5){system_flag=true;if(fade_active)fade_sequence_volume=false;return;}
        if(command==0xd1){fade_command(0x80,1);return;}
        if(command==0x22){bgm_slot=1;return;}
        if(command==0x21)return;  // SE slots 0/3: no native SE playback is modelled.
        if(command==0x2f){index_pack0();return;}
        if(command==0x11){
            if(!bank0_complete())throw std::runtime_error("0x11 transfer before the five bank0 reads landed");
            // audio_bank_body_address_set(17): slot0 0x801B8000, slot1 0x801C8000.
            dispose(0);request_transfer(0);dispose(1);request_transfer(1);bank0_loaded=true;return;
        }
        // 0x200|n, audio_command_bgm case 0.
        int sequence=int(command&0xff);
        if(fade_active){bgm_next=sequence;fade_finish_source();}
        else if(!stopped){bgm_next=sequence;fade_command(0x100,10);}
        else{bgm_sequence=sequence;bgm_next=-1;request_start();}
    }
    void transfer_slot(unsigned slot) {
        if(slot>1)throw std::runtime_error("only bank0 slots 0/1 are modelled");
        uint32_t hp=slot?0x80012000:0x80010000,bp=slot?0x801c8000:0x801b8000,spu=slot?0x11010:0x1010,limit=slot?0x28000:0x10000;
        const auto& header=bank0_data[slot?4:0];
        if(cpu.call(0x8008c1b0,{hp,slot,spu})!=slot || cpu.read(0x801293b0+slot,1)!=2)throw std::runtime_error("bank0 VAB open failed");
        if(cpu.call(0x8008c620,{bp,slot})!=slot || cpu.read(0x801293b0+slot,1)!=1)throw std::runtime_error("bank0 VAB transfer failed");
        // VAG size table of the landed original header bytes (not the SDK-mutated RAM copy).
        auto half=[&](size_t p){if(p+2>header.size())throw std::runtime_error("bank0 VAB table");return uint32_t(header[p])|(uint32_t(header[p+1])<<8);};
        size_t table=0x820+half(0x12)*512;uint32_t address=spu,bytes_total=0;
        for(unsigned i=0;i<=half(0x16);++i){uint32_t bytes=half(table+i*2)*8;if(bytes){if(address+bytes>spu+limit)throw std::runtime_error("bank0 VAG extent");samples.emplace_back(address,address+bytes);}address+=bytes;bytes_total+=bytes;}
        note(7,slot,bytes_total);
    }
    void bank0_pump_commands() {
        if(!command_queue.empty()){
            auto queue=std::move(command_queue);command_queue.clear();
            note(1,int64_t(queue.size()),queue.back());
            for(auto command:queue)dispatch(command);
        }
        // audio_pending_update: one requested slot per pump (audio_bank_transfer_schedule),
        // then audio_bgm_update's deferred start; its fade step follows in render().
        for(unsigned slot=0;slot<4;++slot)if(transfer_requests&(1u<<slot)){transfer_requests&=~(1u<<slot);transfer_slot(slot);break;}
        if(bgm_start_requested && !(transfer_requests&(1u<<bgm_slot)))bgm_start_bank0();
    }
    bool bank0_transfer_done() const {return bank0_loaded && transfer_requests==0;}
    void stop(){if(!initialized || stopped)return;cpu.call(0x8008c720,{0,0,0});cpu.call(0x80090580,{0});cpu.call(0x800907e0,{0});stopped=true;}
};
}
