//
//  rungekutta.h
//  Spring
//
//  Created by Matthew Dillard on 11/4/15.
//
//

#ifndef rungekutta_h
#define rungekutta_h

namespace RK {
    state::vector one(const state::vector& S, const double t, state::vector (*F)(const state::vector&, const double)) {
        return S + (F(S, t) * t);
    }

    state::vector two(const state::vector& S, const double t, state::vector (*F)(const state::vector&, const double)) {
        state::vector K1 = F(S, t);
        state::vector K2 = F(S + (K1 * (t / 2)), t/2.0);
        return S + (K2 * t);
    }

    state::vector four(const state::vector& S, const double t, state::vector (*F)(const state::vector&, const double)) {
        state::vector K1 = F(S, t);
        state::vector K2 = F(S + (K1 * (t / 2)), t/2.0);
        state::vector K3 = F(S + (K2 * (t / 2)), t/2.0);
        state::vector K4 = F(S + (K3 * t), t);
        return S + ((K1 + (K2 * 2) + (K3 * 2) + K4) * (t / 6));
    }
}

#endif /* rungekutta_h */
