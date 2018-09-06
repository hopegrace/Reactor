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
		"//visibility:public",
    ],
	copts = ["-Isrc"],
)

# vim: set syntax=python:
