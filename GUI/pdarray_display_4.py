import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
import seaborn

#initialize serial port
ser = serial.Serial()
ser.port = '/dev/cu.usbserial-0001' #Arduino serial port
ser.baudrate = 115200
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = [] #store trials here (n)
ys1 = []
ys2 = []
ys3 = []
ys4 = []
# rs = [] #for theoretical probability

i = 0   # x-axis
# This function is called periodically from FuncAnimation
def animate(i, xs, ys1, ys2, ys3, ys4):

    #Aquire and parse data from serial port
    line=ser.readline()      #ascii
    # print(line)
    line_as_list = line.split(b',')
    # i = int(line_as_list[0])
    # relProb = line_as_list[1]
    # relProb_as_list = relProb.split(b'\n')
    # relProb_float = float(relProb_as_list[0])
	
	# Add x and y to lists
    i = i + 1
    xs.append(i)
    ys1.append(int(line_as_list[0]))
    ys2.append(int(line_as_list[1]))
    ys3.append(int(line_as_list[2]))
    ys4.append(int(line_as_list[3]))
    # rs.append(0.5)

    # Limit x and y lists to 20 items
    xs = xs[-100:]

    matrix = np.array([ [ys1[-1], ys2[-1]],
                        [ys3[-1], ys4[-1]] ])
    ys1 = ys1[-100:]
    ys2 = ys2[-100:]
    ys3 = ys3[-100:]
    ys4 = ys4[-100:]

    print(f"{xs[-1]}: {ys1[-1]}, {ys2[-1]}, {ys3[-1]}, {ys4[-1]}")

    # Draw x and y lists
    ax.clear()
    im = ax.imshow(matrix, vmin=0, vmax=3000)
    # plt.colorbar()
    
    # ax.plot(xs, ys1, label="Cell 1")
    # Format plot
    plt.xticks(rotation=45, ha='right')
    # plt.subplots_adjust(bottom=0.30)
    plt.title('This is how I roll...')
    plt.ylabel('Relative frequency')
    plt.legend()
    # plt.axis([1, None, 0, 1.1]) #Use for arbitrary number of trials
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys1, ys2, ys3, ys4), interval=100)
plt.show()