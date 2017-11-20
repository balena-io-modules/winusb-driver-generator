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
              "outputs": [ "deps/libwdi/libwdi/libwdi.dll" ],
              "action": [ "call", "deps/libwdi_build.bat" ]
            }
          ]
        } ]

      ],
    },
  ],
}
