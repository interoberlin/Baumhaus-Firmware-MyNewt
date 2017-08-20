#!/usr/bin/python

import re
from subprocess import Popen, PIPE
from shlex import split

AR_FOLDER = "../../bin/targets/floorsensor/app"

def find(filename):
    cmd = "find " + AR_FOLDER + " -name " + filename
    path = Popen(split(cmd), stdout=PIPE).communicate()[0]
    return path.strip()

if __name__ == "__main__":
    f = open("build.log", "r")
    
    for line in f.read().split("\n"):
        # Discard leading and trailing whitespace
        line = line.strip()
        
        # Skip empty lines
        if len(line) == 0:
            continue
        
        # Do not include app-sepcific archives
        if line.find("floorsensor") > -1:
            continue
    
        # Use regular expression to extract archive filename
        match = re.match(r"([0-9]*)[ ]+([0-9]*)[ ]+([A-Za-z0-9\_\-\.]*)", line)
        if match is None:
            continue
    
        archive = match.group(3)
        print "ARS += " + find(archive)
    
    f.close()
