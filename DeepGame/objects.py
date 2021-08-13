import animation
import pygame
import math
from config import *

def sgn(x):
    if x > 0 : return 1
    elif x < 0: return -1
    else: return 0
def abs(x):
    return x if x >= 0 else -x
class Player(animation.Animation):
    def __init__(self) -> None:
        super().__init__(PLAYER_SPRITES_DIR + 'run_f', 4, 0, 0)
        self.v_x = 0
        self.v_y = 0
        self.dead = False
    def set_pos(self, pos_x, pos_y):
        self.rect.x = pos_x * SIZE_SPRITE
        self.rect.y = pos_y * SIZE_SPRITE
    def update(self):
        if(self.v_x != 0 or self.v_y != 0):
            self.update_anim(self.v_x < 0)
        self.rect.x += self.v_x
        self.rect.y += self.v_y
        if self.rect.x < 0 : self.rect.x = 0
        if self.rect.y < 0 : self.rect.y = 0
        if self.rect.right > WIDTH * SIZE_SPRITE : self.rect.right = WIDTH * SIZE_SPRITE
        if self.rect.bottom > HEIGHT * SIZE_SPRITE : self.rect.bottom = HEIGHT * SIZE_SPRITE

    def collision(self, walls):
        for wall in walls:
            if(self.rect.colliderect(wall)):
                return True
        return False
    def step_back(self):
        self.rect.x -= self.v_x
        self.rect.y -= self.v_y
    def set_v_x(self, v_x):
        self.v_x = v_x
    def set_v_y(self, v_y):
        self.v_y = v_y
    def die(self):
        self.dead = True
    def is_dead(self):
        return self.dead

class Spike(animation.Animation):
    def __init__(self,pos_x, pos_y, decal = 0) -> None:
        super().__init__(SPRITES_DIR + 'floor_spikes_anim_f', 4, pos_x, pos_y, 0.02, decal)
        self.deadly = False
    def update(self):
        self.update_anim()
        if self.animation_index >= 2 : self.deadly = True
        else: self.deadly = False
    def is_deadly(self):
        return self.deadly

class Wall(pygame.sprite.Sprite):
    def __init__(self, name, pos_x, pos_y) -> None:
        super().__init__()
        self.image = pygame.image.load(SPRITES_DIR + name + '.png').convert_alpha()
        self.rect = self.image.get_rect()
        self.rect.x = pos_x * SIZE_SPRITE
        self.rect.y = pos_y * SIZE_SPRITE

class Sword(pygame.sprite.Sprite):
    def __init__(self, pos_x1, pos_y1, pos_x2, pos_y2, speed = SWORD_SPEED) -> None:
        super().__init__()
        self.image = pygame.image.load(SPRITES_DIR + 'weapon_anime_sword' + '.png').convert_alpha()
        self.x1 = pos_x1 * SIZE_SPRITE
        self.y1 = pos_y1 * SIZE_SPRITE
        self.x2 = pos_x2 * SIZE_SPRITE
        self.y2 = pos_y2 * SIZE_SPRITE

        self.speed = speed

        w = abs(self.x1 - self.x2)
        h = abs(self.y1 - self.y2)
        pb = (self.x1 if self.x1 <= self.x2 else self.x2, self.y1 if self.y1 <= self.y2 else self.y2)
        self.traj = pygame.rect.Rect(pb, (w, h))
        
        if(self.x1 == self.x2):
            self.v_x = 0
            self.v_y = self.speed
            self.angle = 0
        elif(self.y1 == self.y2):
            self.v_x = self.speed
            self.v_y = 0
            self.angle = 90
        else:
            self.angle = math.atan2(self.x2 - self.x1, self.y2 - self.y1) * 180 / math.pi # degre
            self.v_x = math.sin(self.angle * math.pi / 180) * self.speed
            self.v_y = math.cos(self.angle* math.pi / 180) * self.speed

        self.image = pygame.transform.rotate(self.image, self.angle - 180)
        self.rect = self.image.get_rect()
        self.rect.center = (self.x1 + sgn(self.v_x), self.y1 + sgn(self.v_y))

        self.fx, self.fy = self.rect.center
    def update(self):
        self.fx += self.v_x
        self.fy += self.v_y
        self.rect.center = (self.fx, self.fy)
        if(self.x1 == self.x2): #ligne verticale
            if(not self.y1 <= self.rect.centery <= self.y2):
                self.v_x = -self.v_x
                self.v_y = -self.v_y
                self.image = pygame.transform.rotate(self.image, 180)
        elif(self.y1 == self.y2): #ligne horizontale
            if(not self.x1 <= self.rect.centerx <= self.x2):
                self.v_x = -self.v_x
                self.v_y = -self.v_y
                self.image = pygame.transform.rotate(self.image, 180)
        elif not self.traj.collidepoint(self.rect.center):
            self.v_x = -self.v_x
            self.v_y = -self.v_y
            self.image = pygame.transform.rotate(self.image, 180)

