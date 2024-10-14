# Faust base C++ cmake project

`lib.hpp` demonstrates embedding an external effect (sst-effects) inside a Faust DSP program. It is achieved as part of the `dsp_2` class, used as the parent class of the generated `mydsp` class so that if `dsp_2` has a `foo` method then `ffunction(float foo(float), "", "")` will resolve to it.

Because sst-effects is used with a blocksize of 4, a delay of 4 samples is introduced, it can be compensated in Faust if needed.

`dsp.hpp` contains the raw generated code from this command:  
`./faust -I .../share/faust -a .../faust-base-cpp-project/source/faust_arch.cpp --super-class-name dsp_2 .../faust-base-cpp-project/source/mydsp1.dsp -o .../faust-base-cpp-project/source/dsp.hpp`

```
cmake -Bbuild
cmake --build build -j16
./build/faust-base-cpp
gnuplot -p -e "plot '/tmp/example.dat' using 1:2 with lines, '' using 1:3 with lines;pause mouse close"
```


Based on an example output of
[cmake-init](https://github.com/friendlyanon/cmake-init) version 0.40.9
