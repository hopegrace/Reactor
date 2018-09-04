cc_library(
	name = 'reactor', 
	srcs = glob([
	    'base/*.cc',
	    'base/strings/*.cc',
		'net/*.cc',
    ]),
    hdrs = glob([
	    'base/*.h',
	    'base/strings/*.h',
		'net/*.h',
    ]),
    deps = [
        '@com_google_glog//:glog',
    ],
    visibility = [
        #'//test:__pkg__',
        '//examples:__pkg__',
    ]
)

# vim: set syntax=python:
