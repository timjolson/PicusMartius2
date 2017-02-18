import numpy
from numpy import floor, mod
from array import array

EncoderDat = {'res': 4096, 'offset': [ 0, 0 ], 'ratio': [ 5, 4 ]}
EncoderToAngle = [[0 for x in range(EncoderDat['res'])] for y in range(EncoderDat['res'])]

try:
    EncoderToAngle = numpy.load('/data/EncoderToAngle.npy')
    print('loaded... EncoderToAngle')
except:
    for theta in range(3600)
        x(e[0]+1,e[1]) = theta*3600/EncoderDat['res'];
        x(e[0],e[1]+1) = theta*3600/EncoderDat['res'];
        x(e[0]+1,e[1]+1) = theta*3600/EncoderDat['res'];
        x(e[0],e[1]) = theta*3600/EncoderDat['res'];
        e[0] = floor(mod(theta*EncoderDat['ratio'[0]]*3600/EncoderDat['res'],EncoderDat['res'])+EncoderDat['offset'[0]]);
        e[1] = floor(mod(theta*EncoderDat['ratio'[1]]*3600/EncoderDat['res'],EncoderDat['res'])+EncoderDat['offset'[1]]);
    
    numpy.save('/data/EncoderToAngle.npy')
    print('created... EncoderToAngle')
    
