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
    cv2.rectangle(frame, (114, 116 + 15), (190, 192 + 15), (255, 255, 255), 2)
    cv2.putText(frame, '2', (137, 174 + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255),2)
    cv2.rectangle(frame, (262 + 30, 62 + 5), (338 + 30, 138 + 5), (255, 255, 255),2)
    cv2.putText(frame, '3', (285 + 30, 120 + 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (362, 277), (438, 353), (255, 255, 255), 2)
    cv2.putText(frame, '4', (385, 335), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (472 + 30, 292), (548 + 30, 368), (255, 255, 255), 2)
    cv2.putText(frame, '5', (495 + 30, 350), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    frame, color_objects = detect_color(frame)

    for color, (cx, cy, cw, ch) in color_objects:
        if 62 < cx < 138 and 292 < cy < 368:
            cv2.putText(frame, f'{color} on White 1', (62, 280), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 114 < cx < 190 and (116 + 15) < cy < (192 + 15):
            cv2.putText(frame, f'{color} on White 2', (162, 106 + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255),2)
        elif (262 + 30) < cx < (338 + 30) and (62 + 5) < cy < (138 + 5):
            cv2.putText(frame, f'{color} on White 3', (262 + 30, 50 + 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(255, 255, 255), 2)
        elif 362 < cx < 438 and 277 < cy < 353:
            cv2.putText(frame, f'{color} on White 4', (362, 265), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif (472 + 30) < cx < (548 + 30) and 292 < cy < 368:
            cv2.putText(frame, f'{color} on White 5', (472 + 30, 280), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255),2)

    cv2.imshow('Color Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
