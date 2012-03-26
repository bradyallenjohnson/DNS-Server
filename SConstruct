import os

env = Environment()
variantDir = os.path.join('buildScons', 'linux')

libDirs = ['dnsUtils',
           'dnsLoader',
           'dnsResolver',
           'socketServer']

libs = []
libPath = []
includePath = ['#.']
modules = ['main']

for lib in libDirs:
    modules.append(lib)
    libs.append(lib)
    includePath.append('#%s' % lib)
    libPath.append('#%s' % os.path.join(variantDir, lib))

env.Append(CPPPATH = includePath, LIBPATH = libPath, LIBS = libs)

for dir in modules:
    SConscript(
        os.path.join(dir, 'SConscript'),
        variant_dir = os.path.join(variantDir, dir),
        exports = ['env'],
        duplicate = 0)
