import xml.etree.ElementTree as et
import json

def isHex(x):
    try:
        int(x, 16)
        return True
    except ValueError:
        return False

def getRange(number):
    r = [pos for pos, char in enumerate(number) if char == 'x']
    r = [int(number[i+1:i+3],16) for i in r]
    return range(r[0],r[1]+1)

def generatePageList():
    filename = "53134-PR103-RDS.txt"
    f = open(filename)
    lines = [line.strip() for line in f]
    lines = [i for i in lines if (("Broadcom" not in i and "April" not in i))]
    lines = [i for i in lines if ("Register Programming Guide" not in i)]
    book = {}
    pages = [lines[i] for i in range(len(lines)) if (lines[i][:7] == "Page 0x" and "." not in lines[i])]
    pages = list(set(pages))
    for i in pages:
        start = 0
        end = 0
        description, number = None, None
        for i in range(len(i)):
            if (start == 0 and i[i] == "0"):
                start = i
            elif (end == 0 and start != 0 and (i[i] == ":" or i[i] == " ")):
                end = i
                number = i[start:end]
            elif (end != 0 and i[i] == " "):
                start = i+1
                description = i[start:]
                break
        if "-" in number:
            for i in getRange(number):
                print i
                book[i] = {}
                book[i]["description"] = description
                book[i]["addresses"] = {}
        else:
            number = int(number,16)
            print number
            book[number] = {}
            book[number]["description"] = description
            book[number]["addresses"] = {}
    return book

def getPageOffsetNumbers(div):
    for i in div.iter("p"):
        if ("Page 0x" in i.text):
            index = i.text.index("0x")
            page = int(i.text[index:index+4],16)
            index = i.text.index("Offset")
            offset = int(i.text[index+7:index+11], 16)
            return page, offset

def getAddressName(div):
    return div[0].text

def getDescription(div):
    desc_p = div[2].text
    index = desc_p.index("Description")
    description = desc_p[index+13:]
    return description

def getWordLength(div):
    for table in div.iter("table"):
        length = table[1][0].text
        if ':' in length:
            index = length.index(':')
            length = length[:index]
        return (int(length) + 1) / 8

def getBits(div):
    bits = {}
    prev = None
    for table in div.iter("table"):
        for row in table[1:]:
            if (row[0].text):
                prev = row[0].text
                bits[row[0].text] = {}
                bits[row[0].text]['name'] = row[1].text
                bits[row[0].text]['R/W'] = row[2].text
                if len(row) == 5:
                    if (row[3].text):
                        bits[row[0].text]["description"] = (row[3].text + "\n")
                    else:
                        bits[row[0].text]["description"] = "No Description."
                    if (isHex(row[4].text)):
                        bits[row[0].text]["default"] = int(row[4].text,16)
                    else:
                        bits[row[0].text]["default"] = 0
                else:
                    bits[row[0].text]["description"] = (row[3].text[:-1] + "\n")
                    bits[row[0].text]["default"] = int(row[3].text[-1],16)
            if (row[0].text == None and row[3].text):
                bits[prev]['description'] += (row[3].text + " ")
    return bits

def main():
    book = {}
    tree = et.parse('bcl_2030014872_reduced_further.htm')
    html_tag = tree.getroot()
    for div in html_tag.iter("div"):
        page, offset = getPageOffsetNumbers(div)
        if page not in book:
            book[page] = {"addresses":{}}
        book[page]["addresses"][getAddressName(div)] = {"offset":offset,
                                                        "description":getDescription(div),
                                                        "bits":getBits(div),
                                                        "word-length":getWordLength(div)}
    with open('result.json', 'w') as fp:
        json.dump(book, fp, sort_keys=True, indent = 4)
    with open('result.json') as f:
        book = json.load(f)
    for page in book:
        for address in book[page]['addresses']:
            print "page=0x%X"%int(page),"offset=0x%X"%int(book[page]['addresses'][address]['offset']), "name=%s"%address, "word-length=%d"%int(book[page]['addresses'][address]['word-length'])


if (__name__ == "__main__"):
    main()
