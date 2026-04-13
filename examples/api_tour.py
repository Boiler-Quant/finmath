"""Short demo after building or installing finmath."""

import finmath as fm


def main() -> None:
    print("finmath", getattr(fm, "__version__", "?"))
    print("SIMD backend:", fm.get_simd_backend())
    ci = fm.compound_interest(1000.0, 5.0, 10, 4)
    print("compound_interest:", ci)
    call = fm.black_scholes(fm.OptionType.CALL, strike=100.0, spot=105.0, time=1.0, rate=0.05, volatility=0.2)
    print("Black-Scholes call:", call)
    P = [[0.97, 0.03], [0.15, 0.85]]
    print("Markov steady state:", fm.markov_steady_state(P))


if __name__ == "__main__":
    main()
