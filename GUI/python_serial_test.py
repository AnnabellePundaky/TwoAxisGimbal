# https://forum.arduino.cc/t/serial-input-basics-updated/382007
# https://www.youtube.com/watch?v=ZupqryZjIP8  <- stuff about KeyBoardInterrupt
# Two-part series on Python ESP32 serial communication:
# https://www.youtube.com/watch?v=ZupqryZjIP8
# https://www.youtube.com/watch?v=Jr79o_28tSY

import serial
import time

PORT_NAME = '/dev/cu.usbserial-0001'
BAUD_RATE = 115200
TIMEOUT = 1

MAX_BUFFER_LEN = 255

#initialize serial port
ser = serial.Serial(PORT_NAME, BAUD_RATE, timeout=TIMEOUT)

# ser.setDTR(False)
# ser.setRTS(False)

# ser.timeout = 10 #specify timeout when using readline()

# ser.open()
# if ser.is_open==True:
# 	print("\nAll right, serial port now open. Configuration:\n")
# 	print(ser, "\n") #print serial parameters

ser.reset_input_buffer()

# time.sleep(0.5)
# ser.write('#'.encode())

STATE = 0
def read_ser(num_char=1):
	# Read num_char bytes from serial port.
	string = ser.read(num_char)
	return string.decode()

while(1):
	if STATE == 0:
		time.sleep(0.5)
		ser.write('#'.encode())
		STATE = 1
	elif STATE == 1:		
		string = ser.readline().decode()
		print(string, end="")	# print without newline
		if "@" in string:
			STATE = 0
			continue