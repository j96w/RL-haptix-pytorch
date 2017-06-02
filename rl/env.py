from __future__ import absolute_import
from __future__ import division
import numpy as np
from copy import deepcopy
import torch

from multiprocessing import Process, Queue, Value, Array
from ctypes import cdll, c_int, byref, c_float
import os, time, random
import subprocess
import ctypes
import sys    
import math
from time import sleep
import scipy.misc

import string
import matplotlib.pyplot as plt
import Image
sys.modules['Image'] = Image
import socket

from utils.helpers import Experience            # NOTE: here state0 is always "None"
from utils.helpers import preprocessAtari, rgb2gray, rgb2y, scale

class Env(object):
    def __init__(self, args, env_ind=0):
        self.logger     = args.logger
        self.ind        = env_ind               # NOTE: for creating multiple environment instances
        # general setup
        self.mode       = args.mode             # NOTE: save frames when mode=2
        if self.mode == 2:
            try:
                import scipy.misc
                self.imsave = scipy.misc.imsave
            except ImportError as e: self.logger.warning("WARNING: scipy.misc not found")
            self.img_dir = args.root_dir + "/imgs/"
            self.frame_ind = 0

        self.root_dir = args.root_dir
        self.seed       = args.seed + self.ind  # NOTE: so to give a different seed to each instance
        self.visualize  = args.visualize
        if self.visualize:
            self.vis        = args.vis
            self.refs       = args.refs
            self.win_state1 = "win_state1"

        self.env_type   = args.env_type
        self.game       = args.game
        self._reset_experience()

        self.Nowstep = 0
        self.q = Queue()

        self.logger.warning("<-----------------------------------> Env")
        self.logger.warning("Creating {" + self.env_type + " | " + self.game + "} w/ Seed: " + str(self.seed))

    def _reset_experience(self):
        self.exp_state0 = None  # NOTE: always None in this module
        self.exp_action = None
        self.exp_reward = None
        self.exp_state1 = None
        self.exp_terminal1 = None

    def _get_experience(self):
        return Experience(state0 = self.exp_state0, # NOTE: here state0 is always None
                          action = self.exp_action,
                          reward = self.exp_reward,
                          state1 = self._preprocessState(self.exp_state1),
                          terminal1 = self.exp_terminal1)

    def _preprocessState(self, state):
        raise NotImplementedError("not implemented in base calss")

    @property
    def state_shape(self):
        raise NotImplementedError("not implemented in base calss")

    @property
    def action_dim(self):
        return 8

    def render(self):       # render using the original gl window
        raise NotImplementedError("not implemented in base calss")

    def visual(self):       # visualize onto visdom
        raise NotImplementedError("not implemented in base calss")

    def reset(self):
        raise NotImplementedError("not implemented in base calss")

    def step(self, action):
        raise NotImplementedError("not implemented in base calss")

class GymEnv(Env):  # low dimensional observations
    def __init__(self, args, env_ind=0):
        super(GymEnv, self).__init__(args, env_ind)

        assert self.env_type == "gym"
        try: import gym
        except ImportError as e: self.logger.warning("WARNING: gym not found")

        self.env = gym.make(self.game)
        self.env.seed(self.seed)    # NOTE: so each env would be different

        # action space setup
        self.actions     = range(self.action_dim)
        self.logger.warning("Action Space: %s", self.actions)

        # state space setup
        self.logger.warning("State  Space: %s", self.state_shape)

    def _preprocessState(self, state):    # NOTE: here no preprecessing is needed
        return state

    @property
    def state_shape(self):
        return self.env.observation_space.shape[0]

    def render(self):
        if self.mode == 2:
            frame = self.env.render(mode='rgb_array')
            frame_name = self.img_dir + "frame_%04d.jpg" % self.frame_ind
            self.imsave(frame_name, frame)
            self.logger.warning("Saved  Frame    @ Step: " + str(self.frame_ind) + " To: " + frame_name)
            self.frame_ind += 1
            return frame
        else:
            return self.env.render()


    def visual(self):
        pass

    def sample_random_action(self):
        return self.env.action_space.sample()

    def reset(self):
        self._reset_experience()
        self.exp_state1 = self.env.reset()
        return self._get_experience()

    def step(self, action_index):
        self.exp_action = action_index
        self.exp_state1, self.exp_reward, self.exp_terminal1, _ = self.env.step(self.actions[self.exp_action])
        return self._get_experience()

