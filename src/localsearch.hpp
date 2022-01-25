#pragma once

unsigned bi(const Solution &S) {
    pair<int, unsigned> best = make_pair(0, S.x.size()); // delta, index
    unsigned nbest = 0;
    for(unsigned i = 0; i < S.x.size(); i++)
        if(S.flipvalue(i) < get<0>(best)) {
            best = make_pair(S.flipvalue(i), i);
            nbest = 1;
        } else if(get<1>(best) != S.x.size() && S.flipvalue(i) == get<0>(best)) {
            nbest++;
            if((nbest) * random01(rng) <= 1.0)
                best = make_pair(S.flipvalue(i), i);
        }
    return get<1>(best);
}

template<typename Improvement>
unsigned localsearch(Solution &S, Improvement improve, chrono::system_clock::time_point start, int target) {
    chrono::system_clock::time_point last_report;
    unsigned i, steps = 0;
    Solution B(S.I);
    B = S;
    report.newBestKnownValue(B.value);

    do {
        if(S.value <= target)
            break;
        if(chrono::system_clock::now() - start > chrono::duration<int>(timeLimit))
            break;
        steps++;
        i = improve(S);
        if(!isValid(i, S))
            break;
        S.flip(i);
        if(S.value < B.value) {
            B = S;
            B.time = chrono::system_clock::now();
            report.newBestKnownValue(B.value);
        }
    } while (true);
    steps--;
    S = B;
    return steps;
}

void perturbRandomNew(Solution &S, unsigned perturbationSize) {
    vector<bool> flipped(S.x.size(), false);
    unsigned r = perturbationSize;
    while (r-- > 0) {
        unsigned i = randomInt(rng) % S.x.size();
        if (flipped[i] == false) {
            S.flip(i);
            flipped[i] = true;
        }
    }
}

template<typename Improvement, typename Perturbation>
unsigned
iterated_localsearch(Solution &S, Improvement improve, Perturbation perturb, chrono::system_clock::time_point start,
                     int target) {
    chrono::system_clock::time_point last_report;
    unsigned steps = 0;
    Solution B(S.I);
    B = S;
    report.newBestKnownValue(B.value);
    
    do {
        if(S.value <= target)
            break;
        if(chrono::system_clock::now() - start > chrono::duration<int>(timeLimit))
            break;
        
        steps++;
        perturb(S);
        improve(S);
       
        if(S.value < B.value) {
            B = S;
            B.time = chrono::system_clock::now();
            report.newBestKnownValue(B.value);
        }
    } while (true);
    steps--;
    S = B;
    return steps;
}