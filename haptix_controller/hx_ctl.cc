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
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

#ifdef _cplusplus
extern "C"{
#endif
    #include "lua5.2/lua.h"
    #include "lua5.2/lualib.h"
    #include "lua5.2/lauxlib.h"



int running = 1;
int counter = 0;
int port = 0;
int numbig = 11000;
int numsmall = 1;
char armName[100];
int posX = 0;
int posY = 0;
int posZ = 0;
hxRobotInfo robotInfo;
hxCommand cmd;
hxSensor sensor;


namespace gazebo
{
  class ModelPush : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&ModelPush::OnUpdate, this, _1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
	  double x,y,z;
	  gazebo::math::Pose pose;
      	  pose = this->model->GetWorldPose();
	  math::Vector3 v(0,0,0);
	  v = pose.pos;
	  posX = v.x;
      	  posY = v.y;
      	  posZ = v.z;
      //std::cout << "arm:\n";
      //std::cout << "x :" << x << '\n' 
	//			<< "y :" << y << '\n'
	//			<< "z :" << z << '\n';
      usleep(3000);
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}


void sigHandler(int signo)
{
  // Terminate the program.
  running = 0;
}


static int l_armstart(lua_State *L)
{
  int portt = lua_tointeger(L, -1);
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



static int sleepn(lua_State *L)
{
    int m = lua_tointeger(L, -1);
    usleep(m * 1000);
    // usleep takes microseconds. This converts the parameter to milliseconds.
    // Change this as necessary.
    // Alternatively, use 'sleep()' to treat the parameter as whole seconds.
    return 0;
}

static int ctrl_wait(lua_State *L){
    int pid = lua_tointeger(L, -1);
    waitpid(pid, NULL, 0);
    return 0;
}


static int ctrl_kill(lua_State *L){
    int pid = lua_tointeger(L, -1);
    kill(pid, SIGKILL);
    return 0;
}


extern char **environ;
void run_cmd(char *cmd)
{
    char allcmd[100];
    char *str1 = (char *)"gazebo worlds/arat.world";
    char *str3 = (char *)"arm";
    stpcpy(allcmd, str1);
    //strcat(allcmd, cmd);
    //strcat(allcmd, str2);


    char newPort[100];
    char *str4 = (char *)"http://127.0.0.1:";
    stpcpy(newPort, str4);
    strcat(newPort, cmd);


    char newArm[100];
    stpcpy(newArm, str3);
    strcat(newArm, cmd + 4);
    stpcpy(armName, newArm);

    pid_t pid;
    char *argv[4];
	argv[0] = (char *)"sh";
	argv[1] = (char *)"-c";
	argv[2] = allcmd;
	argv[3] = NULL;

    int status;
    
    setenv("IGN_PARTITION", newArm, 1);
    setenv("GAZEBO_MASTER_URI", newPort, 1);
    
    char* test;
    test = getenv("IGN_PARTITION");
    printf("IGN_PARTITION is    %s\n", test);
    test = getenv("GAZEBO_MASTER_URI");
    printf("GAZEBO_MASTER_URI is    %s\n", test);

    printf("Run command: %s\n", allcmd);
    status = posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, environ);
    if (status == 0) {
        printf("Child pid: %i\n", pid);
    } else {
        printf("posix_spawn: %s\n", strerror(status));
    }
}



static int newArm(lua_State *L)
{
     char *str = (char *)lua_tostring(L, -1);
     run_cmd(str);
     return 1;
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



static int l_armstop(){
  //if (hx_close() != hxOK)
  //{
  //  printf("hx_close(): Request error.\n");
  //  return -1;
 // }
}

static void checktoptype(lua_State *L){
    if(lua_isnumber(L, -1))
        printf("number");
    else if(lua_isstring(L, -1))
        printf("string");
    else if(lua_isfunction(L, -1))
        printf("function");
    else if(lua_isboolean(L, -1))
        printf("boolean");
    else if(lua_isnil(L, -1))
        printf("nil");
    else if(lua_istable(L, -1))
        printf("table");
    else
        printf("userdata or thread");
}

static int l_printstate(){
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
       sensor.imu_orientation[i][0], sensor.imu_orientation[i][1], sensor.imu_orientation[i][2], sensor.imu_orientation[i][3]);
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



static int l_handDown(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
      printf("bxhabshbhsadjbcd %f\n", angle);
      printf("update failed\n");
    }
    return 1;
}

static int l_handRotate(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
    }
    return 1;
}

