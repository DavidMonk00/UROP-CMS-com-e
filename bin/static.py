from libI2C import *
import couchdb
import datetime
import pycurl
import json

def getData():
    ATCA = ATCABoard("SEMA")
    now = datetime.datetime.now()
    board_dict = {'_id': 'static'}
    buses = [str(i) for i in ATCA.getBuses()]
    with open('active.json') as data_file:
        active = json.load(data_file)
    for bus in buses:
        board_dict[bus] = {}
        ATCA.setBus(bus)
        devices = [str(i) for i in ATCA.getDevices()]
        for device in devices:
            board_dict[bus][device] = {}
            ATCA.setDevice(device)
            properties = [str(i) for i in ATCA.getProperties()]
            for prop in properties:
                if (active[bus][device][prop] == False):
                    board_dict[bus][device][prop] = str(ATCA.read(prop))
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    doc = db['static']
    board_dict['_rev'] = doc['_rev']
    doc = board_dict
    for i in doc:
        print i
    doc_id, doc_rev = db.save(doc)

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
