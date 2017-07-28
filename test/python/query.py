import couchdb
import numpy as np
from matplotlib import pyplot as plt

def main():
    S = couchdb.Server()
    db = S['atca001']
    print db['_design/atca/views']
    dat = db.view('_design/atca/CPUtemp')
    print dat
    # y = np.array([row.value for row in dat])
    # x = np.array([int(row.key) for row in dat])
    # m = 0
    # print dat[0]
    # print max(y)
    # plt.plot(y)
    # plt.show()

if (__name__ == '__main__'):
    main()
