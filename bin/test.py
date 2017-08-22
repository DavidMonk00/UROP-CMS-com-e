import couchdb
import numpy as np
import matplotlib.pyplot as plt
import datetime
import pandas as pd

def movingaverage(interval, window_size):
    window = numpy.ones(int(window_size))/float(window_size)
    return numpy.convolve(interval, window, 'same')

def main():
    server = couchdb.Server()
    database = server['atca001']
    map_fun =   '''function(doc) {
                    emit(null,parseFloat(doc.internal.CPU.temperature));
                }'''
    format = '%Y%m%d%H%M%S'
    data = np.array([(datetime.datetime.strptime(i.id,format),i.value) for i in database.query(map_fun)])
    x = data[:,0]
    y = data[:,1]
    df = pd.DataFrame(data,columns=['Time','Temperature'])
    df.index = df['Time']
    del df['Time']
    df = df[datetime.datetime(2017, 8, 9):]
    df['hour'] = [ts.hour for ts in df.index]
    df.Temperature = pd.asFloat
    print df.Temperature.resample('D')
    # plt.show()

if (__name__ == '__main__'):
    main()
