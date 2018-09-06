cc_library(
    name = 'reactor', 
    srcs = glob([
        'src/reactor/base/*.cc',
        'src/reactor/base/strings/*.cc',
        'src/reactor/net/*.cc',
        'src/reactor/net/http/*.cc',
    ]),
    hdrs = glob([
        'src/reactor/base/*.h',
        'src/reactor/base/strings/*.h',
        'src/reactor/net/*.h',
        'src/reactor/net/http/*.h',
    ]),
    deps = [
        '@com_google_glog//:glog',
    ],
    visibility = [
		"//visibility:public",
    ],
    strip_include_prefix='src/',
    includes=['src'],
)

cc_binary(
    name = 'webserver',
    srcs = ['examples/WebServer.cc'],
    deps = [':reactor'],
    includes=['src'],
)

cc_binary(
    name = 'mini_proxy',
    srcs = ['examples/MiniProxy.cc'],
    deps = [':reactor'],
    includes=['src'],
)

