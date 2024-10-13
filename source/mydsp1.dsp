import("stdfaust.lib");

trigger = ba.beat(30);

sample_sound = so.sound(soundfile("/home/xox/Prod/Samples/RV_FUTURE_DANCEHALL_HITS/RV_FUTURE_DANCEHALL/FDH_SOUNDS_&_FX/FDH_DRUM_HITS/SNARES/FDH_Snare_2.wav",1), 0);
sample = sample_sound.play(1.0,trigger);

process = sample;

// process = os.osc(220), 0.5 : ffunction(float mult(float, float), "", "");
// process = fconstant(int spam, "");
// process = os.osc(220), os.triangle(110) :> _ : *(0.3);