class AtariRamEnv(Env):  # atari games w/ ram states as input
    def __init__(self, args, env_ind=0):
        super(AtariRamEnv, self).__init__(args, env_ind)

        assert self.env_type == "atari-ram"
        try: import gym
        except ImportError as e: self.logger.warning("WARNING: gym not found")

        self.env = gym.make(self.game)
        self.env.seed(self.seed)    # NOTE: so each env would be different

        # action space setup
        self.actions     = range(self.action_dim)
        self.logger.warning("Action Space: %s", self.actions)

        # state space setup
        self.logger.warning("State  Space: %s", self.state_shape)

    def _preprocessState(self, state):    # NOTE: here the input is [0, 255], so we normalize
        return state/255.                 # TODO: check again the range, also syntax w/ python3

    @property
    def state_shape(self):
        return self.env.observation_space.shape[0]

    def render(self):
        return self.env.render()

    def visual(self):   # TODO: try to grab also the pixel-level outputs and visualize
        pass

    def sample_random_action(self):
        return self.env.action_space.sample()

    def reset(self):
        self._reset_experience()
        self.exp_state1 = self.env.reset()
        return self._get_experience()

    def step(self, action_index):
        self.exp_action = action_index
        self.exp_state1, self.exp_reward, self.exp_terminal1, _ = self.env.step(self.actions[self.exp_action])
        return self._get_experience()

class AtariEnv(Env):  # pixel-level inputs
    def __init__(self, args, env_ind=0):
        super(AtariEnv, self).__init__(args, env_ind)

        assert self.env_type == "atari"
        try: import gym
        except ImportError as e: self.logger.warning("WARNING: gym not found")

        self.env = gym.make(self.game)
        self.env.seed(self.seed)    # NOTE: so each env would be different

        # action space setup
        self.actions     = range(self.action_dim)
        self.logger.warning("Action Space: %s", self.actions)
        # state space setup
        self.hei_state = args.hei_state
        self.wid_state = args.wid_state
        self.preprocess_mode = args.preprocess_mode if not None else 0 # 0(crop&resize) | 1(rgb2gray) | 2(rgb2y)
        #assert self.hei_state == self.wid_state
        self.logger.warning("State  Space: (" + str(self.hei_state) + " * " + str(self.wid_state) + ")")

    def _preprocessState(self, state):
        if self.preprocess_mode == 3:   # crop then resize
            state = preprocessAtari(state)
        if self.preprocess_mode == 2:   # rgb2y
            state = scale(rgb2y(state), self.hei_state, self.wid_state) / 255.
        elif self.preprocess_mode == 1: # rgb2gray
            state = scale(rgb2gray(state), self.hei_state, self.wid_state) / 255.
        elif self.preprocess_mode == 0: # do nothing
            pass
        return state.reshape(self.hei_state * self.wid_state)

    @property
    def state_shape(self):
        return self.hei_state

    def render(self):
        return self.env.render()

    def visual(self):
        if self.visualize:
            self.win_state1 = self.vis.image(np.transpose(self.exp_state1, (2, 0, 1)), env=self.refs, win=self.win_state1, opts=dict(title="state1"))
        if self.mode == 2:
            frame_name = self.img_dir + "frame_%04d.jpg" % self.frame_ind
            self.imsave(frame_name, self.exp_state1)
            self.logger.warning("Saved  Frame    @ Step: " + str(self.frame_ind) + " To: " + frame_name)
            self.frame_ind += 1

    def sample_random_action(self):
        return self.env.action_space.sample()

    def reset(self):
        # TODO: could add random start here, since random start only make sense for atari games
        self._reset_experience()
        self.exp_state1 = self.env.reset()
        return self._get_experience()

    def step(self, action_index):
        self.exp_action = action_index
        self.exp_state1, self.exp_reward, self.exp_terminal1, _ = self.env.step(self.actions[self.exp_action])
        return self._get_experience()

class LabEnv(Env):
    def __init__(self, args, env_ind=0):
        super(LabEnv, self).__init__(args, env_ind)

        assert self.env_type == "lab"

