# Panko

Panko is a dedicated packet dissection and analysis tool with a modern and sane API for dissectors.

It's primary input are compressed or raw [PCAP] and [PCAPNG] files, it then runs dissectors and analysis plugins on the packet stream, and the data can be inspected or exported.

> [!IMPORTANT]
> Panko doesn't do ***ANY*** capturing, nor does it have the ability to do so, and implementing the
> needed machinery for it is unplanned. It's mainly geared for ingesting captures from external
> software or Wireshark itself.
>
> There might be the possibility for an [extcap]-like interface in the future to allow for live
> dissection of an active capture, but once again, Panko won't be doing the capture itself.

## Panko vs Wireshark

Panko was developed in response to the absolute pain using the [Wireshark] C API is, as well as the discarding of potentially vital capture information from the capture files, such as which interface a packet the dissector is looking at came in on, and also any ancillary metadata attached to the packet capture files.

## Dissectors and Analysis plugins

```
TODO(aki): Document
```

## Configuring and Building

The following steps describe how to build Panko, it should be consistent for Linux, macOS, and Windows, but macOS and Windows remain untested.

> [!NOTE]
> The minimum C++ standard to build Panko is C++23.

### Prerequisites

To build Panko, ensure you have the following build time dependencies:

* git
* meson
* ninja
* g++ >= 14 or clang++ >= 19

In addition, the following dependencies are needed to build but have built-in build support if not present on the build system:

* [brotli] (only if `with_brotli` is specified)
* [cxxopts]
* [libbzip2]
* [liblz4]
* [liblzma]
* [lua] or [luajit]
* [pybind11]
* [snappy] (only if `with_snappy` is specified)
* [spdlog]
* [tomlplusplus]
* [zlib]
* [zstd]

[brotli] and or [snappy] are only needed if the respective `with_` options are enabled, they are used as part of the dissector API to allow decompression of [brotli] and [snappy] compressed buffers respectively.

To build the GUI, you need the following in addition to the above:

* [Qt6]
* [KF6] (Only if on Linux and `KDE_INTEGRATION` is enabled)

The following dependencies are needed for testing and fuzzing:

* [Doctest]
* [protobuf]
* [libprotobuf-mutator]
* [llvm] >= 19

The [protobuf], [libprotobuf-mutator], and [llvm] dependencies are only needed if you're doing fuzzing, otherwise only [Doctest] is used.

### Configuring

You can build Panko with the default options, all of which can be found in [`meson.options`]. You can change these by specifying `-D<OPTION_NAME>=<VALUE>` at initial meson invocation time, or with `meson configure` in the build directory post initial configure.

To change the install prefix, which is `/usr/local` by default ensure to pass `--prefix <PREFIX>` when running meson for the first time.

In either case, simply running `meson setup build` from the root of the repository will be sufficient and place all of the build files in the `build` subdirectory.

### Building

Once you have configured Panko appropriately, to simply build and install simply run the following:

```
$ ninja -C build
$ ninja -C build test # Optional: Run Tests
$ ninja -C build install
```

This will build and install Panko into the default prefix which is `/usr/local`, to change that see the configuration steps above.

### Notes to Package Maintainers

If you are building Panko for inclusion in a distributions package system then ensure to set `DESTDIR` prior to running meson install.

There is also a `bugreport_url` configuration option that is set to this repositories issues tracker by default, it is recommended to change it to your distributions bug tracking page.

## License

Panko is licensed under the [BSD-3-Clause] license. The full text of which can be found in the [`LICENSE`] file.

The documentation is licensed under the Creative Commons [CC-BY-SA 4.0] and can be found in the [`LICENSE.docs`] file

[PCAP]: https://ietf-opsawg-wg.github.io/draft-ietf-opsawg-pcap/draft-ietf-opsawg-pcap.html
[PCAPNG]: https://ietf-opsawg-wg.github.io/draft-ietf-opsawg-pcap/draft-ietf-opsawg-pcapng.html
[extcap]: https://www.wireshark.org/docs/wsdg_html_chunked/ChCaptureExtcap.html
[Wireshark]: https://gitlab.com/wireshark/wireshark
[brotli]: https://github.com/google/brotli
[libbzip2]: https://www.sourceware.org/bzip2/
[liblz4]: https://github.com/lz4/lz4
[liblzma]: https://github.com/tukaani-project/xz
[lua]: https://www.lua.org/
[luajit]: https://luajit.org/
[pybind11]: https://github.com/pybind/pybind11
[snappy]: https://github.com/google/snappy
[spdlog]: https://github.com/gabime/spdlog
[tomlplusplus]: https://github.com/marzer/tomlplusplus
[cxxopts]: https://github.com/jarro2783/cxxopts
[zlib]: https://www.zlib.net/
[zstd]: https://github.com/facebook/zstd
[Qt6]: https://www.qt.io/product/qt6
[KF6]: https://develop.kde.org/products/frameworks/
[Doctest]: https://github.com/doctest/doctest
[protobuf]: https://github.com/protocolbuffers/protobuf
[libprotobuf-mutator]: https://github.com/google/libprotobuf-mutator
[llvm]: https://llvm.org/
[`meson.options`]: ./meson.options
[BSD-3-Clause]: https://spdx.org/licenses/BSD-3-Clause.htm
[`LICENSE`]: ./LICENSE
[CC-BY-SA 4.0]: https://creativecommons.org/licenses/by-sa/4.0/
[`LICENSE.docs`]: ./LICENSE.docs
