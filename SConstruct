#!/usr/bin/env python
import os.path
from glob import glob

print "DO NOT USE SCONS!! try 'make'"
Exit()

prog_target = "rcbcview"
loadcheck_target = "loadcheck"
lib_target = "rcbc"
lib_sources = glob("*.c")
#lib_sources.remove('rcbcview.c')
lib_sources.remove('loadcheck.c')
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
debug_flag = ARGUMENTS.get('debug', 0)

extra_objects=[]

if int(debug_flag):
	env.Append(CCFLAGS = ['-g'])
	env.MergeFlags('-D_DEBUG')

# Setup libs
if int(win32):
	env.Tool('crossmingw', toolpath = ['scons-tools'])
	env.AppendUnique (LIBS=['glut32', 'opengl32', 'glu32', 'm', 'IL', 'mxml', 'png12', 'jpeg'])
	extra_objects.append('./glut32.lib')
	extra_objects.append('/usr/'+env['MINGWPREFIX']+'/lib/DevIL.lib')
else:
	env.AppendUnique (LIBS=['GL', 'GLU', 'glut'])

env.AppendUnique (LIBS=['m', 'IL', 'mxml'])

# Building
env.Tool('colourful', toolpath=['scons-tools'])
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
#loadchecktarget = env.Program(target = loadcheck_target, source=objects)

#if not int(win32):
	
# Installing
bin_dir = '$PREFIX/bin'
lib_dir = '$PREFIX/lib'
inc_dir = '$PREFIX/include'

if not int(win32):
	installed_bin = env.Install(bin_dir, target)
else:
	installed_bin = ""
	
installed_lib = env.Install(lib_dir, [shared_lib, static_lib])
installed_inc = env.Install(inc_dir, ['rcbc.h', 'rcbc_data.h', 'rcbc_data_hookup.h', 'List.h', 'ooc.h'])

ib = env.Alias('install-bin', bin_dir)
il = env.Alias('install-lib', lib_dir)
ii = env.Alias('install-inc', inc_dir)
ina = env.Alias('install', [ib, il, ii])

# Uninstall
uninstall_files = env.Command('uninstall-files-com', [], [ Delete(installed_bin), Delete(installed_lib), Delete(installed_inc)])
env.Alias('uninstall', uninstall_files)

Default(target)
#Default(loadchecktarget)
Default(shared_lib)
Default(static_lib)
