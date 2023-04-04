#!/bin/python3

from pathlib import Path
import os
import subprocess
import threading as th
 
directory = 'benchmark_set'
 
# iterate over files in
# that directory
files = sorted(Path(directory).glob('*.hgr'))

def doProcessing(bin):
    out = []
    processes = []
    for file in files:
        process = subprocess.check_output([f'./bin/{bin}', file])
        processes.append(process)
    for i in range(len(files)):
        print(f"{i}th iteration")
        processes[i].wait()
        with open(files[i] + '.part.2') as f:
            out.append(f.readlines())
        print(out[i])
    return out

def printFile(data, file):
    for result in data:
        file.write(result.decode('utf-8'))

def main():
    org = open("Fiduccia.out", 'w')
    mod = open("FiducciaMod.out", 'w')
    data_org = doProcessing("Fiduccia")
    data_mod = doProcessing("FiducciaMod")
    printFile(data_org, org)
    printFile(data_mod, mod)

if __name__ == "__main__":
    main()