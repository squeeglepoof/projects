import os, sys
import pygame
from pygame.locals import *
import numpy as np
#from numpy import genfromtxt
import random
import time

if not pygame.font: print 'Warning, fonts disabled'
if not pygame.mixer: print 'Warning, sound disabled'

def rc():
	return int(random.random() * 255)

def green_red_grad(level, cap):
	if level > cap:
		level = cap
	
	halfway = (cap - 1) / 2.0
	if level == halfway:
		return (255, 255, 0) # yellow

	if (level == cap):
		return (128, 0, 0) # dark red means over capacity

	bill = (level - halfway) / halfway
	if bill < 0:
		return (int(255*(1-abs(bill))), 255, 0)
	else:
		return (255, int(255*(1-abs(bill))), 0)

class Viz:
	
	def __init__(self):
		"""Initialize PyGame"""
		pygame.init()
		self.load_data()
		self.screen = pygame.display.set_mode((WIDTH + PADDING*2, HEIGHT + PADDING*2 + HEADER)) 


	def load_data(self):
		self.sectors = np.genfromtxt(NODES_PATH, delimiter=',')
		w = int(np.ceil(np.max(self.sectors[:,0])))
		h = int(np.ceil(np.max(self.sectors[:,1])))

		self.scale_x = w / float(WIDTH)
		self.scale_y = h / float(HEIGHT)

		self.links = np.genfromtxt(EDGES_PATH, delimiter=',')

		self.link_UAVs_tr = np.genfromtxt(LINK_UAVS_PATH + 'trained.csv', delimiter=',')
		self.link_UAVs_untr = np.genfromtxt(LINK_UAVS_PATH + 'untrained.csv', delimiter=',')
		self.sector_UAVs_tr = np.genfromtxt(SECTOR_UAVS_PATH + 'trained.csv', delimiter=',')
		self.sector_UAVs_untr = np.genfromtxt(SECTOR_UAVS_PATH + 'untrained.csv', delimiter=',')

	def draw_sectors(self, t, trained):
		for i in range(self.sectors.shape[0]):
			color = None
			if trained:
				color = green_red_grad(self.sector_UAVs_tr[t, i], CAPACITY)
			else:
				color = green_red_grad(self.sector_UAVs_untr[t, i], CAPACITY)
			x = self.sectors[i,0] / self.scale_x
			y = self.sectors[i,1] / self.scale_y
			pos = [int(x) + PADDING, int(y) + PADDING + HEADER]
			pygame.draw.circle(self.screen, color, pos, SECTOR_RADIUS)

	def draw_links(self, t, trained):
		for i in range(self.links.shape[0]):	
			color = None
			if (trained):
				color = green_red_grad(self.link_UAVs_tr[t, i], CAPACITY)
			else:
				color = green_red_grad(self.link_UAVs_untr[t, i], CAPACITY)
			pt1 = self.sectors[self.links[i,0]]
			pt2 = ((self.sectors[self.links[i,1]] - pt1) / 2) + pt1
			pt1 = [int(pt1[0] / self.scale_x) + PADDING, int(pt1[1] / self.scale_y) + PADDING + HEADER]
			pt2 = [int(pt2[0] / self.scale_x) + PADDING, int(pt2[1] / self.scale_y) + PADDING + HEADER]
			pygame.draw.line(self.screen, color, pt1, pt2, LINK_WIDTH)

	def loop(self):
		if not os.path.isdir('frames'):
			os.path.mkdir('frames')
		tr = [False, True]
		for trained in tr:
			strTr = 'Trained' if trained else 'Untrained'
			for t in range(200):
				strStep = str(t)
				for event in pygame.event.get():
					if event.type == pygame.QUIT: 
						sys.exit()
				self.screen.fill((0,0,0))

				myFont = pygame.font.SysFont('monospace', 30)
				text = myFont.render(strTr + ', ' + strStep, True, (255,255,255))
				self.screen.blit(text, (self.screen.get_width()//2 - text.get_width()//2, 0))
					
				self.draw_links(t, trained)
				self.draw_sectors(t, trained)
				pygame.display.flip()
				# save frames to disk
				pygame.image.save(self.screen, 'frames/' + strTr + str(t) + '.png')
				time.sleep(0.05)
						  
if __name__ == "__main__":
	if len(sys.argv) < 2:
		print "Usage: viz.py /dir/to/data"
		print "Current folder is: " + os.path.dirname(os.path.realpath(__file__))
		sys.exit()
	
	ROOT_PATH = sys.argv[1]
	print ROOT_PATH
	EDGES_PATH = ROOT_PATH + "edges.csv"
	NODES_PATH = ROOT_PATH + "nodes.csv"
	LINK_UAVS_PATH = ROOT_PATH + "link_UAVs_"
	SECTOR_UAVS_PATH = ROOT_PATH + "sector_UAVs_"

	SECTOR_RADIUS = 6
	LINK_WIDTH = 3
	PADDING = 10
	HEADER = 10

	GREEN = (0, 255, 0)
	CAPACITY = 11

	WIDTH = 500
	HEIGHT = 500

	viz = Viz()
	viz.loop()
