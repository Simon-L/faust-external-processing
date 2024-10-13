/* ------------------------------------------------------------
name: "mydsp1"
Code generated with Faust 2.75.5 (https://faust.grame.fr)
Compilation options: -a /home/xox/Sync/tr909/faust/faust-base-cpp-project/source/faust_arch.cpp -lang cpp -ct 1 -scn dsp_2 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

// Begin FAUST architecture

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif


class mydsp : public dsp_2 {
	
 private:
	
	Soundfile* fSoundfile0;
	int fSampleRate;
	float fConst0;
	int iConst1;
	int iVec0[2];
	int iRec2[2];
	int iVec1[2];
	int iRec3[2];
	float fConst2;
	float fRec0[2];
	int iRec1[2];
	
	void allocate() {
		fSoundfile0 = 0;
	}
	
 public:
	mydsp() {
		allocate();
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-a /home/xox/Sync/tr909/faust/faust-base-cpp-project/source/faust_arch.cpp -lang cpp -ct 1 -scn dsp_2 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "mydsp1.dsp");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "1.3.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "mydsp1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("soundfiles.lib/name", "Faust Soundfile Library");
		m->declare("soundfiles.lib/version", "1.7.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		iConst1 = int(2.0f * fConst0);
		fConst2 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		if (uintptr_t(fSoundfile0) == 0) {
			fSoundfile0 = defaultsound;
		}
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec2[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec3[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec0[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec1[l5] = 0;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("mydsp1");
		ui_interface->addSoundfile("home/xox/Prod/Samples/RV_FUTURE_DANCEHALL_HITS/RV_FUTURE_DANCEHALL/FDH_SOUNDS_&_FX/FDH_DRUM_HITS/SNARES/FDH_Snare_2.wav", "{'home/xox/Prod/Samples/RV_FUTURE_DANCEHALL_HITS/RV_FUTURE_DANCEHALL/FDH_SOUNDS_&_FX/FDH_DRUM_HITS/SNARES/FDH_Snare_2.wav'}", &fSoundfile0);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		Soundfile* fSoundfile0ca = fSoundfile0;
		int* fSoundfile0ca_ra0 = fSoundfile0ca->fSR;
		float fSlow0 = fConst2 * float(fSoundfile0ca_ra0[0]);
		float fSlow1 = std::floor(fSlow0);
		int iSlow2 = int(fSlow0);
		int* fSoundfile0ca_le0 = fSoundfile0ca->fLength;
		int iSlow3 = fSoundfile0ca_le0[0] + -1;
		int* fSoundfile0ca_of0 = fSoundfile0ca->fOffset;
		float** fSoundfile0ca_bu0 = static_cast<float**>(fSoundfile0ca->fBuffers);
		float* fSoundfile0ca_bu_ch0 = fSoundfile0ca_bu0[0];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			iRec2[0] = (iVec0[1] + iRec2[1]) % iConst1;
			int iTemp0 = iRec2[0] <= iRec2[1];
			iVec1[0] = iTemp0;
			iRec3[0] = ((iTemp0) ? iTemp0 : iRec3[1]);
			int iTemp1 = iRec3[0] * (iTemp0 <= iVec1[1]);
			float fTemp2 = fSlow0 + (fRec0[1] - fSlow1);
			fRec0[0] = (fSlow0 + (fRec0[1] - std::floor(fTemp2) - fSlow1)) * float(iTemp1);
			iRec1[0] = iTemp1 * (iSlow2 + iRec1[1] + int(fTemp2));
			output0[i0] = FAUSTFLOAT(fSoundfile0ca_bu_ch0[fSoundfile0ca_of0[0] + std::max<int>(0, std::min<int>(iRec1[0], iSlow3))]);
			iVec0[1] = iVec0[0];
			iRec2[1] = iRec2[0];
			iVec1[1] = iVec1[0];
			iRec3[1] = iRec3[0];
			fRec0[1] = fRec0[0];
			iRec1[1] = iRec1[0];
		}
		fSoundfile0 = fSoundfile0ca;
	}

};

// End FAUST architecture

#endif
