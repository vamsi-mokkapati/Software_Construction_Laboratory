#!/usr/bin/python

import sys
from optparse import OptionParser

class commFunc:
    def __init__(self, f1, f2, op1, op2, op3, unsorted):
        if f1 == "-":
            self.file1lines = sys.stdin.readlines()
        else:
            file1 = open(f1, 'r')
            self.file1lines = file1.readlines()
            file1.close()

        if f2 == "-":
            self.file2lines = sys.stdin.readlines()
        else:
            file2 = open(f2, 'r')
            self.file2lines = file2.readlines()
            file2.close()

        self.opt1 = op1
        self.opt2 = op2
        self.opt3 = op3
        self.unsorted = unsorted

    def printFunc(self, op1, op2, op3, col1, col2, col3):
        length = len(col2)
        x = 0

        # The following loop replaces the column arrays with nothing
        # if the respective option is chosen to hide the column
        while(x < length):
            if(op1 == True):
                col1[x] = ''
            if(op2 == True):
                col2[x] = ''
            if(op3 == True):
                col3[x] = ''
            line = ''.join([col1[x], col2[x], col3[x]])

            # Deleting a column might leave excess tabs and spaces in a line,
            # and lines containing no words should not be printed.
            if(line != '' and line != '\t' and line != '\t\t'):
                print(line)
            x += 1

    def assignToCols(self, all, c1, c2, c3):
        x = 0
        while x < len(all):
            if(all[x] in self.file1lines and
               all[x] in self.file2lines):
                c1.append("\t")
                c2.append("\t")
                c3.append(all[x].rstrip('\n'))
            else:
                if(all[x] in self.file1lines):
                    c1.append(all[x].rstrip('\n'))
                    c2.append("")
                    c3.append("")
                if(all[x] in self.file2lines):
                    c1.append("\t")
                    c2.append(all[x].rstrip('\n'))
                    c3.append("")
            x += 1
        self.printFunc(self.opt1, self.opt2, self.opt3, c1, c2, c3)
    
    def compare(self):
        # Sorted inputs
        if(self.unsorted == False):
            col1=[] # contains words only in the first file
            col2=[] # contains words only in the second file
            col3=[] # contains words found in both the first and second files
            allLines=[] # The allLines array is going to contain all the lines
                        # from the first and second file combined.

            for x in range(0, len(self.file1lines)):
                allLines.append(self.file1lines[x])
            for x in range(0, len(self.file2lines)):
                allLines.append(self.file2lines[x])
            allLines=list(set(allLines)) # Removes duplicates from allLines
            allLines.sort()              # Sorts the allLines array 

            self.assignToCols(allLines, col1, col2, col3) # This function found above assigns the values
                                                          # from allLines into col1, col2, and col3.
        # Unsorted Inputs
        else:
            uCol1=[]
            uCol2=[]
            uCol3=[]
            totalWithReps=[]
            uAllLines=[]

            for x in range(0, len(self.file1lines)):
                totalWithReps.append(self.file1lines[x])
            for x in range(0, len(self.file2lines)):
                totalWithReps.append(self.file2lines[x])
            for x in range(0, len(totalWithReps)):     # This loop removes the repetitions from the
                if(totalWithReps[x] not in uAllLines): # totalWithReps array systematically, maintaining
                    uAllLines.append(totalWithReps[x]) # the order of lines in the unsorted files.

            self.assignToCols(uAllLines, uCol1, uCol2, uCol3)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Implement the POSIX comm command and add an extra option for unsorted files, -u"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="op1", default=False,
                      help="suppress column 1")
    parser.add_option("-2", action="store_true", dest="op2", default=False,
                      help="suppress column 2")
    parser.add_option("-3", action="store_true", dest="op3", default=False,
                      help="suppress column 3")
    parser.add_option("-u", action="store_true", dest="unsorted", default=False,
                      help="Input files are not sorted")

    options, args = parser.parse_args(sys.argv[1:])
    
    # Exception Handling Code
    
    if(len(args) != 2):
        parser.error("Wrong Number of Operands")
    f1 = args[0]
    f2 = args[1]

    try:
        generator = commFunc(f1, f2, options.op1, options.op2, options.op3, options.unsorted)
        generator.compare()
    except IOError as err:
        errno, strerror = err.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()