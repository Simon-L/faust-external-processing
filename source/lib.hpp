#pragma once

#include <string>
#include <memory>

#include "faust/gui/SoundUI.h"
#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"
#include "faust/dsp/dsp.h"

#include "sst/voice-effects/distortion/BitCrusher.h"
#include "sst/voice-effects/utilities/VolumeAndPan.h"
#include "sst/voice-effects/dynamics/Compressor.h"

#include <cstring>
#include <sndfile.hh>

void create_file (const char * fname, float* buffer, int length);
void create_dat_file(int num_outputs, int length, float**buffer);

struct DbToLinearProvider
{
    static constexpr size_t nPoints{512};
    float table_dB[nPoints];
    
    void init()
    {
        for (auto i = 0U; i < nPoints; i++)
        table_dB[i] = powf(10.f, 0.05f * ((float)i - 384.f));
    }
    
    float dbToLinear(float db) const
    {
        db += 384;
        int e = (int)db;
        float a = db - (float)e;
        return (1.f - a) * table_dB[e & (nPoints - 1)] + a * table_dB[(e + 1) & (nPoints - 1)];
    }
};

struct SSTFX
{
    std::array<float, 256> fb{};
    std::array<int, 256> ib{};
    float sampleRate;
    DbToLinearProvider dbtlp;
    
    struct FxConfig
    {
        using BaseClass = SSTFX;
        static constexpr int blockSize{4};
        static void setFloatParam(BaseClass *b, int i, float f) { b->fb[i] = f; }
        static float getFloatParam(const BaseClass *b, int i) { return b->fb[i]; }
        
        static void setIntParam(BaseClass *b, int i, int v) { b->ib[i] = v; }
        static int getIntParam(const BaseClass *b, int i) { return b->ib[i]; }
        
        static float dbToLinear(const BaseClass *b, float f) { return b->dbtlp.dbToLinear(f); }
        
        static float equalNoteToPitch(const BaseClass *, float f)
        {
            return pow(2.f, (f + 69) / 12.f);
        }
        
        static float getSampleRate(const BaseClass *b) { return b->sampleRate; }
        static float getSampleRateInv(const BaseClass *b) { return 1.0 / b->sampleRate; }
        
        static void preReservePool(BaseClass *, size_t) {}
        
        static void preReserveSingleInstancePool(BaseClass *, size_t) {}
        
        static uint8_t *checkoutBlock(BaseClass *, size_t n)
        {
            printf("checkoutBlock %zu\n", n);
            uint8_t *ptr = (uint8_t *)malloc(n);
            return ptr;
        }
        
        static void returnBlock(BaseClass *, uint8_t *ptr, size_t n)
        {
            printf("returnBlock %zu\n", n);
            free(ptr);
        }
    };
    
    // std::unique_ptr<sst::voice_effects::distortion::BitCrusher<FxConfig>> fx;
    
    // std::unique_ptr<sst::voice_effects::dynamics::Compressor<FxConfig>> fx;
    
    SSTFX() { dbtlp.init(); }
};

class dsp_2 : public dsp {
public:
    int accumulator = 0;
    
    float* input_buffer = new float[4];
    float* output_buffer = new float[4];
    
    std::unique_ptr<sst::voice_effects::utilities::VolumeAndPan<SSTFX::FxConfig>> fx;
    
    dsp_2():dsp() {
        fx = std::make_unique<sst::voice_effects::utilities::VolumeAndPan<SSTFX::FxConfig>>();
        fx->sampleRate = 48000;
        fx->initVoiceEffectParams();
        fx->setFloatParam(sst::voice_effects::utilities::VolumeAndPan<SSTFX::FxConfig>::fpVolume, -3);
    }
    virtual ~dsp_2 () {}
    
    float process_fx(float input) {
        input_buffer[accumulator] = input;
        printf("%d\n", accumulator);
        printf("\treturn output_buffer[%d]\n", accumulator);
        float out = output_buffer[accumulator];
        accumulator++;
        if (accumulator > 3) {
            printf("\tProcessing one block, content of output changes\n");
            fx->processStereo((const float *)&input_buffer[0], (const float *)&input_buffer[0],
                output_buffer, output_buffer, 1);
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
        // 
        // create_file("/tmp/foo.wav", outputs[0], blockSize);
        
        create_dat_file(getNumOutputs(), blockSize, outputs);
        
    }
    
    void stop()
    {
    }
    
    float **inputs;
    float **outputs;
    
    static constexpr int blockSize{128};
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
