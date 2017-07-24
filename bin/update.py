from libI2C import *
import couchdb
import datetime
import pycurl
import json

def getData():
    A = ATCABoard("SEMA")
    now = datetime.datetime.now()
    board_dict = {'_id': now.strftime("%Y%m%d%H%M%S")}
    buses = [str(i) for i in A.getBuses()]
    with open('active.json') as data_file:
        active = json.load(data_file)
    for bus in buses:
        board_dict[bus] = {}
        A.setBus(bus)
        devices = [str(i) for i in A.getDevices()]
        for device in devices:
            board_dict[bus][device] = {}
            A.setDevice(device)
            properties = [str(i) for i in A.getProperties()]
            for prop in properties:
                if active[bus][device][prop]:
                    board_dict[bus][device][prop] = str(A.read(prop))
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    doc_id, doc_rev = db.save(board_dict)

def updateServer():
    c = pycurl.Curl()
    url = 'http://127.0.0.1:5984/_replicate'
    target = [line.strip() for line in open('config.txt')][0]
    data = json.dumps({"source":"data",
                       "target":target,
                       "filter":"filters/replicate_filter"})
    c.setopt(pycurl.URL, url)
    c.setopt(pycurl.HTTPHEADER, ['Content-Type:application/json'])
    c.setopt(pycurl.POST, 1)
    c.setopt(pycurl.POSTFIELDS, data)
    c.perform()

def main():
    getData()
    updateServer()

if (__name__ == '__main__'):
    main()
