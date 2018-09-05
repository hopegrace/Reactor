cc_library(
    name = 'reactor', 
    srcs = glob([
        'reactor/base/*.cc',
        'reactor/base/strings/*.cc',
        'reactor/net/*.cc',
    ]),
    hdrs = glob([
        'reactor/base/*.h',
        'reactor/base/strings/*.h',
        'reactor/net/*.h',
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
