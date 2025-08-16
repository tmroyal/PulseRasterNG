#include "SC_Server.hpp"
#include "lo/lo.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>




SC_Server::SC_Server(const char* host, const char* port){
    // Determine directory for SCSYNTH
    const char* env_dir = std::getenv("SCSYNTH_DIR");
    if (!env_dir){
        throw std::runtime_error(std::string("Environment variable not set: SCSYNTH_DIR"));
    }
    std::cout << "Loading SCSYNTH_DIR: " << env_dir << std::endl;
    std::string dir = env_dir;
    // Ensure leading slash
    if (dir[0] != '/'){
        dir = "/" + dir;
    }

    // Initialize address and load synthdefs
    sc = lo_address_new(host, port);
    if (!sc) throw std::runtime_error("lo_address_new failed");

    lo_send(sc, "/d_loadDir", "s", dir.c_str());
    wait_sync(42);
}

bool SC_Server::wait_sync(int tag, int timeout_ms) {
    lo_server s = lo_server_new(nullptr, nullptr); // ephemeral port

    struct Ctx { int want; bool done; } ctx{ tag, false };

    lo_server_add_method(s, "/synced", "i",
        [](const char*, const char*, lo_arg** a, int, lo_message, void* ud)->int {
            auto* c = static_cast<Ctx*>(ud);
            if (a[0]->i == c->want) c->done = true;
            return 0;
        },
        &ctx
    );

    lo_send(sc, "/sync", "i", tag);

    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
    while (!ctx.done && std::chrono::steady_clock::now() < deadline) {
        lo_server_recv_noblock(s, 10);
    }

    lo_server_free(s);
    return ctx.done;
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