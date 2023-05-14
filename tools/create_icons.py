#!/usr/bin/python

# Requires Python v3 Interpreter with pillow module installed

import itertools
import PIL.Image
import os

# Also create the application icons (in data/icons)
if not os.path.exists('data/icons'): os.makedirs('data/icons')
base = PIL.Image.open('data/icon-32x32.png').convert('RGBA')

for size in [16, 24, 32, 48, 64, 128, 256, 512]:
	img = base.resize((size, size), PIL.Image.Resampling.NEAREST)
	img.save('data/icons/icon-%d.png' % size)

img.close()
img = PIL.Image.open('data/icon-32x32.png').convert('RGBA')
img.save('data/icon.ico')
img.close()
