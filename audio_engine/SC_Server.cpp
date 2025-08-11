#include "SC_Server.hpp"
#include "lo/lo.h"
#include <cstdlib>
#include <stdexcept>

SC_Server::SC_Server(const char* host, const char* port){
    // Determine directory for SCSYNTH
    std::string dir = std::getenv("SCSYNTH_DIR");
    if (dir.empty()){
        throw std::runtime_error(std::string("Environment variable not set: SCSYNTH_DIR"));
    }
    // Ensure leading slash
    if (dir[0] != '/'){
        dir = "/" + dir;
    }    

    // Initialize address and load synthdefs
    sc = lo_address_new(host, port);
    if (!sc) throw std::runtime_error("lo_address_new failed");

    lo_send(sc, "/d_loadDir", "s", dir.c_str());
}

SC_Server::~SC_Server(){
    for (node_id active_node : active_nodes){
        lo_send(sc, "/n_free", "i", active_node);
    }
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
    lo_send(sc, "/s_new", "siii", "bear", id, 1, 0);
    return id;
}