import subprocess

files = ['c05100', 'c05200', 'c10100', 'c10200', 'c10400', 'c20100', 'c20200', 'c20400', 'c40400',
         'd05100', 'd05200', 'd10100', 'd10200', 'd10400', 'd20100', 'd20200', 'd20400', 'd40400',
         'e05100', 'e05200', 'e10100', 'e10200', 'e10400', 'e20100', 'e20200', 'e20400', 'e40400']
results = []

cmd = "cat data/" + files[0] + " | ./gap timelim 300 givesol 0"
output = subprocess.call(cmd, shell=True)
print("OUT", output)
# for fn in files:
    # cmd = "cat data/" + fn + " | ./gap timelim 300 givesol 0"
    # output = subprocess.call(cmd, shell=True)
    # print("OUT", output)
