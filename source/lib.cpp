#include "lib.hpp"

#include <iostream>
#include <fstream>

library::library()
    : name {"faust base cpp"}
    , dsp_impl ()
{
  dsp = std::make_unique<sst_dsp>(&dsp_impl);
  dsp->start();
  // dsp->stop();
}

void create_file(const char * fname, float* buffer, int length)
{
    
    SndfileHandle file ;
    int channels = 1 ;
    int srate = 48000 ;
    
    printf ("Creating file named '%s'\n", fname) ;
    
    file = SndfileHandle (fname, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, channels, srate) ;
    file.write (buffer, 4096) ;
}

void create_dat_file(int num_outputs, int length, float**buffer) {
  std::ofstream dat_file ("/tmp/example.dat");
  if (dat_file.is_open())
  {
    for (size_t s = 0; s < length; s++) {
        dat_file << s << "\t";
        for (size_t ch = 0; ch < num_outputs; ch++) {
            dat_file << buffer[ch][s] << "\t";
        }
        dat_file << "\n";
        
    }
    dat_file.close();
  }
}