# Chopper experiments

This repository is a mix of benchmarks for Chopper/AutoChopper evaluation, forked from https://github.com/andreamattavelli/chopper-experiments.

- the benchmarks bc, libosip, libtasn1, libyaml are to be run with Chopper
- the benchmarks auto-bc, auto-libosip, auto-libtasn1, auto-libyaml are to be run with [AutoChopper](https://github.com/srg-imperial/klee-chopper-patches/)

## Configuration

You must edit the common.mk root file to configure paths to your tools, especially LLVM tools and KLEE-Chopper.

Additionally, each AutoChopper experiments folder has a `chopper.cfg` file that you should edit to select the AutoChopper binary path (`klee.cfg` also if you are doing KLEE experiments).

The AutoChopper experiments link to other git repositories as submodules: you must initialize them by running `git submodule update --init`.

## Running benchmarks

You can run the original benchmarks with vanilla Chopper using the `Makefile` in each directory.

You can run libtasn1 CVEs with AutoChopper using the script `libtasn1/run-chopit.sh`. (`./run-chopit.sh --help`)

In order to run the auto benchmarks, you should use the `chopem.py` script from AutoChopper. It will read from `chopper.cfg`. Some documentation will be displayed using `--help`, AutoChopper benchmarks are ran using

```
chopem.py --execute-chopper=1 --log=info --new-revision=master <repository folder>
```

## auto-libtasn1

OLD: For auto-libtasn1, we use `--new-revision=libtasn1_2_10` as the start point.

NEW: We use `--new-revision=libtasn1_4_4`. The `configure` file is absent, so you need to do `touch ./ChangeLog; autoreconf --install --verbose`, and possibly install some missing packages.