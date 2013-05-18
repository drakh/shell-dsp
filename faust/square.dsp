declare name "shell-square";
declare author "Alexandre BIQUE";
declare copyright "Alexandre BIQUE";
declare version "1.0";
declare license "MIT";

import("shell-core.lib");
import("shell-wave.lib");

freq = nentry("freq", 1000, 0, 24000, 0.1);
gain = hslider("gain", 100, 0, 100, 1);
gate = button("play");
ratio = hslider("ratio", 0.5, 0, 1, 0.01);

process = shell_square(shell_time, freq, ratio, 1, 0) * gate;
