# http://www.mikeburdis.com/wp/notes/plotting-serial-port-data-using-python-and-matplotlib/

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import serial

PORT_NAME = '/dev/cu.usbserial-0001'
BAUD_RATE = 9600
TIMEOUT = 1

MAX_BUFFER_LEN = 255

#initialize serial port
ser = serial.Serial(PORT_NAME, BAUD_RATE, timeout=TIMEOUT)

#initialize serial port
# ser = serial.Serial()
# ser.port = '/dev/cu.usbserial-0001' #Arduino serial port
# ser.baudrate = 115200
# # ser.baudrate = 9600
# ser.timeout = 10 #specify timeout when using readline()
# ser.open()
if ser.is_open==True:
	print("\Serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters
# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
secs = []
# msecs = []
ys = []

ser.write('#'.encode())
i = 0   # x-axis
# This function is called periodically from FuncAnimation
def animate(i, secs, ys):

    #Aquire and parse data from serial port
    line=ser.readline()      #ascii
    # print(line)
    line_as_list = line.split(b',')
    # i = int(line_as_list[0])
	
	# Add x and y to lists
    i = i + 1
    # xs.append(i)
    
    try:
        # Combine secs and msecs to produce seconds elapsed value with decimals
        secs.append(int(line_as_list[0]) + (float(line_as_list[1]) / 1000.0))
        ys.append(float(line_as_list[2]))

        # Limit x and y lists to 100 items
        secs = secs[-100:]
        ys = ys[-100:]

        print(line.decode(), end="")
        # print(secs[-1])

        # Draw x and y lists
        ax.clear()
        ax.plot(secs, ys, label="Distance [mm]")
        
        # Format plot
        plt.xticks(rotation=45, ha='right')
        # plt.subplots_adjust(bottom=0.30)
        plt.title('Distance vs. Time')
        plt.xlabel('Time elapsed [s]')
        plt.ylabel('Distance [mm]')
        plt.ylim((0, 4000))
        plt.grid(alpha=0.25)
        # plt.legend()
        # plt.axis([1, None, 0, 1.1]) #Use for arbitrary number of trials
        #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo
    except ValueError:
        pass

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(secs, ys), interval=10)
plt.show()