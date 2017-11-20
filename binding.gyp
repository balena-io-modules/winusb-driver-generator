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
              "action": [ "call", "deps/libwdi_build.bat" ],
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
          "library_dirs": [
            "<(module_root_dir)\\deps\\libwdi\\libwdi",
          ],
          "sources": [
            "src/generator.cpp",
          ],
          "libraries": [
            "-llibwdi",
          ],
        } ],
      ],
    },
  ],
}
