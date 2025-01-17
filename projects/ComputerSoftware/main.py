### Main file for the computer project

from thread import *

import sys
import os
sys.path.append(os.path.abspath("../inc"))

### Main function

def main():
    initialize_threads()
    print("Main thread is running")
    

if __name__ ==   '__main__':
    main()
