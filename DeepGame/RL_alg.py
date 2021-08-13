import gym 
import os
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.vec_env import VecFrameStack
from stable_baselines3.common.evaluation import evaluate_policy

from gym import Env
from gym.spaces import Discrete, Box, Dict, Tuple, MultiBinary, MultiDiscrete 
import numpy as np
import random

import objects
import pygame
from sys import exit
from config import *
import csv
from stable_baselines3.common.env_checker import check_env


pygame.init()

environment_name = "CartPole-v0"

log_path = os.path.join('training', 'logs')
model_path = os.path.join('training', 'models')




class DeepGameEnv(Env):
    def __init__(self, level):
        # Actions we can take, down, stay, up
        self.action_space = Discrete(5)
        # Temperature array
        self.observation_space = Box(0, 255, (WIDTH * SIZE_SPRITE, HEIGHT * SIZE_SPRITE, 3), dtype=np.uint8)
        # Set start image
        self.screen = pygame.display.set_mode((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
        self.state = pygame.surfarray.array3d(self.screen)
        self.count = 0
        #Create the game
        self.player = objects.Player()
        self.spikes = []
        self.walls = []
        self.swords = []
        self.coins = []
        self.floors = []
        self.door = objects.Door()
        self.levelname = LEVELS_DIR + 'level' + str(level) + '.csv'
        self.load(self.levelname)

        #Creation of background
        self.background_surface = pygame.Surface((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
        self.background_surface.fill('Black')
        for floor in self.floors:
            self.background_surface.blit(floor.image, floor.rect)
        for wall in self.walls:
            self.background_surface.blit(wall.image, wall.rect)
        
    def step(self, action):
        # Apply action
        # 0 No movement
        # 1 Left
        # 2 Right 
        # 3 Up
        # 4 Down
        if action == 0:
            self.player.set_v_x(0)
            self.player.set_v_y(0)
        elif action == 1:
            self.player.set_v_x(-PLAYER_SPEED)
            self.player.set_v_y(0)
        elif action == 2:
            self.player.set_v_x(PLAYER_SPEED)
            self.player.set_v_y(0)
        elif action == 3:
            self.player.set_v_y(-PLAYER_SPEED)
            self.player.set_v_x(0)
        elif action == 4:
            self.player.set_v_y(PLAYER_SPEED)
            self.player.set_v_x(0)
        #COUNTER
        self.count+=1
        #Update objects
        self.door.update()
        for spike in self.spikes:
            spike.update()
        for coin in self.coins:
            coin.update()
        self.player.update()
        if(self.player.collision(self.walls)):
            self.player.step_back()
        for sword in self.swords:
            sword.update()
        #Update screen
        self.screen.blit(self.background_surface, (0,0))
        
        self.screen.blit(self.door.image, self.door.rect)
        for spike in self.spikes:
            self.screen.blit(spike.image, spike.rect)
        for coin in self.coins:
            self.screen.blit(coin.image, coin.rect)
        self.screen.blit(self.player.image, self.player.rect)
        for sword in self.swords:
            self.screen.blit(sword.image, sword.rect)
        #Update objectives
        op = True
        for coin in self.coins:
            if(self.player.rect.colliderect(coin.rect)):
                coin.take()
            if not coin.is_taken():
                op = False
        if op : 
            self.door.open()
        for spike in self.spikes:
            if(self.player.rect.colliderect(spike.rect)):
                self.player.die()
        for sword in self.swords:
            if(self.player.rect.colliderect(sword.rect)):
                self.player.die()
        
        # Calculate reward and check if it's done
        reward = 0
        done = False
        for coin in self.coins:
            if coin.is_taken():
                reward += 1
        if self.door.opened:
            reward += 10
            if self.door.rect.colliderect(self.player.rect):
                reward += 100
                done = True
        if self.player.dead or self.count >= 500:
            reward -= 100
            done = True
        
        
        # Set placeholder for info
        info = {}
        self.state = pygame.surfarray.array3d(self.screen)
        # Return step information
        return self.state, reward, done, info

    def render(self):        
        pygame.display.update()
        pass
    
    def reset(self):
        #Create the game
        self.player = objects.Player()
        self.spikes = []
        self.walls = []
        self.swords = []
        self.coins = []
        self.floors = []
        self.door = objects.Door()
        self.count = 0

        self.load(self.levelname)

        #Creation of background
        self.background_surface = pygame.Surface((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
        self.background_surface.fill('Black')
        for floor in self.floors:
            self.background_surface.blit(floor.image, floor.rect)
        for wall in self.walls:
            self.background_surface.blit(wall.image, wall.rect)
        return pygame.surfarray.array3d(self.screen)
    def load(self, file_path):
        csvfile = open(file_path, 'r', newline='')
        spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
        for row in spamreader:
            if row[0] == 'wall_mid':
                self.walls.append(objects.Wall('wall_mid', int(row[1]), int(row[2])))
            if row[0] == 'floor_spikes_anim_f0':
                self.spikes.append(objects.Spike(int(row[1]), int(row[2])))
            if row[0] == 'floor_1':
                self.floors.append(objects.Floor(int(row[1]), int(row[2])))
            if row[0] == 'knight_m_run_anim_f0':
                self.player.set_pos(int(row[1]), int(row[2]))
            if row[0] == 'weapon_anime_sword':
                self.swords.append(objects.Sword(int(row[1]), int(row[2]), int(row[3]), int(row[4])))
            if row[0] == 'coin_anim_f0':
                self.coins.append(objects.Coin(int(row[1]), int(row[2])))
            if row[0] == 'floor_ladder':
                self.door.set_pos(int(row[1]), int(row[2]))
        
env = DeepGameEnv(4)
env.observation_space.sample()
check_env(env, warn=True)

#Test env
def test_env():
    episodes = 5
    for episode in range(1, episodes+1):
        state = env.reset()
        done = False
        score = 0 
        
        while not done:
            env.render()
            action = env.action_space.sample()
            n_state, reward, done, info = env.step(action)
            score+=reward
        print('Episode:{} Score:{}'.format(episode, score))
    env.close()

log_path = os.path.join('training', 'logs')
models_path = os.path.join('training', 'models')
test_env()
#model = PPO("CnnPolicy", env, verbose=1, tensorboard_log=log_path)
#model.learn(total_timesteps=4000)

#model.save(models_path)
