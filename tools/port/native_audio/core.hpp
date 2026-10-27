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
                if(pc==0x80095b70 || pc==0x80097948) {
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
    }
    static int volume(uint32_t raw){if(raw&0x8000)throw std::runtime_error("volume sweep unsupported");return int(raw&0x7fff)-((raw&0x4000)?0x8000:0);}
    void flush() {
        for(int i=0;i<24;++i)cpu.write(0x1f801c0c+i*16,voices[i].live?voices[i].env.level:0,2);
        cpu.writes.clear();cpu.call(0x8008e1d0);
        for(auto command:cpu.writes)for(int ch=0;ch<24;++ch)if(command.second&(1u<<ch)){
            auto& v=voices[ch];if(!command.first){if(v.live)v.env.release();continue;}
            uint32_t addr=cpu.read(0x1f801c06+ch*16,2)*8;auto s=std::find_if(samples.begin(),samples.end(),[&](auto row){return row.first==addr;});
            if(s==samples.end())throw std::runtime_error("key-on outside original sample inventory");
            if(v.live){++replacements;if(v.env.level>0)++busy_replacements;}
            v=Voice{};v.live=true;v.begin=v.cursor=v.next_cursor=v.loop=s->first;v.end=s->second;v.env.a=cpu.read(0x1f801c08+ch*16,2);v.env.b=cpu.read(0x1f801c0a+ch*16,2);v.history[3]=v.next_sample(sound);
        }
        for(int ch=0;ch<24;++ch){auto& v=voices[ch];v.pitch=std::min<uint32_t>(cpu.read(0x1f801c04+ch*16,2),0x4000);v.left=volume(cpu.read(0x1f801c00+ch*16,2));v.right=volume(cpu.read(0x1f801c02+ch*16,2));v.env.a=cpu.read(0x1f801c08+ch*16,2);v.env.b=cpu.read(0x1f801c0a+ch*16,2);}
    }
    std::array<int16_t,1470> render(bool wet_enabled=true) {
        if(!initialized)throw std::runtime_error("engine not initialized");
        flush();if(!stopped)cpu.call(0x80092500,{0,0});
        std::array<int16_t,1470> pcm{};uint32_t mask=cpu.read(0x1f801d98)&0xffffff;
        if(cpu.read(0x1f801d90)&0xfffffe || cpu.read(0x1f801d94))throw std::runtime_error("noise/pitch modulation unsupported");
        for(int i=0;i<735;++i){int l=0,r=0,sl=0,sr=0;uint32_t count=0;for(int ch=0;ch<24;++ch){auto& v=voices[ch];if(!v.live)continue;++count;auto p=v.tick(sound);l+=p.first;r+=p.second;if(mask&(1u<<ch)){sl+=p.first;sr+=p.second;}}maximum_voices=std::max(maximum_voices,count);auto wet=wet_enabled?reverb.tick(sl,sr,cpu,sound):std::pair<int,int>{0,0};wet_peak=std::max({wet_peak,std::abs(wet.first),std::abs(wet.second)});l+=wet.first;r+=wet.second;peak=std::max({peak,std::abs(l),std::abs(r)});pcm[2*i]=int16_t(clamp16(l));pcm[2*i+1]=int16_t(clamp16(r));if(l || r)++nonzero;++frames;}
        return pcm;
    }
    void stop(){if(!initialized || stopped)return;cpu.call(0x8008c720,{0,0,0});cpu.call(0x80090580,{0});cpu.call(0x800907e0,{0});stopped=true;}
};
}
