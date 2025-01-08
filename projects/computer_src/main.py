
from thread import *

import sys
import os
sys.path.append(os.path.abspath("../inc"))

def main():
    initialize_threads()
    print("Main thread is running")
    

if __name__ ==   '__main__':
    main()
