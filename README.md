# **Fossil Algorithms by Fossil Logic**

**Fossil Algorithm** is a lightweight, portable library providing a collection of algorithmic solutions written in pure C with zero external dependencies. Designed for performance and clarity, Fossil Algorithm offers efficient implementations of common data processing techniques, utilities, and patterns suitable for embedded systems, high-performance computing, and cross-platform applications. Its minimal footprint and clean design make it easy to integrate into any project without sacrificing maintainability or speed.

### Key Features  

- **Cross-Platform Support**  
  Consistent behavior on Windows, macOS, Linux, and embedded targets.  

- **Zero External Dependencies**  
  Written in clean, portable C for maximum transparency and easy integration.  

- **Wide Range of Algorithmic Solutions**  
  Includes efficient implementations for sorting, searching, hashing, and other core algorithms.  

- **Lightweight and Efficient**  
  Optimized for performance and minimal memory usage—ideal for constrained environments.  

- **Self-Contained & Auditable**  
  Readable, well-structured source code makes verification and debugging straightforward.  

- **Modular Design**  
  Select only the algorithm modules you need for a lean, customized build. 

## ***Prerequisites***

To get started, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson `1.8.0` or newer installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Dependency

#### Adding via Meson Git Wrap

To add a git-wrap, place a `.wrap` file in `subprojects` with the Git repo URL and revision, then use `dependency('fossil-algorithm')` in `meson.build` so Meson can fetch and build it automatically.

#### Integrate the Dependency:

Add the `fossil-algo.wrap` file in your `subprojects` directory and include the following content:

```ini
[wrap-git]
url = https://github.com/fossillogic/fossil-algo.git
revision = v0.1.1

[provide]
dependency_names = fossil-algo
```

**Note**: For the best experience, always use the latest releases. Visit the [releases](https://github.com/fossillogic/fossil-algorithm/releases) page for the latest versions.

## Build Configuration Options

Customize your build with the following Meson options:
	•	Enable Tests
To run the built-in test suite, configure Meson with:

```sh
meson setup builddir -Dwith_test=enabled
```

### Tests Double as Samples

The project is designed so that **test cases serve two purposes**:

- ✅ **Unit Tests** – validate the framework’s correctness.  
- 📖 **Usage Samples** – demonstrate how to use these libraries through test cases.  

This approach keeps the codebase compact and avoids redundant “hello world” style examples.  
Instead, the same code that proves correctness also teaches usage.  

This mirrors the **Meson build system** itself, which tests its own functionality by using Meson to test Meson.  
In the same way, Fossil Logic validates itself by demonstrating real-world usage in its own tests via Fossil Test.  

```bash
meson test -C builddir -v
```

Running the test suite gives you both verification and practical examples you can learn from.

## Contributing and Support

For those interested in contributing, reporting issues, or seeking support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are always welcome.
