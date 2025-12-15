dir = include_directories('.')
cc  = meson.get_compiler('c')

dep = [
    cc.find_library('m', required : false),
    dependency('threads', required : false)
]

fossil_algorithm_lib = library('fossil_algo',
    files(
        'sort.c',
        'search.c',
        'cstring.c',
        'shuffle.c',
        'graph.c',
        'dynamic.c'
        ),
    install: true,
    dependencies: dep,
    include_directories: dir
)

fossil_algorithm_dep = declare_dependency(
    link_with: [fossil_algorithm_lib],
    dependencies: dep,
    include_directories: dir)

meson.override_dependency('fossil-algo', fossil_algorithm_dep)