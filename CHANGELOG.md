# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## 1.2.3 - 2019-01-11

# v2.1.2
## (2024-02-14)

* Replace deprecated flowzone input tests_run_on [Kyle Harding]

# v2.1.1
## (2023-12-19)

* Remove repo config from flowzone.yml [Kyle Harding]

# v2.1.0
## (2023-03-01)

* chore(deps): update deps/libwdi digest to 90278c5 [Renovate Bot]

# v2.0.0
## (2023-02-13)

* patch: fix linting long lines [mcraa]
* major: migrate to NAPI [mcraa]

# v1.5.0
## (2023-01-20)

* Add support for Node 18 [Akis Kesoglou]

# v1.4.0
## (2023-01-05)

* chore(deps): update deps/libwdi digest to 6722681 [Renovate Bot]

# v1.3.0
## (2023-01-05)

* Switch to Flowzone [ab77]

## 1.2.7 - 2020-03-12

* Remove prepublish script [Alexis Svinartchouk]
* Update nan to ^2.14 [Alexis Svinartchouk]
* Fix(generator): Pass context to value conversions [Jonas Hermsmeier]

## 1.2.6 - 2020-03-03

* Doc(README): Update requirements for building from source [Jonas Hermsmeier]

## 1.2.5 - 2019-01-25

* Fix AppVeyor build folder [Lorenzo Alberto Maria Ambrosi]
* Rename resin to balena in README [Lorenzo Alberto Maria Ambrosi]

## 1.2.4 - 2019-01-22

* Add .resinci.yml to only trigger ci on windows [Alexis Svinartchouk]
* Pull git submodules before publishing [Alexis Svinartchouk]
* Update prebuild and prebuild-install [Alexis Svinartchouk]

* Don't try to install on platforms other than win32 [Alexis Svinartchouk]

## 1.2.2 - 2018-07-11

* Fix(prebuild): Fix prebuild double upload [Jonas Hermsmeier]

## 1.2.1 - 2018-07-03

* Fix(ci): Fix build_v8_with_gn when prebuilding [Jonas Hermsmeier]
* Fix(binding): Set build_v8_with_gn to fix prebuilding [Jonas Hermsmeier]

## 1.2.0 - 2018-06-20

* Fix(binding): Fix code being generated on MacOS [Jonas Hermsmeier]
* Feat(binding): Build libwdi with gyp & VS2015 [Jonas Hermsmeier]
* Chore(ci): Add prebuild targets for new Node/Electron versions [Jonas Hermsmeier]

## v1.1.7 - 2018-03-23

- Update libwdi submodule
- Also remove prebuilding for Electron 1.8.2

## v1.1.6 - 2018-03-23

- Fix prebuild generation, again

## v1.1.5 - 2018-03-23

- Fix prebuild generation

## v1.1.4 - 2018-03-13

- Install prebuild on rebuild if available 

## v1.1.3 - 2018-03-05

- Prebuild for all targets & runtimes

## v1.1.2 - 2018-01-05

- Fix `libwdi_build.bat` script receiving a wrong architecture

## v1.1.1 - 2017-12-07

- Prebuild native add-on for Node.js v6

## v1.1.0 - 2017-11-30

- Implement the `.listDriverlessDevices()` function

## v1.0.1 - 2017-11-30

- Fix a logical error in `.hasDriver()` that would cause it to return true even
  if the device had no driver
