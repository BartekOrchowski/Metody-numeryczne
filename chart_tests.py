# testowe pytonowe skrypty do testowania porpawnosci wykresów

import tkinter as tk
from tkinter import ttk

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Stałe fizyczne
Q = 1.602176634e-19  # ładunek elektronu [C]
K_B = 1.380649e-23   # stała Boltzmanna [J/K]


def diode_current(V, Is, n, T, Vb, Ib, Vmin):
    """
    I(V) według wzoru z obrazka.
    Vb = |V_BR|, Ib = prąd przebicia przy V = Vmin (reverse).
    """
    VT = K_B * T / Q  # napięcie termiczne

    I = np.zeros_like(V)

    # region V >= 0
    mask_fwd = V >= 0
    x = V[mask_fwd] / (n * VT)
    x = np.clip(x, -80.0, 40.0)
    I[mask_fwd] = Is * (np.exp(x) - 1.0)

    # region V < 0
    mask_rev = ~mask_fwd
    V_rev = V[mask_rev]
    I_rev = np.zeros_like(V_rev)

    # jeśli V > -Vb lub Vb == 0: I = -Is
    cond1 = (V_rev > -Vb) | (Vb == 0)
    I_rev[cond1] = -Is

    # jeśli V <= -Vb: interpolacja między -Is (przy V = -Vb) i Ib (przy V = Vmin)
    cond2 = ~cond1
    if np.any(cond2):
        V_rev2 = V_rev[cond2]
        denom = max(abs(Vmin) - Vb, 1e-9)
        t = (np.abs(V_rev2) - Vb) / denom
        t = np.clip(t, 0.0, 1.0)
        I_rev[cond2] = (1.0 - t) * (-Is) + t * Ib

    I[mask_rev] = I_rev
    return I


class DiodeGUI:
    def __init__(self, master):
        self.master = master
        master.title("Charakterystyka diody (Shockley)")

        # --- panel z parametrami ---
        frame = ttk.Frame(master, padding=5)
        frame.pack(side=tk.LEFT, fill=tk.Y)

        self.entries = {}

        def add_row(label, default):
            row = ttk.Frame(frame)
            row.pack(fill=tk.X, pady=2)
            ttk.Label(row, text=label, width=22).pack(side=tk.LEFT)
            e = ttk.Entry(row, width=15)
            e.insert(0, str(default))
            e.pack(side=tk.LEFT)
            self.entries[label] = e

        add_row("Prąd nasycenia Is [A]", 2e-7)
        add_row("Wsp. idealności n", 1.0)
        add_row("Temperatura T [K]", 300.0)
        add_row("Minimalne napięcie Vmin [V]", -5.0)
        add_row("Maksymalne napięcie Vmax [V]", 1.0)
        add_row("Napięcie przebicia |Vb| [V]", 50.0)
        add_row("Prąd przebicia Ib [A]", -0.01)
        add_row("Liczba próbek", 600)

        btn = ttk.Button(frame, text="Rysuj charakterystykę", command=self.plot_iv)
        btn.pack(pady=10, fill=tk.X)

        # --- obszar wykresu ---
        fig, ax = plt.subplots(figsize=(6, 4), dpi=100)
        self.fig = fig
        self.ax = ax

        self.canvas = FigureCanvasTkAgg(fig, master=master)
        self.canvas.get_tk_widget().pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        self.plot_iv()  # pierwszy wykres

    def get_float(self, label):
        return float(self.entries[label].get())

    def plot_iv(self):
        Is = self.get_float("Prąd nasycenia Is [A]")
        n = self.get_float("Wsp. idealności n")
        T = self.get_float("Temperatura T [K]")
        Vmin = self.get_float("Minimalne napięcie Vmin [V]")
        Vmax = self.get_float("Maksymalne napięcie Vmax [V]")
        Vb = self.get_float("Napięcie przebicia |Vb| [V]")
        Ib = self.get_float("Prąd przebicia Ib [A]")
        num = int(self.get_float("Liczba próbek"))

        V = np.linspace(Vmin, Vmax, num)
        I = diode_current(V, Is, n, T, Vb, Ib, Vmin)

        self.ax.clear()
        self.ax.plot(V, I)
        self.ax.set_xlabel("Napięcie V [V]")
        self.ax.set_ylabel("Prąd I [A]")
        self.ax.set_title("Charakterystyka I–V diody")
        self.ax.grid(True)
        self.canvas.draw()


if __name__ == "__main__":
    root = tk.Tk()
    app = DiodeGUI(root)
    root.mainloop()
