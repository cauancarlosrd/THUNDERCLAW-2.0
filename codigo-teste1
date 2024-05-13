# Importa a biblioteca OpenCV para processamento das imagens
import cv2
# Importa a biblioteca NumPy para manipulação de arrays
import numpy as np

# Define os intervalos de cor para cada cor no espaço de cores HSV
lower_blue = np.array([90, 50, 50])
upper_blue = np.array([130, 255, 255])

lower_red = np.array([0, 50, 50])
upper_red = np.array([10, 255, 255])

lower_green = np.array([50, 100, 100])
upper_green = np.array([70, 255, 255])

# Define uma função para detectar objetos de cores específicas em um quadro de imagem
def detect_color(frame):
    # Converte o espaço de cores do quadro da imagem de BGR para HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Cria máscaras para cada cor com base nos intervalos definidos
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
    mask_red = cv2.inRange(hsv, lower_red, upper_red)
    mask_green = cv2.inRange(hsv, lower_green, upper_green)

    # Inicializa uma lista para armazenar os objetos detectados
    color_objects = []

    # Itera sobre cada máscara e sua respectiva cor
    for contours, color in zip([mask_blue, mask_red, mask_green], ['Blue', 'Red', 'Green']):
        # Encontra os contornos na máscara
        contours, _ = cv2.findContours(contours, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # Itera sobre cada contorno encontrado
        for cnt in contours:
            # Calcula a área do contorno
            area = cv2.contourArea(cnt)
            # Verifica se a área do contorno é maior que 500 pixels (ajuste necessário)
            if area > 500:
                # Obtém as coordenadas do retângulo delimitador do contorno
                x, y, w, h = cv2.boundingRect(cnt)
                # Desenha apenas o contorno do retângulo ao redor do objeto detectado
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                # Adiciona o texto com a cor e as coordenadas do objeto ao quadro
                cv2.putText(frame, f'{color} ({x}, {y})', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                # Adiciona a cor e as coordenadas do objeto à lista de objetos detectados
                color_objects.append((color, (x, y, w, h)))

    # Retorna o quadro de imagem modificado e a lista de objetos detectados
    return frame, color_objects

# Inicializa a captura de vídeo a partir da câmera padrão (índice 0)
cap = cv2.VideoCapture(0)

# Loop principal para capturar e processar os quadros de vídeo
while True:
    # Captura um quadro de vídeo
    ret, frame = cap.read()

    # Verifica se o quadro foi capturado corretamente
    if not ret:
        break

    # Espelha horizontalmente o quadro de vídeo
    frame = cv2.flip(frame, 1)

    cv2.rectangle(frame, (62, 62 + 260), (138, 138 + 260), (255, 255, 255), 2)
    cv2.putText(frame, '1', (85, 120 + 260), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (162, 62 + 160), (238, 138 + 160), (255, 255, 255), 2)  # Ajuste aqui
    cv2.putText(frame, '2', (185, 120 + 160), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)  # Ajuste aqui
    cv2.rectangle(frame, (262, 62), (338, 138), (255, 255, 255), 2)
    cv2.putText(frame, '3', (285, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (362, 62 + 160), (438, 138 + 160), (255, 255, 255), 2)
    cv2.putText(frame, '4', (385, 120 + 160), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.rectangle(frame, (462, 62 + 260), (538, 138 + 260), (255, 255, 255), 2)
    cv2.putText(frame, '5', (485, 120 + 260), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    # Ajuste aqui

    # Detecta objetos coloridos no quadro de vídeo e obtém a lista de objetos detectados
    frame, color_objects = detect_color(frame)

    # Itera sobre a lista de objetos detectados
    for color, (cx, cy, cw, ch) in color_objects:
        # Verifica se o centro do objeto está sobre uma das bases brancas
        if 62 < cx < 138 and 62 + 260 < cy < 138 + 260:
            cv2.putText(frame, f'{color} on White 1', (62, 50 + 260), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 162 < cx < 238 and 62 + 160 < cy < 138 + 160:
            cv2.putText(frame, f'{color} on White 2', (162, 50 + 160), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255),
                        2)
        elif 262 < cx < 338 and 62 < cy < 138:
            cv2.putText(frame, f'{color} on White 3', (262, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        elif 362 < cx < 438 and 62 + 160 < cy < 138 + 160:  # Ajuste aqui
            cv2.putText(frame, f'{color} on White 4', (362, 50 + 160), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255),
                        2)  # Ajuste aqui
        elif 462 < cx < 538 and 62 + 260 < cy < 138 + 260:
            cv2.putText(frame, f'{color} on White 5', (462, 50 + 260), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255),
                        2)

    # Exibe o quadro de vídeo com a detecção de cores
    cv2.imshow('Color Detection', frame)

    # Verifica se a tecla 'q' foi pressionada para sair do loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libera os recursos da câmera e fecha todas as janelas abertas
cap.release()
cv2.destroyAllWindows()
