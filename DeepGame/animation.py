import pygame
from config import *

class Animation(pygame.sprite.Sprite):
    def __init__(self, name, nb, pos_x, pos_y, anim_speed = ANIM_SPEED, decal = 0) -> None:
        super().__init__()

        self.anim = []
        self.anim_speed = anim_speed

        for i in range(nb):
            self.anim.append(pygame.image.load(name + str(i) + '.png').convert_alpha())

        if decal <= len(self.anim):
            self.animation_index = decal
        else:
            self.animation_index = len(self.anim) - 1

        self.image = self.anim[int(self.animation_index)]
        self.rect = self.image.get_rect(topleft = (pos_x * SIZE_SPRITE, pos_y * SIZE_SPRITE))

    def update_anim(self, flip = False):
        self.animation_index += self.anim_speed
        if(self.animation_index >= len(self.anim)): self.animation_index = 0
        if(not flip):
            self.image = self.anim[int(self.animation_index)]
        else:
            self.image = pygame.transform.flip(self.anim[int(self.animation_index)], True, False)
        