class ArmEnv(Env):
    def __init__(self, args, env_ind=0, env_beg=0):
        print("armenv", env_ind)
        super(ArmEnv, self).__init__(args, env_ind)
        if env_beg == 0:
            new_arm_env = os.environ.copy()
            new_arm_env['GAZEBO_MASTER_URI'] = 'http://127.0.0.1:1123' + str(env_ind)
            new_arm_env['IGN_PARTITION'] = 'arm' + str(env_ind)
            new_arm_env['IGN_PARTITION_PORT'] = '2333' + str(env_ind)

            def armCtrl(n, a):
                so = ctypes.CDLL(self.root_dir + '/libarmctl.so')
                os.environ['IGN_PARTITION'] = 'arm' + str(int(a[0]))
                os.environ['IGN_PARTITION_PORT'] = '2333' + str(int(a[0]))

                host = '127.0.0.1'
                port = int('2333' + str(int(a[0])))
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.bind((host,port))
                s.listen(1)
                conn, addr = s.accept()
                data = ''

                so.l_armstart()

                while True:
                    time.sleep(0.1)
                    data = ''
                    if n.value == 2.0:
                        if a[1] == 0:
                            so.l_up()
                        if a[1] == 1:
                            so.l_down()
                        if a[1] == 2:
                            so.l_right()
                        if a[1] == 3:
                            so.l_left()
                        if a[1] == 4:
                            so.l_in()
                        if a[1] == 5:
                            so.l_out()
                        if a[1] == 6:
                            so.l_catch()
                        if a[1] == 7:
                            so.l_decatch()

                        so.getHandlocation()
                        so.getWoodlocation()

                        a[2] = so.getHandlocationX()
                        a[3] = so.getHandlocationY()
                        a[4] = so.getHandlocationZ()
                        a[5] = so.getWoodlocationX()
                        a[6] = so.getWoodlocationY()
                        a[7] = so.getWoodlocationZ()


                        while 1:
                            data = conn.recv(80000)
                            #print(len(data))
                            if (len(data) > 21100 and len(data) < 21200):
                                break
                            #print(data)
                            #print(len(data))
                            #if len(data) == 8000:
                            #    break
                        #img = np.array(Image.new("RGB",(84,84),(0, 0, 0)))
                        now = 10;
                        for i in range(84):
                            for j in range(84):
                                s1 = data[3 * (i * 84 + j)]
                                h1 = ord(s1)
                                s2 = data[3 * (i * 84 + j) + 1]
                                h2 = ord(s2)
                                s3 = data[3 * (i * 84 + j) + 2]
                                h3 = ord(s3)
                                a[now] = int((h1 * 0.299 + h2 * 0.587 + h3 * 0.114))
                                now = now + 1
                        #plt.imshow(img)
                        #plt.show()
                        n.value = 1.0

                    if n.value == 3.0:
                        so.l_reset()
                        so.getHandlocation()
                        so.getWoodlocation()

                        a[2] = so.getHandlocationX()
                        a[3] = so.getHandlocationY()
                        a[4] = so.getHandlocationZ()
                        a[5] = so.getWoodlocationX()
                        a[6] = so.getWoodlocationY()
                        a[7] = so.getWoodlocationZ()

                        while 1:
                            data = conn.recv(80000)
                            #print(len(data))
                            if (len(data) > 21100 and len(data) < 21200):
                                break

                        now = 10;
                        for i in range(84):
                            for j in range(84):
                                s1 = data[3 * (i * 84 + j)]
                                h1 = ord(s1)
                                s2 = data[3 * (i * 84 + j) + 1]
                                h2 = ord(s2)
                                s3 = data[3 * (i * 84 + j) + 2]
                                h3 = ord(s3)
                                a[now] = int((h1 * 0.299 + h2 * 0.587 + h3 * 0.114))
                                now = now + 1
                        n.value = 1.0

            self.num = Value('d', 0.0)
            self.arr = Array('f',range(8000))
            self.arr[0] = env_ind
            p = Process(target=armCtrl, args=(self.num, self.arr))
            p.start()

            subprocess.Popen(['gazebo', 'worlds/arat.world'], env = new_arm_env)
            sleep(20)
        #assert self.env_type == "arm"

        #self.env = gym.make(self.game)
        #self.env.seed(self.seed)    # NOTE: so each env would be different

        # action space setup
        self.actions     = 8
        self.logger.warning("Action Space: %s", self.actions)
        # state space setup
        self.hei_state = args.hei_state
        self.wid_state = args.wid_state
        self.preprocess_mode = args.preprocess_mode if not None else 0 # 0(crop&resize) | 1(rgb2gray) | 2(rgb2y)
        #assert self.hei_state == self.wid_state
        self.logger.warning("State  Space: (" + str(self.hei_state) + " * " + str(self.wid_state) + ")")

    def _preprocessState(self, state):
        if self.preprocess_mode == 3:   # crop then resize
            state = preprocessAtari(state)
        if self.preprocess_mode == 2:   # rgb2y
            state = scale(rgb2y(state), self.hei_state, self.wid_state) / 255.
        elif self.preprocess_mode == 1: # rgb2gray
            state = scale(rgb2gray(state), self.hei_state, self.wid_state) / 255.
        elif self.preprocess_mode == 0: # do nothing
            pass
        #print(self.hei_state)
        #print(self.wid_state)
        #print(state)
        return state

    @property
    def state_shape(self):
        return self.hei_state

    def render(self):
        pass
        #return self.env.render()

    def visual(self):
        pass
    #    if self.visualize:
    #        self.win_state1 = self.vis.image(np.transpose(self.exp_state1, (2, 0, 1)), env=self.refs, win=self.win_state1, opts=dict(title="state1"))
    #    if self.mode == 2:
    #        frame_name = self.img_dir + "frame_%04d.jpg" % self.frame_ind
    #        self.imsave(frame_name, self.exp_state1)
    #        self.logger.warning("Saved  Frame    @ Step: " + str(self.frame_ind) + " To: " + frame_name)
    #        self.frame_ind += 1

    def sample_random_action(self):
        return random.randint(0, 7)

    def reset(self):
        self._reset_experience()
        self.Nowstep = 0
        self.num.value = 3.0
        a = []
        b = []
        self.locat = []
        now = 0
        while True:
            time.sleep(0.1)
            if self.num.value == 1.0:
                for i in range(2, 8):
                    self.locat.append(int(self.arr[i]))
                for i in range(84):
                    for j in range(84):
                        b.append(float(self.arr[i * 84 + j + 10]))
                    a.append(b)
                    b = []
                break
        self.exp_state1 = np.array(a)



        return self._get_experience()


    def step(self, action_index):
        print(self.ind, action_index)
        self.Nowstep = self.Nowstep + 1
        self.exp_action = action_index
        self.arr[1] = action_index
        self.num.value = 2.0
        a = []
        b = []
        self.locat = []
        now = 0
        while True:
            time.sleep(0.1)
            if self.num.value == 1.0:
                for i in range(2, 8):
                    self.locat.append(int(self.arr[i]))
                for i in range(84):
                    for j in range(84):
                        b.append(float(self.arr[i * 84 + j + 10]))
                    a.append(b)
                    b = []
                break
        self.exp_state1 = np.array(a)


        #print(self.Nowstep)

        r1 = abs(self.locat[0] - self.locat[3])
        r2 = abs(self.locat[1] - self.locat[4])
        r3 = abs(self.locat[2] - self.locat[5])
        #print(r1, r2, r3, self.locat[3], self.locat[4], self.locat[5])
        reward = 0

        if (r1 < 30) and (r2 < 30) and (r3 < 30):
            reward = 1
        if (r1 < 8) and (r2 < 8) and (r3 < 8):
            reward = 20
        if ((r1 + r2 + r3) > 200):
            reward = -10
        if (self.locat[5] > 110):
            reward = 400

        #print(self.exp_state1[0], self.exp_state1[1], self.exp_state1[2], self.exp_state1[3], self.exp_state1[4], self.exp_state1[5])
        #print(reward)

        self.exp_reward = reward
        if (self.locat[3] < 15) or (self.locat[3] > 25):
            self.exp_terminal1 = True
        if (self.locat[4] < -32) or (self.locat[4] > -22):
            self.exp_terminal1 = True
        if (self.locat[5] < 100) or (self.locat[5] > 150):
            self.exp_terminal1 = True
        if self.Nowstep > 400:
            self.exp_terminal1 = True

        return self._get_experience()
