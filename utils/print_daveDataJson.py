#!/usr/bin/env  python3

HOME='/home/tovli/MRGPG3'
import sys
sys.path.insert(1,HOME+"/plib")

import daveDataJson

# print("carlDataJson contents:")
# lcarlData = carlDataJson.getCarlData()
# for i in lcarlData:
#     print("  ",i," : ",lcarlData[i])

daveDataJson.printData()
