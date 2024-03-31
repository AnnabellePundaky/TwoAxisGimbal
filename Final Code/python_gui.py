# https://forum.arduino.cc/t/serial-input-basics-updated/382007
# https://www.youtube.com/watch?v=ZupqryZjIP8  <- stuff about KeyBoardInterrupt
# Two-part series on Python ESP32 serial communication:
# https://www.youtube.com/watch?v=ZupqryZjIP8
# https://www.youtube.com/watch?v=Jr79o_28tSY

import serial
import time

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np

# Serial port parameters
PORT_NAME = '/dev/cu.usbserial-0001'
BAUD_RATE = 9600
TIMEOUT = 1
# MAX_BUFFER_LEN = 255

# Initialize serial port
ser = serial.Serial(PORT_NAME, BAUD_RATE, timeout=TIMEOUT)

if ser.is_open==True:
    print("\nSerial port is open. Configuration:\n")
    print(ser, "\n") #print serial parameters

# Code states
START_STATE = 0
MAIN_STATE = 1

# ser.setDTR(False)
# ser.setRTS(False)
# ser.timeout = 10 #specify timeout when using readline()
# ser.reset_input_buffer()
# time.sleep(0.5)
# ser.write('#'.encode())

state = 0

def read_ser(num_char=1):
    # Read num_char bytes from serial port.
    string = ser.read(num_char)
    return string.decode()

def select_mode():
    print("\nSelect mode:")
    print(" 1 = Gimbal pointing and distance tracking test")
    print(" 2 = Photodiode array demo")
    user_input = input("Enter selection (1/2): ")
    if user_input == "2":
        mode = 2
    else:
        mode = 1
    return mode


while(1):
    try:
        # string = ser.readline().decode()
        # print(string, end="")	# print without newline
        # User mode selection
        if state == START_STATE:
            string = ser.readline().decode().strip()
            print(string)
            time.sleep(0.1)
            user_mode = select_mode()
            if user_mode == 1:
                # Create figure for plotting
                fig = plt.figure()
                ax = fig.add_subplot(1, 1, 1)
                secs = []
                # msecs = []
                ys = []
                i = 0   # x-axis
                ser.write('#'.encode())		# Write mode 1
            if user_mode == 2:
                fig = plt.figure()
                ax = fig.add_subplot(1, 1, 1)
                ser.write('$'.encode())		# Write mode 2
                matrix = np.array([[0, 0],
                                   [0, 0]])
                im = ax.imshow(matrix, vmin=0, vmax=3000)
                fig.colorbar(im)
            state = MAIN_STATE
        elif state == MAIN_STATE:
            string = ser.readline().decode().strip()
            print(string)
            if user_mode == 1:
                line_as_list = string.split(',')
                secs.append(int(line_as_list[0]) + (float(line_as_list[1]) / 1000.0))
                ys.append(float(line_as_list[2]))

                # Limit x and y lists to 100 items
                secs = secs[-50:]
                ys = ys[-50:]

                ax.clear()
                plt.plot(secs, ys, label="Distance [mm]")

                # Plot settings
                plt.title('Distance vs. Time')
                plt.xlabel('Time elapsed [s]')
                plt.ylabel('Distance [mm]')
                plt.ylim((0, 2000))

                # For animation
                plt.pause(0.05)
            elif user_mode == 2:
                line_as_list = string.split(',')
                im = ax.imshow(matrix, vmin=0, vmax=3000)

                ys1 = int(line_as_list[0])
                ys2 = int(line_as_list[1])
                ys3 = int(line_as_list[2])
                ys4 = int(line_as_list[3])

                matrix = np.array([ [ys1, ys2],
                                    [ys3, ys4] ])
                
                ax.clear()
                im = ax.imshow(matrix, vmin=0, vmax=3000)

                # Plot settings
                plt.title('Photodiode Array')

                # For animation
                plt.pause(0.05)
                
    except KeyboardInterrupt:
        print()
        print("Keyboard exit command detected. Stopping execution.")
        exit()
    except UnicodeDecodeError:
        print()
        print("Detected EN button press though serial decode. Stopping execution.")
        exit()

# plt.show()

# Set up plot to call animate() function periodically
# print("REACHED BOTTOM")
# ani = animation.FuncAnimation(fig, animate, fargs=(secs, ys), interval=10)
# plt.show()