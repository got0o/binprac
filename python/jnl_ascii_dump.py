#!/usr/bin/env python3

import sys

argv = sys.argv
argc = len(argv)

class jnl_header:
    def __init__(self, header):
        self.year = header[:4]
        self.month = header[4:6]
        self.day = header[6:8]
        self.hour = header[8:10]
        self.minute = header[10:12]
        self.second = header[12:14]
        self.msecond = header[14:17]
        self.kind = header[17:18]
        self.dataLen = header[18:26]
    def data_len(self):
        return int(self.dataLen)


def printHeader(jnlh):
    jnlh_str = "%s/%s/%s,%s:%s:%s.%s,%s,%s" % (jnlh.year,jnlh.month,jnlh.day,jnlh.hour,jnlh.minute,jnlh.second,jnlh.msecond,jnlh.kind,jnlh.dataLen)
    print("HEADER," + jnlh_str)

def printData(data):
    ascii_data = ""
    for byte in data:
        if not byte:
            break
        if (0x20 <= byte and byte <= 0x7e):
            ascii_data += chr(byte)
        else:
            ascii_data += '.'
    print("DATA:" + ascii_data)

def dumpStream():
    stdin = sys.stdin.buffer
    while(1):
        header_size = 26
        header = stdin.read(header_size).decode()
        if not header:
            break
        jnlh = jnl_header(header)
        printHeader(jnlh)
        data = stdin.read(jnlh.data_len())
        printData(data)

def asciiDumpArgs(argc,argv):
    for i in range(1,argc):
        with open(argv[i], "rb") as f:
            while(1):
                header_size = 26
                header = f.read(header_size).decode()
                if not header:
                    break
                jnlh = jnl_header(header)
                printHeader(jnlh)
                data_len = jnlh.dataLen
                data = f.read(int(data_len))
                printData(data)


def main(argc, argv):
    isUseStdin = 0
    if argc== 1:
        isUseStdin = 1
    elif argc == 2 and argv[1] == "-h":
        print("Usage : %s ファイル名\n" % argv[0])
        sys.exit()

    if isUseStdin:
        dumpStream()
    else:
        asciiDumpArgs(argc, argv)


#main(argc, argv)

if __name__ == "__main__":
    main(argc, argv)