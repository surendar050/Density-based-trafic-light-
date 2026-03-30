import cv2
from ultralytics import YOLO
import serial
import time

# --- SERIAL SETUP ---
# Replace 'COM3' with your Arduino port (e.g., '/dev/ttyUSB0' on Linux)
try:
    arduino = serial.Serial(port='COM3', baudrate=9600, timeout=0.1)
    time.sleep(2) # Wait for Arduino to reset
    print("Arduino connected!")
except:
    print("Could not connect to Arduino. Check the port.")
    arduino = None

model = YOLO('yolov8n.pt')
# Replace 'yourip/video' with your actual video stream URL (e.g., from an IP web camera)
url = 'yourip/video' 
cap = cv2.VideoCapture(url)
vehicle_classes = [2, 3, 5, 7]

while True:
    success, frame = cap.read()
    if not success:
        break

    results = model(frame, classes=vehicle_classes, conf=0.4, verbose=False)
    detections = results[0].boxes
    
    # Example logic: Assigning counts to a, b, c, d
    # In a real 4-way setup, you would have 4 'cap' streams or 4 Regions of Interest
    a = len(detections) # Current camera
    b, c, d = 0, 0, 0   # Placeholders for other directions

    # Create a string packet: "a,b,c,d\n"
    data_packet = f"{a},{b},{c},{d}\n"

    # Send to Arduino
    if arduino:
        arduino.write(bytes(data_packet, 'utf-8'))

    # Visuals
    annotated_frame = results[0].plot()
    cv2.putText(annotated_frame, f"A:{a} B:{b} C:{c} D:{d}", (20, 50),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.imshow("Traffic Control System", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

if arduino:
    arduino.close()
cap.release()
cv2.destroyAllWindows()