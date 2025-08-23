#include "SC_Server.hpp"
#include "lo/lo.h"
#include <cstdlib>
#include <stdexcept>


SC_Server::SC_Server(const char* host, const char* port){
    // Initialize address and load synthdefs
    sc = lo_address_new(host, port);
    if (!sc) throw std::runtime_error("lo_address_new failed");
}

SC_Server::~SC_Server(){
    lo_address_free(sc);
}

void SC_Server::set(node_id node, const char* name, int value){
    lo_send(sc, "/n_set", "isi", node, name, value);
}

void SC_Server::set(node_id node, const char* name, double value){
    lo_send(sc, "/n_set", "isf", node, name, value);
}

node_id SC_Server::synth(const std::string& name){
    node_id id = current_node.fetch_add(1, std::memory_order_relaxed);
    {
        std::lock_guard<std::mutex> lock(m);
        active_nodes.insert(id);
    }
    // lo_send(sc, "/freq", "sf", name.c_str(), 440);
    lo_send(sc, "/freq", "f", 440);
    return id;
}