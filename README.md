# OMT Receiver for Resolume Arena

Standalone FFGL **Source** plugin for receiving video over Open Media Transport (OMT).

This repository is independent from the Máscara MultiPainel project.

## Status

Initial Windows x64 build setup. The source is based on the OMTReceive implementation from [tomspace/ffgl-omt](https://github.com/tomspace/ffgl-omt), with its original copyright and license notice retained.

The plugin has **not yet been compiled or tested in Resolume Arena 7.22**. A successful GitHub Actions build and an in-app network test are still required before treating it as ready to use.

## Build

GitHub Actions builds the plugin on pushes to `main` and via **Actions → Build OMT Receiver → Run workflow**.

The workflow checks out:
- this repository;
- the Resolume FFGL SDK;
- the OMT dependency package from the reference project.

The build artifact contains `OMTReceiver.dll` and the OMT runtime DLLs. Install/test instructions will be added after the first successful build.

## Notes

- This is a Source plugin (`FF_SOURCE`), not an effect.
- The receiver discovers OMT sources on the network and provides a source selector.
- Third-party SDK and runtime licenses must be preserved when redistributing dependencies.
