#include <stdio.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/touch_sensor.h>

#define SPEED 5
#define TIME_STEP 64

int main() {
  WbDeviceTag bumper;
  WbDeviceTag camera, left_motor, right_motor;
  wb_robot_init();
  int count = 0;

  /* Get the camera device, enable it and the recognition */
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);
  wb_camera_recognition_enable(camera, TIME_STEP);

  bumper = wb_robot_get_device("bumper");
  wb_touch_sensor_enable(bumper, TIME_STEP);
  
  
  /* get a handler to the motors and set target position to infinity (speed control). */
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);


  /* Main loop */
  while (wb_robot_step(TIME_STEP) != -1) 
  {
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);
    int num = wb_camera_recognition_get_number_of_objects(camera);
    if (num == 0)
    {
       wb_motor_set_velocity(left_motor, SPEED);
       wb_motor_set_velocity(right_motor, SPEED);
       // changing direction
       if (wb_touch_sensor_get_value(bumper) > 0)
          {
          count = 20;
          printf("\n Obstacle Detected with touch sensor! \n");
         }
       if (count == 0) 
       {
       wb_motor_set_velocity(left_motor, SPEED);
       wb_motor_set_velocity(right_motor, SPEED);
       }
       else if (count >= 9) 
       {
       wb_motor_set_velocity(left_motor, -SPEED);
       wb_motor_set_velocity(right_motor, -SPEED);
       count--;
       } 
       else 
       {
       wb_motor_set_velocity(left_motor, -SPEED);
       wb_motor_set_velocity(right_motor, SPEED);
       count--;
       }
   }
   else
   {
    printf("\n Soccerball found. Moving towards it! \n");
    if (objects[0].position_on_image[0] < 100)
    {
     wb_motor_set_velocity(left_motor, -SPEED);
     wb_motor_set_velocity(right_motor, SPEED);
     printf("\n Aligning to the left \n");
    }
    else if (objects[0].position_on_image[0] > 150)
    {
     wb_motor_set_velocity(left_motor, SPEED);
     wb_motor_set_velocity(right_motor, -SPEED);
     printf("\n Aligning to the right \n");
    }
    else if (objects[0].position[0] < 0.1)
    {
     wb_motor_set_velocity(left_motor, 0.0);
     wb_motor_set_velocity(right_motor, 0.0);
     printf("\n Soccerball reached! Objective completed! :) \n");
     break;
    }
    else
    {
     wb_motor_set_velocity(left_motor, SPEED);
     wb_motor_set_velocity(right_motor, SPEED);
    }
   }

 } 

  wb_robot_cleanup();

  return 0;
}
