import couchdb

def main():
    local = couchdb.Server("http://127.0.0.1:5984")
    for db in local:
        print db
    db = local['atca001']
    for doc in db:
        #print db[doc]
        print db[doc]['time']

if (__name__ == '__main__'):
    main()
