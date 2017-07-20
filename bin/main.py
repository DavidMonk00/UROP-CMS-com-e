from libI2C import *
import couchdb

def main():
    A = ATCABoard("SEMA")
    buses = [str(i) for i in A.getBuses()]
    A.setBus(buses[0])
    devices = [str(i) for i in A.getDevices()]
    A.setDevice(devices[0])
    properties = [str(i) for i in A.getProperties()]
    d = {}
    for i in properties:
        d[i] = A.read(i)
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    doc_id, doc_rev = db.save(d)
    print doc_id
    print doc_rev


if (__name__ == '__main__'):
    main()
