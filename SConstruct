from glob import glob

prog_target = "rcbcview"
lib_target = "rcbc"
lib_sources = glob("*.c")

env = Environment()

# Add glut libs
env.AppendUnique (LIBS=['GL', 'GLU', 'glut', 'm', 'IL', 'mxml'])

objects = env.Object(lib_sources)

env.SharedLibrary(target = lib_target, source = lib_sources)
env.StaticLibrary(target = lib_target, source = lib_sources)
env.Program(target = prog_target, source=objects)
