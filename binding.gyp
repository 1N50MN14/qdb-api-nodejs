{
  "conditions": [ [ "OS=='freebsd'", {"make_global_settings": [ ["CC", "/usr/bin/clang"], ["CXX", "/usr/bin/clang++"] ]} ] ],

  "targets": [
    {
      "target_name": "qdb",
      "sources": [ 
        "src/qdb_api.cpp", 
        "src/entity.hpp", 
        "src/expirable_entity.hpp", 
        "src/blob.cpp", 
        "src/blob.hpp",
        "src/cluster.cpp", 
        "src/cluster.hpp", 
        "src/hset.cpp", 
        "src/hset.hpp", 
        "src/integer.cpp", 
        "src/integer.hpp",
        "src/queue.cpp", 
        "src/queue.hpp", 
        "src/tag.cpp",
        "src/tag.hpp",
        "src/utilities.hpp" ],
      "conditions": [ 
            [ "OS=='win'", { "copies": [ { 
                                            "destination" : "<(module_root_dir)/build/Release/",
                                            "files": [ "<(module_root_dir)/deps/qdb/bin/qdb_api.dll" ] 
                                        } ],
                              "include_dirs": [ "<(module_root_dir)/deps/qdb/include" ], 
                              "msvs_settings": { "VCCLCompilerTool": { "ExceptionHandling": "2", "DisableSpecificWarnings": [ "4530" ] } },
                              "link_settings": { "libraries": [ "<(module_root_dir)/deps/qdb/lib/qdb_api.lib" ] } }, 
                            { "include_dirs": [ "/usr/local/include", "<(module_root_dir)/deps/qdb/include" ], 
                              "libraries": [ "-L/usr/local/lib", "-L<(module_root_dir)/deps/qdb/lib", "-lqdb_api"],
                               "cflags": [ "-std=c++11", "-Wno-strict-aliasing" ] } ]
        ]
    }
  ]
}
