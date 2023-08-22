import RPi.GPIO as GPIO
import time

# Configuração inicial
servo_pin = 23  # Usando o pino GPIO 23
GPIO.setmode(GPIO.BCM)
GPIO.setup(servo_pin, GPIO.OUT)

# Inicializa o PWM com frequência de 50Hz
pwm = GPIO.PWM(servo_pin, 50)
pwm.start(0)

def move_servo(angle):
    duty_cycle = angle / 18 + 2  # Mapeia ângulo para ciclo de trabalho (2 a 12)
    pwm.ChangeDutyCycle(duty_cycle)
    time.sleep(0.5)  # Aguarda um curto período para que o servo alcance a posição

try:
    while True:
        angle = float(input("Digite o ângulo (0 a 180 graus): "))
        if 0 <= angle <= 180:
            move_servo(angle)
        else:
            print("Ângulo fora do intervalo válido (0 a 180 graus)")

except KeyboardInterrupt:
    pass

# Encerra o PWM e limpa as configurações do GPIO
pwm.stop()
GPIO.cleanup()

