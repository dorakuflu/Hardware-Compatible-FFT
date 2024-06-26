#include <iostream>
#include <fstream>
#include <cmath>

std::pair <double, double> * fft (std::pair <double, double> * x, int N);\
void bitRevArr(std::pair <double, double> * x, int N);
void butterfly(std::pair <double, double> * x, int i, int j, int k);

int main(int argc, char * argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <N> <input.txt>" << std::endl;
        return 1;
    }

    const int N = atoi(argv[1]);
    std::pair <double, double> * x = new std::pair <double, double> [N];

    std::ifstream input(argv[2]);
    if (!input.is_open()) {
        std::cout << "Error: cannot open file " << argv[2] << std::endl;
        return 1;
    }

    double real, imag;
    for (int i = 0; i < N; i++) {
        input >> real >> imag;
        x[i] = std::make_pair(real, imag);
    }

    input.close();

    std::pair <double, double> * X = fft(x, N);

    delete [] x;
    for (int i = 0; i < N; i++) {
        std::cout << X[i].first << " " << X[i].second << std::endl;
    }
    delete [] X;

    return 0;
}

std::pair <double, double> * fft (std::pair <double, double> * x, int N) {
    std::pair <double, double> * X = new std::pair <double, double> [N];
    int stages = log2(N);
    bitRevArr(x, N);

    for (int i = 0; i < stages; i++) {

        int blocks = N / pow(2, i + 1);
        int range = pow(2, i);

        for (int j = 0; j < blocks; j++) {
            for (int k = 0; k < range; k++) {
                butterfly(x, range, j, k);
            }
        }
    }
    for (int i = 0; i < N; i++) {
        X[i] = x[i];
    }

    return X;
}

void bitRevArr(std::pair <double, double> * x, int N) {
    std::pair <double, double> * temp = new std::pair <double, double> [N];
    int numBits = log2(N);
    
    for (int i = 0; i < N; i++) {
        int revIndex = 0;
        for (int j = 0; j < numBits; j++) {
            revIndex |= ((i >> j) & 1) << (numBits - 1 - j);
        }
        temp[revIndex] = x[i];
    }

    for (int i = 0; i < N; i++) {
        x[i] = temp[i];
    }

    delete [] temp;
}

void butterfly(std::pair <double, double> * x, int range, int j, int k) {
    int index = j * range * 2 + k;
    int index1 = index + range;
    int N = range * 2;

    std::pair <double, double> w;
    w.first = cos(2 * M_PI * k / N);
    w.second = -sin(2 * M_PI * k / N);

    std::pair <double, double> temp; // w * x[index1]
    temp.first = w.first * x[index1].first - w.second * x[index1].second;
    temp.second = w.first * x[index1].second + w.second * x[index1].first;

    x[index1].first = x[index].first - temp.first;
    x[index1].second = x[index].second - temp.second;
    x[index].first = x[index].first + temp.first;
    x[index].second = x[index].second + temp.second;

}

