from libI2C import *
import couchdb
import datetime
import pycurl
import json

def sendFlag(data, metadata):
    target = [line.strip() for line in open('/root/I2C/bin/config.txt')]
    server = couchdb.Server(target[0])
    db = server['flags']
    now = datetime.datetime.now()
    flag_dict = {
                    '_id': target[1] + "-" + now.strftime("%Y%m%d%H%M%S"),
                    'properties': data,
                    'metadata': metadata
                 }
    doc_id, doc_rev = db.save(flag_dict)

def getData():
    flag = False
    ATCA = ATCABoard("SEMA")
    board_dict = {'_id': 'static'}
    buses = [str(i) for i in ATCA.getBuses()]
    with open('/root/I2C/bin/active.json') as data_file:
        active = json.load(data_file)
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    doc = db['static']
    metadata = []
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
                    register = str(ATCA.read(prop))
                    board_dict[bus][device][prop] = register
                    if (register != doc[bus][device][prop]):
                        flag = True
                        metadata.append([bus, device, prop])
    if (flag):
        sendFlag(board_dict,metadata)
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
    target = [line.strip() for line in open('/root/I2C/bin/config.txt')]
    data = json.dumps({"source":"data",
                       "target":target[0] + target[1],
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