class Coin(animation.Animation):
    def __init__(self, pos_x, pos_y) -> None:
        super().__init__(SPRITES_DIR + 'coin_anim_f', 4, pos_x + 1/4, pos_y + 1/4)
        self.taken = False
    def update(self):
        self.update_anim()
        if(self.taken):
            self.image.set_alpha(0)
    def is_taken(self):
        return self.taken
    def take(self):
        self.taken = True

class Floor(pygame.sprite.Sprite):
    def __init__(self, pos_x, pos_y) -> None:
        super().__init__()
        self.image = pygame.image.load(SPRITES_DIR + 'floor_' + '1' + '.png').convert_alpha()
        self.image.set_alpha(ALPHA_FLOOR)
        self.rect = self.image.get_rect(topleft = (pos_x * SIZE_SPRITE, pos_y * SIZE_SPRITE))
        

class Door(pygame.sprite.Sprite):
    def __init__(self, pos_x = 0, pos_y = 0) -> None:
        super().__init__()
        self.img_closed = pygame.image.load(SPRITES_DIR + 'hole' + '.png').convert_alpha()
        self.img_open = pygame.image.load(SPRITES_DIR + 'floor_ladder' + '.png').convert_alpha()
        self.image = self.img_closed
        self.rect = self.image.get_rect(topleft = (pos_x * SIZE_SPRITE, pos_y * SIZE_SPRITE))
        self.opened = False
    def set_pos(self, pos_x, pos_y):
        self.rect.x = pos_x * SIZE_SPRITE
        self.rect.y = pos_y * SIZE_SPRITE
    def update(self) -> None:
        if(self.opened):
            self.image = self.img_open
        else:
            self.image = self.img_closed
    def open(self):
        self.opened = True
    def close(self):
        self.opened = False

##Object for map generator

class MapButton(pygame.sprite.Sprite):
    def __init__(self, name, pos_x, pos_y) -> None:
        super().__init__()
        self.name = name
        self.image = pygame.image.load(SPRITES_DIR + self.name + '.png').convert_alpha()
        self.rect = self.image.get_rect(topleft = (pos_x * SIZE_SPRITE, pos_y * SIZE_SPRITE))
        if(self.name == 'floor_1'):
            self.image.set_alpha(ALPHA_FLOOR)

class MapButtons():
    def __init__(self, menu_rect) -> None:
        self.list = [MapButton('wall_mid', 1, menu_rect.top/SIZE_SPRITE + 1)]
        self.x = 1
        self.y = 1
        self.menu_rect = menu_rect
    def add(self, name):
        if self.x < WIDTH - 1 :
            self.x += 2
        else:
            self.x = 1
            self.y += 2
        self.list.append(MapButton(name, self.x, self.menu_rect.top/SIZE_SPRITE + self.y))
    def get_Button(self, name):
        for button in self.list:
            if button.name == name:
                return button