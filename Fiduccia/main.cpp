#include "include.hpp"

#include <chrono>
#include <filesystem>
#include <boost/program_options.hpp>
//#include <format>

namespace po = boost::program_options;
namespace fs = std::filesystem;

struct Settings {
  fs::path iFile;
  fs::path oFile;
  bool isMod;
  explicit operator bool () const {
    return fs::exists (iFile) && fs::exists (oFile);
  }
};

Settings parseArgs (int argc , char* argv []);

int main(int argc, char *argv[]) {
  Settings settings = parseArgs(argc, argv);
  if (!settings)
    return 0;

  //std::cout << std::format ("Input file: {}\nOutput file: {}\n" , 
  //  settings.iFile.string () , settings.oFile.string ());

  std::ifstream File{settings.iFile};
  std::ofstream OutFile{settings.oFile};

  HGraph Graph{File};
  Partitions Prt{Graph};

  auto const Start = std::chrono::steady_clock::now();
  unsigned const Iterations = FM(Graph, Prt);
  auto const End = std::chrono::steady_clock::now();

  OutFile << settings.iFile.string() << ' ' << Graph.getEdges().size() << ' '
        << Graph.getVertices().size() << ' ' << Prt.getCost() << ' '
        << (End - Start).count()<< ' ' << Iterations << '\n';
  return 0;
}

Settings parseArgs(int argc, char *argv[]) {
  // Declare the supported options.
  po::options_description desc ("Allowed options");
  desc.add_options ()
    ("help,h" , "How to use project")
    ("file,f" , po::value<fs::path> () , "Path to source file")
    ("output,o" , po::value<fs::path> () , "Path to output file")
    ("modified,m" , "Use modified algorithm");

  po::variables_map vm;
  po::store (po::parse_command_line (argc , argv , desc) , vm);
  po::notify (vm);

  Settings result;
  if (vm.count("file")) 
    result.iFile = vm["file"].as<fs::path> ();
  if (vm.count ("output"))
    result.oFile = vm["output"].as<fs::path> ();
  result.isMod = vm.count ("modified");

  if (vm.count ("help") || !result)
    std::cout << desc << "\n";
  
  return result;
}