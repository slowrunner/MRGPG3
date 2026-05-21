#!/usr/bin/env  python3

import sys
import json
import os

def getgpg3_config(dataname=None):

    home=os.getenv('HOME')
    try:
        with open(home + '/.gpg3_config.json', 'r') as infile:
            lgpg3_config = json.load(infile)
            if (dataname == None):
                return lgpg3_config
            else:
                return lgpg3_config[dataname]
    except Exception as e:
        print("   getgpg3_config() exception:", e)
        return None

def printgpg3_config():
    home = os.getenv('HOME')
    print(home + "/.gpg3_config.json contents:")
    lgpg3_config = getgpg3_config()
    for i in lgpg3_config:
        print("  ",i," : ",lgpg3_config[i])

printgpg3_config()
