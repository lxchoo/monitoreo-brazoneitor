import socket
import time
from tkinter import *
import random

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)\

def sensor():
    for i in range(10):
        dato = (random.randrange(0, 100))
        dato = str(dato)
        dato1 = dato.encode()
        s.sendto(dato1, ('localhost', 2001))
        print(dato1)
        time.sleep(0.5)
        v.after(250, sensor())
    s.close()


v = Tk()
v.geometry('100x100')
L = Label(v, text='sensor 1')
L.place(x=30, y=10)

B = Button(v, text='ACTIVAR', command=sensor)
B.place(x=30, y=50)

v.mainloop()