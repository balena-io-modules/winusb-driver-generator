{
  "targets": [
    {
      "target_name": "libwdi_configuration",
      "type": "none",
      "conditions": [
        [ "OS=='win'", {
          "actions": [
            {
              "action_name": "detect_arch",
              "message": "Detecting build architecture",
              "inputs": [],
              "outputs": [ "<(module_root_dir)/deps/libwdi/build64.h" ],
              "action": [ "call", "deps/detect-arch.bat", "<(target_arch)" ]
            },
            {
              "action_name": "configure",
              "message": "Configuring libwdi",
              "inputs": [],
              "outputs": [ "<(module_root_dir)/deps/libwdi/msvc/config.h" ],
              "action": [ "call", "deps/config.bat" ]
            }
          ]
        }]
      ]
    },
    {
      "target_name": "libwdi",
      "conditions": [
        [ "OS=='win'", {
          "type": "static_library",
          "dependencies": [
            "libwdi_configuration",
            "installer_x86",
            "installer_x64",
            "embedder"
          ],
          "actions": [
            {
              "action_name": "embed",
              "message": "Embedding binary resources",
              "inputs": [],
              "outputs": [ "<(module_root_dir)/deps/libwdi/libwdi/embedded.h" ],
              "action": [ "call", "deps/embed.bat" ]
            }
          ],
          "conditions": [
            [ "target_arch=='x64'", {
              "defines": [
                "_CRT_SECURE_NO_WARNINGS",
                "_WIN32",
                "_WIN64",
                "_LIB"
              ]
            }, {
              "defines": [
                "_CRT_SECURE_NO_WARNINGS",
                "_WIN32",
                "_LIB"
              ]
            }]
          ],
          "include_dirs": [
            "<(module_root_dir)/deps/libwdi/libwdi",
            "<(module_root_dir)/deps/libwdi/msvc"
          ],
          "sources": [
            "<(module_root_dir)/deps/libwdi/libwdi/libwdi.c",
            "<(module_root_dir)/deps/libwdi/libwdi/libwdi_dlg.c",
            "<(module_root_dir)/deps/libwdi/libwdi/logging.c",
            "<(module_root_dir)/deps/libwdi/libwdi/pki.c",
            "<(module_root_dir)/deps/libwdi/libwdi/tokenizer.c",
            "<(module_root_dir)/deps/libwdi/libwdi/vid_data.c"
          ],
          "configurations": {
            "Release": {
              "msvs_settings": {
                "VCCLCompilerTool": {
                  "WarningLevel": 3,
                  "ExceptionHandling": 1
                }
              }
            }
          }
        }]
      ]
    },
    {
      "target_name": "embedder",
      "conditions": [
        [ "OS=='win'", {
          "target_arch": "ia32",
          "type": "executable",
          "win_delay_load_hook": "false",
          "dependencies": [
            "libwdi_configuration",
            "installer_x86",
            "installer_x64"
          ],
          "include_dirs": [
            "<(module_root_dir)/deps/libwdi/libwdi",
            "<(module_root_dir)/deps/libwdi/msvc"
          ],
          "defines": [
            "_CRT_SECURE_NO_WARNINGS"
          ],
          "sources": [
            "<(module_root_dir)/deps/libwdi/libwdi/embedder.c"
          ]
        }]
      ]
    },
    {
      "target_name": "installer_x64",
      "conditions": [
        [ "OS=='win'", {
          "type": "executable",
          "win_delay_load_hook": "false",
          "dependencies": [
            "libwdi_configuration"
          ],
          "include_dirs": [
            "<(module_root_dir)/deps/libwdi/libwdi",
            "<(module_root_dir)/deps/libwdi/msvc"
          ],
          "defines": [
            "_CRT_SECURE_NO_WARNINGS",
            "_WIN64"
          ],
          "sources": [
            "<(module_root_dir)/deps/libwdi/libwdi/installer.c"
          ],
          "link_settings": {
            "libraries": [
              "-lnewdev.lib",
              "-lsetupapi.lib"
            ]
          },
          "configurations": {
            "Release": {
              "msvs_settings": {
                "VCCLCompilerTool": {
                  "WarningLevel": 3,
                  "ExceptionHandling": 1
                }
              }
            }
          }
        }]
      ]
    },
    {
      "target_name": "installer_x86",
      "conditions": [
        [ "OS=='win'", {
          "target_arch": "ia32",
          "type": "executable",
          "win_delay_load_hook": "false",
          "dependencies": [
            "libwdi_configuration"
          ],
          "include_dirs": [
            "<(module_root_dir)/deps/libwdi/libwdi",
            "<(module_root_dir)/deps/libwdi/msvc"
          ],
          "defines": [
            "_CRT_SECURE_NO_WARNINGS"
          ],
          "sources": [
            "<(module_root_dir)/deps/libwdi/libwdi/installer.c"
          ],
          "link_settings": {
            "libraries": [
              "-lnewdev.lib",
              "-lsetupapi.lib"
            ]
          },
          "configurations": {
            "Release": {
              "msvs_settings": {
                "VCCLCompilerTool": {
                  "WarningLevel": 3,
                  "ExceptionHandling": 1
                }
              }
            }
          }
        }]
      ]
    },
    {
      "target_name": "Generator",
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "."
      ],
      "conditions": [
        [ "OS=='win'", {
          "dependencies" : [
            "libwdi"
          ],
          "include_dirs": [
            "<(module_root_dir)/deps/libwdi/libwdi"
          ],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "WarningLevel": 3,
              "AdditionalOptions": [ "/GL-", "/MD" ]
            },
          },
          "link_settings": {
            "library_dirs+": [
              "<(module_root_dir)/build/Release"
            ],
            "libraries": [
              "-lsetupapi.lib",
              "-llibwdi.lib"
            ]
          },
          "sources": [
            "src/generator.cpp",
          ],
          "configurations": {
            "Release": {
              "msvs_settings": {
                "VCCLCompilerTool": {
                  "WarningLevel": 3,
                  "ExceptionHandling": 1
                }
              }
            }
          }
        }]
      ]
    }
  ]
}
