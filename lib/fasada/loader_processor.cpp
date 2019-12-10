/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "fasada.hpp"
#include "loader_processor.h"
#include <boost/lexical_cast.hpp>
#include <string>

namespace fasada
{

loader_processor::loader_processor(const char* name):
        form_processor(name)
{}

loader_processor::~loader_processor()
{}


//default HTML form for this processor
std::string loader_processor::Form=
        ""
        ;

std::string replace_all_variables(std::string template_version,URLparser& request)
{
    //MODYFIKACJE ZMIENNYMI Z REQUEST
    //...
    return template_version;//Już zmodyfikowana
}

void loader_processor::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)   //TODO
//Wspólna funkcja formularza dla wszystkich loaderów
{
    o+=replace_all_variables(Form,request);
    //throw(tree_processor_exception("PTREE LOADER PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A READER!"));
//See: https://www.boost.org/doc/libs/1_32_0/libs/filesystem/doc/operations.htm#last_write_time
//OR   https://en.cppreference.com/w/cpp/filesystem/last_write_time  
}


void loader_processor::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
