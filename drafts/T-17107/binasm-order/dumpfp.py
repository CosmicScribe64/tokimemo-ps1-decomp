import os, shutil, sys
sys.path.insert(0, "/work/tools")
import frame_pass
def main(argv):
    real, args = argv[2], argv[3:]
    valued = ("-G", "-o", "-t", "-temp"); i = 0
    while i < len(args):
        if args[i] == "-t": shutil.copy(args[i + 1], os.environ["DUMP_G"] + ".T"); i += 2
        elif args[i] in valued: i += 2
        elif args[i].startswith("-"): i += 1
        else: shutil.copy(args[i], os.environ["DUMP_G"]); i += 1
    open(os.environ["DUMP_G"] + ".args", "w").write("\n".join(argv))
    frame_pass.main(argv)
main(sys.argv)
