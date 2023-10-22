#!/usr/bin/python3

import argparse
import os, sys
from io import open as iopen
import imageio
from PIL import Image
from pilkit.processors import ProcessorPipeline, ResizeToFit, SmartResize



# Function that downloads images
def updateFavicon(originalImage, directory):
    index = 0

    sizes = [
    #
    #    FileName        LogoSize        BoxSize
    #
        ["android-chrome-192x192",               [192,192],       [192,192]],
        ["android-chrome-256x256",               [256,256],       [256,256]],
        ["android-chrome-512x512",               [512,512],       [512,512]],
    #    ["android-chrome-maskable-192x192",      [192,192],       [192,192]],
    #    ["android-chrome-maskable-512x512",      [512,512],       [512,512]],
    #    ["apple-touch-icon-60x60",               [60,60],         [60,60]],
    #    ["apple-touch-icon-76x76",               [76,76],         [76,76]],
    #    ["apple-touch-icon-120x120",             [120,120],       [120,120]],
    #    ["apple-touch-icon-152x152",             [152,152],       [152,152]],
    #    ["apple-touch-icon-180x180",             [180,180],       [180,180]],
        ["apple-touch-icon",                     [180,180],       [180,180]],
        ["favicon-16x16",                        [16,16],         [16,16]],
        ["favicon-32x32",                        [32,32],         [32,32]],
        ["msapplication-icon-144x144",           [144,144],       [144,144]],
        ["mstile-150x150",                       [150,150],       [150,150]],
    ]

    outfile = os.path.splitext(originalImage)[0] + ".png"

    for size in sizes:
        im = Image.open(originalImage)
        processor = ProcessorPipeline([ResizeToFit(size[1][0], size[1][1])])
        result = processor.process(im)
        background = Image.new('RGBA', size[2], (255, 255, 255, 0))
        background.paste(
            result, (int((size[2][0] - result.size[0]) / 2), int((size[2][1] - result.size[1]) / 2))
        )
        background = background.convert("P", palette=Image.ADAPTIVE, colors=256)
        background.save(os.path.join(directory ,size[0] + ".png"), optimize=True)

    img = imageio.v2.imread(originalImage)
    processor = ProcessorPipeline([ResizeToFit(48, 48)])
    result = processor.process(im)
    img = Image.new('RGBA', [48,48], (255, 255, 255, 0))
    img.paste(
        result, (int((48 - result.size[0]) / 2), int((48 - result.size[1]) / 2))
    )
    parentDir = os.path.abspath(os.path.join(directory, os.pardir))
    imageio.imwrite(os.path.join(parentDir ,'favicon.ico'), img)


def main(originalImage, directoryName):
    # Creates the directory
    if not os.path.exists(directoryName):
        os.makedirs(directoryName)

    # Start the favicon generator
    updateFavicon(originalImage, directoryName)



if __name__ == "__main__":
    projectPath = os.path.dirname(os.path.realpath(__file__))
    if os.path.basename(projectPath) == "utils":
        projectPath = os.path.join(projectPath,os.pardir)

    defaultOutputPath = os.path.join(projectPath,"html","icons")
    defaultLogoImage = os.path.join(projectPath,"logo","icon.png")
    main(defaultLogoImage,defaultOutputPath)

