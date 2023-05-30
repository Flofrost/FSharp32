from serial import Serial

samples = [x for x in range(-128, 128)]
samples = [128 - abs(s) if s < 0 else s for s in samples]

port = Serial("/dev/ttyUSB0", 115200)

port.write(samples)
