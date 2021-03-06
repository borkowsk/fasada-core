/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
///
#define UNIT_IDENTIFIER "tree_attributes"
#include "fasada.hpp"
#include "tree_processor.h"
#include "tree/ptree_foreach.hpp"

namespace pt = boost::property_tree;

namespace fasada
{

/// Each node may have special subnodes called properties saved under xmlattr subtree
/// Most important properties is "_source", "loader", "saver", "alternative_savers", "oth_actions".
/// Standard attributes are presented in HTML by _implement_attributes() method
/// Maybe this should be the same as <xmlattr> but this is disputable...
/// https://stackoverflow.com/questions/18875437/writing-more-complex-than-trivial-xml-with-boost-property-tree
///
void insert_property(pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& PropertyValue)
{
    Node.put_child(std::string("attributes.")+FasadaPropertyName,pt::ptree{PropertyValue});
}

const std::string& get_property(const pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& WhenNotPresent)
{
    auto child=Node.find(std::string("attributes.")+FasadaPropertyName);

    if(child==Node.not_found())
        return WhenNotPresent;

     return child->second.data();
}

void tree_processor::_implement_attributes(ShmString& o,const pt::ptree& top,URLparser& request,std::string nameOfTop/*=""*/)
{
    auto attr=top.find("attributes");

    if(attr!=top.not_found())
    {
        if(nameOfTop.length()>0 && nameOfTop.at(0)!='/')
            nameOfTop="/"+nameOfTop;

        for_true_branches(attr->second, //Search only in attributes!
                          std::string("attr"),//Just cosmetic meaning
                          [&o,&request,&nameOfTop](const ptree& t,std::string k)
        {
            auto posit=k.rfind("/");

            if(posit==k.npos)
                return true;//RATHER INVALID, BUT MAYBE IS SOMETHING BELOW

            std::string field=k.substr(posit+1);
            if(field.at(0)=='_')
                return false;//HIDDEN SUBTREE

            if(t.data()!="")
                o+="&nbsp;"+getActionLink(request.getFullPath()+nameOfTop+"?"+t.data()+"&html",field,k+": "+t.data());

            return true;
        },
                          std::string("/") //PATH SEPARATOR
                         );
     }
}

//Currently used atrribute actions:
/*
 insert_property(top,"oth_actions.Check_All_Files", "runTree&action=checkFile");
  insert_property(top,"oth_actions.Load_All_Jsons","runTree&action=Json");

 insert_property(top,"oth_actions.decode","ContentAsTabDelimited");
 insert_property(top,"oth_actions.decode","decodeCsvContent");

 insert_property(top,"alternative_savers.json","saveAsJson");
 insert_property(top,"saver", "saveJson");
 insert_property(top,"alternative_savers.xml","saveAsXml");
 insert_property(top,"saver", "saveCsv");
 insert_property(top,"saver", "saveTxt");

 insert_property(top,"viewer","viewCsv");
*/


} //namespace fasada


/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/

