//------------------------------------------------------------------------------
//
// driver.cpp -- main entry point
//
//------------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <boost/program_options.hpp>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

#include "parser.hpp"
#include <fstream>

IScope* currentScope = nullptr;

llvm::LLVMContext* Context;
llvm::IRBuilder<>* Builder;
llvm::Module* Module;
llvm::Function* mainFunction;

static int currentinlinePos = 0;
std::string ParseOptions (int argc , char* argv []);

class Printer
{
  public:
  Printer ()
  {
    // @.str = constant [4 x i8] c"%d\0A\00", align 1
    llvm::Type* PRINT_STRTy = llvm::ArrayType::get (
      Builder->getInt8Ty () , 4);
    Module->getOrInsertGlobal (".int_string" , PRINT_STRTy);
    int_string = Module->getNamedGlobal (".int_string");
    int_string->setLinkage (llvm::GlobalValue::InternalLinkage);
    int_string->setAlignment (llvm::MaybeAlign (1));
    int_string->setInitializer (llvm::ConstantDataArray::getString (*Context , "%d\n" , true));

    Module->getOrInsertGlobal (".char_string" , PRINT_STRTy);
    char_string = Module->getNamedGlobal (".char_string");
    char_string->setLinkage (llvm::GlobalValue::InternalLinkage);
    char_string->setAlignment (llvm::MaybeAlign (1));
    char_string->setInitializer (llvm::ConstantDataArray::getString (*Context , "%s\n" , true));

    llvm::FunctionType* PrintfTy = llvm::FunctionType::get (
    Builder->getInt32Ty () ,
    llvm::ArrayRef<llvm::Type*>{} ,
    true);
    llvm::FunctionCallee PrintfTyCallee =
      Module->getOrInsertFunction ("printf" , PrintfTy);
    llvm::FunctionType* PrintTy = llvm::FunctionType::get (
        Builder->getVoidTy () ,
        Builder->getInt32Ty () ,
        false);

    int_print = llvm::Function::Create (
        PrintTy , llvm::Function::ExternalLinkage , "__print_int" , Module);
    llvm::BasicBlock* PrintBB =
      llvm::BasicBlock::Create (*Context , "" , int_print);
    Builder->SetInsertPoint (PrintBB);
    llvm::Value* PrintArg = static_cast<llvm::Value*> (int_print->arg_begin ());
    Builder->CreateCall (PrintfTyCallee , llvm::ArrayRef<llvm::Value*>{
      int_string , PrintArg});
    Builder->CreateRetVoid ();

    //llvm::FunctionType* PrintTy2 = llvm::FunctionType::get (
    //Builder->getVoidTy () ,
    //llvm::ArrayRef<llvm::Type*>{} ,
    //true);
    //char_print = llvm::Function::Create (
    //PrintTy2 , llvm::Function::ExternalLinkage , "__print_char" , Module);
    //llvm::BasicBlock* PrintBB2 =
    //  llvm::BasicBlock::Create (*Context , "" , char_print);
    //Builder->SetInsertPoint (PrintBB2);
    //llvm::Value* PrintArg2 = static_cast<llvm::Value*> (char_print->arg_begin ());
    //Builder->CreateCall (PrintfTyCallee , llvm::ArrayRef<llvm::Value*>{
    //  char_string , PrintArg2});
    //Builder->CreateRetVoid ();
  }

  private:
  llvm::GlobalVariable* int_string = nullptr;
  llvm::GlobalVariable* char_string = nullptr;
  llvm::Function* int_print;
  //llvm::Function* char_print;
};

int main (int argc , char* argv [])
{
  auto fileName = ParseOptions (argc , argv);
  if (fileName.empty ())
    return 1;

  FILE* f = fopen (fileName.data () , "r");
  if (!f)
  {
    perror ("Cannot open file");
    return 1;
  }
  yyin = f;
  currentScope = create_scope ();

  Context = new llvm::LLVMContext;
  Module = new llvm::Module ("pcl.module" , *Context);
  Builder = new llvm::IRBuilder<> (*Context);

  Printer ();

  // prototype for scan function
  llvm::FunctionType* FTScan = llvm::FunctionType::get (
      llvm::Type::getInt32Ty (*Context) , /* va args? */ false);

  llvm::Function::Create (FTScan , llvm::Function::ExternalLinkage , "__pcl_scan" ,
                         Module);

  // single __pcl_start function for void module
  llvm::FunctionType* FT = llvm::FunctionType::get (
      Builder->getInt32Ty () , /* va args? */ false);

  mainFunction = llvm::Function::Create (FT , llvm::Function::ExternalLinkage ,
                                           "main" , Module);

  // basic block to start instruction insertion
  llvm::BasicBlock* BB =
    llvm::BasicBlock::Create (*Context , "" , mainFunction);
  Builder->SetInsertPoint (BB);

  yyparse ();

  Builder->CreateRet (llvm::ConstantInt::get (Builder->getInt32Ty () , 0));

  std::ostringstream s;
  s << std::filesystem::path (fileName).filename ().string () << ".ll";

  std::cout << "Saving module to: " << s.str () << "\n";

  std::error_code EC;
  llvm::raw_fd_ostream outfile { s.str (), EC };
  if (EC)
  {
    llvm::errs () << EC.message ().c_str () << "\n";
  }

  Module->print (outfile , nullptr);
  outfile.close ();

  if (outfile.has_error ())
  {
    llvm::errs () << "Error printing to file: " << outfile.error ().message ()
      << "\n";
  }

  fclose (f);

  delete Builder;
  delete Context;
  delete currentScope;
  return 0;
}

void PrintError (char const* errmsg)
{
  std::cerr << "Error: " << errmsg << " - Line " << yylineno << ", Column "
    << currentinlinePos << std::endl;
  exit (1);
}

void BeginToken (char* t , int* yyinlinePos)
{
  yylloc.first_line = yylineno;
  yylloc.first_column = *yyinlinePos;
  yylloc.last_line = yylineno;
  *yyinlinePos += strlen (t);
  yylloc.last_column = *yyinlinePos;

  currentinlinePos = *yyinlinePos;
}


namespace po = boost::program_options;
std::string ParseOptions (int argc , char* argv [])
{
  po::options_description desc ("Allowed options");
  desc.add_options ()
    ("help" , "Usage: ./pcl <file>.pcl");
  desc.add_options ()
    ("file,f" , po::value<std::string> () , "File to compile");

  po::variables_map args;
  po::store (
      po::command_line_parser (argc , argv).options (desc).run () ,
      args
  );
  po::notify (args);

  if (args.count ("file"))
    return  args["file"].as< std::string> ();
  else
    std::cout << desc;

  return std::string {};
}