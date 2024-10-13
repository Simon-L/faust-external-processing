import("stdfaust.lib");

process = os.osc(220), 0.5 : ffunction(float mult(float, float), "", "");
// process = fconstant(int spam, "");
// process = os.osc(220), os.triangle(110) :> _ : *(0.3);
