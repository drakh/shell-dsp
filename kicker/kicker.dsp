declare name "shell-kicker";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("music.lib");
import("oscillator.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);

gate = button("Kick!");
f1 = nentry("f1", 0, 0, 24000, 1);
f2 = nentry("f2", 500, 0, 24000, 1);
duration = nentry("duration", 250, 0, 10000, 1);
t = (+(1) ~ _ * gate);

//f = f1 + (f2 - f1) * (t * 1000 / SR) / duration;

// 0: osci
// 1: saw1
waver_select = nentry("waver", 0, 0, 1, 1);
waver(f) = select2(waver_select, osci(f), saw1(f));

linear_variator(x, x0, x1, y0, y1) = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
tau = nentry("tau", 5, 1, 10, 1);
exp_variator(x, x0, x1, y0, y1) = y0 + (y0 - y1) * (exp(-tau * (x - x0) / (x1 - x0)));
variator_select = nentry("variator", 1, 0, 1, 1);
variator(x, x0, x1, y0, y1) = select2(variator_select,
                                      linear_variator(x, x0, x1, y0, y1),
                                      exp_variator(x, x0, x1, y0, y1));

kicker = waver(variator(t, 0, SR * duration / 1000, f1, f2)) * gate * (1000 * t / SR < duration);

process = kicker;
