declare name "shell-kicker";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("shell-wave.lib");
import("shell-fm.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);
gain = hslider("gain", 100, 0, 100, 1);
gate = button("Kick!");
f1 = nentry("f1", 0, 0, 24000, 1);
f2 = nentry("f2", 500, 0, 24000, 1);
duration = nentry("duration", 250, 0, 10000, 1);
tau = nentry("tau", 5, 1, 10, 1);
osc_select = nentry("wave", 0, 0, 1, 1);
fm_select = nentry("fm", 1, 0, 1, 1);


waver(f) = select2(osc_select, shell_osc(t, f), shell_square(t, f, 0.5, 1, -1));

fm(x, x0, x1, y0, y1) = select2(fm_select,
                                      shell_fm_linear(x, x0, x1, y0, y1),
                                      shell_fm_exp(x, x0, x1, y0, y1, tau));
g = exp(-5 * 1000 * t / (SR * duration));

t = (+(1) ~ _ * gate);
kicker = select2(gate, 0,
           waver(fm(t, 0, SR * duration / 1000, f1, f2)) * g);

process = kicker;
