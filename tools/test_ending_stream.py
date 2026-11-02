"""T-39517: production epilogue queue streams complete text through a bounded atlas."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EndingStreamTest(unittest.TestCase):
    def test_long_summary_reaches_every_row_without_loading_it_all(self):
        source = (ROOT / 'src/ovl/ending/career_and_epilogue_summary.c').read_text()
        start = source.index('#define ENDING_ENGLISH_ROWS')
        queue = source[start:source.index('\n#endif', start)]
        fixture = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
#define ENGLISH_GLYPH_SLOTS 384
typedef s32 (*EnglishTextCapture)(s16,s16,u8,u8 *,s32);
static EnglishTextCapture english_text_capture;
typedef struct { s16 x,y; u8 attribute,glyph_count,extra; signed char active; } TextEntry;
static TextEntry text_list[52];
static struct { int count,total,mark,pos; } text_state;
static int seen[40], peak, loads;
static void k_reset(int count) {
    assert(count==0);text_state.count=text_state.total=text_state.pos=0;
}
static int set_kanji_string(int x,int y,int color,u8 *text,int slot) {
    int len=(int)strlen((char *)text), id=-1;
    (void)color;(void)slot;
    assert(y>=-80 && y<=80);
    assert(text_state.total+len<=ENGLISH_GLYPH_SLOTS);
    assert(text_state.count<52);
    text_list[text_state.count].x=x;text_list[text_state.count].y=y;
    text_state.total+=len;loads++;
    if(text_state.total>peak)peak=text_state.total;
    sscanf((char *)text,"%d",&id);
    if(id>=0 && id<40)seen[id]=1;
    return ++text_state.count;
}
'''
        checks = r'''
int main(void) {
    int i, tick, previous;
    char row[160], expected[40][40];
    ending_english_begin();
    for(i=0;i<40;i++) {
        sprintf(row,"%02d abcdefghijklmnopqrstuvwxyz",i);
        strcpy(expected[i],row);
        assert(english_text_capture(-72,72+i*16,0,(u8 *)row,0)==i+1);
    }
    english_text_capture=0;
    assert(ending_english_used>1024 && text_state.total==0);
    for(tick=0;tick<900;tick++) {
        ending_english_scroll=tick;
        ending_english_refresh();
        previous=loads;
        ending_english_refresh();
        assert(loads==previous);
        assert(text_state.pos==text_state.total && text_state.total<=384);
        for(i=0;i<text_state.count;i++) assert(text_list[i].y>=-80 && text_list[i].y<=80);
    }
    assert(ending_english_overflow==0 && peak>0 && peak<=384);
    for(i=0;i<40;i++) {
        assert(seen[i]);
        assert(!strcmp((char *)&ending_english_text[ending_english_rows[i].offset],expected[i]));
    }
    ending_english_begin();
    for(i=0;i<128;i++) assert(ending_english_capture(0,i*16,0,(u8 *)"x",0)>0);
    assert(ending_english_capture(0,0,0,(u8 *)"x",0)==-1);
    assert(ending_english_overflow==1 && ending_english_count==128);
    ending_english_begin();memset(row,'X',159);row[159]=0;
    for(i=0;i<25;i++)assert(ending_english_capture(0,i*16,0,(u8 *)row,0)>0);
    assert(ending_english_capture(0,0,0,(u8 *)row,0)==-1);
    assert(ending_english_used==4000 && ending_english_overflow==1);
    ending_english_begin();
    for(i=0;i<4;i++) assert(ending_english_capture(0,i*16,0,(u8 *)row,0)>0);
    english_text_capture=0;
    ending_english_refresh();
    assert(ending_english_overflow==2 && text_state.count==2);
    text_list[2].y=1234;
    ending_english_refresh();
    assert(text_list[2].y==1234);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            code,binary=Path(tmp)/'stream.c',Path(tmp)/'stream'
            code.write_text(fixture+queue+checks)
            subprocess.run(['cc','-std=c89','-Wall','-Werror',str(code),'-o',str(binary)],check=True)
            subprocess.run([str(binary)],check=True)
