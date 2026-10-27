#include "VectorProcessor.h"

double VectorProcessor::multiply(const std::vector<double>& vec) {
    if (vec.empty()) return 1.0;
    
    double result = 1.0;
    for (double val : vec) {
        result *= val;
    }
    return result;
}
