#pragma once

#include <string>
#include <memory>

#include "faust/gui/SoundUI.h"
#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"
#include "faust/dsp/dsp.h"

#include "sstfx.hpp"

#include <cstring>
#include <sndfile.hh>

void create_file (const char * fname, float* buffer, int length);
void create_dat_file(int num_outputs, int length, float**buffer);

class dsp_2 : public dsp {
public:
    int accumulator = 0;
    
    float* input_buffer = new float[4];
    float* output_buffer = new float[4];
    
    std::unique_ptr<sst::voice_effects::utilities::VolumeAndPan<SstVfx::VfxConfig>> vfx;
    std::unique_ptr<sst::effects::reverb2::Reverb2<FxConfig>> fx;
    
    FxConfig::GlobalStorage gs = FxConfig::GlobalStorage(48000);
    FxConfig::EffectStorage es = FxConfig::EffectStorage();
    
    dsp_2():dsp() {
        vfx = std::make_unique<sst::voice_effects::utilities::VolumeAndPan<SstVfx::VfxConfig>>();
        vfx->sampleRate = 48000;
        vfx->initVoiceEffectParams();
        vfx->setFloatParam(sst::voice_effects::utilities::VolumeAndPan<SstVfx::VfxConfig>::fpVolume, -6);

        fx = std::make_unique<sst::effects::reverb2::Reverb2<FxConfig>>(&gs, &es, nullptr);
        
        for (int i = 0; i < sst::effects::reverb2::Reverb2<FxConfig>::numParams; ++i)
            fx->paramStorage[i] = fx->paramAt(i).defaultVal;
        
        fx->initialize();
    }
    virtual ~dsp_2 () {}
    
    float process_fx(float input) {
        input_buffer[accumulator] = input;
        float out = output_buffer[accumulator];
        accumulator++;
        if (accumulator > 3) {
            // vfx->processStereo((const float *)&input_buffer[0], (const float *)&input_buffer[0],
            //     output_buffer, output_buffer, 1);
            
            fx->processBlock(input_buffer, input_buffer);
            for (size_t s = 0; s < 4; s++) {
                output_buffer[s] = input_buffer[s];
            }
            
            accumulator = 0;
        }
        return out;
    }
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
        
        compute(blockSize, inputs, outputs);
        
        // for (size_t i = 0; i < 128; i++) {
        //   printf("%d -> %f\n", i, outputs[0][i]);
        // }
        
        create_file("/tmp/out.wav", outputs[0], blockSize);
        
        // create_dat_file(getNumOutputs(), blockSize, outputs);
        
    }
    
    void stop()
    {
    }
    
    float **inputs;
    float **outputs;
    
    static constexpr int blockSize{48000 * 2};
};

struct library
{
    library();
    
    std::string name;
    mydsp dsp_impl;
    std::unique_ptr<sst_dsp> dsp;
};
