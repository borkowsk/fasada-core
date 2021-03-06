/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
///
#define UNIT_IDENTIFIER "save_as_json"
#include "fasada.hpp"
#include "save_as_json.h"
#include "saver_processor.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fasada
{

save_as_json::save_as_json(const char* name):
    saver_processor(name)
{}

save_as_json::~save_as_json()
{}

void save_as_json::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)    //TODO
{
    bool        html=request.asHTML();
    if(html)
    {
       o+=ipc::string(EXT_PRE)+"htm\n";
       o+=getPageHeader(request.getFullPath())+"\n<PRE>\n";
    }

    std::string discPath=_make_path(".json",request);

    o+="SAVING INTO: \n";
    o+=discPath;

    pt::write_json( discPath, top);

    o+="\nDONE";//Jesli nie ma wyjatku...

    if(html)
    {
       o+="\n</PRE>\n";
       o+=getActionLink(request.getFullPath()+"?ls&html&long","LSL","List as long content of "+request["&path"])+"&nbsp;&nbsp; ";
       o+=getPageClosure(_compiled);
    }}

}//namespace "fasada"


/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/


