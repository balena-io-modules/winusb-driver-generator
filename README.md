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

Note that because of WinDDK, you can only build this module using Visual Studio
2013, and can't target a Node v9.0.0 ABI, which uses C++ constructs unsupported
on such Visual Studio version.

Documentation
-------------

### `Boolean .hasDriver(Number vendorId, Number productId)`

Check if there is a driver associated with a USB device given its vendor and
product ID pairs.

For example:

```js
if (!winusbDriverGenerator.hasDriver(0x0a5c, 0x2764)) {
  console.log('There is no driver for this device');
}
```

### `Void .associate(Number vendorId, Number productId, String description)`

Associate a USB device with the generic WinUSB driver.

For example:

```js
winusbDriverGenerator.associate(0x0a5c, 0x2764, 'Raspberry Pi USB boot');
```

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
