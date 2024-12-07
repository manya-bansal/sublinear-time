#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip> // For std::hex
#include <iostream>
#include <random>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/properties.hpp>

#include "benchmark.h"

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, no_property,
                       property<edge_weight_t, double>>
    Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator edge_iterator;

// Should be enough for 95% confidence interval
#define SAMPLES 384

// #define DEBUG_

#ifdef DEBUG_
#define DEBUG(x) x;
#define RUNS 1
#define ITERATIONS 1
#else
#define DEBUG(x) // No-op if DEBUG is not defined
#define RUNS 5
#define ITERATIONS 1
#endif

typedef double (*HammingDistanceFunc)(int, int, unsigned *, unsigned *);

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

// Add k dummy arg to make it match interface of estimateHammingDistance
double preciseHammingDistance(int N, int k, unsigned *f1, unsigned *f2) {
  int disagreement = 0;
  for (int i = 0; i < N; i++) {
    if (f1[i] != f2[i]) {
      disagreement++;
    }
  }
  return static_cast<double>(disagreement) / N;
}

// Function to estimate the Hamming distance using random sampling
double estimateHammingDistance(int N, int k, unsigned *f1, unsigned *f2) {
  // Step 1: Initialize random number generator for sampling
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, N - 1); // Sample from 1 to N
  std::vector<int> randomNumbers;
  for (int i = 0; i < k; ++i) {
    randomNumbers.push_back(dis(gen));
  }
  std::sort(randomNumbers.begin(), randomNumbers.end());

  // Step 2: Count the number of disagreements in the sample
  int disagreements = 0;

  for (int i = 0; i < k; ++i) {
    int x = randomNumbers[i]; // Randomly pick a point from the domain
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

std::pair<Graph, std::vector<Edge>> diff(std::vector<char *> filenames,
                                         HammingDistanceFunc f, int size) {
  size_t num_files = filenames.size();
  Graph g((num_files * (num_files - 1)) / 2);
  for (size_t i = 0; i < num_files; i++) {

    char *data = filenames[i];
    for (size_t j = i + 1; j < num_files; j++) {
      char *cur_file = filenames[j];
      DEBUG(std::cout << "Hamming distance b/w " << filenames[i] << " and "
                      << filenames[j] << " is "
                      << f(size / (sizeof(unsigned)), SAMPLES, (unsigned *)data,
                           (unsigned *)cur_file)
                      << std::endl);
      add_edge(i, j,
               f(size / (sizeof(unsigned)), SAMPLES, (unsigned *)data,
                 (unsigned *)cur_file),
               g);
    }
  }
  std::vector<Edge> mst_edges;
  kruskal_minimum_spanning_tree(g, std::back_inserter(mst_edges));
  DEBUG(for (const auto &e : mst_edges) {
    std::cout << source(e, g) << " - " << target(e, g) << endl;
  })
  return {g, mst_edges};
}

double generate_percent_storage(Graph g, std::vector<Edge> mst_edges,
                                std::vector<std::string> filenames) {
  size_t total_bytes = 0;
  double compressed_bytes = 0;
  for (const auto &e : mst_edges) {
    DEBUG(std::cout << source(e, g) << " - " << target(e, g) << endl);
    total_bytes += 1;
    int size = 0;
    char *base = readFileToCharPtr(filenames[source(e, g)], size);
    char *comp = readFileToCharPtr(filenames[target(e, g)], size);
    double fraction = estimateHammingDistance(
        size / (sizeof(unsigned)), SAMPLES, (unsigned *)base, (unsigned *)comp);
    DEBUG(std::cout << "weight = " << fraction << std::endl;);
    if (fraction > 0.5) {
      compressed_bytes += 1;
      DEBUG(std::cout << "skipping" << std::endl);
      continue;
    }

    fraction = preciseHammingDistance(size / (sizeof(unsigned)), 0,
                                      (unsigned *)base, (unsigned *)comp);
    compressed_bytes += (fraction * 2);
  }
  return compressed_bytes / total_bytes;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file1> <file2> ... <fileN>"
              << std::endl;
    return 1; // Exit if no filenames are provided
  }

  std::vector<std::string> filenames;
  std::vector<char *> data;
  int size = 0;
  for (int i = 1; i < argc; ++i) {
    std::string filename = argv[i];
    filenames.push_back(filename);
    data.push_back(readFileToCharPtr(filename, size));
  }

  std::function<void(void)> func_sub = [&]() {
    diff(data, estimateHammingDistance, size);
  };

  std::function<void(void)> func_precise = [&]() {
    diff(data, preciseHammingDistance, size);
  };

  // std::cout << my_benchmark::benchmark(RUNS, ITERATIONS, func_sub) <<
  // std::endl; std::cout << my_benchmark::benchmark(RUNS, ITERATIONS,
  // func_precise)
  //           << std::endl;

  auto mst = diff(data, estimateHammingDistance, size);
  std::cout << generate_percent_storage(mst.first, mst.second, filenames)
            << std::endl;

  for (int i = 1; i < argc; ++i) {
    delete data[i];
  }

  return 0;
}
