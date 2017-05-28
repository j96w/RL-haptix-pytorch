import numpy as np
import matplotlib.pyplot as plt
import Image

class global_var:
    name = np.array(Image.new("RGB",(84,84),(0,0,0)))

def set_name(name):
    global_var.name = name
def get_name():
    return global_var.name