static int l_handTurn(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
    }
    return 1;
}


static int l_finger1(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
    }
    return 1;
}


static int l_finger2down(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
    }
    return 1;
}

static int l_finger3down(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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

static int l_finger4down(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
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
    }
    return 1;
}

static int l_finger5down(lua_State *L)
{
    int i;
    double angle = lua_tonumber(L, -1);
    for (i = 0; i < robotInfo.motor_count; ++i)
    {
	if(i == 12){
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


int l_forceget0(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[0]);return 1;}
int l_forceget1(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[1]);return 1;}
int l_forceget2(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[2]);return 1;}
int l_forceget3(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[3]);return 1;}
int l_forceget4(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[4]);return 1;}
int l_forceget5(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[5]);return 1;}
int l_forceget6(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[6]);return 1;}
int l_forceget7(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[7]);return 1;}
int l_forceget8(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[8]);return 1;}
int l_forceget9(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[9]);return 1;}
int l_forceget10(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[10]);return 1;}
int l_forceget11(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[11]);return 1;}
int l_forceget12(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[12]);return 1;}
int l_forceget13(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[13]);return 1;}
int l_forceget14(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[14]);return 1;}
int l_forceget15(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[15]);return 1;}
int l_forceget16(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[16]);return 1;}
int l_forceget17(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[17]);return 1;}
int l_forceget18(lua_State* L)
{int i;lua_pushnumber(L, sensor.contact[18]);return 1;}


int l_getAllforce(lua_State* L)
{
	int sum = 0;
	int i;
	for(i = 0; i <= 18; i++){
		sum = sum + sensor.contact[i];
	}
	lua_pushnumber(L, sum);
}


int getNumbig(lua_State* L)
{lua_pushnumber(L, numbig); numbig = numbig + 1; return 1;}
int getNumsmall(lua_State* L)
{lua_pushnumber(L, numsmall); numsmall = numsmall + 1; return 1;}
int getArmname(lua_State* L)
{lua_pushstring(L, armName); return 1;}


int getposX(lua_State* L)
{lua_pushnumber(L, posX); return 1;}
int getposY(lua_State* L)
{lua_pushnumber(L, posY); return 1;}
int getposZ(lua_State* L)
{lua_pushnumber(L, posZ); return 1;}


static const struct luaL_Reg l_lib[] = {
	{"luaArmstart", l_armstart},
	{"luaPrintrobotinfo", l_printrobotinfo},
	{"luaPrintstate", l_printstate},
 	{"luaArmstop", l_armstop},
	{"luaHanddown", l_handDown},
	{"luaHandrotate", l_handRotate},
	{"luaHandturn", l_handTurn},
	{"luaFinger1", l_finger1},
	{"luaFinger2down", l_finger2down},
	{"luaFinger3down", l_finger3down},
	{"luaFinger4down", l_finger4down},
	{"luaFinger5down", l_finger5down},
  	{"luaGetforce0", l_forceget0},
	{"luaGetforce1", l_forceget1},
	{"luaGetforce2", l_forceget2},
	{"luaGetforce3", l_forceget3},
	{"luaGetforce4", l_forceget4},
	{"luaGetforce5", l_forceget5},
	{"luaGetforce6", l_forceget6},
	{"luaGetforce7", l_forceget7},
	{"luaGetforce8", l_forceget8},
	{"luaGetforce9", l_forceget9},
	{"luaGetforce10", l_forceget10},
	{"luaGetforce11", l_forceget11},
	{"luaGetforce12", l_forceget12},
	{"luaGetforce13", l_forceget13},
	{"luaGetforce14", l_forceget14},
	{"luaGetforce15", l_forceget15},
	{"luaGetforce16", l_forceget16},
	{"luaGetforce17", l_forceget17},
	{"luaGetforce18", l_forceget18},
	{"luaGetallforce", l_getAllforce},
  	{"newArm", newArm},
    	{"ctrl_wait", ctrl_wait},
    	{"ctrl_kill", ctrl_kill},
	{"sleepn", sleepn},
	{"getNumbig", getNumbig},
	{"getNumsmall", getNumsmall},
	{"getArmname", getArmname},
	{"getposX", getposX},
	{"getposY", getposY},
	{"getposZ", getposZ},
	{NULL, NULL}
};

int luaopen_libarmctl (lua_State *L){
    luaL_newlib(L, l_lib);
    return 1;
}

#ifdef _cplusplus
}
#endif
