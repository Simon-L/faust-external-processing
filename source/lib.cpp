#include "lib.hpp"

library::library()
    : name {"faust base cpp"}
    , dsp_impl ()
{
  dsp = std::make_unique<sst_dsp>(&dsp_impl);
  dsp->start();
  // dsp->stop();
}

void create_file (const char * fname, float* buffer, int length)
{
    
    SndfileHandle file ;
    int channels = 1 ;
    int srate = 48000 ;
    
    printf ("Creating file named '%s'\n", fname) ;
    
    file = SndfileHandle (fname, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, channels, srate) ;
    file.write (buffer, 4096) ;
}