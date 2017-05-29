import sys
import string
import numpy as np
import matplotlib.pyplot as plt
import Image
sys.modules['Image'] = Image
import socket

host = '127.0.0.1'
port = 23334
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host,port))
s.listen(1)
conn, addr = s.accept()
img = np.array(Image.new("RGB",(84,84),(0,0,0)))
data = ''

def reimg():
  for i in range(84):
    for j in range(84):
      s1 = data[3*(i*84+j)]
      h1 = ord(s1)
      s2 = data[3*(i*84+j)+1]
      h2 = ord(s2)
      s3 = data[3*(i*84+j)+2]
      h3 = ord(s3)
      img[i,j,0] = h1
      img[i,j,1] = h2
      img[i,j,2] = h3
  plt.imshow(img)
  plt.show()

while 1:
  data = conn.recv(211680)
  if not data:
    continue
  reimg()
