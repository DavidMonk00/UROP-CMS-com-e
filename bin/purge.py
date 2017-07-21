import couchdb
import datetime

def main():
    server = couchdb.Server("http://127.0.0.1:5984")
    db = server['data']
    docs = [id for id in db if (id[0] != '_')]
    docs = [int(i) for i in docs]
    now = datetime.datetime.now()
    time = int(now.strftime("%Y%m%d%H%M%S"))
    for i in docs:
        if ((time-i)>10000):
            del db[str(i)]

if (__name__ == '__main__'):
    main()
