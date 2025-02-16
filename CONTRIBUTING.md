# Contribution Guidelines

> [!IMPORTANT]
> Contributions that were generated in whole or in-part from any
> language model or AI, such as GitHub Copilot, ChatGPT, BARD, or any other such tool
> are explicitly forbidden and will result in your permanent ban from contributing
> to this project.

## Contributing

Contributions to Panko are released under the [BSD-3-Clause] license, the full text of which can be found in the [`LICENSE`] file.

The documentation is licensed under the Creative Commons [CC-BY-SA 4.0] and can be found in the [`LICENSE.docs`] file

## Development and Testing

As noted in the README, this project requires [meson] as it's build system.
For a development setup, we recommend running `meson setup --buildtype debugoptimized build` rather than just `meson setup`.
We also recommend the use of the 'build' directory name as this works with the existing .gitignore rules
and keeps a nice commonality between all developer setups.

It is also recommended to enable asan and ubsan when doing development, you can enable them by running `meson configure -Db_sanitize=address,undefined` in the `build` directory. When the address sanitize is enabled, so is the leak sanitizer, however the stack traces it produces on exit can be incomplete. In order to see the full leak trace, export `LSAN_OPTIONS=fast_unwind_on_malloc=0` into your build environment to have lsan produce full traces. It is also recommended to tell LSAN about the [`lsan_suppressions.txt`] file as to quiet it down about external leaks we can't do anything about by specifying `suppressions=/path/to/lsan_suppressions.txt`.

## Common tasks

These are intended to be run from inside your build directory

* Building the library component and tests: `ninja` (`meson compile`)
* Running tests: `ninja test` (`meson test`), or to view the raw output `meson test -v`
* Debugging tests: `meson test --gdb`

## Fuzzing

If you are going to be running the fuzzing harnesses, then note the following dependencies are needed:

* [protobuf]
* [libprotobuf-mutator]
* [llvm] >= 17
* clang >= 17

Both [protobuf] and [libprotobuf-mutator] will be handled by meson automatically when building.

Fuzzing is only supported using clang, as gcc does not have `-fsanitize=fuzzer` support which is needed.

To set up your fuzzing build directory, you can simply run the following from the source root:

```
$ CC=clang CXX=clang++ meson setup --buildtype debugoptimized -Db_sanitize=address,undefined build-fuzzing
```

The `fuzzing_tests` option is set by default, and so meson will handle all the setup for you.

Some changes occur in this configuration, `b_lundef` is set to `false` as that breaks clang linking the sanitizers, and also the `b_sanitize` option is modified to include linking to the `fuzzer` sanitizer.

## Submitting a Pull Request

 1. [Fork] and clone the repository
 2. Create a new branch: `git switch -c branch-name` (`git checkout -b branch-name` in the old syntax)
 3. Make your change, create unit tests and sure all tests new and old pass
 4. Push to your fork and submit a [pull request]

Additionally, please write good and descriptive commit messages that both summarize the change and,
if necessary, expand on the summary using description lines.
"Patched the layout editor" is, while terse and correct, an example of a bad commit message.
"Fixed a but in the layout editor preventing grid snapping from working properly" is an example of a better commit message.

We would like to be able to look back through the commit history and tell what happened, when, and why without having
to dip into the commit descriptions as this improves the `git bisect` experience and improves everyone's lives.

We use rebasing to merge pull requests, so please keep this in mind and aim to keep a linear history.

[BSD-3-Clause]: https://spdx.org/licenses/BSD-3-Clause.htm
[`LICENSE`]: ./LICENSE.md
[CC-BY-SA 4.0]: https://creativecommons.org/licenses/by-sa/4.0/
[`LICENSE.docs`]: ./LICENSE.docs
[Contributor Code of Conduct]: ./CODE_OF_CONDUCT.md
[meson]: https://meson.build/
[`lsan_suppressions.txt`]: ./contrib/lsan_suppressions.txt
[Fork]: https://github.com/lethalbit/Panko/fork
[pull request]: https://github.com/lethalbit/Panko/compare
[protobuf]: https://github.com/protocolbuffers/protobuf
[libprotobuf-mutator]: https://github.com/google/libprotobuf-mutator
[llvm]: https://llvm.org/
