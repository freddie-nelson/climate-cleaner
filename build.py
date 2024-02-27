import os
import shutil

exit_code = os.system('meson compile -C build')
if exit_code != 0:
    exit(exit_code)

if os.path.exists('build/assets'):
    shutil.rmtree('build/assets')

shutil.copytree('assets', 'build/assets')

shutil.copytree('externals/lib', 'build', dirs_exist_ok=True)

if os.path.exists('build/main.exe'):
    os.system('cd build && main.exe')