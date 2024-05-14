import cv2
import numpy as np

lower_blue = np.array([90, 50, 50])
upper_blue = np.array([130, 255, 255])

lower_red = np.array([0, 50, 50])
upper_red = np.array([10, 255, 255])

lower_green = np.array([50, 100, 100])
upper_green = np.array([70, 255, 255])

def detect_color(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
    mask_red = cv2.inRange(hsv, lower_red, upper_red)
    mask_green = cv2.inRange(hsv, lower_green, upper_green)

    color_objects = []

    for contours, color in zip([mask_blue, mask_red, mask_green], ['Blue', 'Red', 'Green']):
        contours, _ = cv2.findContours(contours, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > 500:
                x, y, w, h = cv2.boundingRect(cnt)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                cv2.putText(frame, f'{color} ({x}, {y})', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                color_objects.append((color, (x, y, w, h)))

    return frame, color_objects

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    if not ret:
        break

    frame = cv2.flip(frame, 1)

    cv2.rectangle(frame, (62, 292), (138, 368), (255, 255, 255), 2)
    cv2.putText(frame, '1', (85, 350), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (114, 131), (190, 207), (255, 255, 255), 2)
    cv2.putText(frame, '2', (137, 189), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (283, 67), (359, 143), (255, 255, 255), 2)
    cv2.putText(frame, '3', (306, 125), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (452, 121), (528, 207), (255, 255, 255), 2)
    cv2.putText(frame, '4', (475, 189), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (505, 292), (580, 368), (255, 255, 255), 2)
    cv2.putText(frame, '5', (525, 350), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    frame, color_objects = detect_color(frame)

    for color, (cx, cy, cw, ch) in color_objects:
        if 62 < cx < 138 and 292 < cy < 368:
            cv2.putText(frame, f'{color} on White 1', (62, 280), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 114 < cx < 190 and 131 < cy < 207:
            cv2.putText(frame, f'{color} on White 2', (162, 121), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 283 < cx < 359 and 67 < cy < 143:
            cv2.putText(frame, f'{color} on White 3', (283, 55), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 452 < cx < 528 and 131 < cy < 207:
            cv2.putText(frame, f'{color} on White 4', (392, 121), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 502 < cx < 578 and 292 < cy < 368:
            cv2.putText(frame, f'{color} on White 5', (502, 280), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)

    cv2.imshow('Color Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
