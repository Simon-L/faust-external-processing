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
