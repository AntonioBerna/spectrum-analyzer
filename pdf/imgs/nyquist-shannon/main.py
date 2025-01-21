import numpy as np
import matplotlib.pyplot as plt

# Signal parameters
amplitude = 1                                  # Signal amplitude
f_max = 10                                     # Signal maximum frequency
t = np.arange(0, 1, 0.01)                      # Time axis
x = amplitude * np.sin(2 * np.pi * f_max * t)  # x(t)

plt.figure(figsize=(12, 10))

plt.subplot(2, 2, 1)
plt.plot(t, x)
plt.title(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")
plt.xlabel(r"$t$")
plt.ylabel(r"$x(t) = \sum_{k = -\infty}^{+\infty} x(k \cdot \Delta t) \cdot sinc \left ( \frac{t}{\Delta t} - k \right )$")

# (1) f_s > 2 * f_max
k = 2
f_s = k * 2 * f_max
sampling_time = 1 / f_s
t = np.arange(0, 1 + sampling_time, sampling_time) # k · Δt
x = amplitude * np.sin(2 * np.pi * f_max * t)      # x(t)

plt.subplot(2, 2, 2)
plt.stem(t, x)
plt.title(r"$f_s > 2 f_{\text{max}}$")
plt.xlabel(r"$t = k \cdot \Delta t$")
plt.ylabel(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")

# (2) f_s < 2 * f_max
f_s = 10
sampling_time = 1 / f_s
t = np.arange(0, 1 + sampling_time, sampling_time) # k · Δt
x = amplitude * np.sin(2 * np.pi * f_max * t)      # x(t)

plt.subplot(2, 2, 3)
plt.stem(t, x)
plt.title(r"$f_s < 2 f_{\text{max}}$")
plt.xlabel(r"$t = k \cdot \Delta t$")
plt.ylabel(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")

# (3) f_s = 2 * f_max
k = 1
f_s = k * 2 * f_max
sampling_time = 1 / f_s
t = np.arange(0, 1 + sampling_time, sampling_time) # k · Δt
x = amplitude * np.sin(2 * np.pi * f_max * t)      # x(n)

plt.subplot(2, 2, 4)
plt.stem(t, x)
plt.title(r"$f_s = 2 f_{\text{max}}$")
plt.xlabel(r"$t = k \cdot \Delta t$")
plt.ylabel(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")

plt.tight_layout()
plt.show()
