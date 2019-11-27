#include "fasada.hpp"
#include "tree_processor.h"
#include "loader_txt.h"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

using namespace boost::filesystem;

#if(0)
#include <boost/filesystem/string_file.hpp> //only new boost
namespace fasada
{

#else
namespace fasada
{

inline
void load_string_file(const path& p, std::string& str)
{
  ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(p, std::ios_base::binary);
  std::size_t sz = static_cast<std::size_t>(file_size(p));
  str.resize(sz, '\0');
  file.read(&str[0], sz);
}
#endif

loader_txt::loader_txt(const char* name):
    tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{}

void loader_txt::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("READ FUNCTION FOR PTREE PROCESSOR "+procName+" NOT IMPLEMENTED!"));
}

void loader_txt::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string discPath=request["&private_directory"]+request["&path"];
    boost::replace_all(discPath,"//","/");
    unsigned    noc=top.size();//czy ma elementy składowe?

    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT LOAD NOT-LEAF NODE!"));
    }

    //....
    o+="LOADING:";
    o+=discPath;
    o+="\n";

    std::string content;
    //boost::filesystem::
    load_string_file(discPath,content);

    top.add_child("_content",pt::ptree{content});

    //Jeśli nie ma wyjątku to nazwę procesora likwidujemy
    top.data()="";

    // Most important properties is "source", "loader", "viever", "saver", "alternative_savers", "oth_actions":
    insert_property(top,"_source",discPath);
    insert_property(top,"loader",procName);
    insert_property(top,"saver", "saveTxt");
    insert_property(top,"alternative_savers.json","saveAsJson");
    insert_property(top,"alternative_savers.xml","saveAsXml");
    insert_property(top,"oth_actions.decode","ContentAsTabDelimited");

    o+="DONE";
}


}
