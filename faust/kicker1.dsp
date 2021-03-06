declare name "shell-kicker1";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("shell-wave.lib");
import("shell-fm.lib");
import("shell-env.lib");

gate = button("gate");
f1 = nentry("f1", 0, 0, 10000, 1);
f2 = nentry("f2", 500, 0, 10000, 1);
duration = nentry("duration", 250, 0, 3000, 1);
tau = nentry("tau", 5, 1, 10, 1);

// start t as the gate is activated
t = (+(1) ~ _ * gate);

fm  = shell_fm_exp(t, 0, duration * SR / 1000, f1, f2, 5);
osc = shell_sin(t, fm);
env = shell_env_exp(t, duration * SR / 1000 / tau);

out = select2(gate, 0, osc * env);

process = out, out;
