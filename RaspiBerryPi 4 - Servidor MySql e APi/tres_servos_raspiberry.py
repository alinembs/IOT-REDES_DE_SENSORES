import RPi.GPIO as GPIO
import time

# Configuração inicial
servo_pins = [18, 23, 24]  # Pinos GPIO para os três servos
GPIO.setmode(GPIO.BCM)
for pin in servo_pins:
    GPIO.setup(pin, GPIO.OUT)

# Inicializa os PWMs com frequência de 50Hz
pwms = [GPIO.PWM(pin, 50) for pin in servo_pins]
for pwm in pwms:
    pwm.start(0)

def move_servos(angles):
    for i, angle in enumerate(angles):
        duty_cycle = angle / 18 + 2  # Mapeia ângulo para ciclo de trabalho (2 a 12)
        pwms[i].ChangeDutyCycle(duty_cycle)
    
    time.sleep(0.5)  # Aguarda um curto período para que os servos alcancem as posições

try:
    while True:
        servo_angles = []
        for i in range(3):
            angle = float(input(f"Digite o ângulo do servo {i + 1} (0 a 180 graus): "))
            if 0 <= angle <= 180:
                servo_angles.append(angle)
            else:
                print("Ângulo fora do intervalo válido (0 a 180 graus)")
                servo_angles.append(0)  # Define o ângulo para 0

        move_servos(servo_angles)

except KeyboardInterrupt:
    pass

# Encerra os PWMs e limpa as configurações do GPIO
for pwm in pwms:
    pwm.stop()
GPIO.cleanup()

