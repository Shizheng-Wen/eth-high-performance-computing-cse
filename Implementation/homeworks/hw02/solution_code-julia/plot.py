#!/usr/bin/env python3
# Auxiliary code for HPCSE I (2018HS) Exam, 11.01.2019
# Prof. P. Koumoutsakos
# Coding 1: Julia Set

import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from PIL import Image

import os

def render_image():
    # Load the textual image.
    try:
        image = np.loadtxt('image.txt', dtype=np.float)
    except FileNotFoundError:
        print("File `image.txt` not found, not plotting the Julia set.")
        return

    # Add a palette.
    image += 1 # offset from zero
    limage = np.log10(image)
    min_value = limage.min()
    max_value = limage.max()
    image = 1.0 - np.maximum(0.0, np.minimum((limage-min_value)/(max_value-min_value), 1.0)) # invert

    # Mirror the lower half and concatenate
    image = np.concatenate((image[::-1, ::-1], image))

    # Store into a file.
    min_value = np.min(np.min(image))
    max_value = np.max(np.max(image))
    image = (image - min_value) / (max_value - min_value)
    image = plt.cm.cubehelix(image)
    image = (255 * image).astype('uint8')
    im = Image.fromarray(image)
    im.save('image.png')
    print("Julia set plot saved to `image.png`.")


def render_histogram():
    # Using a simple bar-plot would completely hide the column with iterator
    # count == MAX_ITERATIONS, so we split the x-axis in two parts.
    # https://stackoverflow.com/questions/32185411/break-in-x-axis-of-matplotlib
    try:
        histogram = np.loadtxt('histogram.txt')
    except FileNotFoundError:
        print("File `histogram.txt` not found, not plotting the histogram.")
        return
    if not histogram.any():
        print("File `histogram.txt` contains only zeroes, not plotting the histogram.")
        return
    f, (ax1, ax2) = plt.subplots(1, 2, sharey=True, facecolor='w')
    ax1.set_yscale('log')
    ax2.set_yscale('log')
    ax1.set_ylabel('Pixel count')
    ax1.bar(range(len(histogram)), histogram, width=1.0)
    ax2.bar(range(len(histogram)), histogram, width=1.0)
    ax1.set_xlim(0, 105)
    ax2.set_xlim(900, 1005)
    ax1.spines['right'].set_visible(False)
    ax2.spines['left'].set_visible(False)
    ax1.yaxis.tick_left()
    ax1.tick_params(labelright=False)
    ax2.yaxis.tick_right()

    d = .015 # how big to make the diagonal lines in axes coordinates
    # arguments to pass plot, just so we don't keep repeating them
    kwargs = dict(transform=ax1.transAxes, color='k', clip_on=False)
    ax1.plot((1-d,1+d), (-d,+d), **kwargs)
    ax1.plot((1-d,1+d),(1-d,1+d), **kwargs)

    kwargs.update(transform=ax2.transAxes)  # switch to the bottom axes
    ax2.plot((-d,+d), (1-d,1+d), **kwargs)
    ax2.plot((-d,+d), (-d,+d), **kwargs)

    ax1.grid(True, linestyle='--', linewidth=0.5)
    ax2.grid(True, linestyle='--', linewidth=0.5)
    plt.savefig('histogram.png')
    plt.close()
    print("Histogram plot saved to `histogram.png`.")


render_image()
render_histogram()
