/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///

#ifndef FASADA_TREE_H
#define FASADA_TREE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <map>

namespace fasada
{
    namespace pt = boost::property_tree;
    /// Each node in fasada data files may have special subnodes called properties saved under xmlattr subtree
    /// Most important properties is "_source", "loader", "viewer", "saver", "alternative_savers", "oth_actions".
    /// Standard attributes are presented in HTML by _implement_attributes() method
    void insert_property(pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& PropertyValue);
    const std::string& get_property(const pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& WnenNotPresent);
    std::string  getNodePanel(const pt::ptree& node,const std::string& fullpath,URLparser& request);
}//namespace "fasada"

/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/
#endif
