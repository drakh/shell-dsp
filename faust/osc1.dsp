declare name "shell-osc1";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("shell-core.lib");
import("shell-wave.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);
//gain = hslider("gain", 100, 0, 100, 1);
gate = button("gate");
ratio = hslider("ratio", 0.5, 0, 1, 0.01);
wave = nentry("wave", 0, 0, 2, 1);

w = select2(wave != 0, shell_sin(shell_time, freq),
    select2(wave != 1, shell_square(shell_time, freq, ratio, 1, 0),
    select2(wave != 2, abs(shell_sin(shell_time, freq)),
                       shell_saw(shell_time, freq, ratio, 1, -1, 1))));

out = w * gate;

process = out, out;
