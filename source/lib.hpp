#pragma once

#include <string>
#include <memory>

#include "faust/gui/SoundUI.h"
#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"
#include "faust/dsp/dsp.h"

#include <cstring>
#include <sndfile.hh>

void create_file (const char * fname, float* buffer, int length);

// blocksize 4 -> 0.083 ms (12000.000 Hz)
// blocksize 8 -> 0.167 ms (6000.000 Hz)
// blocksize 16 -> 0.333 ms (3000.000 Hz)
// blocksize 32 -> 0.667 ms (1500.000 Hz)
// blocksize 64 -> 1.333 ms (750.000 Hz)
// blocksize 128 -> 2.667 ms (375.000 Hz)
// blocksize 256 -> 5.333 ms (187.500 Hz)


class dsp_2 : public dsp {
public:
  dsp_2():dsp() {}
  virtual ~dsp_2 () {}
  
  // fvariable(float foobar, "")
  float foobar = 10;
  
  // ffunction(float mult(float, float), "", "")
  float mult(float a, float b) { return a * b; }
  
  // fconstant(int spam, "")
  static constexpr int spam =  1337;
};

#include "dsp.hpp" // generated dsp code with:
// faust -I .../share/faust -a .../faust_arch.cpp --super-class-name dsp_2 .../mydsp1.dsp > .../dsp.hpp

class sst_dsp : public decorator_dsp {
  dsp_2* fDSP2;
  
public:
  sst_dsp(dsp* dsp):decorator_dsp(dsp),
    fDSP2(dynamic_cast<dsp_2*>(fDSP))
  {
    outputs = new float*[getNumOutputs()];
    for (size_t i = 0; i < getNumOutputs(); i++) {
      outputs[i] = new float[blockSize];
    }
    
    inputs = new float*[getNumInputs()];
    for (size_t i = 0; i < getNumInputs(); i++) {
      inputs[i] = new float[blockSize];
    }
    
  }
  virtual ~sst_dsp()
  {
    delete[] outputs;
    delete[] inputs;
    fDSP = nullptr;
  }
  
  void start()
  {
    init(48000);
    
    SoundUI s;
    
    buildUserInterface(&s);
    
    fDSP2->foobar = 30;
    
    compute(blockSize, inputs, outputs);
    
    for (size_t i = 0; i < 128; i++) {
      printf("%d -> %f\n", i, outputs[0][i]);
    }
    
    create_file("/tmp/foo.wav", outputs[0], blockSize);
  }
  
  void stop()
  {
  }
  
  float **inputs;
  float **outputs;
  
  static constexpr int blockSize{8192};
};

/**
 * @brief The core implementation of the executable
 *
 * This class makes up the library part of the executable, which means that the
 * main logic is implemented here. This kind of separation makes it easy to
 * test the implementation for the executable, because the logic is nicely
 * separated from the command-line logic implemented in the main function.
 */
struct library
{
  /**
   * @brief Simply initializes the name member to the name of the project
   */
  library();

  std::string name;
  mydsp dsp_impl;
  std::unique_ptr<sst_dsp> dsp;
};
