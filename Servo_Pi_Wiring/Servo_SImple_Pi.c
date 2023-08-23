#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO_PIN 18 // Use o número do pino GPIO que desejar

int main() {
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Erro ao inicializar a biblioteca WiringPi.\n");
        return 1;
    }

    // Inicialize o pino do servo motor
    softPwmCreate(SERVO_PIN, 0, 200); // Pino, valor inicial, faixa de valores

    printf("Movendo o servo motor para a posição central...\n");
    softPwmWrite(SERVO_PIN, 15); // Valor correspondente à posição central

    delay(1000); // Espere 1 segundo

    printf("Movendo o servo motor para a posição oposta...\n");
    softPwmWrite(SERVO_PIN, 24); // Valor correspondente à posição oposta

    delay(1000); // Espere 1 segundo

    // Desligue o pino do servo motor
    softPwmWrite(SERVO_PIN, 0);

    return 0;
}

