from sys import argv


f = open(argv[1], "rb")

f.seek(46)
data = list(f.read())
data.pop()
data = [data[int((i/256) * len(data))] - 128 for i in range(256)]
maximum = max(max(data), -min(data))
gain = 127 / maximum

print([int(data[i] * gain) for i in range(256)])

f.close()