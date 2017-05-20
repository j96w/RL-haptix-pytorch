from __future__ import absolute_import
from __future__ import division
import numpy as np
from copy import deepcopy

from multiprocessing import Process, Queue
from ctypes import cdll, c_int, byref, c_float
import os, time, random
import subprocess
import ctypes
import sys    
import math
from time import sleep

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
        self.Onepos = 0
        self.Twopos = 0
        self.Trepos = 0
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
        return 6

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

            new_ctrl_env = os.environ.copy()
            new_ctrl_env['IGN_PARTITION'] = 'arm' + str(env_ind)

            subprocess.Popen(['gazebo', 'worlds/arat.world'], env = new_arm_env)
            sleep(10)

            self.so = ctypes.CDLL(self.root_dir + '/libarmctl.so')
        #assert self.env_type == "arm"

        #self.env = gym.make(self.game)
        #self.env.seed(self.seed)    # NOTE: so each env would be different

        # action space setup
        self.actions     = 6
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
        return random.randint(0, 5)

    def reset(self):
        def Armreset(q, i, so):
            os.environ['IGN_PARTITION'] = 'arm' + str(i)

            so.l_armstart()

            for tt in range(0, 20):
                so.l_up()
            for tt in range(0, 20):
                so.l_out()
            for tt in range(0, 20):
                so.l_right()

            #print(q.empty())

            #so.l_fingerdown(byref((c_float)(Onepos)), byref((c_float)(Twopos)), byref((c_float)(Trepos)))

            so.getHandlocation()
            so.getWoodlocation()

            q.put(so.getHandlocationX())
            q.put(so.getHandlocationY())
            q.put(so.getHandlocationZ())
            q.put(so.getWoodlocationX())
            q.put(so.getWoodlocationY())
            q.put(so.getWoodlocationZ())
            sleep(10)

        self._reset_experience()
        self.Nowstep = 0

        pr = Process(target = Armreset, args = (self.q, self.ind, self.so))
        pr.start()
        pr.join()
        pr.terminate()
        pr.join()

        self.exp_state1 = []
        for i in range(0, 6):
            self.exp_state1.append(self.q.get())
        #print(self.q.empty())
        return self._get_experience()


    def step(self, action_index):
        #print(self.ind, action_index)
        def Armstep(q, i, so):
            #print(i)
            os.environ['IGN_PARTITION'] = 'arm' + str(i)

            so.l_armstart()

            todo = q.get()

            if todo == 0:
                so.l_up()
            if todo == 1:
                so.l_down()
            if todo == 2:
                so.l_left()
            if todo == 3:
                so.l_right()
            if todo == 4:
                so.l_in()
            if todo == 5:
                so.l_out()

            #so.l_fingerdown(byref((c_float)(Onepos)), byref((c_float)(Twopos)), byref((c_float)(Trepos)))
            so.getHandlocation()
            so.getWoodlocation()

            q.put(so.getHandlocationX())
            q.put(so.getHandlocationY())
            q.put(so.getHandlocationZ())
            q.put(so.getWoodlocationX())
            q.put(so.getWoodlocationY())
            q.put(so.getWoodlocationZ())
            
        self.Nowstep = self.Nowstep + 1
        self.exp_action = action_index

        self.q.put(self.exp_action)

        ps = Process(target = Armstep, args = (self.q, self.ind, self.so))
        ps.start()
        ps.join()
        ps.terminate()
        ps.join()
        #print(self.Nowstep)


        self.exp_state1 = []
        for i in range(0, 6):
            self.exp_state1.append(self.q.get())

        #print(self.q.empty())
        reward1 = abs(self.exp_state1[0] - self.exp_state1[3])
        reward2 = abs(self.exp_state1[1] - self.exp_state1[4])
        reward3 = abs(self.exp_state1[2] - self.exp_state1[5])
        reward = 10000 / (1 + reward1 + reward2 + reward3)
        #print(self.exp_state1[0], self.exp_state1[1], self.exp_state1[2], self.exp_state1[3], self.exp_state1[4], self.exp_state1[5])
        #print(reward)

        self.exp_reward = reward

        if self.Nowstep > 600:
            self.exp_terminal1 = True


        return self._get_experience()
