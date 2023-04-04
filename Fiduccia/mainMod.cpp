#include "includeMod.hpp"

#include <chrono>
#include <filesystem>
//#include <format>

namespace fs = std::filesystem;

struct Settings {
  fs::path iFile;
  fs::path oFile;
  bool isMod;
  explicit operator bool () const {
    return fs::exists (iFile);
  }
};

Settings parseArgs (int argc , char* argv []);

int main(int argc, char *argv[]) {
  Settings settings = parseArgs(argc, argv);
  if (!settings) {
    std::cout << "FileIn wasn't found!";
    return 1;
  }

  std::ifstream File{settings.iFile};
  std::ofstream OutFile{settings.oFile};

  HGraph Graph{File};
  Partitions Prt{Graph};

  auto const Start = std::chrono::steady_clock::now();
  unsigned const Iterations = Algorithm(Graph, Prt);
  auto const End = std::chrono::steady_clock::now();

  OutFile << settings.iFile.string() << ' ' << Graph.getEdges().size() << ' '
        << Graph.getVertices().size() << ' ' << Prt.getCost() << ' '
        << (End - Start).count()<< ' ' << Iterations << '\n';
  return 0;
}

Settings parseArgs(int argc, char * argv[]) {
  Settings set;
  if (argc != 2) {
    std::cout << "Please select files: <bin> <fileIn>\n";
    return set;
  }
  std::string file = std::string(argv[1]);
  set.iFile = fs::path(file);
  set.oFile = fs::path(file + ".part.2");
  std::cout << "Input file: " << fs::absolute(set.iFile) << "\nOutput file: "
  << fs::absolute(set.oFile) << "\n";
  return set;
}

// For Windows usage I has to abondone it :(

//Settings parseArgs(int argc, char *argv[]) {
//  // Declare the supported options.
//  po::options_description desc ("Allowed options");
//  desc.add_options ()
//    ("help,h" , "How to use project")
//    ("file,f" , po::value<fs::path> () , "Path to source file")
//    ("output,o" , po::value<fs::path> () , "Path to output file")
//    ("modified,m" , "Use modified algorithm");
//
//  po::variables_map vm;
//  po::store (po::parse_command_line (argc , argv , desc) , vm);
//  po::notify (vm);
//
//  Settings result;
//  if (vm.count("file"))
//    result.iFile = vm["file"].as<fs::path> ();
//  if (vm.count ("output"))
//    result.oFile = vm["output"].as<fs::path> ();
//  result.isMod = vm.count ("modified");
//
//  if (vm.count ("help") || !result)
//    std::cout << desc << "\n";
//
//  return result;
//}
