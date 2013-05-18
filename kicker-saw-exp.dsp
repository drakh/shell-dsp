declare name "shell-kicker-saw-exp";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("shell-wave.lib");
import("shell-fm.lib");
import("shell-env.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);
gain = hslider("gain", 100, 0, 100, 1);
gate = button("Kick!");
f1 = nentry("f1", 0, 0, 24000, 1);
f2 = nentry("f2", 500, 0, 24000, 1);
ratio = hslider("ratio", 0.5, 0, 1, 0.01);
duration = nentry("duration", 250, 0, 10000, 1);
tau = nentry("tau", 5, 1, 10, 1);

// start t as the gate is activated
t = (+(1) ~ _ * gate);

fm  = shell_fm_exp(t, 0, duration * SR / 1000, f1, f2, 5);
osc = shell_saw(t, fm, ratio, 1, -1, 1);
env = shell_env_exp(t, duration * SR / 1000 / 5);

process = select2(gate, 0, osc * env) * gate;
