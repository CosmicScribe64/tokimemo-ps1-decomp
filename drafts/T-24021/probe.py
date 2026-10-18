"""T-24021 bounded call-path / block-order probes; Docker only."""
from pathlib import Path
import sys, json
sys.path.insert(0, 'tools')
import funcdiff, order_search
c = funcdiff.locate('dialogue_speak_part', 'main')
base = Path(c.src).read_text()
score = order_search.make_text_scorer(c, 'dialogue_speak_part', resolve=True)
root = Path('build/t24021')
root.mkdir(parents=True, exist_ok=True)
pairs = [
('temp_s1 += 0x10;\n            dialogue_render(-0x28, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);', '(s32) arg6'),
('temp_s1 += 0x10;\n            dialogue_render(-0x28, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);', '2')]
variants = {'baseline': base}
for mode in ('backward_goto', 'backward_goto_reversed', 'do_goto', 'shared_call'):
    text = base
    for i, (old, color) in enumerate(pairs):
        call = 'dialogue_render(-0x28, (s32) temp_s1, %s, (s32) sp7C, 0, "", "", 0);' % color
        if mode == 'backward_goto':
            new = 'goto inc%d;\nrender%d:\n            %s\n            goto end%d;\ninc%d:\n            temp_s1 += 0x10;\n            goto render%d;\nend%d:;' % (i,i,call,i,i,i,i)
        elif mode == 'backward_goto_reversed':
            new = 'goto inc%d;\nrender%d:\n            %s\n            goto end%d;\ninc%d:\n            temp_s1 += 0x10;\n            goto render%d;\nend%d:;' % (i,i,call,i,i,i,i)
            if i == 0: continue
        elif mode == 'do_goto':
            new = 'goto inc%d;\n            do {\n                %s\n                break;\ninc%d:\n                temp_s1 += 0x10;\n            } while (1);' % (i,call,i)
        else:
            new = 'temp_s1 += 0x10;\n            goto shared%d;' % i
            # Place shared blocks before the switch: uopt can schedule them at their uses.
            marker = '    switch (*(u8 *)&arg1) {'
            text = text.replace(marker, '    goto switch_start%d;\nshared%d:\n    %s\n    goto chain_end;\nswitch_start%d:\n%s' % (i,i,call,i,marker))
        assert old in text
        text = text.replace(old,new,1)
    if mode == 'shared_call':
        text = text.replace('        dialogue->unk_14 = 1;\n        break;', 'chain_end:\n        dialogue->unk_14 = 1;\n        break;',1)
    variants[mode]=text
# Natural common tail with explicit coordinates and colour across the final chain.
text = base
start = text.index('        } else if (strcmp(sp7C, "そして…")')
end = text.index('        dialogue->unk_14 = 1;', start)
chain = text[start:end]
for localtype in ('s32','s16'):
    altered = chain
    import re
    altered = re.sub(r'dialogue_render\((-0x(?:28|80)), \(s32\) temp_s1, (.*?), \(s32\) sp7C, 0, "", "", 0\);', r'x = \1; color = \2;', altered)
    # Only execute common tail for the final chain: put it in an outer else.
    inner = altered[len('        } else '):]
    common = '        } else {\n            %s x;\n            s32 color;\n            %s\n            dialogue_render(x, (s32)temp_s1, color, (s32)sp7C, 0, "", "", 0);\n        }\n' % (localtype, inner)
    variants['common_tail_'+localtype] = text[:start]+common+text[end:]
for label,text in variants.items():
    out=root/label
    result=score(text,out,label)
    print(label, result, flush=True)
    with (root/'results.jsonl').open('a') as f: f.write(json.dumps({'variant':label,'aligned_cost':result})+'\n')
