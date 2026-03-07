import cv2
import numpy as np
from ultralytics import YOLO

# 1. Load Model (YOLOv8 Nano - Optimized for Lenovo IdeaPad Gaming 3)
model = YOLO("yolov8n.pt") 

# 2. ESP32 Stream URL (Ensure your Mobile Hotspot is active)
url = "http://xxxx.xxxx.xxxx.xxxx:xxxx/stream" #your ip address for web cam
cap = cv2.VideoCapture(url)

# Wildlife Class Mapping: 
# 14:Bird(Peacock), 15:Cat(Cheetah/Lion/Tiger), 16:Dog(Hyena), 
# 18:Sheep(Goat/Deer), 19:Cow
WILDLIFE_IDS = [14, 15, 16, 17, 18, 19, 20, 21, 22, 23]

# Output Size: 1088x612 (Exact 15% reduction from 720p HD for optimal UI fit)
TARGET_W, TARGET_H = 1088, 612 

print(f"Monitoring active. Display size: {TARGET_W}x{TARGET_H}")

while True:
    ret, frame = cap.read()
    if not ret:
        print("Waiting for ESP32 stream...")
        break

    annotated_frame = frame.copy()

    # --- PART A: ANIMAL DETECTION (UNTOUCHED LOGIC) ---
    # imgsz=416 provides better accuracy for your IdeaPad GPU
    results = model(frame, conf=0.35, verbose=False, imgsz=416)

    for r in results:
        for box in r.boxes:
            cls_id = int(box.cls[0])
            
            # BLACKLIST: Explicitly skip Person (ID 0)
            if cls_id == 0:
                continue 

            if cls_id in WILDLIFE_IDS:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                w_box, h_box = x2 - x1, y2 - y1
                aspect_ratio = w_box / h_box
                
                # GEOMETRIC FILTER: Ignores square shapes typical of human faces
                if aspect_ratio > 1.25 or aspect_ratio < 0.75:
                    cv2.rectangle(annotated_frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                    cv2.putText(annotated_frame, "WILDLIFE", (x1, y1 - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

    # --- PART B: REFINED FIRE DETECTION (WHITE LIGHT REJECTION) ---
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Fire Color Ranges (Orange and Red Glow)
    lower_orange = np.array([0, 110, 120]) 
    upper_orange = np.array([25, 255, 255])
    mask_orange = cv2.inRange(hsv, lower_orange, upper_orange)
    
    lower_red = np.array([160, 110, 100]) 
    upper_red = np.array([180, 255, 255])
    mask_red = cv2.inRange(hsv, lower_red, upper_red)
    
    # Bright Core detection (White/Yellow center)
    lower_core = np.array([0, 0, 210]) 
    upper_core = np.array([180, 80, 255])
    mask_core = cv2.inRange(hsv, lower_core, upper_core)
    
    # Combined Mask for potential fire areas
    color_glow = cv2.bitwise_or(mask_orange, mask_red)
    combined_fire = cv2.bitwise_or(color_glow, mask_core)
    combined_fire = cv2.medianBlur(combined_fire, 5) 
    
    contours, _ = cv2.findContours(combined_fire, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > 450: 
            x, y, w, h = cv2.boundingRect(cnt)
            
            # ROI Validation: Fire has high average saturation; white lights do not.
            # This is the primary fix for the white light false alerts.
            roi_hsv = hsv[y:y+h, x:x+w]
            avg_saturation = np.mean(roi_hsv[:,:,1])
            
            if h > w * 0.75 and avg_saturation > 75:
                cv2.rectangle(annotated_frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
                cv2.putText(annotated_frame, "!!! FIRE !!!", (x, y - 10),
                            cv2.FONT_HERSHEY_DUPLEX, 0.8, (0, 0, 255), 2)

    # --- PART C: RESIZE & DISPLAY ---
    # Scaled down for your Lenovo IdeaPad screen
    final_output = cv2.resize(annotated_frame, (TARGET_W, TARGET_H), interpolation=cv2.INTER_AREA)

    cv2.imshow("Wildlife & Fire Monitor", final_output)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()