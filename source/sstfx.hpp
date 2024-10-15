#include "sst/voice-effects/distortion/BitCrusher.h"
#include "sst/voice-effects/utilities/VolumeAndPan.h"
#include "sst/voice-effects/dynamics/Compressor.h"

#include "sst/effects/Reverb2.h"

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

struct SstVfx
{
    std::array<float, 256> fb{};
    std::array<int, 256> ib{};
    float sampleRate;
    DbToLinearProvider dbtlp;
    
    struct VfxConfig
    {
        using BaseClass = SstVfx;
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
    
    // std::unique_ptr<sst::voice_effects::distortion::BitCrusher<VfxConfig>> fx;
    
    // std::unique_ptr<sst::voice_effects::dynamics::Compressor<VfxConfig>> fx;
    
    SstVfx() { dbtlp.init(); }
};

struct FxConfig
{
    struct BC
    {
        static constexpr uint16_t maxParamCount{20};
        float paramStorage[maxParamCount];
        template <typename... Types> BC(Types...) {}
    };
    struct GS
    {
        double sampleRate;
        DbToLinearProvider dbtlp;
        // It is painful that sst-filters makes us over-adapt
        // this class
        GS(double sr) : sampleRate(sr) {
          dbtlp.init();
        }
    };
    struct ES
    {
    };
    using BaseClass = BC;
    using GlobalStorage = GS;
    using EffectStorage = ES;
    using ValueStorage = float *;
    using BiquadAdapter = FxConfig;

    static constexpr int blockSize{4};

    static inline float floatValueAt(const BaseClass *const e, const ValueStorage *const v, int idx)
    {
        return e->paramStorage[idx];
    }
    static inline int intValueAt(const BaseClass *const e, const ValueStorage *const v, int idx)
    {
        return (int)std::round(e->paramStorage[idx]);
    }
    static inline float envelopeRateLinear(GlobalStorage *s, float f)
    {
        return 1.f * blockSize / (s->sampleRate) * pow(-2, f);
    }
    static inline float temposyncRatio(GlobalStorage *s, EffectStorage *e, int idx) { return 1; }
    static inline bool isDeactivated(EffectStorage *e, int idx) { return false; }
    static inline bool isExtended(EffectStorage *e, int idx) { return false; }
    static inline float rand01(GlobalStorage *s) { return (float)rand() / (float)RAND_MAX; }
    static inline double sampleRate(GlobalStorage *s) { return s->sampleRate; }
    static inline double sampleRateInv(GlobalStorage *s) { return 1.0 / s->sampleRate; }
    static inline float noteToPitch(GlobalStorage *s, float p) { return pow(2.0, p / 12); }
    static inline float noteToPitchIgnoringTuning(GlobalStorage *s, float p) { return noteToPitch(s, p); }
    static inline float noteToPitchInv(GlobalStorage *s, float p) { return 1.0 / noteToPitch(s, p); }
    static inline float dbToLinear(GlobalStorage *s, float f) { return s->dbtlp.dbToLinear(f); }
};