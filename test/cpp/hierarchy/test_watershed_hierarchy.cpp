/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <boost/test/unit_test.hpp>
#include "../test_utils.hpp"
#include "higra/hierarchy/watershed_hierarchy.hpp"
#include "higra/image/graph_image.hpp"
#include "higra/algo/tree.hpp"

using namespace hg;
using namespace std;

BOOST_AUTO_TEST_SUITE(algo_watershed_hierarchy);


    BOOST_AUTO_TEST_CASE(test_watershed_hierarchy_by_area) {


        auto g = hg::get_4_adjacency_graph({1, 19});
        array_1d<int> edge_weights{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0};

        auto res = watershed_hierarchy_by_area(g, edge_weights);
        auto &t = res.tree;
        auto &altitudes = res.altitudes;

        array_1d<index_t> ref_parents{19, 19, 20, 20, 20, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 23, 23, 23, 24,
                                      24, 25, 26, 26, 25, 27, 27, 27};
        tree ref_tree(ref_parents);
        array_1d<int> ref_altitudes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 5};

        BOOST_CHECK(test_tree_isomorphism(t, ref_tree));
        BOOST_CHECK(altitudes == ref_altitudes);
    }

    BOOST_AUTO_TEST_CASE(test_watershed_hierarchy_by_dynamics) {

        auto g = hg::get_4_adjacency_graph({1, 7});
        array_1d<int> edge_weights{1, 4, 1, 0, 10, 8};

        auto res = watershed_hierarchy_by_dynamics(g, edge_weights);
        auto &t = res.tree;
        auto &altitudes = res.altitudes;

        array_1d<index_t> ref_parents{7, 7, 8, 8, 8, 9, 9, 11, 10, 10, 11, 11};
        tree ref_tree(ref_parents);
        array_1d<int> ref_altitudes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3};

        BOOST_CHECK(test_tree_isomorphism(t, ref_tree));
        BOOST_CHECK(altitudes == ref_altitudes);
    }


    BOOST_AUTO_TEST_CASE(test_watershed_hierarchy_by_volume) {

        auto g = hg::get_4_adjacency_graph({1, 7});
        array_1d<int> edge_weights{1, 4, 1, 0, 10, 8};

        auto res = watershed_hierarchy_by_volume(g, edge_weights);
        auto &t = res.tree;
        auto &altitudes = res.altitudes;

        array_1d<index_t> ref_parents{7, 7, 8, 8, 8, 9, 9, 11, 10, 10, 11, 11};
        tree ref_tree(ref_parents);
        array_1d<int> ref_altitudes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6};

        BOOST_CHECK(test_tree_isomorphism(t, ref_tree));
        BOOST_CHECK(altitudes == ref_altitudes);
    }


BOOST_AUTO_TEST_SUITE_END();