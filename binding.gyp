{
  "targets": [
    {
      "target_name": "libwdi",
      "type": "none",
      "conditions": [
        [ 'OS=="win"', {
          "actions": [
            {
              "action_name": "wdk_build",
              "inputs": [ "" ],
              "outputs": [ "deps/libwdi/libwdi/libwdi.lib" ],
              "action": [ "call", "deps/libwdi_build.bat", "<(target_arch)" ],
            }
          ],
        } ],
      ],
    },
    {
      "target_name": "Generator",
      "dependencies" : [
        "libwdi",
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
      ],
      "conditions": [
        [ 'OS=="win"', {
          "include_dirs": [
            "<(module_root_dir)\\deps\\libwdi\\libwdi",
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [ '/GL-', '/MD' ],
            },
          },
          "link_settings": {
            "libraries": [
              "-llibwdi.lib",
              "-llibwdi.obj",
              "-lsetupapi.lib",
            ],
            "conditions": [
              [ 'target_arch=="x64"', {
                "library_dirs": [ "<(module_root_dir)\\deps\\libwdi\\libwdi\\objfre_win7_amd64\\amd64" ],
              }, {
                "library_dirs": [ "<(module_root_dir)\\deps\\libwdi\\libwdi\\objfre_win7_x86\\i386" ],
              } ]
            ],
          },
          "sources": [
            "src/generator.cpp",
          ],
        } ],
      ],
    },
  ],
}
