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
hxsVector3 p;
hxRobotInfo robotInfo;
hxCommand cmd;
hxSensor sensor;


void sigHandler(int signo)
{
  running = 0;
}


int l_armstart()
{
  if (hx_connect(NULL, 0) != hxOK)
  {
    printf("hx_connect(): Request error.\n");
    return -1;
  }
  if (signal(SIGINT, sigHandler) == SIG_ERR)
  {
    printf("Error catching SIGINT\n");
    return -1;
  }
  if (signal(SIGTERM, sigHandler) == SIG_ERR)
  {
    printf("Error catching SIGTERM\n");
    return -1;
  }
  return 1;
}


int update()
{
  if (hx_update(&cmd, &sensor) != hxOK)
  {
    printf("hx_update(): Request error.\n");
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
  if (hx_close() != hxOK)
  {
    printf("hx_close(): Request error.\n");
    return 1;
  }
  return 1;
}



int l_handDown(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 2){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}

int l_handRotate(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 0){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}

int l_handTurn(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 1){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}


int l_finger1(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 3){
		cmd.ref_pos[i] = 150;
	}
	if(i == 4){
		cmd.ref_pos[i] = angle;
	}
	if(i == 5){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}


int l_finger2down(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 8){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
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
      return -1;
    }
    return 0;
}

int l_finger4down(double angle)
{
    int i;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 10){
		cmd.ref_pos[i] = angle;
	}
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}

int l_fingerdown(double angle1, double angle2, double angle3)
{
    int i;
    cmd.ref_pos[3] = 150;
    cmd.ref_pos[4] = angle1;
    cmd.ref_pos[5] = angle1;
    cmd.ref_pos[8] = angle2;
    cmd.ref_pos[9] = angle2;
    cmd.ref_pos[10] = angle2;
    cmd.ref_pos[12] = angle3;
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	cmd.ref_vel[i] = 1.0;
    }
    cmd.ref_vel_enabled = 1;
    cmd.ref_pos_enabled = 1;
    if(update() < 0)
    {
      printf("update failed\n");
      return -1;
    }
    return 0;
}


int l_left()
{
	hxsSimInfo *info;
	hxsTransform pt;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info -> models[i].name[0] == 'm' && info->models[i].name[1] == 'p'){
			pt = info->models[i].transform;
			pt.pos.x -= 0.02;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}

int l_right()
{
	hxsSimInfo *info;
	hxsTransform pt;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info -> models[i].name[0] == 'm' && info->models[i].name[1] == 'p'){
			pt = info->models[i].transform;
			pt.pos.x += 0.02;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}



int getlocation()
{
	hxsSimInfo *info;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info->models[i].name[0] == 'm' && info->models[i].name[1] == 'p'){
			p = info->models[i].transform.pos;
		}
	}
	free(info);
	return 0;
}

float getlocationX(){return p.x;}
float getlocationY(){return p.y;}
float getlocationZ(){return p.z;}

int l_forceget0()
{return sensor.contact[0];}
int l_forceget1()
{return sensor.contact[1];}
int l_forceget2()
{return sensor.contact[2];}
int l_forceget3()
{return sensor.contact[3];}
int l_forceget4()
{return sensor.contact[4];}
int l_forceget5()
{return sensor.contact[5];}
int l_forceget6()
{return sensor.contact[6];}
int l_forceget7()
{return sensor.contact[7];}
int l_forceget8()
{return sensor.contact[8];}
int l_forceget9()
{return sensor.contact[9];}
int l_forceget10()
{return sensor.contact[10];}
int l_forceget11()
{return sensor.contact[11];}
int l_forceget12()
{return sensor.contact[12];}
int l_forceget13()
{return sensor.contact[13];}
int l_forceget14()
{return sensor.contact[14];}
int l_forceget15()
{return sensor.contact[15];}
int l_forceget16()
{return sensor.contact[16];}
int l_forceget17()
{return sensor.contact[17];}
int l_forceget18()
{return sensor.contact[18];}

