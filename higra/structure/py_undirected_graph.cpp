/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "py_undirected_graph.hpp"
#include "py_common_graph.hpp"

namespace py = pybind11;

template<typename graph_t>
struct def_add_edges {
    template<typename value_t, typename C>
    static
    void def(C &c, const char *doc) {
        c.def("add_edges", [](graph_t &g,
                              pyarray<value_t> sources,
                              pyarray<value_t> targets
              ) {
                  hg::add_edges(sources, targets, g);
              },
              doc,
              py::arg("sources"),
              py::arg("targets"));
    }
};

template<typename graph_t, typename class_t>
void init_graph(class_t &c) {

    using edge_t = typename hg::graph_traits<graph_t>::edge_descriptor;
    using vertex_t = typename hg::graph_traits<graph_t>::vertex_descriptor;
    using edge_index_t = typename hg::graph_traits<graph_t>::edge_index;

    c.def(py::init<const hg::size_t>(), "Create a new graph with no edge.",
          py::arg("number_of_vertices") = 0);

    add_edge_accessor_graph_concept<graph_t, decltype(c)>(c);
    add_incidence_graph_concept<graph_t, decltype(c)>(c);
    add_bidirectionnal_graph_concept<graph_t, decltype(c)>(c);
    add_adjacency_graph_concept<graph_t, decltype(c)>(c);
    add_vertex_list_graph_concept<graph_t, decltype(c)>(c);
    add_edge_list_graph_concept<graph_t, decltype(c)>(c);
    add_edge_index_graph_concept<graph_t, decltype(c)>(c);

    c.def("add_edge", [](graph_t &g,
                         const vertex_t source,
                         const vertex_t target) {
              return cpp_edge_2_python(hg::add_edge(source, target, g));
          },
          "Add an (undirected) edge between 'vertex1' and 'vertex2'. Returns the new edge.",
          py::arg("source"),
          py::arg("target"));

    add_type_overloads<def_add_edges<graph_t>, int, unsigned int, long long, unsigned long long>
            (c, "Add all edges given as a pair of arrays (sources, targets) to the graph.");

    c.def("add_vertex", [](graph_t &g) {
              return hg::add_vertex(g);
          },
          "Add a vertex to the graph, the index of the new vertex is returned");
    c.def("set_edge", &graph_t::set_edge,
          py::arg("edge_index"),
          py::arg("source"),
          py::arg("target"),
          "Modify the source and the target of the given edge.");
    c.def("remove_edge", &graph_t::remove_edge,
          py::arg("edge_index"),
          "Remove the given edge from the graph (the edge is not really removed: "
                  "its source and target are attached to a virtual node of index -1).");
}

void py_init_undirected_graph(py::module &m) {
    xt::import_numpy();
    auto c = py::class_<hg::undirected_graph<hg::vecS> >(m, "UndirectedGraph");
    init_graph<hg::ugraph>(c);

    auto c2 = py::class_<hg::undirected_graph<hg::hash_setS>>(m, "UndirectedGraphOptimizedDelete");
    init_graph<hg::undirected_graph<hg::hash_setS >>(c2);
}


