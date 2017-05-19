#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <spawn.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <haptix/comm/haptix.h>
#include <haptix/comm/haptix_sim.h>


#include "lua5.2/lua.h"
#include "lua5.2/lualib.h"
#include "lua5.2/lauxlib.h"

int running = 1;
hxRobotInfo robotInfo;
hxCommand cmd;
hxSensor sensor;


void sigHandler(int signo)
{
  // Terminate the program.
  running = 0;
}


int l_armstart()
{
  // Connect to the simulator / hardware
  //printf("%d", portt);
  if (hx_connect(NULL, 0) != hxOK)
  {
    printf("hx_connect(): Request error.\n");
    return -1;
  }
  // Capture SIGINT signal.
  if (signal(SIGINT, sigHandler) == SIG_ERR)
  {
    printf("Error catching SIGINT\n");
    return -1;
  }

  // Capture SIGTERM signal.
  if (signal(SIGTERM, sigHandler) == SIG_ERR)
  {
    printf("Error catching SIGTERM\n");
    return -1;
  }
}




int update()
{
  //printf("the num is %d\n", numsmall);
  if (hx_update(&cmd, &sensor) != hxOK)
  {
    printf("hx_update(): Request error.\n");
    //printf("the num is %d\n", numsmall);
    return -1;
  }
  if (hx_robot_info(&robotInfo) != hxOK)
  {
    printf("hx_getrobotinfo(): Request error.\n");
    return -1;
  }
  return 1;
}



int l_armstop(){
  //if (hx_close() != hxOK)
  //{
  //  printf("hx_close(): Request error.\n");
  //  return -1;
 // }
}


int l_printstate(){
  update();
  int i;

  printf("\tTime: %d.%09d\n",
    sensor.time_stamp.sec, sensor.time_stamp.nsec);

   printf("\tMotors:\n");
   for (i = 0; i < robotInfo.motor_count; ++i)
   {
     printf("\t\tMotor %d\n", i);
     printf("\t\t\tPosition: %f rads.\n", sensor.motor_pos[i]);
     printf("\t\t\tVelocity: %f rads./sec.\n", sensor.motor_vel[i]);
     printf("\t\t\tTorque: %f N. m.\n" , sensor.motor_torque[i]);
   }

   printf("\tJoints:\n");
   for (i = 0; i < robotInfo.joint_count; ++i)
   {
     printf("\t\tJoint %d\n", i);
     printf("\t\t\tPosition: %f rads.\n", sensor.joint_pos[i]);
     printf("\t\t\tVelocity: %f rads./sec.\n", sensor.joint_vel[i]);
   }

   printf("\tContact sensors:\n");
   for (i = 0; i < robotInfo.contact_sensor_count; ++i)
   {
     printf("\t\t# %d\n", i);
     printf("\t\t\tvalue: %f N.\n", sensor.contact[i]);
   }


   printf("\tIMUs:\n");
   for (i = 0; i < robotInfo.imu_count; ++i)
   {
     printf("\t\t# %d\n", i);
     printf("\t\t\t3D Location: (%f, %f, %f, %f) m./seg2.\n",
       sensor.imu_orientation[i][0], sensor.imu_orientation[i][1], sensor.imu_orientation[i][2],sensor.imu_orientation[i][3]);
     printf("\t\t\tAngular velocity: (%f, %f, %f) rads./sec.\n",
       sensor.imu_angular_vel[i][0], sensor.imu_angular_vel[i][1],
       sensor.imu_angular_vel[i][2]);
   }

  return 1;
}


static int l_printrobotinfo(){
  update();
  printf("Robot information received:\n");
  printf("Num motors: %d\n", robotInfo.motor_count);
  printf("Num joints: %d\n", robotInfo.joint_count);
  printf("Num contact sensors: %d\n", robotInfo.contact_sensor_count);
  printf("Num IMUs: %d\n", robotInfo.imu_count);
  printf("Update rate: %f\n", robotInfo.update_rate);
  printf("Actuated joint limits: \n");

   // Print joint limits.
   int i;
   for (i = 0; i < robotInfo.motor_count; ++i)
   {
     printf("\tJoint associated to motor %d:\n", i);
     printf("\t\t Min: %f rads.\n", robotInfo.joint_limit[i][0]);
     printf("\t\t Max: %f rads.\n", robotInfo.joint_limit[i][1]);
   }
  return 1;
}


int l_finger3down(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 9){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
    }
    return 1;
}



int l_forceget0()
{return sensor.contact[0];}

