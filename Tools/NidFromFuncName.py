import sys, os
import struct
import hashlib
import re

NID_SUFFIX = bytes.fromhex('518D64A635DED8C1E6B039B1C3E55230')


def sha1(data):
	return hashlib.sha1(data).digest()

def calculate_nid(symbol):
	buff = symbol.encode() + NID_SUFFIX
	return struct.unpack('<Q', sha1(buff)[:8])[0]

print(calculate_nid(sys.argv[1]))
