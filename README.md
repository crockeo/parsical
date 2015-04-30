# parsical

parsical is a parsing library written in C++ based on the
[parsec](http://hackage.haskell.org/package/parsec) library. Of course since C++
and Haskell are very, very different languages these aren't going to be exact
copies.

### Installation

Minimum CMake version is set to `3.1`, but if you open up the `CMakeLists.txt`
you probably ought to be able to fiddle with that value to see what works.

Assuming you've got that all done:

```bash
$ git clone http://github.com/crockeo/parsical
$ cd parsical
$ cmake .
$ make install
```

If you want to uninstall the library, run the `uninstall.sh` script found in the
directory.

### Licensing

For licensing information refer to the `LICENSE` file.
