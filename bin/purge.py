import couchdb
import datetime
import pycurl

def main():
    s = [line.strip() for line in open("/root/I2C/bin/config.txt")]
    conn_test = couchdb.Server(s[0])
    print conn_test['_all_dbs']
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    docs = [id for id in db if (id[0] == '2')]
    docs = [int(i) for i in docs]
    now = datetime.datetime.now()
    time = int(now.strftime("%Y%m%d%H%M%S"))
    for i in docs:
        if ((time-i)>10000):
            del db[str(i)]
    c = pycurl.Curl()
    url = 'http://127.0.0.1:5984/data/_compact'
    c.setopt(pycurl.URL, url)
    c.setopt(pycurl.HTTPHEADER, ['Content-Type:application/json'])
    c.setopt(pycurl.POST, 1)
    c.perform()


if (__name__ == '__main__'):
    main()
