declare name "shell-kicker";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("music.lib");
import("oscillator.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);

gate = button("Kick!");
f1 = nentry("f1", 500, 0, 24000, 1);
f2 = nentry("f2", 1, 0, 24000, 1);
duration = nentry("duration", 250, 0, 10000, 1);
t = (+(1) ~ _ * gate);

f = f1 + (f2 - f1) * (t * 1000 / SR) / duration;

process = osci(f) * gate * (1000 * t / SR < duration);
