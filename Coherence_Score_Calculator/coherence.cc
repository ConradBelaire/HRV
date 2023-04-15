#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <tuple>
#include <algorithm>
#include <fftw3.h>
std::vector<double> calculatePowerSpectrum(const std::vector<double>& heartRates);
double calculateCoherenceRatio(const std::vector<double>& powerSpectrum);
std::tuple<double, std::string> getCoherenceLevel(double coherenceScore, int challengeLevel);

std::vector<double> calculatePowerSpectrum(const std::vector<double>& heartRates) {
    int n = heartRates.size();
    fftw_complex *in, *out;
    fftw_plan plan;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

    for (int i = 0; i < n; i++) {
        in[i][0] = heartRates[i];
        in[i][1] = 0;
    }

    plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    std::vector<double> powerSpectrum(n);
    for (int i = 0; i < n; i++) {
        powerSpectrum[i] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return powerSpectrum;
}

double calculateCoherenceRatio(const std::vector<double>& powerSpectrum) {
    // Calculate coherence ratio based on the provided criteria
    // Placeholder values, replace with the actual calculation.
    double peakPower = 0.0;
    double totalPower = 0.0;

    // Find the maximum peak in the 0.04-0.26 Hz range and calculate the total power
    for (double power : powerSpectrum) {
        if (power > peakPower) {
            peakPower = power;
        }
        totalPower += power;
    }

    // Calculate the coherence ratio
    double coherenceRatio = peakPower / (totalPower - peakPower);
    return coherenceRatio;
}

double calculateCoherenceScore(const std::vector<double>& heartRates) {
    std::vector<double> rrIntervals(heartRates.size() - 1);
    for (size_t i = 0; i < heartRates.size() - 1; ++i) {
        rrIntervals[i] = 60000.0 / heartRates[i + 1] - 60000.0 / heartRates[i];
    }

    std::vector<double> powerSpectrum = calculatePowerSpectrum(rrIntervals);
    double coherenceRatio = calculateCoherenceRatio(powerSpectrum);

    // Scale the coherence ratio to a coherence score between 0 and 16
    double coherenceScore = coherenceRatio * 16.0;
    return coherenceScore;
}

std::tuple<double, std::string> getCoherenceLevel(double coherenceScore, int challengeLevel) {
    double lowThreshold = 0.0;
    double highThreshold = 0.0;

    // Set the thresholds for low, medium, and high coherence levels based on the challenge level
    switch (challengeLevel) {
        case 1:
            lowThreshold = 0.5;
            highThreshold = 0.9;
            break;
        case 2:
            lowThreshold = 0.6;
            highThreshold = 2.1;
            break;
        case 3:
            lowThreshold = 1.8;
            highThreshold = 4.0;
            break;
        case 4:
            lowThreshold = 4.0;
            highThreshold = 6.0;
            break;
    }

    if (coherenceScore < lowThreshold) {
        return std::make_tuple(coherenceScore, "low");
    } else if (coherenceScore >= lowThreshold && coherenceScore <= highThreshold) {
        return std::make_tuple(coherenceScore, "medium");
    } else {
        return std::make_tuple(coherenceScore, "high");
    }
}


int main() {
    std::vector<double> heartRates = {80, 75, 72, 78, 70, 68, 74, 76, 69, 71};
    int challengeLevel = 2;

    double coherenceScore = calculateCoherenceScore(heartRates);
    std::tuple<double, std::string> coherenceLevel = getCoherenceLevel(coherenceScore, challengeLevel);

    std::cout << "Coherence Score: " << std::get<0>(coherenceLevel) << std::endl;
    std::cout << "Coherence Level: " << std::get<1>(coherenceLevel) << std::endl;

    return 0;
}

