import sys
import string
import numpy as np
import matplotlib.pyplot as plt
import Image
sys.modules['Image'] = Image
import mid
import os
import socket

host = '127.0.0.1'
port = int(os.environ['IGN_PARTITION_PORT'])
ss = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ss.connect((host,port))

def reimg(s):
  ss.send(s)

def getImg():
  return img

