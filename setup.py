import os
import shutil
import subprocess
import argparse

argparser = argparse.ArgumentParser(description='Build the game')
argparser.add_argument('--mode', default='debug', help='Build mode (debug, release)', choices=['debug', 'release'])
argparser.add_argument('--target', default='native', help='Build target (native, web)', choices=['native', 'wasm'])

args = argparser.parse_args()

mode = args.mode
target = args.target

shutil.rmtree('build', ignore_errors=True)

if target == 'wasm':
    os.system(f'meson setup build --buildtype {mode} --cross-file cross/{target}.txt')
else:
    os.system(f'meson setup build --buildtype {mode}')