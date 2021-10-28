# vCard c++ simple library

This is a C++ library for parsing, writing and manipulating vCards.
Inspired by libvcard 1.0.0 and ez-vcard

## Install

### Dependencies
- vCard depends on having the `2.x` branch of [Catch2](https://github.com/catchorg/Catch2) installed as the unit tests depend on the cmake files that are installed with it.

### Installing
```
mkdir build
cd build
cmake ..
make
sudo make install
```


## Code exmaples

Reading text .vcf file:

```c++
#include "vcard.h"
#include "text_io.h"
...
std::ifstream ifs;
ifs.open("test.vcf");
TextReader tr(ifs);
std::vector<vCard> cards = tr.parseCards();

```

Creating vCard objects and write it to string buffer (or it can be file stream):

```c++
vCardParamMap params;
params.addParam("TYPE", "home");
params.addParam("TYPE", "work");
params.addParam("LANGUAGE", "en");

vCardProperty p = vCardProperty::createAddress("Street", "City", "Nevada", "112233", "USA");
p << params;

vCard card;
card << p;

std::stringstream s;
TextWriter tw(s);
tw << card;
```

More writers and readers will be added soon (for xCards and jCards formats)

For more examples check test/tests.cpp file

## Testing

After building the app using the steps above, you can run all the unit tests by calling `make test` inside the build directory.

To rerun one particular test with all debug information, use `ctest -R [NAME OF TEST] -VV` where `[NAME OF TEST]` is where you put the name of the unit test you want to run, such as `vCard_test:vCard`. See https://stackoverflow.com/a/6933301/ for more information.

In general these test names will be of the format `vCard_test:` followed by the name of the test as defined in `tests.cpp`.
