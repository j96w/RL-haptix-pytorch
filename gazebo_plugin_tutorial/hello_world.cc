#include "gazebo/gazebo.hh"
#include "gazebo/plugins/CameraPlugin.hh"
#include <fstream>
#include <iostream>
#include </usr/include/python2.7/Python.h>
#include <string>
namespace gazebo
{
  std::ofstream out("res.txt");
  std::ofstream out1("res1.txt");
  std::ofstream out2("res2.txt");
  std::ofstream out3("res3.txt");
  class CameraDump : public CameraPlugin
  {
    public: CameraDump() : CameraPlugin(), saveCount(0) {}
	public: ~CameraDump(){Py_Finalize();}

    public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
    {
      // Don't forget to load the camera plugin
      	CameraPlugin::Load(_parent, _sdf);
		Py_Initialize();

    	std::string path = "/home/jeremywang/gazebo_plugin_tutorial/pylib";//int.py path
    	std::string chdir_cmd = std::string("sys.path.append(\"") + path + "\")";
   		const char* cstr_cmd = chdir_cmd.c_str();
   		PyRun_SimpleString("import sys");
    	PyRun_SimpleString(cstr_cmd);

		std::cout << "starting to use py\n";

		modName = PyString_FromString("int");
		pyMod = PyImport_Import(modName);//import py module
    }

    // Update the controller
    public: void OnNewFrame(const unsigned char *_image,
        unsigned int _width, unsigned int _height, unsigned int _depth,
        const std::string &_format)
    {
		const unsigned char * str = this->parentSensor->GetCamera()->ImageData();//get image
		const char* str1 = static_cast<const char*>( static_cast<void*>( const_cast<unsigned char*>(str)));
		std::string str2(str1);
	

		if (!pyMod){
    	   std::cout << "[ERROR] Python get module failed.\n";
     	   return;
   		}
		std::cout << "succeed get module\n";
		pyFunc = PyObject_GetAttrString(pyMod,"reimg");//get function

  	 	if (!pyFunc || !PyCallable_Check(pyFunc)){
		    std::cout << "[ERROR] Can't find funftion (test_add)\n";
		    return;
		}
		std::cout << "succeed find fun\n";
		pyParams = PyTuple_New(1);
		PyTuple_SetItem(pyParams,0,PyString_FromString(str2.c_str()));
		PyObject_CallObject(pyFunc,pyParams);//call function
	
		std::cout << "end of one img\n";
}

    private: int saveCount;
	PyObject *modName;
	PyObject *pyMod;
	PyObject *pyFunc;
	PyObject *pyParams;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(CameraDump)
}
