from picamera2 import Picamera2
import cv2
import imutils
import numpy as np
import time
from motor_movement import Serial_Motor_Control

def calculate_centroid_from_moments(moments: dict) -> tuple:
    if moments["m10"] and moments["m00"] and moments["m01"] and moments["m00"]:
        return (int(moments["m10"] / moments["m00"]), int(moments["m01"] / moments["m00"]))
    raise ValueError("Invalisd moments dictionary passed. Use cv2.moments() on a contour.", moments)

# Initialize motor control
Motors = Serial_Motor_Control()

# Define some settings
RESIZE_PROCESSING_WIDTH = 800
RESIZE_DISPLAY_WIDTH = 1100
ACCEPTABLE_X_ERROR = 20
ACCEPTABLE_Y_ERROR = 20

# Define object to track with HSV colors
color_lower = (0, 155, 50) #f changed from 50 to 75or this , Hue ranges from 0-179, and the other two range from 0 to 255. SO REMEBER TO SCALE UP YOUR VALUE.
color_upper = (12, 239,255)

# Initialize Picamera2
picam = Picamera2()
config = picam.create_preview_configuration()
picam.configure(config)
Y_PIXEL_OFFSET = -30;
X_PIXEL_OFFSET = -63;
picam.start()
time.sleep(2)  # Allow camera warm-up

while True:
    frame2 = picam.capture_array()
    frame = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)
    frame = imutils.resize(frame, width=RESIZE_PROCESSING_WIDTH)
    blurred = cv2.GaussianBlur(frame, (31, 31), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    
    mask = cv2.inRange(hsv, color_lower, color_upper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    
    contours = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = imutils.grab_contours(contours)

    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = calculate_centroid_from_moments(M)

        cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
        cv2.circle(frame, center, 5, (0, 0, 255), -1)
        
        # Calculate frame center
        frame_center = (RESIZE_PROCESSING_WIDTH // 2, frame.shape[0] // 2)
        adjusted_frame_center = (frame_center[0] - X_PIXEL_OFFSET, frame_center[1] - Y_PIXEL_OFFSET)
        #delta_x = np.subtract(center, frame_center)
        delta_x, delta_y = np.subtract(center, adjusted_frame_center)
        #delta_x, delta_y = np.subtract(center, frame_center)
        # Motor control based on centroid position
        if delta_x > ACCEPTABLE_X_ERROR: # Move left
            Motors.move_x_ccw()
        elif delta_x< -ACCEPTABLE_X_ERROR: # Move right
            Motors.move_x_cw()
        if delta_y> ACCEPTABLE_Y_ERROR: # Move down
            Motors.move_y_ccw()
        elif delta_y < -ACCEPTABLE_Y_ERROR: # Move up
            Motors.move_y_cw()

    cv2.imshow("Frame", imutils.resize(frame, width=RESIZE_DISPLAY_WIDTH))
    cv2.imshow("Mask", imutils.resize(mask, width=RESIZE_DISPLAY_WIDTH))

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

picam.stop()
cv2.destroyAllWindows()
