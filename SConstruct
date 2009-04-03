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
		  help='installation prefix')

env = Environment(PREFIX = GetOption('prefix'))

# Add glut libs
env.AppendUnique (LIBS=['GL', 'GLU', 'glut', 'm', 'IL', 'mxml'])

objects = env.Object(lib_sources)

shared_lib = env.SharedLibrary(target = lib_target, source = lib_sources)
static_lib = env.StaticLibrary(target = lib_target, source = lib_sources)
target = env.Program(target = prog_target, source=objects)

env.Install('/usr/lib', [shared_lib, static_lib])
env.Install('/usr/bin', target)

ib = env.Alias('install-bin', '/usr/bin')
il = env.Alias('install-lib', '/usr/lib')
env.Alias('install', [ib, il])
