/*
 * Copyright (C) 2014-2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifdef _WIN32
#include <windows.h>
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <haptix/comm/haptix.h>
#ifdef _WIN32
#include <windows.h>
#endif

int running = 1;

//////////////////////////////////////////////////
void sigHandler(int signo)
{
  // Terminate the program.
  running = 0;
}

//////////////////////////////////////////////////
void printState(const hxRobotInfo *_robotInfo, const hxSensor *_sensor)
{
  int i;

  printf("\tTime: %d.%09d\n",
    _sensor->time_stamp.sec, _sensor->time_stamp.nsec);

  printf("\tMotors:\n");
  for (i = 0; i < _robotInfo->motor_count; ++i)
  {
    printf("\t\tMotor %d\n", i);
    printf("\t\t\tPosition: %f rads.\n", _sensor->motor_pos[i]);
    printf("\t\t\tVelocity: %f rads./sec.\n", _sensor->motor_vel[i]);
    printf("\t\t\tTorque: %f N. m.\n" , _sensor->motor_torque[i]);
  }

  printf("\tJoints:\n");
  for (i = 0; i < _robotInfo->joint_count; ++i)
  {
    printf("\t\tJoint %d\n", i);
    printf("\t\t\tPosition: %f rads.\n", _sensor->joint_pos[i]);
    printf("\t\t\tVelocity: %f rads./sec.\n", _sensor->joint_vel[i]);
  }

  printf("\tContact sensors:\n");
  for (i = 0; i < _robotInfo->contact_sensor_count; ++i)
  {
    printf("\t\t# %d\n", i);
    printf("\t\t\tvalue: %f N.\n", _sensor->contact[i]);
  }

  printf("\tIMUs:\n");
  for (i = 0; i < _robotInfo->imu_count; ++i)
  {
    printf("\t\t# %d\n", i);
    printf("\t\t\tLinear acceleration: (%f, %f, %f) m./seg2.\n",
      _sensor->imu_linear_acc[i][0], _sensor->imu_linear_acc[i][1],
      _sensor->imu_linear_acc[i][2]);
    printf("\t\t\tAngular velocity: (%f, %f, %f) rads./sec.\n",
      _sensor->imu_angular_vel[i][0], _sensor->imu_angular_vel[i][1],
      _sensor->imu_angular_vel[i][2]);
  }
}

//////////////////////////////////////////////////
void printRobotInfo(const hxRobotInfo *_robotInfo)
{
  printf("Robot information received:\n");
  printf("Num motors: %d\n", _robotInfo->motor_count);
  printf("Num joints: %d\n", _robotInfo->joint_count);
  printf("Num contact sensors: %d\n", _robotInfo->contact_sensor_count);
  printf("Num IMUs: %d\n", _robotInfo->imu_count);
  printf("Update rate: %f\n", _robotInfo->update_rate);
  printf("Actuated joint limits: \n");

  // Print joint limits.
  int i;
  for (i = 0; i < _robotInfo->motor_count; ++i)
  {
    printf("\tJoint associated to motor %d:\n", i);
    printf("\t\t Min: %f rads.\n", _robotInfo->joint_limit[i][0]);
    printf("\t\t Max: %f rads.\n", _robotInfo->joint_limit[i][1]);
  }
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
  int i;
  int counter = 0;
  hxRobotInfo robotInfo;
  hxCommand cmd;
  hxSensor sensor;

  // Capture SIGINT signal.
  if (signal(SIGINT, sigHandler) == SIG_ERR)
    printf("Error catching SIGINT\n");

  // Capture SIGTERM signal.
  if (signal(SIGTERM, sigHandler) == SIG_ERR)
    printf("Error catching SIGTERM\n");

  // Connect to the simulator / hardware
  if (hx_connect(NULL, 0) != hxOK)
  {
    printf("hx_connect(): Request error.\n");
    return -1;
  }

  // Requesting robot information.
  if (hx_robot_info(&robotInfo) != hxOK)
  {
    printf("hx_getrobotinfo(): Request error.\n");
    return -1;
  }

  // Print the robot information.
  printRobotInfo(&robotInfo);

  // Uncomment this block to start logging.
  // if (hxs_start_logging("/tmp/log/") != hxOK)
  //   printf("hxs_start_logging(): error.\n");

  int steps = 0;

  // Send commands at ~100Hz.
  while (steps < 3000)
  {
    // Create a new command based on a sinusoidal wave.
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
      // Set the desired position of this motor
      cmd.ref_pos[i] = (float)(350 * 0.5 *
        sin(0.05 * 2.0 * M_PI * counter * 0.01));
      // We could set a desired maximum velocity
      // cmd.ref_vel[i] = 1.0;
      // cmd.ref_vel_max[i] = 1.0;
      // We could set a desired controller position gain
      // cmd.gain_pos[i] = 1.0;
      // We could set a desired controller velocity gain
      // cmd.gain_vel[i] = 1.0;
    }
    // Indicate that the positions we set should be used.
    cmd.ref_pos_enabled = 1;
    // We're not setting it, so indicate that ref_vel should be ignored.
    cmd.ref_vel_enabled = 0;
    // We're not setting it, so indicate that ref_vel_max should be ignored.
    cmd.ref_vel_max_enabled = 0;
    // We're not setting it, so indicate that gain_pos should be ignored.
    cmd.gain_pos_enabled = 0;
    // We're not setting it, so indicate that gain_vel should be ignored.
    cmd.gain_vel_enabled = 0;

    // Send the new joint command and receive the state update.
    if (hx_update(&cmd, &sensor) != hxOK)
    {
      printf("hx_update(): Request error.\n");
      continue;
    }

    // Debug output: Print the state.
    if (!(counter % 100))
      printState(&robotInfo, &sensor);

    if (++counter == 10000)
      counter = 0;

    ++steps;

    // Here is where you would do your other work, such as reading from EMG
    // sensors, decoding that data, computing your next control command,
    // etc.  In this example, we're just sleeping for 10ms.
    //
    // You might also want to sleep in your code, because there's a maximum
    // rate at which the limb can process new commands and produce new
    // sensor readings.  Depending on how long your computation takes, you
    // might want to wait here until it's time to send a new command.  Or
    // you might want to run as fast as possible, computing and sending new
    // commands constantly (but knowing that not all of them will be
    // executed by the limb).
    unsigned int sleeptime_us = 10000;
#ifdef _WIN32
    Sleep((DWORD)(sleeptime_us / 1e3));
#else
    usleep(sleeptime_us);
#endif
  }

  // Uncomment this block to stop logging.
  // if (hxs_stop_logging() != hxOK)
  //   printf("hxs_stop_logging(): error.\n");

  // Disconnect from the simulator / hardware
  if (hx_close() != hxOK)
  {
    printf("hx_close(): Request error.\n");
    return -1;
  }

  return 0;
}
