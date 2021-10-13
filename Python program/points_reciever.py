#encoding:utf-8

import serial
import time
import win32api,win32con

com = input('Quel COM ?\n')
ser = serial.Serial(com)
calibration = 0

pointIR = [[2,2],[2,2],[2,2],[2,2]]

def click(x,y):
    win32api.SetCursorPos((x,y))
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN,x,y,0,0)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP,x,y,0,0)

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
    j,k,r=0,0,0
    while(j<2):
        if (data[j+k]==",") :
            pointIR[0][j]=float(data[j+k-r:j+k])
            j+=1
            r=1
        else :
            r+=1
        k+=1


    if pointIR[0][0]+pointIR[0][1] < 4 :
        click(int(pointIR[0][0]*1920),int(pointIR[0][1]*1080))

    print(pointIR)




''' i,j,k,r=0,0,0,0
    while(i<4):
        while(j<2):
            if (data[i+j+k]==",") :
                pointIR[i][j]=float(data[i+j+k-r:i+j+k])
                j+=1
                r=1
            else :
                r+=1
            k+=1
        r=0
        j=0
        i+=1 '''
