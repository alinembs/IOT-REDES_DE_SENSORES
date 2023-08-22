#!/usr/bin/env python
# -*- coding: utf-8 -*-
import RPi.GPIO as GPIO
import time
def AngleToDuty(ang):
  return float(pos)/10.+5.
  
#Setup servoPin as PWM output of frequancy 100Hz
servoPin1=23
servoPin2=24 
servoPin3 = 25
GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPin1,GPIO.OUT)
pwm1=GPIO.PWM(servoPin,100)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPin2,GPIO.OUT)
pwm2=GPIO.PWM(servoPin,100)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPin3,GPIO.OUT)
pwm3=GPIO.PWM(servoPin,100)
#setup sweep parameters
depart =0
arrivee=180
DELAY=0.1
incStep=5
pos=depart
if __name__ == '__main__' :
    pwm1.start(AngleToDuty(pos)) #star pwm
    pwm2.start(AngleToDuty(pos)) #star pwm
    pwm3.start(AngleToDuty(pos)) #star pwm
    nbRun=3
    i=0
    while i<nbRun:
        print("--------------------------run {}".format(i)) 
        for pos in range(depart,arrivee,incStep):
            duty=AngleToDuty(pos)
            pwm1.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        for pos in range(arrivee,depart,-incStep):
            duty=AngleToDuty(pos)
            pwm1.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        i=i+1
      
    pwm1.stop() #stop sending value to output
    while i<nbRun:
        print("--------------------------run {}".format(i)) 
        for pos in range(depart,arrivee,incStep):
            duty=AngleToDuty(pos)
            pwm2.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        for pos in range(arrivee,depart,-incStep):
            duty=AngleToDuty(pos)
            pwm2.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        i=i+1
      
    pwm2.stop() #stop sending value to output
    while i<nbRun:
        print("--------------------------run {}".format(i)) 
        for pos in range(depart,arrivee,incStep):
            duty=AngleToDuty(pos)
            pwm3.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        for pos in range(arrivee,depart,-incStep):
            duty=AngleToDuty(pos)
            pwm3.ChangeDutyCycle(duty)
            time.sleep(DELAY)
        print("position: {}° -> duty cycle : {}%".format(pos,duty))
        
        i=i+1
      
    pwm3.stop() #stop sending value to output


    GPIO.cleanup() #release channel
