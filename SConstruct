
include_paths = [
                 Dir('deps/http-parser'),
                 Dir('src'),
                ]

sources = []
for path in include_paths:
   sources += Glob(str(path) + '/*.cc')

bolt_env = Environment(LIBS=['ev'],
                       CPPPATH=include_paths)

debug = ARGUMENTS.get('debug', 0)
if int(debug):
    bolt_env.Append(CCFLAGS='-DDEBUG -ggdb')

bolt_env.Alias('bolt', 'build/bolt')
http_parser_lib = bolt_env.Library(Glob('deps/http-parser/*.c'))
bolt_env.Program('build/bolt', sources + http_parser_lib)

