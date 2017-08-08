from book import *

def getPages(lines):
    book = []
    pages = [lines[i] for i in range(len(lines)) if (lines[i][:7] == "Page 0x" and "." not in lines[i])]
    pages = list(set(pages))
    for i in pages:
        start = 0
        end = 0
        description, number = None, None
        for j in range(len(i)):
            if (start == 0 and i[j] == "0"):
                start = j
            elif (end == 0 and start != 0 and (i[j] == ":" or i[j] == " ")):
                end = j
                number = i[start:end]
            elif (end != 0 and i[j] == " "):
                start = j+1
                description = i[start:]
                break
        book.append(Page(number, description))
    return book

def main():
    filename = "53134-PR103-RDS.txt"
    f = open(filename)
    lines = [line.strip() for line in f]
    print "Total lines", len(lines)
    lines = [i for i in lines if (("Broadcom" not in i and "April" not in i))]
    print "Total lines", len(lines)
    lines = [i for i in lines if ("Register Programming Guide" not in i)]
    print "Total lines", len(lines)
    book = getPages(lines)
    lines = [i for i in lines if (i[:4] != "Page")]
    for i in book:
        print "%s,%s"%(i.number,i.description)

if (__name__ == "__main__"):
    main()
