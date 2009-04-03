#!/usr/bin/env python
import os.path
from glob import glob

prog_target = "rcbcview"
lib_target = "rcbc"
lib_sources = glob("*.c")

env = Environment()

AddOption('--prefix',
		  dest='prefix',
		  type='string',
		  nargs=1,
		  action='store',
		  metavar='DIR',
		  help='installation prefix',
		  default='/usr/local')

env = Environment(PREFIX = GetOption('prefix'))
win32 = ARGUMENTS.get('win32', 0)

extra_objects=[]

# Setup libs
if int(win32):
	env.Tool('crossmingw', toolpath = ['scons-tools'])
	env.AppendUnique (LIBS=['glut32', 'opengl32', 'glu32', 'm', 'IL', 'mxml'])
	extra_objects.append('./glut32.lib')
	extra_objects.append('/usr/'+env['MINGWPREFIX']+'/lib/DevIL.lib')
else:
	env.AppendUnique (LIBS=['GL', 'GLU', 'glut'])

env.AppendUnique (LIBS=['m', 'IL', 'mxml'])

# Building
objects = env.Object(lib_sources)
lib_sources.append(extra_objects)
static_lib = env.StaticLibrary(target = lib_target, source = lib_sources)

if not int(win32): # Don't build a shared lib for windows
	shared_lib = env.SharedLibrary(target = lib_target, source = lib_sources)
else:
	shared_lib = ""

#shared_lib = env.SharedLibrary(target = lib_target, source = lib_sources)

objects.append(extra_objects)
target = env.Program(target = prog_target, source=objects)

#if not int(win32):
	
Default(target)
Default(shared_lib)
Default(static_lib)

# Installing
bin_dir = '$PREFIX/bin'
lib_dir = '$PREFIX/lib'

if not int(win32):
	installed_bin = env.Install(bin_dir, target)
else:
	installed_bin = ""
	
installed_lib = env.Install(lib_dir, [shared_lib, static_lib])

ib = env.Alias('install-bin', bin_dir)
il = env.Alias('install-lib', lib_dir)
ina = env.Alias('install', [ib, il])

# Uninstall
uninstall_files = env.Command('uninstall-files-com', [], [ Delete(installed_bin), Delete(installed_lib)])
env.Alias('uninstall', uninstall_files)

