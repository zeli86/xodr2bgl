/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <iostream>
#include <sstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
using namespace std;

struct Trans {
    int t;
};

struct EdgeInfoProperty {
    int score;
    //is the trans from v to u, where u<v
    Trans trans;
};

struct NodeInfoProperty {
    int index;
};

/* you cannot read a graph without node property. By default, read_graphviz() assume
 * nodes have a "node_id" property map...
 */
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, NodeInfoProperty, EdgeInfoProperty > Graph;

template<class Index> class noeud_writer {
public:
    noeud_writer(Index id) : idm(id){}
    template<class Noeud> void operator()(std::ostream & out, const Noeud & n) {
        out << "[index=" << idm[n] << "]"; }
private:
    Index idm;
};

template< class IdMap> inline noeud_writer<IdMap> make_noeud_writer(IdMap idm) {
    return noeud_writer<IdMap>(idm); }

template<class Score> class edge_writer {
public:
    edge_writer(Score s) : score(s){}
    template<class Noeud> void operator()(std::ostream & out, const Noeud & n) {
        out << "[score=" << score[n] << "]"; }
private:
    Score score;
};

template< class Score> inline edge_writer<Score> make_edge_writer(Score score) {
    return edge_writer<Score>(score); }


int main(int argc, char * argv[])
{
    Graph G(0);
    boost::dynamic_properties dp;
    dp.property("index", boost::get(&NodeInfoProperty::index,G));

    auto score = boost::get(&EdgeInfoProperty::score, G);
    dp.property("score", score);

    std::istringstream isg("graph G {0[index=0];1[index=1];0--1[score=-3];}");
    bool status = boost::read_graphviz(isg, G, dp,"index");

    write_graphviz(std::cout, G, make_noeud_writer(boost::get(&NodeInfoProperty::index, G)),
                                  make_edge_writer(boost::get(&EdgeInfoProperty::score, G)));
    return 0;
} 
