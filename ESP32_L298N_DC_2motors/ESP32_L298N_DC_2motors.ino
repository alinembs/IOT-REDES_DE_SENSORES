/*
 * Library Example for L298N Module to control DC motors
 * 
 * This code is to control single motor. For two motor control, please open L298N_DC_2_Motors
 * this code is ready for ESP32
 * Watch video instructions for this code:  https://youtu.be/2JTMqURJTwg
 * 
 * Written by Ahmad Shamshiri on Dec 24, 2019 
 * in Ajax, Ontario, Canada. www.robojax.com
 * 
  Need wiring diagram from this code: Purchase My course on Udemy.com http://robojax.com/L/?id=62
 * 
  * 
 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Robojax_L298N_DC_motor.h>
// motor 1 settings
#define CHA 0
#define ENA 14 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 27
#define IN2 26

// motor 2 settings
#define IN3 25
#define IN4 33
#define ENB 32// this pin must be PWM enabled pin if Arduino board is used
#define CHB 1

const int CCW = 2; // do not change
const int CW  = 1; // do not change

#define motor1 1 // do not change
#define motor2 2 // do not change

// for single motor
//Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, true);	

// for two motors without debug information	// Watch video instruciton for this line: https://youtu.be/2JTMqURJTwg
Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA,  IN3, IN4, ENB, CHB);

// fore two motors with debut information
//Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB, true);

void setup() {
  Serial.begin(115200);
  robot.begin();
  //L298N DC Motor by Robojax.com

}

void loop() {
  
 // robot.demo(1);
  robot.rotate(motor1, 80, CW);//run motor1 at 60% speed in CW direction
  robot.rotate(motor2, 70, CCW);//run motor1 at 60% speed in CW direction
  
  delay(3000);

  robot.brake(1);
  robot.brake(2);  
  delay(2000);


  robot.rotate(motor1, 100, CW);//run motor1 at 60% speed in CW direction
  delay(3000);
  
  robot.rotate(motor2, 100, CCW);//run motor1 at 60% speed in CW direction
  
  robot.brake(1);
  robot.brake(2);   
  delay(2000);  

  for(int i=0; i<=100; i++)
  {
    robot.rotate(motor1, i, CW);// turn motor1 with i% speed in CW direction (whatever is i) 
    delay(100);
  }
  delay(2000);
  
  robot.brake(1);
  delay(2000);  
  
  for(int i=0; i<=100; i++)
  {
    robot.rotate(motor2, i, CW);// turn motor1 with i% speed in CW direction (whatever is i) 
    delay(100);
  }
  delay(2000);
  
  robot.brake(2);
  delay(2000);    
  // Robojax L298N Library. Watch video instruciton https://youtu.be/2JTMqURJTwg
}
