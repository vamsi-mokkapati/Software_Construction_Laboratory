#!/usr/bin/python

def main():
    arrOcts = []
    x = 0
    while (x < 256):
        frobval = x ^ 42
        octval = oct(frobval)
        arrOcts.append("\\" + octval)
        x += 1
    print ''.join(map(str, arrOcts))
if __name__ == "__main__":
    main()