#!/bin/python3

from pathlib import Path
import os
import subprocess
import threading as th

def start(bin : str, dump : str):
    out = []
    processes = []
    files = Path('benchmark_set').glob('*.hgr')
    length = len(files)
    for file in files:
        process = subprocess.Popen([f'./bin/{bin}', file])
        processes.append(process)
    for i in range(length):
        print(f"Finished {int(100 * i / length)}% : dump", flush=True)
        processes[i].wait()
        with open(str(files[i]) + '.part.2') as f:
            out.append(f.read())
    return out

def printFile(data, file):
    for result in data:
        file.write(result)

def main():
    org = open("Dump.out", 'w')
    mod = open("DumpMod.out", 'w')
    data_org = start("Fiduccia", 'original')
    data_mod = start("FiducciaMod", 'modified')
    printFile(data_org, org)
    printFile(data_mod, mod)

if __name__ == "__main__":
    main()
