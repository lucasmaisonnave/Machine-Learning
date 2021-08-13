import pygame
from pygame import mouse
from pygame.cursors import sizer_x_strings
from config import *
import objects
from sys import exit
import csv
import tkinter as tk
from tkinter import filedialog

root = tk.Tk()
root.withdraw()

#file_path = filedialog.askopenfilename()

screen = pygame.display.set_mode((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
pygame.display.set_caption('DeepGame Map')
clock = pygame.time.Clock()

background_surface = pygame.Surface((SIZE_SPRITE * WIDTH, SIZE_SPRITE * HEIGHT))
background_surface.fill('Black')

menu_surface = pygame.Surface((SIZE_SPRITE*WIDTH, SIZE_SPRITE*5))
menu_surface.fill('White')
menu_rect = menu_surface.get_rect(bottom = SIZE_SPRITE * HEIGHT)
background_surface.blit(menu_surface, menu_rect)

map_buttons = objects.MapButtons(menu_rect)
map_buttons.add('floor_spikes_anim_f0')
map_buttons.add('floor_ladder')
map_buttons.add('knight_m_run_anim_f0')
map_buttons.add('weapon_anime_sword')
map_buttons.add('coin_anim_f0')
map_buttons.add('floor_1')

cursor = objects.MapButton('wall_mid', 0, 0)

map = [['' for y in range(HEIGHT)] for x in range(WIDTH)]
floors = [['' for y in range(HEIGHT)] for x in range(WIDTH)]
swords = [[[] for y in range(HEIGHT)] for x in range(WIDTH)]
sword_choice = False
sword_pos = [0,0,0,0]

def save(file_path):
    csvfile = open(file_path, 'w', newline='')
    spamwriter = csv.writer(csvfile, delimiter=' ',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
    #['name', 'x1', 'y1', 'x2', 'y2'])
    for x in range(WIDTH):
        for y in range(HEIGHT):
            if floors[x][y] != '':
                spamwriter.writerow([floors[x][y], str(x), str(y), str(0), str(0)])
            if map[x][y] != '':
                if map[x][y] != 'weapon_anime_sword':
                    spamwriter.writerow([map[x][y], x, y, 0, 0])
                else:
                    spamwriter.writerow([map[x][y], x, y, swords[x][y][0], swords[x][y][1]])
def load(file_path):
    csvfile = open(file_path, 'r', newline='')
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
        if(row[0] == 'floor_1'):
            floors[int(row[1])][int(row[2])] = 'floor_1'
        elif(row[0] == 'weapon_anime_sword'):
            swords[int(row[1])][int(row[2])] = [int(row[1]), int(row[2]), int(row[3]), int(row[4])]
        else:
            map[int(row[1])][int(row[2])] = row[0]

file_name = filedialog.askopenfilename()
if(len(file_name)):
    load(file_name)
while True:
    mouse_pos = pygame.mouse.get_pos()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_s: #Save
                file_path = filedialog.asksaveasfilename()
                save(file_path)
        if event.type == pygame.MOUSEBUTTONDOWN:
            if cursor.name == 'weapon_anime_sword' and not menu_rect.collidepoint(mouse_pos) and not sword_choice:
                map[mouse_pos[0] // SIZE_SPRITE][mouse_pos[1] // SIZE_SPRITE] = cursor.name
                sword_choice = True
                sword_pos[0] = mouse_pos[0] // SIZE_SPRITE
                sword_pos[1] = mouse_pos[1] // SIZE_SPRITE
        if event.type == pygame.MOUSEBUTTONUP:
            if cursor.name == 'weapon_anime_sword' and not menu_rect.collidepoint(mouse_pos) and sword_choice:
                sword_pos[2] = mouse_pos[0] // SIZE_SPRITE
                sword_pos[3] = mouse_pos[1] // SIZE_SPRITE
                sword_choice = False
                swords[sword_pos[0]][sword_pos[1]] = [sword_pos[2], sword_pos[3]]
    cursor.rect.x = mouse_pos[0] - mouse_pos[0] % SIZE_SPRITE
    cursor.rect.y = mouse_pos[1] - mouse_pos[1] % SIZE_SPRITE

    if pygame.mouse.get_pressed()[0]: #click left button
        if not menu_rect.collidepoint(mouse_pos):
            #if cursor.name == 'coin_anim_f0':
            #    rect = image.get_rect(topleft = (mouse_pos[0] - mouse_pos[0]%SIZE_SPRITE, mouse_pos[1]- mouse_pos[1]%SIZE_SPRITE))
            if cursor.name == 'floor_1':
                floors[mouse_pos[0] // SIZE_SPRITE][mouse_pos[1] // SIZE_SPRITE] = cursor.name
            elif cursor.name != 'weapon_anime_sword' and cursor.name != 'floor_1':
                map[mouse_pos[0] // SIZE_SPRITE][mouse_pos[1] // SIZE_SPRITE] = cursor.name
        else:
            for button in map_buttons.list:
                if button.rect.collidepoint(mouse_pos):
                    cursor.image = button.image
                    cursor.name = button.name
    if pygame.mouse.get_pressed()[2]: #right click, remove
        if not menu_rect.collidepoint(mouse_pos):
            map[mouse_pos[0] // SIZE_SPRITE][mouse_pos[1] // SIZE_SPRITE] = ''
            floors[mouse_pos[0] // SIZE_SPRITE][mouse_pos[1] // SIZE_SPRITE] = ''            
    if pygame.mouse.get_pressed()[1]: #middle click, reset background
            background_surface.fill('Black')
            background_surface.blit(menu_surface, menu_rect)
            map = [['' for y in range(HEIGHT)] for x in range(WIDTH)]
            floors = [['' for y in range(HEIGHT)] for x in range(WIDTH)]
    #Recreate background
    background_surface.fill('Black')
    background_surface.blit(menu_surface, menu_rect)
    for x in range(WIDTH):
        for y in range(HEIGHT):
            if(floors[x][y] != ''):
                button = map_buttons.get_Button(floors[x][y])
                img = button.image
                img.set_alpha(ALPHA_FLOOR)
                rect = img.get_rect(topleft = (x*SIZE_SPRITE, y*SIZE_SPRITE))
                background_surface.blit(img, rect)
            if(map[x][y] != ''):
                button = map_buttons.get_Button(map[x][y])
                img = button.image
                rect = img.get_rect(topleft = (x*SIZE_SPRITE, y*SIZE_SPRITE))
                background_surface.blit(img, rect)
    screen.blit(background_surface, (0,0))
    screen.blit(cursor.image, cursor.rect)
    for button in map_buttons.list:
        screen.blit(button.image, button.rect)
    pygame.display.update()
    clock.tick(FPS)