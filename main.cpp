#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// Function to read the content of a file into a char*
char *readFileToCharPtr(const std::string &filename, int &size) {
  std::ifstream file(filename,
                     std::ios::binary |
                         std::ios::ate); // Open file in binary and at end mode

  if (!file) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return nullptr;
  }

  // Get the size of the file
  int fileSize = file.tellg();
  size = fileSize;
  file.seekg(0, std::ios::beg); // Move the file pointer back to the beginning

  // Allocate memory for the file content
  char *buffer = new char[fileSize];

  // Read the file content into the buffer
  file.read(buffer, fileSize);
  if (!file) {
    std::cerr << "Error: Failed to read the entire file " << filename
              << std::endl;
    delete[] buffer; // Clean up memory
    return nullptr;
  }

  return buffer;
}

double preciseHammingDistance(int N, char *f1, char *f2) {
  int disagreement = 0;
  for (int i = 0; i < N; i++) {
    if (f1[i] != f2[i]) {
      disagreement++;
    }
  }
  return static_cast<double>(disagreement) / N;
}

// Function to estimate the Hamming distance using random sampling
double estimateHammingDistance(int N, int k, char *f1, char *f2) {
  // Step 1: Initialize random number generator for sampling
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, N); // Sample from 1 to N

  // Step 2: Count the number of disagreements in the sample
  int disagreements = 0;

  for (int i = 0; i < k; ++i) {
    int x = dis(gen); // Randomly pick a point from the domain
    if (f1[x] != f2[x]) {
      disagreements++;
    }
  }

  // Step 3: Estimate the fraction of disagreements
  double p_hat = static_cast<double>(disagreements) / k;

  // Step 4: Estimate the total Hamming distance using the Chernoff bound
  double totalHammingDistanceEstimate = p_hat;

  return totalHammingDistanceEstimate;
}

void precise_diff(std::vector<std::string> filenames) {

  size_t num_files = filenames.size();
  for (size_t i = 0; i < num_files; i++) {
    int size = 0;
    char *data = readFileToCharPtr(filenames[i], size);
    for (size_t j = i + 1; j < num_files; j++) {
      int size_local = 0;
      char *cur_file = readFileToCharPtr(filenames[j], size_local);
      assert(size == size_local);
      std::cout << "Precise hamming distance b/w " << filenames[i] << " and "
                << filenames[j] << " is "
                << preciseHammingDistance(size, data, cur_file) << std::endl;
      delete cur_file;
    }
    delete data;
  }
}

void sublinear_diff(std::vector<std::string> filenames) {

  size_t num_files = filenames.size();
  for (size_t i = 0; i < num_files; i++) {
    int size = 0;
    char *data = readFileToCharPtr(filenames[i], size);
    for (size_t j = i + 1; j < num_files; j++) {
      int size_local = 0;
      char *cur_file = readFileToCharPtr(filenames[j], size_local);
      assert(size == size_local);
      std::cout << "Estimated hamming distance b/w " << filenames[i] << " and "
                << filenames[j] << " is "
                << estimateHammingDistance(size, 200, data, cur_file)
                << std::endl;
      delete cur_file;
    }
    delete data;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file1> <file2> ... <fileN>"
              << std::endl;
    return 1; // Exit if no filenames are provided
  }

  std::vector<std::string> filenames;
  for (int i = 1; i < argc; ++i) {
    std::string filename = argv[i];
    filenames.push_back(filename);
  }

  sublinear_diff(filenames);
  precise_diff(filenames);
  return 0;
}
