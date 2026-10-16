import os, sys
sys.path.insert(0, "/work/tools")
import cc
cc.SHIMS["as1"] = os.path.join(os.path.dirname(os.path.abspath(__file__)), "dumpfp.py")
src, out = sys.argv[1], sys.argv[2]
os.environ["DUMP_G"] = out + ".G"
cc.main(["cc.py", src, out, "ido", "5.3"])
