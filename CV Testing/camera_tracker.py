from picamera2 import Picamera2, Preview
import numpy as np
import cv2
import imutils
import time

def calculate_centroid_from_moments(moments: dict) -> tuple:
    if moments["m10"] and moments["m00"] and moments["m01"] and moments["m00"]:
        return (int(moments["m10"] / moments["m00"]), int(moments["m01"] / moments["m00"]))
    raise ValueError("Invalid moments dictionary passed. Use cv2.moments() on a contour.", moments)

# Define some settings
RESIZE_PROCESSING_WIDTH = 800
RESIZE_DISPLAY_WIDTH = 1100
ACCEPTABLE_X_ERROR = 75
ACCEPTABLE_Y_ERROR = 60

# Define object to track with HSV colors
color_lower = (0, 50, 50)
color_upper = (10, 255, 255)


# Initialize Picamera2
picam = Picamera2()
config = picam.create_preview_configuration()
picam.configure(config)
picam.set_controls({
    "AwbMode":0
})
picam.start()

time.sleep(2)  # Allow camera warm-up

# Loop indefinitely
while True:
    # Capture frame-by-frame
    frame2 = picam.capture_array()
    frame = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)
    frame = imutils.resize(frame, width=RESIZE_PROCESSING_WIDTH)
    blurred = cv2.GaussianBlur(frame, (31, 31), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # Create a mask for the color and perform dilations and erosions to remove any small blobs left in the mask
    mask = cv2.inRange(hsv, color_lower, color_upper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # Find contours in the mask
    contours = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = imutils.grab_contours(contours)
    

    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = calculate_centroid_from_moments(M)

        # Only proceed if the radius meets a minimum size

            # Draw the circle and centroid on the frame
        cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
        cv2.circle(frame, center, 5, (0, 0, 255), -1)

    # Display the resulting frame
    cv2.imshow("Frame", imutils.resize(frame, width=RESIZE_DISPLAY_WIDTH))
    cv2.imshow("Mask", imutils.resize(mask, width=RESIZE_DISPLAY_WIDTH))

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
picam.stop()
cv2.destroyAllWindows()
