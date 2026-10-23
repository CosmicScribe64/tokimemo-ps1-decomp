"""T-25604 scratch as1 shim: save the binasm input to $T25604_G, then run frame_pass as usual."""
import sys, os, shutil
sys.path.insert(0, os.path.join(os.getcwd(), 'tools'))
import frame_pass
argv = sys.argv
real = argv[2]; args = argv[3:]
if '-t' in args:
    shutil.copy(args[args.index('-t')+1], os.environ.get('T25604_G', '/tmp/in.G')[:-2] + '.T')
i = 0; valued = ("-G", "-o", "-t", "-temp")
while i < len(args):
    if args[i] in valued: i += 2
    elif args[i].startswith('-'): i += 1
    else:
        shutil.copy(args[i], os.environ.get('T25604_G', '/tmp/in.G')); break
sys.exit(frame_pass.run_as1(real, args))
