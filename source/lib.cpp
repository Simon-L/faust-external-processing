#include "lib.hpp"

library::library()
    : name {"faust base cpp"}
    , dsp_impl ()
{
  dsp = std::make_unique<sst_dsp>(&dsp_impl);
  dsp->start();
  // dsp->stop();
}
