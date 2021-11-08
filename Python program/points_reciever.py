#encoding:utf-8

import serial
import time
import win32api,win32con

com = "COM3"
ser = serial.Serial(com)
calibration = 0

pointIR = [[2,2],[2,2],[2,2],[2,2]]

def click(x,y):
    win32api.SetCursorPos((x,y))
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN,x,y,0,0)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP,x,y,0,0)

def move(x,y):
    win32api.SetCursorPos((x,y))

while calibration == 0 :
    data = str(ser.readline())
    data = data.replace("b'","")
    data = data.replace("\\r\\n'","")
    print(data)
    if data == "Calibration finie" :
        calibration = 1

time.sleep(1)

while 1 :
    data = str(ser.readline())
    data = data.replace("b'","")
    data = data.replace("\\r\\n'","")
    print(data)

    index = []  # creation d'un tableau index
    curr = 0    # position actuelle dans la data string
    nbSeparators = 2   # nombre de séparateurs ,
    for i in range(nbSeparators) :  # on parcours toute la data string et on savegarde les index ou on trouve un ","
        curr = data.index(",",curr+1)
        index.append(curr)
    pointIR[0][0] = float(data[0:index[0]])
    pointIR[0][1] = float(data[index[0]+1:index[1]])

    if pointIR[0][0] >= 0 and pointIR[0][0] <= 1 and pointIR[0][1] >= 0 and pointIR[0][1] <= 1 :
        move(int(pointIR[0][0]*1920),int(pointIR[0][1]*1080))

