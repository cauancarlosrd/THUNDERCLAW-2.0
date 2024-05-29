import cv2
import numpy as np
import serial

ser = serial.Serial('COM7', baudrate=9600, timeout=1, write_timeout=1)

lower_blue = np.array([90, 50, 70])
upper_blue = np.array([130, 255, 255])

lower_red_1 = np.array([0, 50, 70])
upper_red_1 = np.array([10, 255, 255])
lower_red_2 = np.array([170, 50, 70])
upper_red_2 = np.array([180, 255, 255])

lower_green = np.array([40, 50, 50])
upper_green = np.array([80, 255, 255])

def detect_color(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
    mask_red_1 = cv2.inRange(hsv, lower_red_1, upper_red_1)
    mask_red_2 = cv2.inRange(hsv, lower_red_2, upper_red_2)
    mask_red = cv2.add(mask_red_1, mask_red_2)
    mask_green = cv2.inRange(hsv, lower_green, upper_green)

    color_objects = []

    for mask, color in zip([mask_blue, mask_red, mask_green], ['Blue', 'Red', 'Green']):
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > 500:
                x, y, w, h = cv2.boundingRect(cnt)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                cv2.putText(frame, f'{color} ({x}, {y})', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                color_objects.append((color, (x, y, w, h)))

    return frame, color_objects

def update_and_display_color_bases(frame, color_objects):
    color_bases = {
        'Blue': None,
        'Red': None,
        'Green': None
    }

    for color, (cx, cy, cw, ch) in color_objects:
        if 24 < cx < 100 and 262 < cy < 338:
            color_bases[color] = 1
        elif 148 < cx < 224 and 131 < cy < 207:
            color_bases[color] = 2
        elif 283 < cx < 359 and 100 < cy < 173:
            color_bases[color] = 3
        elif 415 < cx < 493 and 145 < cy < 220:
            color_bases[color] = 4
        elif 543 < cx < 623 and 252 < cy < 328:
            color_bases[color] = 5

    color_info = {}

    for color, base in color_bases.items():
        if base is not None:
            color_info[color] = base

    return frame, color_info

def to_byte(value):
    return str(value).encode()

cap = cv2.VideoCapture(1)

# Variáveis para armazenar a posição da cor azul em cada base branca
azul = 0

# Variáveis para armazenar a posição da cor vermelha em cada base branca
vermelho = 0

# Variáveis para armazenar a posição da cor verde em cada base branca
verde = 0

while True:
    ret, frame = cap.read()

    if not ret:
        break

    frame = cv2.flip(frame, 1)

    cv2.rectangle(frame, (24, 262), (100, 338), (255, 255, 255), 2)
    cv2.putText(frame, '1', (62, 350), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (148, 131), (224, 207), (255, 255, 255), 2)
    cv2.putText(frame, '2', (186, 225), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (283, 100), (359, 173), (255, 255, 255), 2)
    cv2.putText(frame, '3', (306, 189), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (415, 145), (493, 220), (255, 255, 255), 2)
    cv2.putText(frame, '4', (452, 189), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (543, 252), (623, 328), (255, 255, 255), 2)
    cv2.putText(frame, '5', (580, 350), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    frame, color_objects = detect_color(frame)

    frame, color_info = update_and_display_color_bases(frame, color_objects)

    # Resetando variáveis caso não haja objetos detectados
    azul = 0
    vermelho = 0
    verde = 0

    # Atualizando variáveis com a posição das cores nas bases brancas
    for color, base in color_info.items():
        if color == 'Blue':
            azul = base
        elif color == 'Red':
            vermelho = base
        elif color == 'Green':
            verde = base

    # Exibindo valores armazenados nas variáveis
    print(f'Azul: {azul}, Vermelho: {vermelho}, Verde: {verde}')

    data = ser.readline()
    print('Resposta %s' % data)
    if data == b'\x01':
        ser.write(to_byte(azul))
    elif data == b'\x02':
        ser.write(to_byte(vermelho))
    elif data == b'\x03':
        ser.write(to_byte(verde))

    cv2.imshow('Color Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
