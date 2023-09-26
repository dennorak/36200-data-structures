import csv
import pygame

# constants
scale = 100
color_queen = (255, 100, 100)
color_taken = (100, 50, 50)

# read csv data
queens = []
with open('./bin/data.csv', 'r') as f:
    reader = csv.reader(f)
    cur_row = 0
    for row in reader:
        if cur_row == 0:
            N = int(row[0])
        else:
            queens.append([int(row[0]), int(row[1])])
        cur_row += 1


# setup pygame
pygame.init()
Background = pygame.display.set_mode((scale*N ,scale*N))
Background.fill(color_taken)

# draw ueens
for queen in queens:
    pygame.draw.rect(Background, color_queen, 
        (queen[0]*scale, queen[1]*scale, scale, scale))

# draw grid
for i in range(scale, scale*N, scale):
    pygame.draw.line(Background, (255, 255, 255), (0, i), (scale*N, i))
    pygame.draw.line(Background, (255, 255, 255), (i, 0), (i, scale*N))

# show + wait for quit
pygame.display.update()
while pygame.event.wait().type != pygame.QUIT:
    pass