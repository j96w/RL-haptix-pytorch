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
float handx = 0.0;
float handy = 0.0;
float handz = 0.0;
float woodx = 0.0;
float woody = 0.0;
float woodz = 0.0;
float resetHandx = 0.24;
float resetHandy = -0.90;
float resetHandz = 1.31;
float resetWoodx = 0.21;
float resetWoody = -0.28;
float resetWoodz = 1.03;


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



int l_reset()
{
	if(resetHandx < 0.44)
	{
		resetHandx += 0.02;
	}	
	else
	{
		resetHandx = 0.04;
	}
	hxsSimInfo *info;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxsTransform pt;
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info->models[i].name[0] == 'm' && info->models[i].name[1] == 'p'){
			pt = info->models[i].transform;
			pt.pos.x = resetHandx;
		 	pt.pos.y = resetHandy;
			pt.pos.z = resetHandz;
			pt.orient.x = 0.0;
			pt.orient.y = 0.0;
			pt.orient.z = 3.1416;

		}
	}
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	for(i = 0; i < n; i++)
	{
		if(info->models[i].name[0] == 'w' && info->models[i].name[10] == '5'){
			pt = info->models[i].transform;
			pt.pos.x = resetWoodx;
			pt.pos.y = resetWoody;
			pt.pos.z = resetWoodz;
			pt.orient.x = 0.0;
			pt.orient.y = 0.0;
			pt.orient.z = 0.0;
		}
	}
	hxs_set_model_transform("wood_cube_5cm", &pt);
	free(info);
	return 0;
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


int l_finger1down(double angle)
{
    int i;
    cmd.ref_pos[3] = 250;
    //cmd.ref_pos[4] = angle;
    //cmd.ref_pos[5] = angle;      
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


int l_finger2down(double angle)
{
    int i;
    cmd.ref_pos[8] = angle;
    cmd.ref_pos[9] = angle;
    cmd.ref_pos[10] = angle;
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

int l_finger3down(double angle)
{
    int i;
    cmd.ref_pos[11] = angle;
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
			pt.pos.x -= 0.005;
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
			pt.pos.x += 0.005;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}


int l_up()
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
			pt.pos.z += 0.010;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}

int l_down()
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
			pt.pos.z -= 0.002;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}


int l_in()
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
			pt.pos.y += 0.005;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}


int l_out()
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
			pt.pos.y -= 0.005;
		}
	}
	free(info);
	hxs_set_model_transform("mpl_haptix_right_forearm", &pt);
	
	return 0;
}


int getHandlocation()
{
	hxsSimInfo *info;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info->models[i].name[0] == 'm' && info->models[i].name[1] == 'p'){
			p = info->models[i].transform.pos;
			handx = p.x;
			handy = p.y;
			handz = p.z;
		}
	}
	free(info);
	return 0;
}

int getWoodlocation()
{
	hxsSimInfo *info;
	info = (hxsSimInfo*)malloc(sizeof(hxsSimInfo));
	hxs_sim_info(info);
	int i, n = info->model_count;
	for(i = 0; i < n; i++)
	{
		if(info->models[i].name[0] == 'w' && info->models[i].name[10] == '5'){
			p = info->models[i].transform.pos;
			woodx = p.x;
			woody = p.y;
			woodz = p.z;
	
		}
	}
	free(info);
	return 0;
}

int getHandlocationX(){return (int)(handx * 100);}
int getHandlocationY(){return (int)(handy * 100 + 60);}
int getHandlocationZ(){return (int)(handz * 100);}
int getWoodlocationX(){return (int)(woodx * 100);}
int getWoodlocationY(){return (int)(woody * 100);}
int getWoodlocationZ(){return (int)(woodz * 100);}

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

