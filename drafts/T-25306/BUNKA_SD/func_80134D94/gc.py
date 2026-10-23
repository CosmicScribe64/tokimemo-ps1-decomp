import sys, os
sys.path.insert(0, 'tools')
import cc
cc.SHIMS['as1'] = os.path.abspath('build/T-25306/as1save.py')
os.environ['T25306_G'] = sys.argv[3]
cc.compile_ido(sys.argv[1], sys.argv[2], '5.3')
