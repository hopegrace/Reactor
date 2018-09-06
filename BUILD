cc_library(
    name = 'reactor', 
    srcs = glob([
        'src/reactor/base/*.cc',
        'src/reactor/base/strings/*.cc',
        'src/reactor/net/*.cc',
    ]),
    hdrs = glob([
        'src/reactor/base/*.h',
        'src/reactor/base/strings/*.h',
        'src/reactor/net/*.h',
    ]),
    deps = [
        '@com_google_glog//:glog',
    ],
    visibility = [
        #'//test:__pkg__',
        '//examples:__pkg__',
    ],
	copts = ["-Isrc"],
)

# vim: set syntax=python:
