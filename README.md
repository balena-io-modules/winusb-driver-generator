winusb-driver-generator
=======================

[![Build status](https://ci.appveyor.com/api/projects/status/p40c1bxpgwmw3dc1/branch/master?svg=true)](https://ci.appveyor.com/project/resin-io/winusb-driver-generator/branch/master)

> A Windows Node.js native add-on to associate USB devices to the generic
> WinUSB driver

Installation
------------

Install `winusb-driver-generator` by running:

```sh
npm install --save winusb-driver-generator
```

Documentation
-------------

### `Boolean winusbDriverGenerator.hasDriver(Number vendorId, Number productId)`

Check if there is a driver associated with a USB device given its vendor and
product ID pairs.

Support
-------

If you're having any problem, please [raise an issue][newissue] on GitHub and
the Resin.io team will be happy to help.

Tests
-----

Run the test suite by doing:

```sh
npm test
```

Contribute
----------

- Issue Tracker: [github.com/resin-io-modules/winusb-driver-generator/issues][issues]
- Source Code: [github.com/resin-io-modules/winusb-driver-generator][source]

### Dependencies

- [cpplint][cpplint]

Before submitting a PR, please make sure that you include tests, and that
linters run without any warning:

```sh
npm run lint
```

License
-------

The project is licensed under the Apache 2.0 license.

[issues]: https://github.com/resin-io-modules/winusb-driver-generator/issues
[newissue]: https://github.com/resin-io-modules/winusb-driver-generator/issues/new
[source]: https://github.com/resin-io-modules/winusb-driver-generator
[cpplint]: https://github.com/cpplint/cpplint
