import objects
import pygame
from sys import exit
from config import *
import csv
import tkinter as tk
from tkinter import filedialog

root = tk.Tk()
root.withdraw()
pygame.init()

screen = pygame.display.set_mode((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
pygame.display.set_caption('DeepGame')
clock = pygame.time.Clock()

#Object added to the level
player = objects.Player()
spikes = []
walls = []
swords = []
coins = []
floors = []
door = objects.Door(8, 12)

#Load a level
def load(file_path):
    csvfile = open(file_path, 'r', newline='')
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
        if row[0] == 'wall_mid':
            walls.append(objects.Wall('wall_mid', int(row[1]), int(row[2])))
        if row[0] == 'floor_spikes_anim_f0':
            spikes.append(objects.Spike(int(row[1]), int(row[2])))
        if row[0] == 'floor_1':
            floors.append(objects.Floor(int(row[1]), int(row[2])))
        if row[0] == 'knight_m_run_anim_f0':
            player.set_pos(int(row[1]), int(row[2]))
        if row[0] == 'weapon_anime_sword':
            swords.append(objects.Sword(int(row[1]), int(row[2]), int(row[3]), int(row[4])))
        if row[0] == 'coin_anim_f0':
            coins.append(objects.Coin(int(row[1]), int(row[2])))
        if row[0] == 'floor_ladder':
            door.set_pos(int(row[1]), int(row[2]))
file_name = filedialog.askopenfilename()
load(file_name)

#Creation of background
background_surface = pygame.Surface((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
background_surface.fill('Black')
for floor in floors:
    background_surface.blit(floor.image, floor.rect)
for wall in walls:
    background_surface.blit(wall.image, wall.rect)


#Main loop
while True:
    #Keyboard events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_q:
                player.set_v_x(-PLAYER_SPEED)
            if event.key == pygame.K_d:
                player.set_v_x(PLAYER_SPEED)
            if event.key == pygame.K_z:
                player.set_v_y(-PLAYER_SPEED)
            if event.key == pygame.K_s:
                player.set_v_y(PLAYER_SPEED)
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_q or event.key == pygame.K_d:
                player.set_v_x(0)
            if event.key == pygame.K_z or event.key == pygame.K_s:
                player.set_v_y(0)
    
    #Update objects
    op = True
    for coin in coins:
        if(player.rect.colliderect(coin.rect)):
            coin.take()
        if not coin.is_taken():
            op = False
    if op : 
        door.open()
    
    for spike in spikes:
        if(player.rect.colliderect(spike.rect)):
            player.die()
    
    for sword in swords:
        if(player.rect.colliderect(sword.rect)):
            player.die()

    #Update screen
    screen.blit(background_surface, (0,0))
    
    door.update()
    screen.blit(door.image, door.rect)
    for spike in spikes:
        spike.update()
        screen.blit(spike.image, spike.rect)
    for coin in coins:
        coin.update()
        screen.blit(coin.image, coin.rect)
    player.update()
    if(player.collision(walls)):
        player.step_back()
    screen.blit(player.image, player.rect)
    for sword in swords:
        sword.update()
        screen.blit(sword.image, sword.rect)
    
    pygame.display.update()
    clock.tick(FPS)