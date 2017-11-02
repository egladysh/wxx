# wxx
A C++ DSL for generating dynamic HTML

## Build

### Requirements
C++11 or later

### Steps
* Suppose you cloned to [HOME]/work/wxx
* For out-of-source, create a build folder in [HOME]/work, and go there.

	$mkdir build

	$cd build

* Run cmake (make sure the build Release for speed).

	$cmake ../wxx/

* Build it.     

	$make

* You can now run a sample (the sample source is in wxx/sample/hello.cpp)
    
	$./hello > index.html

* Open the generated file in the browser

	$open index.html
