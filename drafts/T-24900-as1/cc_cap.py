import sys
sys.path.insert(0, "/work/tools")
import cc
cc.SHIMS["as1"] = "/work/build/as1tr/shim.py"
cc.main(sys.argv)
