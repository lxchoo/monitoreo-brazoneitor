from tkinter import *
import serial
import socket
import threading
import time
socket_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

detener = False


def detener_envio():
    global detener
    detener = True
    socket_udp.sendto(b'Fin', ("localhost", 3000))


x = 1


def leer_datos_serial():
    global detener
    arduino = serial.Serial("COM5", 9600)
    while True:
        dato = arduino.readline().strip().split()
        for num in dato:
            socket_udp.sendto(num, ("localhost", 3000))
        print(str(num.decode()) + " cm")
        
        if detener:
            time.sleep(0.1)
            print("Se ha finalizado la captura de datos")

            break
    arduino.close()


def iniciar_sensor():
    global detener
    detener = False
    threading.Thread(target=leer_datos_serial, daemon=True).start()


ventana = Tk()
ventana.geometry("300x300")
etiqueta = Label(ventana, text="Sensor 1").place(x=100, y=110)
boton_inicio = Button(ventana, text="Iniciar toma de datos",
                      command=iniciar_sensor).place(x=70, y=150)
boton_stop = Button(ventana, text="Detener envio",
                    command=detener_envio).place(x=70, y=180)

ventana.mainloop()
