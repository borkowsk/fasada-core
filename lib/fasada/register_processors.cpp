/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
/// See CURRENT licence file!
///

//Procesory danych json w postaci STATYCZNYCH zmiennych lokalnych autorejestrowanych w konstruktorach
#define UNIT_IDENTIFIER "register_processors"

#include "fasada.hpp"
#include "format_toolbox.h" //getNodePanel

#include "processor_get.h"
#include "processor_set.h"
#include "processor_add.h"
#include "processor_del.h"
#include "processor_ren.h"
#include "processor_ls.h"
#include "processor_dfs.h"
#include "processor_find.h"
#include "processor_info.h"
#include "processor_check_file.h"
#include "loader_txt.h"
#include "loader_csv.h"
#include "loader_json.h"
#include "view_csv.h"
#include "save_as_csv.h"
#include "save_as_txt.h"
#include "save_as_json.h"
#include "save_as_xml.h"

namespace fasada
{

static int _fasflags=0;

void init(bool WithWriters)
//Initialize the fasada::library
{
    register_processors(WithWriters);
    _fasflags |= tree_processor::READER;
    if(WithWriters)
         _fasflags |= tree_processor::WRITER;
}

bool writing_enabled()
{
    return (_fasflags & tree_processor::WRITER)!=0;
}

void register_processors(bool WithWriters)
{
    static fasada::processor_info     INFO;//Istnieje co najmniej jedna zmienna każdego typu
    static fasada::processor_get      GET; //--//---
    static fasada::processor_dfs      DFS; //--//---
    static fasada::processor_ls       LST; //--//---
    static fasada::processor_find     FIND;//--//---
    static fasada::view_csv           VIEW_CSV;
    static fasada::processor_check_file CHECK_FILE;//--//---
    //static fasada::save_as_txt        SAVE_TXT;//still TODO
    //static fasada::save_as_csv        SAVE_CSV;//still TODO
    static fasada::save_as_json       SAVE_JSON;
    static fasada::save_as_xml        SAVE_XML;
                                                            //ale można też robić aliasy
    static fasada::processor_ls       Def("default");
    static fasada::processor_ls       Dir("dir");
    static fasada::processor_ls       Lst("lst");
    static fasada::processor_dfs     Tree("tree");
    static fasada::processor_info    FasadaInfo("fasada_info");
    static fasada::save_as_json      saveJson("saveJson");

    if(WithWriters)
    {
        static fasada::processor_set  SET;//Istnieje co najmniej jedna zmienna każdego typu
        static fasada::processor_add  ADD;//--//---
        static fasada::processor_del  DEL;//--//---
        static fasada::processor_ren  REN;//--//---
        static fasada::loader_txt     TXT;//--//---
        static fasada::loader_csv     CSV;//--//---
        static fasada::loader_json   JSON;//--//---
                                                                //ale można zrobić aliasy
        static fasada::loader_txt     Txt("Txt");//--//---
        static fasada::loader_csv     Csv("Csv");//--//---
        static fasada::loader_json   Json("Json");//--//---
        static fasada::loader_csv    LinkedInCsv("LinkedInCsv");
        static fasada::loader_json   TwitterJson("TwitterJson");//Json'y z Twittera mają takie cosie na początku "window.YTD.tweet.part0 ="
                                                                //które wywalają boostowy parser, ale już je usuwamy i wpisujemy jako atrybut _lead
    }
}

std::string  getNodePanel(const pt::ptree& node,const std::string& fullpath,URLparser& request)
{
    std::string o;
    const std::string& data=node.data();
    std::string proplong=(request.asLONG()?"&long":"");

    o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"?get&html&long","?","Get value");

    if(data=="")    //Czy to liść czy (potencjalny) węzeł?
    {
        if(node.size()>0)
        {
          o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"?ls&html"+proplong,"&hellip;","Print as list");
          o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"?dfs&html"+proplong,"&forall;","Print as tree");
        }

        o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"?add&html","+","Add!");

        auto attr=node.find("local_uri");
        if(attr!=node.not_found())
        {
            //Łatwy podgląd już sprawdzonego pliku. TODO
            o+="&nbsp; "+format_toolbox::getActionLink(attr->second.data(),"&#x1f441;","See as file "
                                       +std::string(attr->first.data())+":"+attr->second.data());//HTML Eye https://www.fileformat.info/info/unicode/char/1f441/index.htm
        }
    }
    else
    {
        if(writing_enabled() && data.at(0)=='!')
        {
            bool haveQ=(data.find('?',0)!=data.npos);//Is parameter list already started?
            o+="&nbsp; "+format_toolbox::getActionLink(fullpath+data+(haveQ?"&html":"?self&html"),"RUN!","Run link read/write");
        }
        else
        if(data.at(0)=='?')
        {
            o+="&nbsp; "+format_toolbox::getActionLink(fullpath+data+"&html","run","Run link as read only");
        }
        else
        if(format_toolbox::isLink(data))
        {
            o+="&nbsp; "+format_toolbox::getActionLink(data,"&#x1F30D;","Follow link");//&#x1F30D; :globe:
        }
        else
        if(format_toolbox::isLocalFile(data))
        {
            if(data.at(0)=='/' || (data.at(0)=='.' && data.at(1)=='/' ))
                o+="&nbsp; "+format_toolbox::getSeeLink(data,request,"&#x1f441;");//Plik sprawdzony

            else  if(writing_enabled())
                o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"!checkFile?self&html","check!");//Plik do sprawdzenia
        }

        auto codeContent=urlEncode(data);//TODO - raczej enkodowane powinny być całe linki CHECK IT!
        auto serverTop=request["&protocol"]+"://"+request["&domain"]+':'+request["&port"];//FIND!
        o+="&nbsp; "+format_toolbox::getActionLink(serverTop+"?find&html&sugestion="+codeContent,"&#x1F50D;","Find similar i values or node names");
    }

    if(writing_enabled())
        o+="&nbsp; "+format_toolbox::getActionLink(fullpath+"?set&html","=","Set value");

    std::string parentpath=fullpath.substr(0,fullpath.rfind("/"));
    o+="&nbsp;"+format_toolbox::getActionLink(parentpath+"?ls&html"+proplong,"&isin;","Is node of "+parentpath);

    return o;
}

}
