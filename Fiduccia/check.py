#!/bin/python3

from pathlib import Path
import subprocess

def percent_complete(step, total_steps, bar_width=60, title="", print_perc=True):
    import sys

    # UTF-8 left blocks: 1, 1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8
    utf_8s = ["█", "▏", "▎", "▍", "▌", "▋", "▊", "█"]
    perc = 100 * float(step) / float(total_steps)
    max_ticks = bar_width * 8
    num_ticks = int(round(perc / 100 * max_ticks))
    full_ticks = num_ticks / 8      # Number of full blocks
    part_ticks = num_ticks % 8      # Size of partial block (array index)
    
    disp = bar = ""                 # Blank out variables
    bar += utf_8s[0] * int(full_ticks)  # Add full blocks into Progress Bar
    
    # If part_ticks is zero, then no partial block, else append part char
    if part_ticks > 0:
        bar += utf_8s[part_ticks]
    
    # Pad Progress Bar with fill character
    bar += "▒" * int((max_ticks/8 - float(num_ticks)/8.0))
    
    if len(title) > 0:
        disp = title + ": "         # Optional title to progress display
    
    # Print progress bar in green: https://stackoverflow.com/a/21786287/6929343
    disp += "\x1b[0;32m"            # Color Green
    disp += bar                     # Progress bar to progress display
    disp += "\x1b[0m"               # Color Reset
    if print_perc:
        # If requested, append percentage complete to progress display
        if perc > 100.0:
            perc = 100.0            # Fix "100.04 %" rounding error
        disp += " {:6.2f}".format(perc) + " %"
    
    # Output to terminal repetitively over the same line using '\r'.
    sys.stdout.write("\r" + disp)
    sys.stdout.flush()

def start(isMod : bool, dump : str):
    out = []
    processes = []
    files = sorted(Path('benchmark_set').glob('*.hgr'))
    length = len(files)
    cmd = ['./bin/Fiduccia', '-m'] if isMod else ['./bin/Fiduccia']
    for file in files:
        process = subprocess.Popen(cmd + [file])
        processes.append(process)
    print(f"Starting {dump}", flush=True)
    for i in range(length):
        percent_complete(i, length, title=dump)
        processes[i].wait()
        with open(str(files[i]) + '.part.2') as f:
            out.append(f.read())
    percent_complete(length, length, title=dump)
    return out

def printFile(data, file):
    for result in data:
        file.write(result)

def main():
    org = open("DumpOrd.out", 'w')
    final = open("DumpFinal.out", 'w')
    data_org = start(False, 'original')
    printFile(data_org, org)
    data_final = start(True, 'modified')
    printFile(data_final, final)

if __name__ == "__main__":
    main()
