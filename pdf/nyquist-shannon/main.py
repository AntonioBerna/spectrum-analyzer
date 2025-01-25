import numpy as np
import matplotlib.pyplot as plt

AMPLITUDE = 1
F_MAX = 10
T_CONTINUOUS = np.arange(0, 1, 0.01)

class Signal:
    def __init__(self, amplitude, frequency):
        self.amplitude = amplitude
        self.frequency = frequency

    def generate(self, time):
        return self.amplitude * np.sin(2 * np.pi * self.frequency * time)

class SignalPlotter:
    @staticmethod
    def plot_continuous_signal(signal, time):
        x_continuous = signal.generate(time)
        plt.plot(time, x_continuous)
        plt.title(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")
        plt.xlabel(r"$t$")
        plt.ylabel(r"$x(t)$")

    @staticmethod
    def plot_sampled_signal(subplot_position, caption, signal, sampling_frequency):
        sampling_time = 1 / sampling_frequency
        t_sampled = np.arange(0, 1 + sampling_time, sampling_time)
        x_sampled = signal.generate(t_sampled)
        plt.subplot(2, 2, subplot_position)
        plt.stem(t_sampled, x_sampled)
        plt.title(caption)
        plt.xlabel(r"$t = k \cdot \Delta t$")
        plt.ylabel(r"$x(t) = \sin(2 \pi f_{\text{max}} t)$")

if __name__ == "__main__":
    signal = Signal(amplitude=AMPLITUDE, frequency=F_MAX)

    plt.figure(figsize=(12, 10))
    plt.subplot(2, 2, 1)
    SignalPlotter.plot_continuous_signal(signal, T_CONTINUOUS)

    SignalPlotter.plot_sampled_signal(2, r"$f_s > 2 f_{\text{max}}$", signal, sampling_frequency=4 * F_MAX) # f_s > 2 * f_max
    SignalPlotter.plot_sampled_signal(3, r"$f_s < 2 f_{\text{max}}$", signal, sampling_frequency=10)        # f_s < 2 * f_max
    SignalPlotter.plot_sampled_signal(4, r"$f_s = 2 f_{\text{max}}$", signal, sampling_frequency=2 * F_MAX) # f_s = 2 * f_max

    plt.tight_layout()
    plt.show()
