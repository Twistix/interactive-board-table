This program is the one flashed on the Arduino connected to the infrared camera.
It first asks for a calibration by indicating 4 points corresponding to the angles of the table.
It then sends by USB at regular intervals the detected position of an IR point (IR led for example) in the plane selected during calibration. The position is returned in x,y format with xE[0,1] and yE[0,1].

![schem_ir_points](https://raw.githubusercontent.com/Twistix/interactive-board-table/main/Arduino%20program/schem_ir_points.png)
